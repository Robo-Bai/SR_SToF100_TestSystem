# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Label.ui'
#
# Created by: PyQt5 UI code generator 5.15.2
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Label(object):
    def setupUi(self, Label):
        Label.setObjectName("Label")
        Label.resize(450, 400)
        Label.setMinimumSize(QtCore.QSize(450, 400))
        Label.setMaximumSize(QtCore.QSize(450, 400))
        self.layoutWidget = QtWidgets.QWidget(Label)
        self.layoutWidget.setGeometry(QtCore.QRect(10, 120, 421, 136))
        self.layoutWidget.setObjectName("layoutWidget")
        self.gridLayout = QtWidgets.QGridLayout(self.layoutWidget)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.lineEdit_type = QtWidgets.QLineEdit(self.layoutWidget)
        self.lineEdit_type.setMinimumSize(QtCore.QSize(280, 40))
        self.lineEdit_type.setMaximumSize(QtCore.QSize(280, 40))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        self.lineEdit_type.setFont(font)
        self.lineEdit_type.setObjectName("lineEdit_type")
        self.gridLayout.addWidget(self.lineEdit_type, 0, 1, 1, 1)
        self.label_2 = QtWidgets.QLabel(self.layoutWidget)
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        self.label_2.setFont(font)
        self.label_2.setObjectName("label_2")
        self.gridLayout.addWidget(self.label_2, 2, 0, 1, 1)
        self.lineEdit_person = QtWidgets.QLineEdit(self.layoutWidget)
        self.lineEdit_person.setMinimumSize(QtCore.QSize(280, 40))
        self.lineEdit_person.setMaximumSize(QtCore.QSize(280, 40))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        self.lineEdit_person.setFont(font)
        self.lineEdit_person.setPlaceholderText("")
        self.lineEdit_person.setObjectName("lineEdit_person")
        self.gridLayout.addWidget(self.lineEdit_person, 2, 1, 1, 1)
        self.label = QtWidgets.QLabel(self.layoutWidget)
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.label_6 = QtWidgets.QLabel(self.layoutWidget)
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        self.label_6.setFont(font)
        self.label_6.setObjectName("label_6")
        self.gridLayout.addWidget(self.label_6, 1, 0, 1, 1)
        self.lineEdit_num = QtWidgets.QLineEdit(self.layoutWidget)
        self.lineEdit_num.setMinimumSize(QtCore.QSize(280, 40))
        self.lineEdit_num.setMaximumSize(QtCore.QSize(280, 40))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        self.lineEdit_num.setFont(font)
        self.lineEdit_num.setObjectName("lineEdit_num")
        self.gridLayout.addWidget(self.lineEdit_num, 1, 1, 1, 1)
        self.layoutWidget1 = QtWidgets.QWidget(Label)
        self.layoutWidget1.setGeometry(QtCore.QRect(110, 270, 218, 93))
        self.layoutWidget1.setObjectName("layoutWidget1")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.layoutWidget1)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.label_3 = QtWidgets.QLabel(self.layoutWidget1)
        self.label_3.setText("")
        self.label_3.setObjectName("label_3")
        self.horizontalLayout.addWidget(self.label_3)
        self.Ok_Button = QtWidgets.QPushButton(self.layoutWidget1)
        self.Ok_Button.setMinimumSize(QtCore.QSize(100, 40))
        self.Ok_Button.setMaximumSize(QtCore.QSize(100, 40))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        self.Ok_Button.setFont(font)
        self.Ok_Button.setObjectName("Ok_Button")
        self.horizontalLayout.addWidget(self.Ok_Button)
        self.label_4 = QtWidgets.QLabel(self.layoutWidget1)
        self.label_4.setText("")
        self.label_4.setObjectName("label_4")
        self.horizontalLayout.addWidget(self.label_4)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem)
        spacerItem1 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem1)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.layoutWidget2 = QtWidgets.QWidget(Label)
        self.layoutWidget2.setGeometry(QtCore.QRect(20, 20, 401, 89))
        self.layoutWidget2.setObjectName("layoutWidget2")
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout(self.layoutWidget2)
        self.horizontalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.label_5 = QtWidgets.QLabel(self.layoutWidget2)
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(10)
        self.label_5.setFont(font)
        self.label_5.setObjectName("label_5")
        self.horizontalLayout_3.addWidget(self.label_5)
        self.Label_textBrowser = QtWidgets.QTextBrowser(self.layoutWidget2)
        self.Label_textBrowser.setObjectName("Label_textBrowser")
        self.horizontalLayout_3.addWidget(self.Label_textBrowser)

        self.retranslateUi(Label)
        QtCore.QMetaObject.connectSlotsByName(Label)
        Label.setTabOrder(self.lineEdit_type, self.lineEdit_num)
        Label.setTabOrder(self.lineEdit_num, self.lineEdit_person)
        Label.setTabOrder(self.lineEdit_person, self.Ok_Button)
        Label.setTabOrder(self.Ok_Button, self.Label_textBrowser)

    def retranslateUi(self, Label):
        _translate = QtCore.QCoreApplication.translate
        Label.setWindowTitle(_translate("Label", "PDF登记"))
        self.lineEdit_type.setToolTip(_translate("Label", "ToF型号(例如：SToF100）"))
        self.label_2.setText(_translate("Label", "    测试人员："))
        self.lineEdit_person.setToolTip(_translate("Label", "测试者姓名(例如：Bai Jiawei或BJW）"))
        self.label.setText(_translate("Label", "  测试ToF型号："))
        self.label_6.setText(_translate("Label", "  测试ToF编号："))
        self.lineEdit_num.setToolTip(_translate("Label", "例如：1"))
        self.Ok_Button.setText(_translate("Label", "确认"))
        self.label_5.setText(_translate("Label", "当前状态"))
        self.Label_textBrowser.setPlaceholderText(_translate("Label", "请先输入本次报告【ToF型号】与【测试人员】的相关信息，再点击【确认】"))
