# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> GUI_0602
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/6/2 9:46
=================================================='''
# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> GUI_0601_pause
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/6/1 17:44
=================================================='''
# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> GUI_0601_1426
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/6/1 14:26
=================================================='''
####################################################################
#                             CAN类                                #
####################################################################
import drivers.pcan.PCANBasic
from drivers.pcan.PCANBasic import *
from Main_code.CSV_write import *
from functools import reduce #将列表里的数字合并成一个数字
from threading import Thread
import matplotlib.pyplot as plt
import numpy as np

# SH200_work_flag = 0
class global_value():
    CAN_init_flag = 0
    SH200_work_flag = 0
    stop_flag = 0
    resume_flag = 0
    DATA_distance = 0


class DriveCAN(PCANBasic):
    """PCAN的类

    继承自PCANBasic类
    """

    def can_init(self,
                 chanel=PCAN_USBBUS1,
                 bautrate=PCAN_BAUD_500K):  # 默认参数：通道、波特率
        """CAN的初始化函数

        热插拔CAN的初始化
        """
        # 初始化可热插拔的CAN
        result = self.Initialize(chanel, bautrate)
        if result != PCAN_ERROR_OK:
            # 错误发生，显示错误信息
            result = self.GetErrorText(result)
            print(result[1])
            # Main_Window.Main_textBrowser.append(str(result[1]))
        else:
            global_value.CAN_init_flag = 1
            print("PCAN-USB (Ch-1) 初始化成功")
            # mainwindow.ChildDialog.textBrowser.append("PCAN-USB (Ch-1) 初始化成功")

    def can_filter(self,
                   chanel=PCAN_USBBUS1,
                   from_id=0, to_id=0x7FF,
                   fileter_mode=PCAN_MODE_STANDARD):  # 默认参数：通道，起始id，结束id，滤波器模式（标准）
        """CAN滤波器配置函数
        """
        #  先关闭CAN滤波器，以确保接受新设的ID
        result = self.SetValue(PCAN_USBBUS1, PCAN_MESSAGE_FILTER, PCAN_FILTER_CLOSE)
        if result != PCAN_ERROR_OK:
            result = self.GetErrorText(result)
            print(result[1])
            # mainwindow.ChildDialog.textBrowser.append(str(result[1]))
        else:
            # 设定滤波器的ID范围，滤波模式，CAN通道
            result = self.FilterMessages(chanel, from_id, to_id, fileter_mode)
            if result != PCAN_ERROR_OK:
                result = self.GetErrorText(result)
                print(result[1])
                # mainwindow.ChildDialog.textBrowser.append(result[1])
            else:
                print("CAN滤波器成功配置，ID范围从 %d 到 %d" % (from_id, to_id))
                # mainwindow.ChildDialog.textBrowser.append("CAN滤波器成功配置，ID范围从 %d 到 %d" % (from_id, to_id))

    def can_read_SH200(self, chanel=PCAN_USBBUS1):  # 默认参数：通道
        '''
        SH200发回的CAN报文接收函数
        :param chanel:
        :return:
        '''
        # while True:
            # 所有初始化的通道被释放
        def thread_SH200_DATA_read():
            # global SH200_work_flag
            Polar_angle = np.linspace((2*np.pi)/7.2, (2*np.pi)/2.769, 320, endpoint=True)
            num = 0
            while global_value.SH200_work_flag == 1:
                # for i in range(400):
                    # num = num + 1
                    readResult = PCAN_ERROR_OK,
                    if (readResult[0] & PCAN_ERROR_QRCVEMPTY) != PCAN_ERROR_QRCVEMPTY:
                        # 确保接受队列可以接受新的信息
                            readResult = self.Read(chanel)
                            if readResult[0] != PCAN_ERROR_QRCVEMPTY:
                                #过滤返回的CAN报文，只取有数据的一组
                                if readResult[1].ID == 0x511:
                                    if readResult[1].DATA[0] == 0x05:
                                        distance = (readResult[1].DATA[2] << 8) | (readResult[1].DATA[1])
                                        signal_strength = (readResult[1].DATA[4] << 8) | (readResult[1].DATA[3])
                                        if distance != 0x96:
                                            num = num + 1
                                            print("num = %s" %(num))
                                            # signal_strength = (readResult[1].DATA[4]<<8)|(readResult[1].DATA[3])
                                            fill_strength(num, signal_strength)
                                            if signal_strength > 0x1388:
                                                fill_distance_strength(Polar_angle[num], distance, signal_strength)
                                                print("num = %s  极角 = %s  Distance = [%s]  信号强度 = %s  十六进制为 = %x"
                                                        % (num, Polar_angle[num],
                                                        distance, signal_strength, signal_strength))
                                            else:
                                                pass
                                        else:
                                            num = num + 1
                                            print("num = %s" % (num))
                                            fill_strength(num, signal_strength)
                                    else:
                                        pass
                                else:
                                    pass
                            else:
                                # 错误发生，显示信息
                                result = self.GetErrorText(readResult[0])
                    else:
                        pass

        thread_SH200 = Thread(target=thread_SH200_DATA_read,
                              args=())
        thread_SH200.start()
        # thread_SH200.join()

##########################################################################################
#    CAN读取数据函数
##########################################################################################
    def CAN_read(self, chanel=PCAN_USBBUS1):  # 默认参数：通道
        '''
        SH200发回的CAN报文接收函数
        :param chanel:
        :return:
        '''
        # 所有初始化的通道被释放
        global readResult
        # global X_Origin_flag
        # global Z_Origin_flag
        # global R_Origin_flag
        readResult = PCAN_ERROR_OK,
        if (readResult[0] & PCAN_ERROR_QRCVEMPTY) != PCAN_ERROR_QRCVEMPTY:
            # 确保接受队列可以接受新的信息
            readResult = self.Read(chanel)
            if readResult[0] != PCAN_ERROR_QRCVEMPTY:
                self.creat_thread()
                # if readResult[1].ID == 0x581 and readResult[1].DATA[4] == 0x37 and readResult[1].DATA[5] == 0xd6:
                #     global_value.X_Origin_flag = 1
                #     # Z_Origin_flag = 1
                #     # R_Origin_flag = 1
                #     print("id = %x data=[%s %s %s %s %s %s %s %s]"
                #               % (readResult[1].ID,
                #                   readResult[1].DATA[0],
                #                   readResult[1].DATA[1],
                #                   readResult[1].DATA[2],
                #                   readResult[1].DATA[3],
                #                   readResult[1].DATA[4],
                #                   readResult[1].DATA[5],
                #                   readResult[1].DATA[6],
                #                   readResult[1].DATA[7]))
                #     print("X_Origin_flag = %x" % (global_value.X_Origin_flag))
                # elif readResult[1].ID == 0x582 and readResult[1].DATA[4] == 0x37 and readResult[1].DATA[5] == 0xd6:
                #     global_value.Z_Origin_flag = 1
                #     print("Z_Origin_flag = %x" % (global_value.Z_Origin_flag))
                # elif readResult[1].ID == 0x583 and readResult[1].DATA[4] == 0x37 and readResult[1].DATA[5] == 0xd6:
                #     global_value.R_Origin_flag = 1
                #     print("R_Origin_flag = %x" % (global_value.R_Origin_flag))
            else:
                # 错误发生，显示信息
                result = self.GetErrorText(readResult[0])
        else:
            pass

    def thread_X_CANread(self):
        global readResult
        if readResult[1].ID == 0x581 and readResult[1].DATA[4] == 0x37 and readResult[1].DATA[5] == 0xd6:
            global_value.X_Origin_flag = 1
            print("id = %x data=[%s %s %s %s %s %s %s %s]"
                  % (readResult[1].ID,
                     readResult[1].DATA[0],
                     readResult[1].DATA[1],
                     readResult[1].DATA[2],
                     readResult[1].DATA[3],
                     readResult[1].DATA[4],
                     readResult[1].DATA[5],
                     readResult[1].DATA[6],
                     readResult[1].DATA[7]))
            print("X_Origin_flag = %x" % (global_value.X_Origin_flag))
        else:
            pass
    def thread_Z_CANread(self):
        global readResult
        if readResult[1].ID == 0x582 and readResult[1].DATA[4] == 0x37 and readResult[1].DATA[5] == 0xd6:
            global_value.Z_Origin_flag = 1
            print("Z_Origin_flag = %x" % (global_value.Z_Origin_flag))
    def thread_R_CANread(self):
        global readResult
        if readResult[1].ID == 0x583 and readResult[1].DATA[4] == 0x37 and readResult[1].DATA[5] == 0xd6:
            global_value.R_Origin_flag = 1
            print("R_Origin_flag = %x" % (global_value.R_Origin_flag))

    def creat_thread(self):
        X_thread = Thread(target=self.thread_X_CANread,
                          args=())
        X_thread.start()

        Z_thread = Thread(target=self.thread_Z_CANread,
                          args=())
        Z_thread.start()

        R_thread = Thread(target=self.thread_R_CANread,
                          args=())
        R_thread.start()

    #使能X轴电机
    def can_knicoX_write_manual(self,
                  chanel=PCAN_USBBUS1,
                  msg_type=PCAN_MESSAGE_STANDARD,
                  frame_id = 0x601,
                  send_data=[]):  # 默认参数：通道，帧类型（标准），帧id，发送数据（列表）
        """CAN的写数据函数
        """
        msg = TPCANMsg()
        msg.ID = frame_id
        msg.MSGTYPE = PCAN_MESSAGE_STANDARD
        msg.LEN = len(send_data)
        for i in range(0, msg.LEN):  # 从0到len-1,如果发送数据有8位，则是0到7
            msg.DATA[i] = send_data[i]

        # 发送数据
        result = self.Write(chanel, msg)
        can_kinco_flag = True
        if result != PCAN_ERROR_OK:
            # 错误发生，显示错误信息
            result = self.GetErrorText(result)
            print(result)
            # mainwindow.ChildDialog.textBrowser.append(result)
        else:
            print("CAN数据发送成功")
            # mainwindow.ChildDialog.textBrowser.append("CAN数据发送成功")

    #使能Z轴电机
    def can_knicoZ_write_manual(self,
                  chanel=PCAN_USBBUS1,
                  msg_type=PCAN_MESSAGE_STANDARD,
                  frame_id = 0x602,
                  send_data=[]):  # 默认参数：通道，帧类型（标准），帧id，发送数据（列表）
        """CAN的写数据函数
        """
        msg = TPCANMsg()
        msg.ID = frame_id
        msg.MSGTYPE = PCAN_MESSAGE_STANDARD
        msg.LEN = len(send_data)
        for i in range(0, msg.LEN):  # 从0到len-1,如果发送数据有8位，则是0到7
            msg.DATA[i] = send_data[i]

        # 发送数据
        result = self.Write(chanel, msg)
        can_kinco_flag = True
        if result != PCAN_ERROR_OK:
            # 错误发生，显示错误信息
            result = self.GetErrorText(result)
            print(result)
            # mainwindow.ChildDialog.textBrowser.append(result)
        else:
            print("CAN数据发送成功")
            # mainwindow.ChildDialog.textBrowser.append("CAN数据发送成功")

    #使能R轴电机
    def can_knicoR_write_manual(self,
                  chanel=PCAN_USBBUS1,
                  msg_type=PCAN_MESSAGE_STANDARD,
                  frame_id = 0x603,
                  send_data=[]):  # 默认参数：通道，帧类型（标准），帧id，发送数据（列表）
        """CAN的写数据函数
        """
        msg = TPCANMsg()
        msg.ID = frame_id
        msg.MSGTYPE = PCAN_MESSAGE_STANDARD
        msg.LEN = len(send_data)
        for i in range(0, msg.LEN):  # 从0到len-1,如果发送数据有8位，则是0到7
            msg.DATA[i] = send_data[i]

        # 发送数据
        result = self.Write(chanel, msg)
        can_kinco_flag = True
        if result != PCAN_ERROR_OK:
            # 错误发生，显示错误信息
            result = self.GetErrorText(result)
            print(result)
            # mainwindow.ChildDialog.textBrowser.append(result)
        else:
            print("CAN数据发送成功")
            # mainwindow.ChildDialog.textBrowser.append("CAN数据发送成功")

    #打开SH200
    def can_SH200_write(self,
                  chanel=PCAN_USBBUS1,
                  msg_type=PCAN_MESSAGE_STANDARD,
                  frame_id = 0x411,
                  send_data=[]):  # 默认参数：通道，帧类型（标准），帧id，发送数据（列表）
        """CAN的写数据函数
        """
        msg = TPCANMsg()
        msg.ID = frame_id
        msg.MSGTYPE = PCAN_MESSAGE_STANDARD
        msg.LEN = len(send_data)
        for i in range(0, msg.LEN):  # 从0到len-1,如果发送数据有8位，则是0到7
            msg.DATA[i] = send_data[i]

        # 发送数据
        result = self.Write(chanel, msg)
        can_kinco_flag = True
        if result != PCAN_ERROR_OK:
            # 错误发生，显示错误信息
            result = self.GetErrorText(result)
            print(result)
            # mainwindow.ChildDialog.textBrowser.append(result)
        else:
            print("CAN数据发送成功")
            # mainwindow.ChildDialog.textBrowser.append("CAN数据发送成功")

####################################################################
#                             CAN任务                              #
####################################################################
# def runcan():
#     """CAN线程执行的任务函数
#
#     """
#     global objPCAN
#     global can_init_flag
#
#
# if __name__ == "__main__":
#     objPCAN = DriveCAN()
#     objPCAN.can_init()
#     objPCAN.can_filter()
#     # can_init_flag = True
#     runcan()



###################################################################
#                             USB2SPI类                           #
###################################################################
from ctypes import *
import platform
from time import sleep
from drivers.usb2xxx.usb_device import *
from drivers.usb2xxx.usb2spi import *
from sys import *


class DriveUSB2SPI():
    """USB2SPI类
    实现配置、写数据、读数据、写读数据，都是主机模式
    """

    def __init__(self,
                 config_mode=SPI_MODE_SOFT_HDX,
                 config_master=SPI_MASTER,
                 config_cpol=0,
                 config_cpha=0,
                 config_lsbfirst=SPI_MSB,
                 config_selpolarity=SPI_SEL_LOW,
                 config_clockspeedhz=500000):

        """构造函数
        扫描可用的SPI，配置并开启，注意这个函数只能打开一个SPI
        """

        self.DevIndex = 0  # 关键参数，设备的ID，默认只有一个设备连接，因此赋值为0
        self.DevHandles = (c_uint * 20)()
        # 扫描设备
        self.ret = USB_ScanDevice(byref(self.DevHandles))
        if (self.ret == 0):
            print("没有SPI设备连接")
            # mainwindow.ChildDialog.textBrowser.append("没有SPI设备连接")
        else:
            print("有 %d SPI设备连接!" % self.ret)
            # mainwindow.ChildDialog.textBrowser.append("有 %d SPI设备连接!" % self.ret)
            # 打开设备
            self.ret = USB_OpenDevice(self.DevHandles[self.DevIndex])
            if (bool(self.ret) == False):
                print("打开SPI设备失败")
                # mainwindow.ChildDialog.textBrowser.append("打开SPI设备失败")
            else:
                print("成功打开SPI设备")
                # mainwindow.ChildDialog.textBrowser.append("成功打开SPI设备")
                # 获取设备信息
                USB2XXXInfo = DEVICE_INFO()
                USB2XXXFunctionString = (c_char * 256)()
                self.ret = DEV_GetDeviceInfo(self.DevHandles[self.DevIndex], byref(USB2XXXInfo),
                                             byref(USB2XXXFunctionString))
                if (bool(self.ret) == False):
                    print("Get device infomation faild!")
                    # mainwindow.ChildDialog.textBrowser.append("Get device infomation faild!")
                else:
                    print("USB2XXX device infomation:")
                    # mainwindow.ChildDialog.textBrowser.append("Get USB2XXX device infomation")
                    print("--Firmware Name: %s" % bytes(USB2XXXInfo.FirmwareName).decode('ascii'))
                    print("--Firmware Version: v%d.%d.%d" % (
                    (USB2XXXInfo.FirmwareVersion >> 24) & 0xFF, (USB2XXXInfo.FirmwareVersion >> 16) & 0xFF,
                    USB2XXXInfo.FirmwareVersion & 0xFFFF))
                    print("--Hardware Version: v%d.%d.%d" % (
                    (USB2XXXInfo.HardwareVersion >> 24) & 0xFF, (USB2XXXInfo.HardwareVersion >> 16) & 0xFF,
                    USB2XXXInfo.HardwareVersion & 0xFFFF))
                    print("--Build Date: %s" % bytes(USB2XXXInfo.BuildDate).decode('ascii'))
                    print("--Serial Number: ", end='')
                    for i in range(0, len(USB2XXXInfo.SerialNumber)):
                        print("%08X" % USB2XXXInfo.SerialNumber[i], end='')
                    print("")
                    print("--Function String: %s" % bytes(USB2XXXFunctionString.value).decode('ascii'))

                    # 配置初始化SPI
                    SPIConfig = SPI_CONFIG()
                    SPIConfig.Mode = config_mode  # 硬件半双工模式
                    SPIConfig.Master = config_master  # 主机模式
                    SPIConfig.CPOL = config_cpol
                    SPIConfig.CPHA = config_cpha
                    SPIConfig.LSBFirst = config_lsbfirst
                    SPIConfig.SelPolarity = config_selpolarity
                    SPIConfig.ClockSpeedHz = config_clockspeedhz
                    self.ret = SPI_Init(self.DevHandles[self.DevIndex], SPI2_CS0, byref(SPIConfig))
                    if (self.ret != SPI_SUCCESS):
                        print("初始化SPI失败")
                        # mainwindow.ChildDialog.textBrowser.append("初始化SPI失败")
                    else:
                        print("成功初始化SPI")
                        # mainwindow.ChildDialog.textBrowser.append("成功初始化SPI")

    def spi_master_read(self):
        """SPI主机读取函数

        读取数据并print
        """
        self.ReadBuffer = (c_ubyte * 16)()
        self.ret = SPI_ReadBytes(self.DevHandles[self.DevIndex], SPI2_CS0, byref(self.ReadBuffer), len(self.ReadBuffer))
        if (self.ret != SPI_SUCCESS):
            print("SPI读取数据失败")
            # mainwindow.ChildDialog.textBrowser.append("SPI读取数据失败")
        else:
            print("SPI 读取的数据为:")
            # mainwindow.ChildDialog.textBrowser.append("SPI 读取的数据为:")
            for i in range(0, len(self.ReadBuffer)):
                print("%02X " % self.ReadBuffer[i], end='')
                # mainwindow.ChildDialog.textBrowser.append("%02X " % self.ReadBuffer[i])
            print("")
            # mainwindow.ChildDialog.textBrowser.append(" ")

    def spi_master_write(self):
        """SPI主机数据发送函数
        """
        self.WriteBuffer = (c_ubyte * 16)()
        for i in range(0, len(self.WriteBuffer)):
            self.WriteBuffer[i] = i
        self.ret = SPI_WriteBytes(self.DevHandles[self.DevIndex], SPI2_CS0, byref(self.WriteBuffer),
                                  len(self.WriteBuffer))
        if (self.ret != SPI_SUCCESS):
            print("SPI发送数据失败")
            # mainwindow.ChildDialog.textBrowser.append("SPI发送数据失败")
        else:
            print("SPI发送数据成功")
            # mainwindow.ChildDialog.textBrowser.append("SPI发送数据成功")

    def spi_master_write_and_read(self):
        """SPI写读函数
        """
        self.ret = SPI_WriteReadBytes(self.DevHandles[self.DevIndex], SPI2_CS0, byref(self.WriteBuffer),
                                      len(self.WriteBuffer), byref(self.ReadBuffer), len(self.ReadBuffer), 10)
        if (self.ret != SPI_SUCCESS):
            print("SPI 写读数据失败")
            # mainwindow.ChildDialog.textBrowser.append("SPI 写读数据失败")
        else:
            print("SPI 写读的数据为:")
            # mainwindow.ChildDialog.textBrowser.append("SPI 写读的数据为:")
            for i in range(0, len(self.ReadBuffer)):
                print("%02X " % self.ReadBuffer[i], end='')
                # mainwindow.ChildDialog.textBrowser.append("%02X " % self.ReadBuffer[i])
            print("")
            # mainwindow.ChildDialog.textBrowser.append("")

    def spi_close(self):
        """SPI关闭函数
        """
        self.ret = USB_CloseDevice(self.DevHandles[self.DevIndex])
        DLL_FreeLib()  # 释放dll资源，关键
        if (bool(self.ret)):
            print("成功关闭SPI设备")
            # mainwindow.ChildDialog.textBrowser.append("成功关闭SPI设备")
        else:
            print("无法关闭SPI设备")
            # mainwindow.ChildDialog.textBrowser.append("无法关闭SPI设备")

####################################################################################################################################################

# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> GUI_0524
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/24 10:16
=================================================='''

