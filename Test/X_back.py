from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from ProjectPath import projectPath
from numpy import *

# def randrange(n, vmin, vmax):
#     '''
#     Helper function to make an array of random numbers having shape (n, )
#     with each number distributed Uniform(vmin, vmax).
#     '''
#     return (vmax - vmin) * np.random.rand(n) + vmin


fig = plt.figure()
ax = fig.gca(projection='3d')
path = 'D:\SR_SToF100_TestSystemV1.5\Matplot_data\Distance_data.csv'
df = pd.read_csv(path)
alt = array(df)
# print(alt)
polar_angle = []
distance = []
k = [0,5,10]
for q in k:
    zs = q
    for a in alt:
        polar_angle.append(a[0])
        distance.append(a[1])
    xs = array(polar_angle)
    ys = array(distance)
    ax.scatter(xs, ys, zs, c='b', marker='o')
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')
print(xs)
print(ys)

DATA3D_img_path = ("D:\SR_SToF100_TestSystemV1.5\Matplot_data\DATA3D_image" + ".png")   #文件名为OutliersRate_image.png
plt.savefig(DATA3D_img_path, dpi=500, bbox_inches='tight')

plt.show()


# import matplotlib.pyplot as plt
#
# import numpy as np
#
# x = np.arange(-180.0,190.0,10)
#
# theta = (np.pi/180.0 )*x # in radians
#
# A0 = 55.0
#
# offset = 60.0
#
# R = [116.225,115.105,114.697,115.008,115.908,117.184,118.61,119.998,121.224,122.216,\
#
# 122.93,123.323,123.343,122.948,122.134,120.963,119.575,118.165,116.941,116.074,115.66\
#
# ,115.706,116.154,116.913,117.894,119.029,120.261,121.518,122.684,123.594,124.059,\
#
# 123.917,123.096,121.661,119.821,117.894,116.225]
#
# fig = plt.figure()
#
# ax = fig.add_axes([0.1,0.1,0.8,0.8],polar=True) # Polar plot
#
# ax.plot(theta,R,lw=2.5)
#
# ax.set_rmax(1.5*(A0)+offset)
#
# plt.show()
#


# from numpy import *
#
# from mpl_toolkits.mplot3d import Axes3D
#
# import matplotlib.pyplot as plt
#
# n = 1000
#
# fig = plt.figure()
#
# ax = fig.gca(projection='3d')
#
# for k in linspace(0, 5, 5):
#
#     THETA = linspace(0, 2*pi, n)
#
# R = ones(THETA.shape)*cos(THETA*k)
#
# # Convert to Cartesian coordinates
#
# X = R*cos(THETA)
#
# Y = R*sin(THETA)
#
# ax.plot(X, Y, k-2)
#
# plt.show()