# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> Image_show
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/27 9:29
=================================================='''
import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QFileDialog
from PyQt5.QtGui import *
from PyQt5 import uic, QtGui, QtWidgets, QtCore
#对应父窗口py文件：SR-SToF100-test-system/Ui/MainUi.py
from Ui.show_image import Ui_show_Image1
import time
# from threading import Thread
# from Test.test_csv import *
# from CSV_Plot import *

path_image = 'c:\SR-SToF100-test-system\CAN-DATA_0526.png'

#主窗口
class Main_Window(QMainWindow, Ui_show_Image1):

    def __init__(self, parent=None):
        super(Main_Window, self).__init__()
        self.setupUi(self)

        self.pushButton.clicked.connect(self.update_image)

    def update_image(self):
        pixmap = QtGui.QPixmap(path_image)
        self.label.setPixmap(QtGui.QPixmap(path_image))
        self.label.setScaledContents(True)
        self.label.setObjectName("label")

#窗口运行函数
if __name__ == "__main__":
    app = QApplication(sys.argv)
    mainWindow = Main_Window()
    mainWindow.show()
    sys.exit(app.exec_())