import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5 import uic, QtGui, QtWidgets, QtCore
#对应父窗口py文件：./Ui/MainUi.py
from Ui.MainUi import Ui_MainGUI
#对应子窗口py文件：./Ui/MotorControl.py
from Ui.MotorControl import Ui_MotorControl
import time
from threading import Thread
from Main_code.CSV_write import *
from Main_code.CSV_Plot import *

#读取图片
path_image = "../Matplot_data/Distance_image.png"
strength_image_path = "../Matplot_data/Strenght_image.png"

#主窗口
class Main_Window(QMainWindow, Ui_MainGUI):
    # valueChange = pyqtSignal(int)
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
        self.Test_start.clicked.connect(self.Test_Start_func)

        #三轴回原点
        self.Origin_Button.clicked.connect(self.Return_Origin)

        #回原点判断
        self.Return_Origin_judge_Button.clicked.connect(self.SH200_close)

        #生成PDF文件
        self.PDF_Button.clicked.connect(PDF_result.pdf_result)

        #暂停测试
        self.Pause_Button.clicked.connect(self.Stop_active)


        self.step = 0


    #方法：打开子窗口--电机控制
    def OpenMotorControl(self):
        self.MotorControl_Window.show()

    # 方法：开启SH200测距
    def SH200_enable(self):
        global_value.SH200_work_flag = 1
        DriveCAN().can_SH200_write(send_data=[0x2F, 0x01, 0x20, 0x01, 0x01, 0x00, 0x00, 0x00])

        def thread_SH200_func():
            # for num in range(2):
                DriveCAN().can_read_SH200()

        thread_SH200_main = Thread(target=thread_SH200_func,
                              args=())
        thread_SH200_main.start()
        # thread_SH200_main.join()
        self.MainUi_textBrowser.append("SH200正在运行")

    def SH200_close(self):
        global_value.SH200_work_flag = 0
        DriveCAN().can_SH200_write(send_data=[0x2F, 0x11, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00])
        DriveCAN().can_read_SH200()

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
            time.sleep(0.1)
            self.Z_Return_Origin()
            time.sleep(0.1)
            self.R_Return_Origin()
        elif global_value.CAN_init_flag == 0:
            self.MainUi_textBrowser.append("PCAN-USB初始化【失败】，请重新打开PCAN-USB驱动或检查线路连接")

    #EU100底层解决，故舍弃或改用
    def Return_Origin_judge(self):
        #检查是否已回原点
        DriveCAN().can_knicoX_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
        time.sleep(0.1)
        DriveCAN().can_knicoZ_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
        time.sleep(0.1)
        DriveCAN().can_knicoR_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
        DriveCAN().CAN_read()
        self.MainUi_textBrowser.append("【绿灯】闪烁【三次】说明三轴均已经回原点")


    #方法：使能XZR三轴
    def XZR_axle_enable(self):
        DriveCAN().can_knicoX_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])
        time.sleep(0.1)
        DriveCAN().can_knicoZ_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])
        time.sleep(0.1)
        DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00])

    #方法：X轴回原点
    def X_Return_Origin(self):
        #选择模式6
        DriveCAN().can_knicoX_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])
        time.sleep(0.1)
        #启动回原点
        DriveCAN().can_knicoX_write_manual(send_data=[0x2F, 0x40, 0x60, 0x00, 0x1F, 0x00, 0x00, 0x00])
    #方法：Z轴回原点
    def Z_Return_Origin(self):
        #选择模式6
        DriveCAN().can_knicoZ_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])
        time.sleep(0.1)
        #启动回原点
        DriveCAN().can_knicoZ_write_manual(send_data=[0x2F, 0x40, 0x60, 0x00, 0x1F, 0x00, 0x00, 0x00])
    #方法：X轴回原点
    def R_Return_Origin(self):
        #选择模式6
        DriveCAN().can_knicoR_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00])
        time.sleep(0.1)
        #启动回原点
        DriveCAN().can_knicoR_write_manual(send_data=[0x2F, 0x40, 0x60, 0x00, 0x1F, 0x00, 0x00, 0x00])

    #方法：CAN驱动判断
    def CAN_init_judge(self):
        if global_value.CAN_init_flag == 1:
            self.MainUi_textBrowser.append("PCAN-USB驱动初始化【成功】")
            # print("PCAN-USB驱动初始化【成功】")
        else:
            self.MainUi_textBrowser.append("PCAN-USB驱动初始化【失败】，请重新打开PCAN-USB驱动或检查线路连接")

