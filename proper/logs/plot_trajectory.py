import numpy as np
import matplotlib.pyplot as plt
import matplotlib.pyplot as plt2
import csv
import sys
trajectory = []
with open('trajectory.txt','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        trajectory.append(line)

print len(trajectory[0])

x = [[]]
y = [[]]
k = -1
for xx,yy in trajectory:
    if xx == 0 and yy == -0.5:
        k = k+1
    x[k].append(xx)
    y[k].append(yy)

#print x[0]
plt.figure(1)
plt.plot(y[k],x[k])
plt.ylabel('some numbers')
plt.draw()

trajectory = []
with open('trajectory2.txt','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        trajectory.append(line)

#print len(trajectory[0])

x = [[]]
y = [[]]
k = -1
for xx,yy in trajectory:
    if xx == 0 and yy == -0.5:
        k = k+1
    x[k].append(xx)
    y[k].append(yy)

plt.figure(2)
plt.plot(y[k],x[k])
plt.ylabel('some numbers')
plt.draw()


contour_x = [];
contour_y = [];
contour_z = [];


with open('contour0_x.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_x.append(line)
with open('contour0_y.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_y.append(line)
with open('contour0_z.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_z.append(line)

plt.figure(3)
plt.contourf(contour_x,contour_y,contour_z)
plt.draw()
with open('contour1_x.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_x.append(line)
with open('contour1_y.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_y.append(line)
with open('contour1_z.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_z.append(line)

plt.figure(4)
plt.contourf(contour_x,contour_y,contour_z)
plt.draw()
with open('contour2_x.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_x.append(line)
with open('contour2_y.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_y.append(line)
with open('contour2_z.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_z.append(line)

plt.figure(5)
plt.contourf(contour_x,contour_y,contour_z)
plt.draw()
plt.show()
