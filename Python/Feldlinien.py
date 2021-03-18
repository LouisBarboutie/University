# import library and set preference
import matplotlib.pyplot as plt
import numpy as np
np.seterr(divide= 'ignore', invalid='ignore')

# blyatiful constants
EPSILON = 8.854187817 * 10 ** -12
Q = +1

# gibMe some inputs
a = 5
a0 = 0

# quantity of arrows in x and y
spreadModifier = 1000
sizeModifier = 11

# gridmesh time
hellOo = np.arange(-sizeModifier, sizeModifier, .05)
x, y = np.meshgrid(hellOo, hellOo)

# getting Ex and Ey
subOne = np.power(np.sqrt(np.power((x-a), 2) + np.power((y-a0), 2)))
subTwo = np.power(np.sqrt(np.power((x+a), 2) + np.power((y-a0), 2)))
Ex = Q / (4*3.14*EPSILON) * (((x-a) / subOne) - ((x+a) / subTwo))
Ey = Q / (4*3.14*EPSILON) * ((y / subOne) - (y / subTwo))

# ain't nobody got time fo dat
fig, ax = plt.subplots(figsize=(sizeModifier, sizeModifier))
ax.streamplot(x, y, Ex, Ey, color='y')

ax.set_aspect("equal")
ax.plot(-5, 0, "-ob")
ax.plot(5, 0, "-or")
ax.set_title("Elektrische Feldlinien")
plt.show()
