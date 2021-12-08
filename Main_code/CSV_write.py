# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> CSV_write
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/6/1 11:27
=================================================='''
##########################################################
#                       OK                               #
##########################################################
import csv
from ProjectPath import projectPath
distance_csv_path = (projectPath + "\\Matplot_data\\Distance_data.csv")
strength_csv_path = (projectPath + "\\Matplot_data\\Strength_data.csv")
outliersRate_csv_path = (projectPath + "\\Matplot_data\\outliersRate.csv")
    # 将数据写入文件
def clear_csv_data():
    with open(distance_csv_path, "w") as cf:
        cf.truncate()
def fill_distance_strength(data1,data2,data3,data4):
    with open(distance_csv_path, "a", newline="") as cf:
        w = csv.writer(cf)
        w.writerow([data1, data2, data3,data4])
        cf.close()

#记录信号强度
def clear_fill_strength():
    with open(strength_csv_path, "w") as sf:
        sf.truncate()
def fill_strength(data1, data2):
    with open(strength_csv_path, "a", newline="") as sf:
        Strength = csv.writer(sf)
        Strength.writerow([data1, data2])
        sf.close()

###########################################################
#                   异常点记录                             #
###########################################################
def clear_fill_outliers():
    with open(outliersRate_csv_path, "w") as rf:
        rf.truncate()
def fill_outliers_rate(data1,data2):
    with open(outliersRate_csv_path, "a", newline="") as rf:
        outliersRate = csv.writer(rf)
        outliersRate.writerow([data1, data2])
        rf.close()



###########################################################
#                      test                               #
###########################################################
# import csv
# distance_csv_path = (".\\Matplot_data\\Distance_data.csv")
# strength_csv_path = (".\\Matplot_data\\Strength_data.csv")
#     # 将数据写入文件
# def clear_csv_data():
#     with open(distance_csv_path, "w") as cf:
#         cf.truncate()
# def fill_distance_strength(data1,data2,data3,data4):
#     with open(distance_csv_path, "a", newline="") as cf:
#         w = csv.writer(cf)
#         w.writerow([data1, data2, data3, data4])
#         cf.close()
#
# #记录信号强度
# def clear_fill_strength():
#     with open(strength_csv_path, "w") as sf:
#         sf.truncate()
# def fill_strength(data1, data2):
#     with open(strength_csv_path, "a", newline="") as sf:
#         Strength = csv.writer(sf)
#         Strength.writerow([data1, data2])
#         sf.close()