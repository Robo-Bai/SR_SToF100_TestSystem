# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> test-csv-read
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/11 17:25
=================================================='''
import csv,os
import matplotlib.pyplot as plt
import pandas as pd
from numpy import *

'''======================测试csv数据运算绘制极坐标图（OK）===================='''

#导入绘制极图所需要的包
import matplotlib.pyplot as plt
import numpy as np

'''=========保存图片的方法（ok）=========='''
def PNGfileSave():
    '''保存生成的图片

    保存至当前目录下：\Test
    '''
    img_path = "CAN-DATA" + ".png"   #文件名为CAN-DATA.png
    plt.savefig(img_path, dpi=500, bbox_inches='tight')

def DrawDataplot():
    #取数据
    df = pd.read_csv('c:\SR-SToF100-test-system\CAN-DATA0524_4.csv')
    alt = array(df)
    # print(alt)
    x0 = []
    y0 = []
    for a in alt:
        x0.append(a[0])
        y0.append(a[1])
    x0 = array(x0)
    y0 = array(y0)
    # print(x0)
    print(y0)

    #设置下面所需要的参数
    barSlices = len(y0)
    #theta指每个标记所在射线与极径的夹角，下面表示均分角度
    # theta = np.linspace(0, (2*np.pi)/3, barSlices, endpoint=True)
    #r表示点距离圆心的距离，np.random/.rand(barSlices)表示返回返回服从“0-1”均匀分布的随机样本值
    angle = x0
    r = y0
    #polar表示绘制极坐标图，颜色，线宽，标志点样式
    plt.polar(angle, r, color="chartreuse", linewidth=0.1, marker=".", mfc="b", ms=10)
    #将生成的图片保存到Test文件夹下
    PNGfileSave()

    #绘图展示
    plt.show()

if __name__ == "__main__":
    DrawDataplot()


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