#################################################################################
#                     正式测试动作                                               #
#################################################################################
    #方法：X轴正式测试动作
    def X_Test_active(self):
        '''
        X轴正式测试动作：
        1.选择工作模式为：1位置模式
        2.根据要求水平以1mm分辨率遍历，1747///      //修改10mm分辨率遍历，17476
        3.相对位置模式：0x4F->0x5F
        :return:
        '''
        def thread_X_Test_active():
            #选择位置模式
            DriveCAN().can_knicoX_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00])
            #设置目标位置每次移动1747inc，1mm
            time.sleep(0.05)
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0x7A, 0x60, 0x00, 0xBC, 0xBB, 0xFF, 0xFF])
            #设置梯形速度为50rpm
            time.sleep(0.05)
            DriveCAN().can_knicoX_write_manual(send_data=[0x23, 0x81, 0x60, 0x00, 0x40, 0xA7, 0x0D, 0x00])
            #0x4F->0x5F，X轴移动一次
            time.sleep(0.05)
            DriveCAN().can_knicoX_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x4F, 0x00, 0x00, 0x00])
            time.sleep(0.05)
            DriveCAN().can_knicoX_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x5F, 0x00, 0x00, 0x00])
            time.sleep(0.05)

        thread_X_test_func = Thread(target=thread_X_Test_active,
                              args=())
        thread_X_test_func.start()
        # thread_X_test_func.join()#等待X轴动作完成

    def Z_Test_active(self):
        '''
        Z轴正式测试动作：
        1.选择工作模式为：1位置模式
        2.根据要求水平以1mm分辨率遍历，1747///      //修改10mm分辨率遍历，17476
        3.相对位置模式：0x4F->0x5F
        :return:
        '''
        def thread_Z_Test_active():
            #选择位置模式
            DriveCAN().can_knicoZ_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00])
            #设置目标位置每次向下移动10mm --> -65536*2 inc
            time.sleep(0.05)
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0x7A, 0x60, 0x00, 0x00, 0x00, 0xFE, 0xFF])
            #设置梯形速度为50rpm
            time.sleep(0.05)
            DriveCAN().can_knicoZ_write_manual(send_data=[0x23, 0x81, 0x60, 0x00, 0x40, 0xA7, 0x0D, 0x00])
            #0x4F->0x5F，z轴移动一次
            time.sleep(0.05)
            DriveCAN().can_knicoZ_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x4F, 0x00, 0x00, 0x00])
            time.sleep(0.05)
            DriveCAN().can_knicoZ_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x5F, 0x00, 0x00, 0x00])
            time.sleep(0.05)

        thread_Z_test_func = Thread(target=thread_Z_Test_active,
                              args=())
        thread_Z_test_func.start()
        # thread_Z_test_func.join()#等待X轴动作完成

    def R_init_position(self):
        '''
        开始测试时，R轴到初始位置
        逆时针旋转40度
        等待R轴回初始位置完成
        :return:
        '''
        def thread_R_init_position():
            global R_init_flag
            #选择位置模式
            DriveCAN().can_knicoR_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00])
            time.sleep(0.1)
            #设置目标位置：40°,根据R轴电机减速比90:1，电机10圈-->10*65536=0xA 0000
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0x7A, 0x60, 0x00, 0x00, 0x00, 0x0A, 0x00])
            time.sleep(0.1)
            # 设置梯形速度为400rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0x81, 0x60, 0x00, 0x06, 0x3A, 0x6D, 0x00])
            time.sleep(0.1)
            # 0x4F->0x5F，R轴回初始位置
            DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x4F, 0x00, 0x00, 0x00])
            time.sleep(0.1)
            DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x5F, 0x00, 0x00, 0x00])
            time.sleep(2)
            # R_init_flag = 1

        thread_R_init_func = Thread(target=thread_R_init_position,
                                   args=())
        thread_R_init_func.start()
        thread_R_init_func.join()#等待R轴回初始位置完成

    def R_work(self):
        '''
        R轴开始工作：
        顺时针旋转80度,速度为200rpm
        :return:
        '''
        def thread_R_work():
            # 选择位置模式
            DriveCAN().can_knicoR_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00])
            time.sleep(0.05)
            # 设置目标位置：-40°,根据R轴电机减速比90:1，电机20圈-->20*65536=0x14 0000，取反
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0x7A, 0x60, 0x00, 0x00, 0x00, 0xEC, 0xFF])
            time.sleep(0.05)
            # 设置梯形速度为100rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0x81, 0x60, 0x00, 0x40, 0xA7, 0x0D, 0x00])
            time.sleep(0.05)
            # 0x4F->0x5F，执行
            DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x4F, 0x00, 0x00, 0x00])
            time.sleep(0.05)
            DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x5F, 0x00, 0x00, 0x00])
            time.sleep(5)
            # time.sleep(6)#延时6秒等待SH200读取数据


        thread_R_work_func = Thread(target=thread_R_work,
                                   args=())
        thread_R_work_func.start()
        # thread_R_work_func.join()

    def R_back(self):
        '''
        R轴返回初始位置
        逆时针旋转80度，速度为400rpm
        :return:
        '''
        def thread_R_back():
            # 选择位置模式
            DriveCAN().can_knicoR_write_manual(send_data=[0x2F, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00])
            time.sleep(0.05)
            # 设置目标位置：40°,根据R轴电机减速比90:1，电机20圈-->20*65536=0x14 0000
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0x7A, 0x60, 0x00, 0x00, 0x00, 0x14, 0x00])
            time.sleep(0.05)
            # 设置梯形速度为400rpm
            DriveCAN().can_knicoR_write_manual(send_data=[0x23, 0x81, 0x60, 0x00, 0x06, 0x3A, 0x6D, 0x00])
            time.sleep(0.05)
            # 0x4F->0x5F，R轴回初始位置
            DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x4F, 0x00, 0x00, 0x00])
            time.sleep(0.05)
            DriveCAN().can_knicoR_write_manual(send_data=[0x2B, 0x40, 0x60, 0x00, 0x5F, 0x00, 0x00, 0x00])
            time.sleep(0.05)

        thread_R_back_func = Thread(target=thread_R_back,
                                   args=())
        thread_R_back_func.start()
        # thread_R_back_func.join()

    def show_distance_image(self):
        '''
        打开图片
        0527-9:58
        :return:
        '''
        pixmap = QtGui.QPixmap(path_image)
        self.label.setPixmap(QtGui.QPixmap(path_image))
        # self.label.setScaledContents(True)
        # self.label.setObjectName("label")

    def show_strength_image(self):
        '''
        打开信号强度图片
        0528-11:57
        :return:
        '''
        strength_pixmap = QtGui.QPixmap(strength_image_path)
        self.label_2.setPixmap(QtGui.QPixmap(strength_image_path))

    def Sum_progress_run(self):
        '''
        单次扫描进度条
        :return:
        '''
        if self.step < 100:
            self.step = self.step + 1
            self.progressBar_2.setValue(self.step)
        else:
            self.MainUi_textBrowser.append("测试完毕，请查看PDF报告")

    def Sub_progress_run(self):
        '''
        单次扫描进度条
        :return:
        '''
        k = 0
        def progress_Thread(n):
            k = 0
            for k in range(101):
            # self.step = self.step + 1
                self.progressBar.setValue(k)
                time.sleep(0.3)

        progress_thread = Thread(target=progress_Thread,
                                 args=(6,))
        progress_thread.start()

    def Stop_active(self):
        global_value.stop_flag = 1
        self.MainUi_textBrowser.append("测试【终止】，请等待本次测试结束")

    def Test_Start_func(self):
        '''

        :return:
        '''
        global R_init_flag
        global_value.stop_flag = 0
        self.SToF_CAN_init()
        self.XZR_axle_enable()
        # def thread_init():
        self.R_init_position()#先让R轴到初始位置
        clear_csv_data()#清空距离和角度记录的数据
        # clear_fill_strength()#清空信号强度记录的数据
        time.sleep(0.5)
        # num_distance = 0
        def thread_work():
            if global_value.stop_flag == 0:
                num_distance = 0
                for vertical_num in range(45):
                    # 垂直遍历Z轴动作
                    if global_value.stop_flag == 0:
                        for num in range(1):
                            if global_value.stop_flag == 0:
                                #水平遍历X轴、R轴动作
                                # global_value.SH200_work_flag = 1
                                self.SH200_enable()  # 打开SH200采集数据

                                self.Sub_progress_run()#单次测试进度条

                                time.sleep(1.5)
                                self.R_work()  # R轴开始工作
                                time.sleep(26)
                                # global_value.SH200_work_flag = 0
                                self.SH200_close()  # 到顺时针极限位置后关闭SH200
                                time.sleep(1)
                                self.R_back()
                                # self.progress_run()

                                DrawDataplot()  # 绘图
                                self.show_distance_image()  # 显示图片
                                Strength_Image()
                                self.show_strength_image()
                                clear_fill_strength()  # 清空信号强度记录的数据
                                time.sleep(2)

                                self.X_Test_active()

                                time.sleep(0.5)
                                # print("第 %s 次"%(num,))
                                num_distance = num_distance + 10
                                global_value.DATA_distance = num_distance
                                print("当前距离为 %d" %(global_value.DATA_distance))

                                self.Sum_progress_run()#测试总进度条
                            else:
                                break
                    else:
                        break
                    self.Z_Test_active()#Z轴动作，向下遍历，分辨率为10mm
                    time.sleep(1)
            else:
                pass

        thread_work_main = Thread(target=thread_work,
                                  args=())
        thread_work_main.start()

