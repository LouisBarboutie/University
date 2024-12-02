'''
    Script to compute the parameters of the antenna on the OMP building in Toulouse.
'''

# ================= #
# === Libraries === #
# ================= #

import json
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os

os.system('cls')

# =========================================================== #
# === Build the azimuth and angle series in snake pattern === #
# =========================================================== #

# interval for the acquisition in degrees
START = -4
STOP  =  4
STEP  =  2
N_POINTS = 25

el = np.zeros((5, 5))
az = np.zeros((5, 5))
for i in range(5):
    values = range(START, STOP+1, STEP)
    el[:, i] = values[::-1]
    az  [i, :] = values

print(el)
print(az)
el_series = np.rot90(el, -1).flatten()
az_series = np.rot90(az, -1).flatten()



# ======================================== #
# === Read the data from the json file === #
# ======================================== #

FILE_NAME = 'data/2024_11_27_12_25_37_27.rad' # cutoff = 1 # skip the first two spectra
# FILE_NAME = 'data/2024_11_27_12_34_18_27.rad' # cutoff = 2 # skip the first two spectra

with open(FILE_NAME) as file:
    data = json.load(file)

series = data['series']
print(f'Found {len(series)} acquisitions')



# ========================== #
# === Plotting fo Brrrrr === #
# ========================== #

LOWER_BOUND = 1421.05
UPPER_BOUND = 1422.14

# --- debug ---
# old_el = series[0]['elNow']
# old_az = series[0]['azNow']
# -------------

cutoff = 1
avg_intensities = np.zeros(N_POINTS)
for i, spectrum in enumerate(series):
    if i < cutoff: continue # skip    first one
    
    # --- debug ---
    # new_el = spectrum['elNow']
    # new_az = spectrum['azNow']
    # print(f'diff = el: {new_el-old_el:.1f}, az: {new_az-old_az:.1f}')
    # old_el = new_el
    # old_az = new_az 
    # -------------

    intensities = np.array(spectrum['value'])
    frequencies = np.arange(spectrum['nFreq'], dtype='float')
    frequencies *= spectrum['freqSep']
    frequencies += spectrum['freq0']

    filtered_frequencies = frequencies[(frequencies > LOWER_BOUND) & (frequencies < UPPER_BOUND)] 
    filtered_intensities = intensities[(frequencies > LOWER_BOUND) & (frequencies < UPPER_BOUND)] 

    avg_intensity = np.sum(filtered_intensities) / len(filtered_intensities)
    avg_intensities[i-cutoff] = avg_intensity
    plt.plot(frequencies, intensities)
# get a surface instead of a series of points
avg_intensities = np.reshape(avg_intensities, az.shape)
print(spectrum['freq0'])
plt.show()


# ============================ #
# === Gotta fit dem curves === #
# ============================ #

def model(input, x0, y0, mag, a, b, bg):
    '''
        See:
            https://en.wikipedia.org/wiki/Gaussian_function#Two-dimensional_Gaussian_function
        at section:
            §Two-dimensional Gaussian function
    '''
    x, y = input
    exponent = a*(x - x0)**2 + b*(y-y0)**2
    return np.ravel(mag * np.exp(-exponent) + bg)

# generate points for the fit
x = range(START, STOP+1, STEP)
y = range(START, STOP+1, STEP)[::-1]
x, y = np.meshgrid(x, y)
x_flat = x.ravel()
y_flat = y.ravel()

# calculate the fit
popt, pcov = curve_fit(model, (x_flat, y_flat), np.ravel(avg_intensities))
fitted_intensities = np.reshape(model((x_flat, y_flat), *popt), az.shape)

params = ['x0', 'y0', 'mag', 'a', 'b', 'bg'] 
print('\nOptimized parameters:\n' + '\n'.join([f'{params[i]}:\t {val:.2e}' for i, val in enumerate(popt)]))

sigma_x = np.sqrt(1 / (2 * popt[3]))
sigma_y = np.sqrt(1 / (2 * popt[4]))
FWHM_x = 2 * np.sqrt(2 * np.log(2)) * sigma_x
FWHM_y = 2 * np.sqrt(2 * np.log(2)) * sigma_y

c = 299792458
frequency = 1421.5e6
wavelength = c / frequency
diameter = 3 # meter
angular_resolution = np.degrees(1.22 * wavelength / diameter)
print('\nTheoretical values:')
print(f'Angular resolution: {angular_resolution}°')

print('\nPhysical variables of interest:')
print(f'sigma_x: {sigma_x:.2f}°')
print(f'sigma_x: {sigma_y:.2f}°')
print(f'FWHM_x:  {FWHM_x:.2f}°')
print(f'FWHM_x:  {FWHM_y:.2f}°')

# plot and save the gaussian fit
fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
ax.plot_surface(az, el, avg_intensities)
ax.plot_surface(az, el, fitted_intensities)
ax.set_xlabel('Azimuth [°]')
ax.set_ylabel('Elevation [°]')
ax.set_zlabel('Intensity [a.u.]')
fig.suptitle('Fitted Gaussian')
fig.savefig('gaussian_fit.png', dpi=300)

# plot and save the residuals
fig, ax = plt.subplots()
ax.contourf(az, el, avg_intensities-fitted_intensities)
ax.set_xlabel('Azimuth [°]')
ax.set_ylabel('Elevation [°]')
fig.suptitle('Residuals')
fig.savefig('residuals.png', dpi=300)

plt.show()