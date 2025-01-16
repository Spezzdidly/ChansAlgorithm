# Generate a graph of the points on the convex hull
import matplotlib.pyplot as plt
import numpy as np

x = []
y = []
hull = []

while True:
    file = input("Enter the name of the file: ")

    # W this shit be working ig
    try:
        with open(file, "r") as f:
            print("File found!")
            for i in f:
                hull.append(i)
            break
    except FileNotFoundError:
        print("That is not a valid file name. Please try again...")


    
for p in hull:
    a, _, b = p.partition(',')
    x.append(a)
    y.append(b[:-2])

lx = [float(i) for i in x]
ly = [float(i) for i in y]

plt.scatter(lx, ly)
plt.show()