#######################################################################################################
#                           生成PDF的类                                                                #
#######################################################################################################
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Image, PageBreak, Table, TableStyle
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.platypus import Image as reportImage
from reportlab.lib.units import mm
from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER, TA_LEFT
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
import time

pdfmetrics.registerFont(TTFont('pingbold', 'PingBold.ttf'))
pdfmetrics.registerFont(TTFont('ping', 'ping.ttf'))
pdfmetrics.registerFont(TTFont('hv', 'Helvetica.ttf'))

# stof100 = 1

# 生成PDF文件
class PDFGenerator:
    def __init__(self, filename):
        self.filename = filename
        self.file_path = ('./PDFs/PDF')
        self.title_style = ParagraphStyle(name="TitleStyle", fontName="pingbold", fontSize=48, alignment=TA_LEFT,)
        self.sub_title_style = ParagraphStyle(name="SubTitleStyle", fontName="hv", fontSize=32,
                                              textColor=colors.HexColor(0x666666), alignment=TA_LEFT, )
        self.content_style = ParagraphStyle(name="ContentStyle", fontName="ping", fontSize=18, leading=25, spaceAfter=20,
                                            underlineWidth=1, alignment=TA_LEFT, )
        self.foot_style = ParagraphStyle(name="FootStyle", fontName="ping", fontSize=14, textColor=colors.HexColor(0xB4B4B4),
                                         leading=25, spaceAfter=20, alignment=TA_CENTER, )
        self.table_title_style = ParagraphStyle(name="TableTitleStyle", fontName="pingbold", fontSize=20, leading=25,
                                                spaceAfter=10, alignment=TA_LEFT, )
        self.sub_table_style = ParagraphStyle(name="SubTableTitleStyle", fontName="ping", fontSize=16, leading=25,
                                                spaceAfter=10, alignment=TA_LEFT, )
        self.basic_style = TableStyle([('FONTNAME', (0, 0), (-1, -1), 'ping'),
                                       ('FONTSIZE', (0, 0), (-1, -1), 12),
                                       ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
                                       ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
                                       ('BOTTOMPADDING', (0, 0), (-1, -1), 6),
                                       # 'SPAN' (列,行)坐标
                                       ('SPAN', (1, 0), (3, 0)),
                                       ('SPAN', (1, 1), (3, 1)),
                                       ('SPAN', (1, 2), (3, 2)),
                                       ('SPAN', (1, 5), (3, 5)),
                                       ('SPAN', (1, 6), (3, 6)),
                                       ('SPAN', (1, 7), (3, 7)),
                                       ('GRID', (0, 0), (-1, -1), 0.5, colors.black),
                                       ])
        self.common_style = TableStyle([('FONTNAME', (0, 0), (-1, -1), 'ping'),
                                      ('FONTSIZE', (0, 0), (-1, -1), 12),
                                      ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
                                      ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
                                      ('BOTTOMPADDING', (0, 0), (-1, -1), 6),
                                      ('GRID', (0, 0), (-1, -1), 0.5, colors.black),
                                     ])

    def genTaskPDF(self, home_data, task_data, basic_data):
        story = []

        # 首页内容
        story.append(Spacer(1, 5 * mm))
        img = Image('../Icon/sr.jpg')
        img.drawHeight = 40 * mm
        img.drawWidth = 150 * mm
        img.hAlign = TA_LEFT
        story.append(img)
        story.append(Spacer(1, 20 * mm))
        story.append(Paragraph("测试报告", self.title_style))
        story.append(Spacer(1, 20 * mm))
        story.append(Paragraph("Test Report of SToF100", self.sub_title_style))
        story.append(Spacer(1, 50 * mm))
        story.append(Paragraph("报告编号：" + home_data['report_code'], self.content_style))
        story.append(Paragraph("计划名称：" + home_data['task_name'], self.content_style))
        story.append(Paragraph("报告日期：" + home_data['report_date'], self.content_style))
        story.append(Paragraph(" 负责人：" + home_data['report_creator'], self.content_style))
        story.append(Spacer(1, 30 * mm))
        story.append(Paragraph("内部文档，请勿外传", self.foot_style))
        story.append(PageBreak())

        # 表格允许单元格内容自动换行格式设置
        stylesheet = getSampleStyleSheet()
        body_style = stylesheet["BodyText"]
        body_style.wordWrap = 'CJK'
        body_style.fontName = 'ping'
        body_style.fontSize = 12

        # 测试计划
        story.append(Paragraph("SToF100测试结果", self.table_title_style))
        story.append(Spacer(1, 3 * mm))
        task_table = Table(task_data, colWidths=[25 * mm, 141 * mm], rowHeights=12 * mm, style=self.common_style)
        story.append(task_table)

        story.append(Spacer(1, 10 * mm))

        # 基础参数
        story.append(Paragraph("基础参数", self.sub_table_style))
        basic_table = Table(basic_data, colWidths=[25*mm, 61*mm, 25*mm, 55*mm], rowHeights=12 * mm, style=self.basic_style)
        story.append(basic_table)

        story.append(Spacer(1, 10 * mm))



        #插入图片
        story.append(Paragraph("SToF100测试结果图", self.table_title_style))
        story.append(Spacer(1, 10 * mm))
        img_uart_path = '../Matplot_data/Distance_image.png'
        img_uart = reportImage(img_uart_path)
        img_uart.drawHeight = 140 * mm
        img_uart.drawWidth = 150 * mm
        img_uart.hAlign = TA_LEFT
        story.append(img_uart)

        doc = SimpleDocTemplate(self.file_path + self.filename + ".pdf",
                                leftMargin=20 * mm, rightMargin=20 * mm, topMargin=20 * mm, bottomMargin=20 * mm)
        doc.build(story)


class PDF_result():
    def pdf_result():
        # location_data = MotorControl_Window()
        # location_distance = location_data.X_distance
        # 格式化成2016-03-20 11:45:39形式
        data = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        """主程序"""
        #获得的产品数据
        home_data = {'report_code': '1',
                   'task_name':'SToF100',
                   'report_date': data ,
                   'report_creator':'BJW'
                  }
        task_data = [
                        ['距离（cm）', global_value.DATA_distance],
                        ['X', 'location_distance'],
                        ['计划ID','1']
                    ]
        basic_data = [
                        ['测试ID','1'],
                        ['测试ID','2','测试','3']
                    ]

        #PDF类，读取产品数据并生成PDF
        test = PDFGenerator("_report")
        test.genTaskPDF(home_data,task_data,basic_data)




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
        # self.X_distance = self.X_spinBox_retreat.value()
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