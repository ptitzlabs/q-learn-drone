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


contour_x0 = [];
contour_y0 = [];
contour_z0 = [];
contour_x1 = [];
contour_y1 = [];
contour_z1 = [];
contour_x2 = [];
contour_y2 = [];
contour_z2 = [];


with open('contour0_x.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_x0.append(line)
with open('contour0_y.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_y0.append(line)
with open('contour0_z.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_z0.append(line)


plt.figure(3)
plt.contourf(contour_x0,contour_y0,contour_z0)
plt.colorbar()
plt.draw()
with open('contour1_x.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_x1.append(line)
with open('contour1_y.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_y1.append(line)
with open('contour1_z.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_z1.append(line)

plt.figure(4)
plt.contourf(contour_x1,contour_y1,contour_z1)
plt.colorbar()
plt.draw()
with open('contour2_x.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_x2.append(line)
with open('contour2_y.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_y2.append(line)
with open('contour2_z.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_z2.append(line)

plt.figure(5)
plt.contourf(contour_x2,contour_y2,contour_z2)
plt.colorbar()
plt.draw()
max_z = -1000000000

contour_action = []

for i in range(0,len(contour_x0)):
    contour_action.append([]*len(contour_y0))
    for j in range(0,len(contour_y0)):
        contour_action[i].append(0)

for i in range(0,len(contour_x0)):
    for j in range(0,len(contour_y0)):
        if contour_z0[i][j] > max_z:
            contour_action[i][j] = -1
            max_z = contour_z0[i][j]
        if contour_z1[i][j] > max_z:
            contour_action[i][j] = 0
            max_z = contour_z1[i][j]
        if contour_z2[i][j] > max_z:
            contour_action[i][j] = 1
        max_z = -10000000

#print contour_action
plt.figure(6)
plt.contourf(contour_x2,contour_y2,contour_action)
plt.colorbar()
plt.draw()



contour_x0 = [];
contour_y0 = [];
contour_z0 = [];
contour_x1 = [];
contour_y1 = [];
contour_z1 = [];
contour_x2 = [];
contour_y2 = [];
contour_z2 = [];

with open('contour_barto0_x.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_x0.append(line)
with open('contour_barto0_y.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_y0.append(line)
with open('contour_barto0_z.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_z0.append(line)


plt.figure(7)
plt.contourf(contour_x0,contour_y0,contour_z0)
plt.colorbar()
plt.draw()
with open('contour_barto1_x.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_x1.append(line)
with open('contour_barto1_y.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_y1.append(line)
with open('contour_barto1_z.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_z1.append(line)

plt.figure(8)
plt.contourf(contour_x1,contour_y1,contour_z1)
plt.colorbar()
plt.draw()
with open('contour_barto2_x.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_x2.append(line)
with open('contour_barto2_y.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_y2.append(line)
with open('contour_barto2_z.csv','rb') as csvfile:
    traj_reader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for line in traj_reader:
        contour_z2.append(line)

plt.figure(9)
plt.contourf(contour_x2,contour_y2,contour_z2)
plt.colorbar()
plt.draw()
max_z = -1000000000


for i in range(0,len(contour_x0)):
    for j in range(0,len(contour_y0)):
        if contour_z0[i][j] > max_z:
            contour_action[i][j] = -1
            max_z = contour_z0[i][j]
        if contour_z1[i][j] > max_z:
            contour_action[i][j] = 0
            max_z = contour_z1[i][j]
        if contour_z2[i][j] > max_z:
            contour_action[i][j] = 1
        max_z = -10000000

print contour_action
plt.figure(10)
plt.contourf(contour_x2,contour_y2,contour_action)
plt.colorbar()
plt.draw()
plt.show()

