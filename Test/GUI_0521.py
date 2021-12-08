# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> GUI_0521
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/21 10:02
=================================================='''


import sys
from PyQt5.QtWidgets import QApplication, QMainWindow
#导入CAN-USB驱动函数
from Test.PCAN_USB_0521 import DriveCAN, global_value
#对应父窗口py文件：SR-SToF100-test-system/Ui/MainUi.py
from Ui.MainUi import Ui_MainGUI
#对应子窗口py文件：SR-SToF100-test-system/Ui/MotorControl.py
from Ui.MotorControl import Ui_MotorControl
from threading import Thread

#主窗口
class Main_Window(QMainWindow, Ui_MainGUI):

    def __init__(self, parent=None):
        super(Main_Window, self).__init__()
        self.setupUi(self)
        #【子窗口--电机控制】在父窗口中的定义
        self.MotorControl_Window = MotorControl_Window()

        #按钮【手动调试】打开子窗口--电机控制
        self.Manual_Operation_Button.clicked.connect(self.OpenMotorControl)

        #按钮【SH200调试】开启SH200测距
        self.SH200_enable_Button.clicked.connect(self.SH200_enable)

        #标题栏【驱动】打开PCAN-USB驱动
        self.actionPCAN_USB.triggered.connect(self.SToF_CAN_init)

        #开始检测
        self.Test_start.clicked.connect(self.CAN_dataRead)

        #三轴回原点
        self.Origin_Button.clicked.connect(self.Return_Origin)

        #回原点判断
        self.Return_Origin_judge_Button.clicked.connect(self.Return_Origin_judge)



    #方法：打开子窗口--电机控制
    def OpenMotorControl(self):
        self.MotorControl_Window.show()

    # 方法：开启SH200测距
    def SH200_enable(self):
        DriveCAN().can_SH200_open(send_data=[0x2F, 0x01, 0x20, 0x01, 0x01, 0x00, 0x00, 0x00])

        def thread_SH200_func():
            # for num in range(2):
                DriveCAN().can_read_SH200()

        thread_SH200_main = Thread(target=thread_SH200_func,
                              args=())
        thread_SH200_main.start()
        # thread_SH200_main.join()
        self.MainUi_textBrowser.append("SH200已手动【开启】")


    # 方法：初始化CAN驱动
    def SToF_CAN_init(self):
        DriveCAN().can_init()
        DriveCAN().can_filter()
        self.CAN_init_judge()

########################################################################
#                     三轴回原点                                        #
########################################################################
    #方法：三轴回原点
    def Return_Origin(self):
        self.SToF_CAN_init()
        if global_value.CAN_init_flag == 1:
            self.XZR_axle_enable()
            self.X_Return_Origin()
            self.Z_Return_Origin()
            self.R_Return_Origin()
        elif global_value.CAN_init_flag == 0:
            self.MainUi_textBrowser.append("PCAN-USB初始化【失败】，请重新打开PCAN-USB驱动或检查线路连接")

    def Return_Origin_judge(self):
        #检查是否已回原点
        DriveCAN().can_knicoX_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
        DriveCAN().can_knicoZ_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
        DriveCAN().can_knicoR_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
        DriveCAN().CAN_read()
        self.MainUi_textBrowser.append("【绿灯】闪烁【三次】说明三轴均已经回原点")


    #方法：使能XZR三轴
    def XZR_axle_enable(self):
        DriveCAN().can_knicoX_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])
        DriveCAN().can_knicoZ_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])
        DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])

    #方法：X轴回原点
    def X_Return_Origin(self):
        #选择模式6
        DriveCAN().can_knicoX_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])
        #启动回原点
        DriveCAN().can_knicoX_write_manual(send_data=[0x2F, 0x40, 0x60, 0x00, 0x1F, 0x00, 0x00, 0x00])
    #方法：Z轴回原点
    def Z_Return_Origin(self):
        #选择模式6
        DriveCAN().can_knicoZ_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])
        #启动回原点
        DriveCAN().can_knicoZ_write_manual(send_data=[0x2F, 0x40, 0x60, 0x00, 0x1F, 0x00, 0x00, 0x00])
    #方法：X轴回原点
    def R_Return_Origin(self):
        #选择模式6
        DriveCAN().can_knicoR_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])
        #启动回原点
        DriveCAN().can_knicoR_write_manual(send_data=[0x2F, 0x40, 0x60, 0x00, 0x1F, 0x00, 0x00, 0x00])

    #方法：CAN驱动判断
    def CAN_init_judge(self):
        if global_value.CAN_init_flag == 1:
            self.MainUi_textBrowser.append("PCAN-USB驱动初始化【成功】")
        else:
            self.MainUi_textBrowser.append("PCAN-USB驱动初始化【失败】，请重新打开PCAN-USB驱动或检查线路连接")
#舍弃不用
    # def CAN_data_read_test(self):
    #     # self.SToF_CAN_init()
    #     DriveCAN().CAN_read()

    # def system_open(self):
    def CAN_dataRead(self):
        self.XZR_axle_enable()


    # def R_motor_active(self):







# class MainUi_function():
#     def CAN_dataRead(self):
#         DriveCAN().CAN_read()

#子窗口--电机控制
class MotorControl_Window(QMainWindow, Ui_MotorControl):

    def __init__(self):
        super(MotorControl_Window, self).__init__()
        self.setupUi(self)

        #使能X轴电机
        self.X_kinco_enable_button.clicked.connect(self.X_kinco_enable)
        #X轴电机速度向后
        self.X_kinco_speed_button_retreat.clicked.connect(self.X_kinco_speed_retreat)
        #X轴电机速度向前
        self.X_kinco_speed_button_forward.clicked.connect(self.X_kinco_speed_forward)
        #关闭X轴电机使能
        self.X_kinco_disable_button.clicked.connect(self.X_kinco_disable)

        #使能Z轴电机
        self.Z_kinco_enable_button.clicked.connect(self.Z_kinco_enable)
        #X轴电机速度向后
        self.Z_kinco_speed_button_down.clicked.connect(self.Z_kinco_speed_down)
        #X轴电机速度向前
        self.Z_kinco_speed_button_up.clicked.connect(self.Z_kinco_speed_up)
        #关闭X轴电机使能
        self.Z_kinco_disable_button.clicked.connect(self.Z_kinco_disable)

        #使能R轴电机
        self.R_kinco_enable_button.clicked.connect(self.R_kinco_enable)
        #X轴电机速度向后
        self.R_kinco_speed_button_right.clicked.connect(self.R_kinco_speed_right)
        #X轴电机速度向前
        self.R_kinco_speed_button_left.clicked.connect(self.R_kinco_speed_left)
        #关闭X轴电机使能
        self.R_kinco_disable_button.clicked.connect(self.R_kinco_disable)

    ##################################################################################
    #                             手动调试--X轴电机动作                                #
    ##################################################################################
    # global SToF_CAN_init_flag
    def X_kinco_enable(self):
        '''
        初始化CAN驱动同时，使能电机
        :return: none
        '''
        #检查PCAN-USB驱动是否连接
        if global_value.CAN_init_flag == 1:
            print("PCAN-USB已连接")
            DriveCAN().can_knicoX_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])
            self.textBrowser.append("X轴电机已使能")
        else:
            self.textBrowser.append("PCAN-USB未连接，请在主窗口【驱动】中打开【PCAN-USB驱动】重试")


    def X_kinco_speed_retreat(self):
        '''设置X轴电机速度--向后

        设置工作模式为速度模式
        读取SpinBox_retreat控件的数值，分别为 10rpm、20rpm、30rpm、40rpm、50rpm
        :return:
        '''
        #设置工作模式为速度模式
        DriveCAN().can_knicoX_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00])

        if self.X_spinBox_retreat.value() == 0:#读取spinBox控件的数值
            #速度为0rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
            self.textBrowser.append("X轴后退的速度为0rpm")
        elif self.X_spinBox_retreat.value() == 10:
            # 速度为10rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x0C, 0xBB, 0x02, 0x00])
            self.textBrowser.append("X轴后退的速度为10rpm")
        elif self.X_spinBox_retreat.value() == 20:
            # 速度为20rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x19, 0x76, 0x05, 0x00])
            self.textBrowser.append("X轴后退的速度为20rpm")
        elif self.X_spinBox_retreat.value() == 30:
            # 速度为30rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x26, 0x31, 0x08, 0x00])
            self.textBrowser.append("X轴后退的速度为30rpm")
        elif self.X_spinBox_retreat.value() == 40:
            # 速度为40rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x33, 0xEC, 0x0A, 0x00])
            self.textBrowser.append("X轴后退的速度为40rpm")
        elif self.X_spinBox_retreat.value() == 50:
            # 速度为50rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x40, 0xA7, 0x0D, 0x00])
            self.textBrowser.append("X轴后退的速度为50rpm")
        else:
            self.textBrowser.append("速度设置错误！请设置为10~50rpm！")

    def X_kinco_speed_forward(self):
        '''设置X轴电机速度--向前

        设置工作模式为速度模式
        读取SpinBox_forward控件的数值，分别为 10rpm、20rpm、30rpm、40rpm、50rpm
        :return:
        '''
        #设置工作模式为速度模式
        DriveCAN().can_knicoX_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00])

        if self.X_spinBox_forward.value() == 0:
            #速度为0rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
            self.textBrowser.append("X轴向前的速度为0rpm")
        elif self.X_spinBox_forward.value() == 10:
            # 速度为10rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xF4, 0x44, 0xFD, 0xFF])
            self.textBrowser.append("X轴向前的速度为10rpm")
        elif self.X_spinBox_forward.value() == 20:
            # 速度为20rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xE7, 0x89, 0xFA, 0xFF])
            self.textBrowser.append("X轴前进的速度为20rpm")
        elif self.X_spinBox_forward.value() == 30:
            # 速度为30rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xDA, 0xCE, 0xF7, 0xFF])
            self.textBrowser.append("X轴前进的速度为30rpm")
        elif self.X_spinBox_forward.value() == 40:
            # 速度为40rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xCD, 0x13, 0xF5, 0xFF])
            self.textBrowser.append("X轴前进的速度为40rpm")
        elif self.X_spinBox_forward.value() == 50:
            # 速度为50rpm
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xC0, 0x58, 0xF2, 0xFF])
            self.textBrowser.append("X轴前进的速度为50rpm")
        else:
            self.textBrowser.append("速度设置错误！请设置为0~50rpm！")

    #关闭X轴使能
    def X_kinco_disable(self):
        DriveCAN().can_knicoX_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])
        self.textBrowser.append("X轴电机已关闭")

##################################################################################
#                             手动调试--Z轴电机动作                                #
##################################################################################

    def Z_kinco_enable(self):
        '''
        初始化CAN驱动同时，使能电机
        :return: none
        '''
        #检查PCAN-USB驱动是否连接
        if global_value.CAN_init_flag == 1:
            print("PCAN-USB已连接")
            DriveCAN().can_knicoZ_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])
            self.textBrowser.append("Z轴电机已使能")
        else:
            self.textBrowser.append("PCAN-USB未连接，请在主窗口【驱动】中打开【PCAN-USB驱动】重试")

    def Z_kinco_speed_up(self):
        '''设置Z轴电机速度--向上

        设置工作模式为速度模式
        读取SpinBox_retreat控件的数值，分别为 10rpm、20rpm、30rpm、40rpm、50rpm
        :return:
        '''
        #设置工作模式为速度模式
        DriveCAN().can_knicoZ_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00])

        if self.Z_spinBox_up.value() == 0:#读取spinBox控件的数值
            #速度为0rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
            self.textBrowser.append("Z轴向上的速度为0rpm")
        elif self.Z_spinBox_up.value() == 10:
            # 速度为10rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x0C, 0xBB, 0x02, 0x00])
            self.textBrowser.append("Z轴向上的速度为10rpm")
        elif self.Z_spinBox_up.value() == 20:
            # 速度为20rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x19, 0x76, 0x05, 0x00])
            self.textBrowser.append("Z轴向上的速度为20rpm")
        elif self.Z_spinBox_up.value() == 30:
            # 速度为30rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x26, 0x31, 0x08, 0x00])
            self.textBrowser.append("Z轴向上的速度为30rpm")
        elif self.Z_spinBox_up.value() == 40:
            # 速度为40rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x33, 0xEC, 0x0A, 0x00])
            self.textBrowser.append("Z轴向上的速度为40rpm")
        elif self.Z_spinBox_up.value() == 50:
            # 速度为50rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x40, 0xA7, 0x0D, 0x00])
            self.textBrowser.append("Z轴向上的速度为50rpm")
        else:
            self.textBrowser.append("速度设置错误！请设置为10~50rpm！")

    def Z_kinco_speed_down(self):
        '''设置Z轴电机速度--向下

        设置工作模式为速度模式
        读取SpinBox_forward控件的数值，分别为 10rpm、20rpm、30rpm、40rpm、50rpm
        :return:
        '''
        #设置工作模式为速度模式
        DriveCAN().can_knicoZ_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00])

        if self.Z_spinBox_down.value() == 0:
            #速度为0rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
            self.textBrowser.append("Z轴向下的速度为0rpm")
        elif self.Z_spinBox_down.value() == 10:
            # 速度为10rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xF4, 0x44, 0xFD, 0xFF])
            self.textBrowser.append("Z轴向下的速度为10rpm")
        elif self.Z_spinBox_down.value() == 20:
            # 速度为20rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xE7, 0x89, 0xFA, 0xFF])
            self.textBrowser.append("Z轴向下的速度为20rpm")
        elif self.Z_spinBox_down.value() == 30:
            # 速度为30rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xDA, 0xCE, 0xF7, 0xFF])
            self.textBrowser.append("Z轴向下的速度为30rpm")
        elif self.Z_spinBox_down.value() == 40:
            # 速度为40rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xCD, 0x13, 0xF5, 0xFF])
            self.textBrowser.append("Z轴向下的速度为40rpm")
        elif self.Z_spinBox_down.value() == 50:
            # 速度为50rpm
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xC0, 0x58, 0xF2, 0xFF])
            self.textBrowser.append("Z轴向下的速度为50rpm")
        else:
            self.textBrowser.append("速度设置错误！请设置为0~50rpm！")

    #关闭Z轴使能
    def Z_kinco_disable(self):
        DriveCAN().can_knicoZ_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])
        self.textBrowser.append("Z轴电机已关闭")

##################################################################################
#                          手动调试--R轴电机动作                                   #
##################################################################################

    def R_kinco_enable(self):
        '''
        初始化CAN驱动同时，使能电机
        :return: none
        '''
        #检查PCAN-USB驱动是否连接
        if global_value.CAN_init_flag == 1:
            print("PCAN-USB已连接")
            DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])
            self.textBrowser.append("R轴电机已使能")
        else:
            self.textBrowser.append("PCAN-USB未连接，请在主窗口【驱动】中打开【PCAN-USB驱动】重试")

    def R_kinco_speed_right(self):
        '''设置R轴电机速度--向右

        设置工作模式为速度模式
        读取SpinBox_retreat控件的数值，分别为 10rpm、20rpm、30rpm、40rpm、50rpm
        :return:
        '''
        #设置工作模式为速度模式
        DriveCAN().can_knicoR_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00])

        if self.R_spinBox_right.value() == 0:#读取spinBox控件的数值
            #速度为0rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
            self.textBrowser.append("R轴向右的速度为0rpm")
        elif self.R_spinBox_right.value() == 10:
            # 速度为10rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x0C, 0xBB, 0x02, 0x00])
            self.textBrowser.append("R轴向右的速度为10rpm")
        elif self.R_spinBox_right.value() == 20:
            # 速度为20rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x19, 0x76, 0x05, 0x00])
            self.textBrowser.append("R轴向右的速度为20rpm")
        elif self.R_spinBox_right.value() == 30:
            # 速度为30rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x26, 0x31, 0x08, 0x00])
            self.textBrowser.append("R轴向右的速度为30rpm")
        elif self.R_spinBox_right.value() == 40:
            # 速度为40rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x33, 0xEC, 0x0A, 0x00])
            self.textBrowser.append("R轴向右的速度为40rpm")
        elif self.R_spinBox_right.value() == 50:
            # 速度为50rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x40, 0xA7, 0x0D, 0x00])
            self.textBrowser.append("R轴向右的速度为50rpm")
        else:
            self.textBrowser.append("速度设置错误！请设置为10~50rpm！")

    def R_kinco_speed_left(self):
        '''设置D轴电机速度--向左

        设置工作模式为速度模式
        读取SpinBox_forward控件的数值，分别为 10rpm、20rpm、30rpm、40rpm、50rpm
        :return:
        '''
        #设置工作模式为速度模式
        DriveCAN().can_knicoR_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00])

        if self.R_spinBox_left.value() == 0:
            #速度为0rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
            self.textBrowser.append("R轴向左的速度为0rpm")
        elif self.R_spinBox_left.value() == 10:
            # 速度为10rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xF4, 0x44, 0xFD, 0xFF])
            self.textBrowser.append("R轴向左的速度为10rpm")
        elif self.R_spinBox_left.value() == 20:
            # 速度为20rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xE7, 0x89, 0xFA, 0xFF])
            self.textBrowser.append("R轴向左的速度为20rpm")
        elif self.R_spinBox_left.value() == 30:
            # 速度为30rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xDA, 0xCE, 0xF7, 0xFF])
            self.textBrowser.append("R轴向左的速度为30rpm")
        elif self.R_spinBox_left.value() == 40:
            # 速度为40rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xCD, 0x13, 0xF5, 0xFF])
            self.textBrowser.append("R轴向左的速度为40rpm")
        elif self.R_spinBox_left.value() == 50:
            # 速度为50rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0xFF, 0x60, 0x00, 0xC0, 0x58, 0xF2, 0xFF])
            self.textBrowser.append("R轴向左的速度为50rpm")
        else:
            self.textBrowser.append("速度设置错误！请设置为0~50rpm！")

    #关闭Z轴使能
    def R_kinco_disable(self):
        DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])
        self.textBrowser.append("R轴电机已关闭")


# #初始化CAN驱动
# def SToF_CAN_init():
#     DriveCAN().can_init()
#     DriveCAN().can_filter()




#窗口运行函数
if __name__ == "__main__":
    app = QApplication(sys.argv)
    mainWindow = Main_Window()
    mainWindow.show()
    sys.exit(app.exec_())