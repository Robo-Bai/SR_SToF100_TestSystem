# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'location.ui'
#
# Created by: PyQt5 UI code generator 5.9.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_location(object):
    def setupUi(self, location):
        location.setObjectName("location")
        location.resize(595, 445)
        self.layoutWidget = QtWidgets.QWidget(location)
        self.layoutWidget.setGeometry(QtCore.QRect(10, 10, 571, 421))
        self.layoutWidget.setObjectName("layoutWidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.layoutWidget)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.location_textBrowser = QtWidgets.QTextBrowser(self.layoutWidget)
        self.location_textBrowser.setMinimumSize(QtCore.QSize(0, 0))
        self.location_textBrowser.setMaximumSize(QtCore.QSize(250, 16777215))
        self.location_textBrowser.setObjectName("location_textBrowser")
        self.horizontalLayout.addWidget(self.location_textBrowser)
        spacerItem = QtWidgets.QSpacerItem(20, 20, QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.groupBox = QtWidgets.QGroupBox(self.layoutWidget)
        self.groupBox.setMinimumSize(QtCore.QSize(0, 0))
        self.groupBox.setMaximumSize(QtCore.QSize(265, 16777215))
        self.groupBox.setObjectName("groupBox")
        self.layoutWidget1 = QtWidgets.QWidget(self.groupBox)
        self.layoutWidget1.setGeometry(QtCore.QRect(12, 25, 243, 381))
        self.layoutWidget1.setObjectName("layoutWidget1")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout(self.layoutWidget1)
        self.verticalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.label = QtWidgets.QLabel(self.layoutWidget1)
        self.label.setObjectName("label")
        self.horizontalLayout_2.addWidget(self.label)
        spacerItem1 = QtWidgets.QSpacerItem(13, 13, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem1)
        self.line = QtWidgets.QFrame(self.layoutWidget1)
        self.line.setFrameShape(QtWidgets.QFrame.VLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.horizontalLayout_2.addWidget(self.line)
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        self.label_3 = QtWidgets.QLabel(self.layoutWidget1)
        self.label_3.setObjectName("label_3")
        self.verticalLayout.addWidget(self.label_3)
        self.X_spinBox = QtWidgets.QSpinBox(self.layoutWidget1)
        self.X_spinBox.setMinimumSize(QtCore.QSize(80, 0))
        self.X_spinBox.setMinimum(50)
        self.X_spinBox.setMaximum(1400)
        self.X_spinBox.setSingleStep(100)
        self.X_spinBox.setObjectName("X_spinBox")
        self.verticalLayout.addWidget(self.X_spinBox)
        self.label_4 = QtWidgets.QLabel(self.layoutWidget1)
        self.label_4.setObjectName("label_4")
        self.verticalLayout.addWidget(self.label_4)
        self.X_goal_spinBox = QtWidgets.QSpinBox(self.layoutWidget1)
        self.X_goal_spinBox.setMinimum(100)
        self.X_goal_spinBox.setMaximum(1450)
        self.X_goal_spinBox.setSingleStep(100)
        self.X_goal_spinBox.setObjectName("X_goal_spinBox")
        self.verticalLayout.addWidget(self.X_goal_spinBox)
        self.horizontalLayout_2.addLayout(self.verticalLayout)
        self.X_confirm_Button = QtWidgets.QPushButton(self.layoutWidget1)
        self.X_confirm_Button.setObjectName("X_confirm_Button")
        self.horizontalLayout_2.addWidget(self.X_confirm_Button)
        self.verticalLayout_3.addLayout(self.horizontalLayout_2)
        self.line_3 = QtWidgets.QFrame(self.layoutWidget1)
        self.line_3.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_3.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_3.setObjectName("line_3")
        self.verticalLayout_3.addWidget(self.line_3)
        spacerItem2 = QtWidgets.QSpacerItem(20, 20, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Fixed)
        self.verticalLayout_3.addItem(spacerItem2)
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.label_2 = QtWidgets.QLabel(self.layoutWidget1)
        self.label_2.setObjectName("label_2")
        self.horizontalLayout_3.addWidget(self.label_2)
        spacerItem3 = QtWidgets.QSpacerItem(13, 15, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem3)
        self.line_2 = QtWidgets.QFrame(self.layoutWidget1)
        self.line_2.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_2.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_2.setObjectName("line_2")
        self.horizontalLayout_3.addWidget(self.line_2)
        self.verticalLayout_2 = QtWidgets.QVBoxLayout()
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.label_5 = QtWidgets.QLabel(self.layoutWidget1)
        self.label_5.setObjectName("label_5")
        self.verticalLayout_2.addWidget(self.label_5)
        self.Z_spinBox = QtWidgets.QSpinBox(self.layoutWidget1)
        self.Z_spinBox.setMinimumSize(QtCore.QSize(80, 0))
        self.Z_spinBox.setMaximum(450)
        self.Z_spinBox.setSingleStep(50)
        self.Z_spinBox.setObjectName("Z_spinBox")
        self.verticalLayout_2.addWidget(self.Z_spinBox)
        self.label_6 = QtWidgets.QLabel(self.layoutWidget1)
        self.label_6.setObjectName("label_6")
        self.verticalLayout_2.addWidget(self.label_6)
        self.Z_goal_spinBox = QtWidgets.QSpinBox(self.layoutWidget1)
        self.Z_goal_spinBox.setMinimum(50)
        self.Z_goal_spinBox.setMaximum(500)
        self.Z_goal_spinBox.setSingleStep(50)
        self.Z_goal_spinBox.setObjectName("Z_goal_spinBox")
        self.verticalLayout_2.addWidget(self.Z_goal_spinBox)
        self.horizontalLayout_3.addLayout(self.verticalLayout_2)
        self.Z_confrim_Button = QtWidgets.QPushButton(self.layoutWidget1)
        self.Z_confrim_Button.setObjectName("Z_confrim_Button")
        self.horizontalLayout_3.addWidget(self.Z_confrim_Button)
        self.verticalLayout_3.addLayout(self.horizontalLayout_3)
        self.line_4 = QtWidgets.QFrame(self.layoutWidget1)
        self.line_4.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_4.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_4.setObjectName("line_4")
        self.verticalLayout_3.addWidget(self.line_4)
        spacerItem4 = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Fixed)
        self.verticalLayout_3.addItem(spacerItem4)
        self.motor_enable_Button = QtWidgets.QPushButton(self.layoutWidget1)
        self.motor_enable_Button.setObjectName("motor_enable_Button")
        self.verticalLayout_3.addWidget(self.motor_enable_Button)
        self.horizontalLayout.addWidget(self.groupBox)

        self.retranslateUi(location)
        QtCore.QMetaObject.connectSlotsByName(location)

    def retranslateUi(self, location):
        _translate = QtCore.QCoreApplication.translate
        location.setWindowTitle(_translate("location", "位置操作"))
        location.setToolTip(_translate("location", "！！"))
        self.location_textBrowser.setPlaceholderText(_translate("location", "【注意！！】：结束点数值务必【大于】起始点数值"))
        self.groupBox.setTitle(_translate("location", "位置操作"))
        self.label.setText(_translate("location", "水平X轴"))
        self.label_3.setText(_translate("location", "起始点"))
        self.X_spinBox.setSuffix(_translate("location", "mm"))
        self.label_4.setText(_translate("location", "结束点"))
        self.X_goal_spinBox.setSuffix(_translate("location", "mm"))
        self.X_confirm_Button.setText(_translate("location", "确定"))
        self.label_2.setText(_translate("location", "垂直Z轴"))
        self.label_5.setText(_translate("location", "起始点"))
        self.Z_spinBox.setSuffix(_translate("location", "mm"))
        self.label_6.setText(_translate("location", "结束点"))
        self.Z_goal_spinBox.setSuffix(_translate("location", "mm"))
        self.Z_confrim_Button.setText(_translate("location", "确定"))
        self.motor_enable_Button.setText(_translate("location", "电机使能"))
