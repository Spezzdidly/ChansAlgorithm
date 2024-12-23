import random
import math

file_name = 	input("Enter the name you want for the set: ")
num_pts = 	input("Enter the number of points you want generated: ")
range_for_pts = input("Enter a number that will represent the positive and"
		      " negative ends of the range for the points: ")
num_pts =       int(num_pts)
range_for_pts = int(range_for_pts)
fout =          open(file_name, 'w', encoding="utf-8")

for coord in range(num_pts):
    coord_x = random.uniform(-range_for_pts, range_for_pts)
    coord_y = random.uniform(-range_for_pts, range_for_pts)
    x = str(round(coord_x, 3))
    y = str(round(coord_y, 3))
    line = x + ' ' + y + '\n'
    fout.write(line)

fout.close()

