# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Version.ui'
#
# Created by: PyQt5 UI code generator 5.15.2
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Version(object):
    def setupUi(self, Version):
        Version.setObjectName("Version")
        Version.resize(600, 500)
        Version.setMinimumSize(QtCore.QSize(600, 500))
        Version.setMaximumSize(QtCore.QSize(600, 500))
        font = QtGui.QFont()
        font.setFamily("3ds")
        font.setPointSize(10)
        Version.setFont(font)
        self.label_2 = QtWidgets.QLabel(Version)
        self.label_2.setGeometry(QtCore.QRect(60, 30, 471, 131))
        self.label_2.setText("")
        self.label_2.setPixmap(QtGui.QPixmap("../Icon/sr.jpg"))
        self.label_2.setScaledContents(True)
        self.label_2.setObjectName("label_2")
        self.label = QtWidgets.QLabel(Version)
        self.label.setGeometry(QtCore.QRect(70, 160, 111, 21))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        font.setBold(True)
        font.setWeight(75)
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.label_3 = QtWidgets.QLabel(Version)
        self.label_3.setGeometry(QtCore.QRect(70, 210, 371, 16))
        font = QtGui.QFont()
        font.setFamily("Times New Roman")
        font.setPointSize(10)
        self.label_3.setFont(font)
        self.label_3.setObjectName("label_3")
        self.label_4 = QtWidgets.QLabel(Version)
        self.label_4.setGeometry(QtCore.QRect(70, 230, 401, 41))
        font = QtGui.QFont()
        font.setFamily("Times New Roman")
        font.setPointSize(10)
        self.label_4.setFont(font)
        self.label_4.setObjectName("label_4")
        self.label_5 = QtWidgets.QLabel(Version)
        self.label_5.setGeometry(QtCore.QRect(70, 280, 221, 21))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        font.setBold(True)
        font.setWeight(75)
        self.label_5.setFont(font)
        self.label_5.setObjectName("label_5")
        self.label_6 = QtWidgets.QLabel(Version)
        self.label_6.setGeometry(QtCore.QRect(70, 325, 481, 21))
        font = QtGui.QFont()
        font.setFamily("Times New Roman")
        font.setPointSize(10)
        self.label_6.setFont(font)
        self.label_6.setObjectName("label_6")
        self.label_8 = QtWidgets.QLabel(Version)
        self.label_8.setGeometry(QtCore.QRect(150, 350, 401, 21))
        font = QtGui.QFont()
        font.setFamily("Times New Roman")
        font.setPointSize(10)
        self.label_8.setFont(font)
        self.label_8.setObjectName("label_8")
        self.label_7 = QtWidgets.QLabel(Version)
        self.label_7.setGeometry(QtCore.QRect(70, 370, 371, 31))
        font = QtGui.QFont()
        font.setFamily("Times New Roman")
        font.setPointSize(10)
        self.label_7.setFont(font)
        self.label_7.setObjectName("label_7")
        self.label_9 = QtWidgets.QLabel(Version)
        self.label_9.setGeometry(QtCore.QRect(70, 410, 371, 31))
        font = QtGui.QFont()
        font.setFamily("Times New Roman")
        font.setPointSize(10)
        self.label_9.setFont(font)
        self.label_9.setObjectName("label_9")

        self.retranslateUi(Version)
        QtCore.QMetaObject.connectSlotsByName(Version)

    def retranslateUi(self, Version):
        _translate = QtCore.QCoreApplication.translate
        Version.setWindowTitle(_translate("Version", "版本信息"))
        self.label.setText(_translate("Version", "Version 1.0"))
        self.label_3.setText(_translate("Version", "CopeRight   2021 Standard Robots"))
        self.label_4.setText(_translate("Version", "All rights reserved"))
        self.label_5.setText(_translate("Version", "Technical support:"))
        self.label_6.setText(_translate("Version", "Bai Jiawei,   King\'s College London"))
        self.label_8.setText(_translate("Version", " 158-168 Strand,  London,  WC2R 2LS, United Kingdom"))
        self.label_7.setText(_translate("Version", "Email:  bjw_0933@163.com"))
        self.label_9.setText(_translate("Version", "Github:"))