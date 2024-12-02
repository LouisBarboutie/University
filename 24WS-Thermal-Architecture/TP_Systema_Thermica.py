import matplotlib.pyplot as plt
import os

def parse_csv(file_path):
    with open(file_path) as file:
        lines = file.readlines()

    headers = lines[2].split(';')
    columns = len(headers) 
    data = [[] for i in range(columns)]

    for i, line in enumerate(lines):
        # skip lines without data
        if i < 3:
            continue 
        
        # split the row into a list
        values = (line.split(';'))

        # store the value for return
        for j, val in enumerate(values):
            data[j].append(float(val))

    return headers, data

if __name__ == '__main__':
    
    os.system('cls')
    
    headers, data = parse_csv('Cube-BOL_albedo.csv')

    for i in range(1, len(data)):
        plt.plot(data[0], data[i], label=headers[i])
    plt.legend()
    plt.show()