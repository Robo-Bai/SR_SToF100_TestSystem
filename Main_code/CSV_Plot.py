# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> CSV_Plot
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/6/1 11:23
=================================================='''

import csv,os
import matplotlib.pyplot as plt
import pandas as pd
from numpy import *

'''======================测试csv数据运算绘制极坐标图（OK）===================='''

#导入绘制极图所需要的包
import matplotlib.pyplot as plt
import numpy as np
from ProjectPath import projectPath

# '''=========保存图片的方法（ok）=========='''
# def PNGfileSave():
#     '''保存生成的图片
#
#     保存至当前目录下
#     '''
#     img_path = (projectPath + "\\Matplot_data\\Distance_image" + ".png")   #文件名为Distance_image.png
#     plt.savefig(img_path, dpi=500, bbox_inches='tight')
#
# def PNG_strength_save():
#     '''保存生成的信号强度图片
#
#     保存至当前目录下
#     :return:
#     '''
#     strength_img_path = (projectPath + "\\Matplot_data\\Strenght_image" + ".png")   #文件名为Strength_image.png
#     plt.savefig(strength_img_path, dpi=500, bbox_inches='tight')
#
# def PNG_outliersImage_save():
#     '''保存生成的异常率图片
#
#     保存至当前目录下
#     :return:
#     '''
#     outliers_img_path = (projectPath + "\\Matplot_data\\OutliersRate_image" + ".png")   #文件名为OutliersRate_image.png
#     plt.savefig(outliers_img_path, dpi=500, bbox_inches='tight')

########################################################################
#                         测距结果图                                    #
########################################################################
def DrawDataplot():
    # plt.ion()  #交互模式
    #取数据
    df = pd.read_csv(projectPath + "\\Matplot_data\\Distance_data.csv")
    alt = array(df)
    # print(alt)
    polar_angle = []
    distance = []
    for a in alt:
        polar_angle.append(a[0])
        distance.append(a[1])
    polar_angle = array(polar_angle)
    distance = array(distance)
    # print(x0)
    print(distance)

    #设置下面所需要的参数
    angle = polar_angle  #极角
    r = distance         #距离
    #polar表示绘制极坐标图，颜色=蓝色；线的类型=空；线宽=0；标志点样式="."；标志点的大小=4
    plt.polar(angle, r, color ="b", linestyle='None' ,marker=".", ms=4)
    #将生成的图片保存到/Matplot_data/文件夹下
    # PNGfileSave()
    Distance_img_path = (projectPath + "\\Matplot_data\\Distance_image" + ".png")   #文件名为Distance_image.png
    plt.savefig(Distance_img_path, dpi=500, bbox_inches='tight')

    #绘图展示
    # plt.show(block = False)
    plt.close()

########################################################################
#                           信号强度图                                  #
########################################################################
def Strength_Image():
    # plt.ion()  # 交互模式
    strength_image_data = pd.read_csv(projectPath + "\\Matplot_data\\Strength_data.csv")
    strength_alt = array(strength_image_data)
    # print(alt)
    strength_num = []
    strength_DATA = []
    for b in strength_alt:
        strength_num.append(b[0])
        strength_DATA.append(b[1])
    strength_num = array(strength_num)
    strength_DATA = array(strength_DATA)
    # print(x0)
    print(strength_DATA)

    Strength_NUM = strength_num
    Strength_DATA = strength_DATA
    plt.plot(Strength_NUM, Strength_DATA)

    plt.xlabel('number')
    plt.ylabel('strength_data')

    # PNG_strength_save()
    Strength_img_path = (projectPath + "\\Matplot_data\\Strenght_image" + ".png")   #文件名为Strength_image.png
    plt.savefig(Strength_img_path, dpi=500, bbox_inches='tight')

    # plt.show(block = False)
    plt.close()

#####################################################################
#                          异常点率                                  #
#####################################################################
def OutliersRate_image():
    # plt.ion()  # 交互模式
    #取数据
    outliers_image_data = pd.read_csv(projectPath + "\\Matplot_data\\outliersRate.csv")
    outliers_alt = array(outliers_image_data)
    rate_num = []
    outliers_num = []
    for c in outliers_alt:
        rate_num.append(c[0])
        outliers_num.append(c[1])
    Rate_num = array(rate_num)
    Outliers_num = array(outliers_num)
    print(outliers_num)

    plt.plot(Rate_num, Outliers_num, marker = 'o', mec = 'r', mfc = 'w', label = 'Outlier Rate (%)')

    plt.xlabel('num')
    plt.ylabel('Outliers Rate %')

    # PNG_outliersImage_save()
    Outliers_img_path = (projectPath + "\\Matplot_data\\OutliersRate_image" + ".png")   #文件名为OutliersRate_image.png
    plt.savefig(Outliers_img_path, dpi=500, bbox_inches='tight')

    # plt.show(block = False)
    plt.close()
#
# #####################################################################
# #                          三维图                                  #
# #####################################################################
# from mpl_toolkits.mplot3d import Axes3D
# import matplotlib.pyplot as plt
# import numpy as np
#
#
# def randrange(n, vmin, vmax):
#     '''
#     Helper function to make an array of random numbers having shape (n, )
#     with each number distributed Uniform(vmin, vmax).
#     '''
#     return (vmax - vmin) * np.random.rand(n) + vmin
#
#
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')



if __name__ == "__main__":
    DrawDataplot()
    Strength_Image()
    OutliersRate_image()


'''=====================测试一个图（ok）================='''
# df = pd.read_csv('c:\SR-SToF100-test-system\CAN-DATA4.csv')  #好用的
#
# alt = array(df)
# print(alt)
# x0 = []
# y0 = []
# for a in alt:
#     x0.append(a[0])
#     y0.append(a[1])
# x0,y0 = array(x0),array(y0)
# print(x0)
# print(y0)
#
# plt.figure()
# plt.scatter(x0,y0,s=50,c='red',label = 'spot')
# plt.xlabel('time')
# plt.ylabel('data')
# plt.title('CAN')
# plt.show()

'''======================测试两个图（ok）===================='''
# df = pd.read_csv('c:\SR-SToF100-test-system\CAN-DATA4.csv')
#
# alt = array(df)
# print(alt)
# x0 = []
# y0 = []
# for a in alt:
#     x0.append(a[0])
#     y0.append(a[1])
# x0,y0 = array(x0),array(y0)
# print(x0)
# print(y0)
#
# plt.figure(1)
# ax1 = plt.subplot(1,2,1)
# ax2 = plt.subplot(1,2,2)
# #选择ax1
# plt.sca(ax1)
# plt.scatter(x0,y0,s=50,c='red',label = 'spot')
# plt.xlabel('time')
# plt.ylabel('data')
# plt.title('CAN')
# #选择ax2
# plt.sca(ax2)
# plt.scatter(x0,y0,s=50,c='blue',label = 'spot')
# plt.xlabel('time')
# plt.ylabel('data')
# plt.title('CAN')
# plt.show()


