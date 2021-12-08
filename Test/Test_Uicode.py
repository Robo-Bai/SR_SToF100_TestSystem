# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> Test_Uicode
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/13 10:15
=================================================='''

'''=============测试GUI控电机(OK)==============='''

from PyQt5.QtWidgets import QApplication
from PyQt5 import uic
from Test.PCAN_USB_Fin import DriveCAN




class Stats:

    def __init__(self):
        # 动态从文件中加载UI定义
        self.ui = uic.loadUi("Test/Test-Ui/enable_driver.ui")

        self.ui.kinco_enable_button.clicked.connect(self.kinco_enable)
        self.ui.kinco_disable_button.clicked.connect(self.kinco_disable)


    def kinco_enable(self):
        DriveCAN().can_init()
        DriveCAN().can_filter()
        DriveCAN().can_knico_write_601(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])

    def kinco_disable(self):
        DriveCAN().can_knico_write_601(send_data=[0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])

class childimage:

    def __init__(self):
        # 动态从文件中加载UI定义
        self.childui = uic.loadUi("Test/Test-Ui/Image2Show.ui")


app = QApplication([])
stats = Stats()
stats.ui.show()
app.exec_()