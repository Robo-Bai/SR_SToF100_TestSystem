# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> test-csv
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/11 14:27
=================================================='''
import csv

    # 将数据写入文件
def clear_csv_data():
    with open("c:\SR-SToF100-test-system\CAN-DATA0525_fin2.csv", "w") as cf:
        cf.truncate()
def fill_distance_strength(data1,data2,data3):
    with open("c:\SR-SToF100-test-system\CAN-DATA0525_fin2.csv", "a", newline="") as cf:
        w = csv.writer(cf)
        w.writerow([data1, data2, data3])
        cf.close()

#记录信号强度
def clear_fill_strength():
    with open("c:\SR-SToF100-test-system\strength_data.csv", "w") as sf:
        sf.truncate()
def fill_strength(data1, data2):
    with open("c:\SR-SToF100-test-system\strength_data.csv", "a", newline="") as sf:
        Strength = csv.writer(sf)
        Strength.writerow([data1, data2])
        sf.close()