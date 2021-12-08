# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> PCAN-USB20210510
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/10 10:21
=================================================='''
####################################################################
#                             CAN类                                #
####################################################################
import drivers.pcan.PCANBasic
from drivers.pcan.PCANBasic import *
from Test.test_csv import *
from functools import reduce #将列表里的数字合并成一个数字
# from GUI import *

# class global_value():
#     CAN_init_flag = 0
#     X_Origin_flag = 0
#     Z_Origin_flag = 0
#     R_Origin_flag = 0

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
            CAN_init_flag = 1
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
        # 所有初始化的通道被释放
        readResult = PCAN_ERROR_OK,
        timer = 0
        if (readResult[0] & PCAN_ERROR_QRCVEMPTY) != PCAN_ERROR_QRCVEMPTY:
            # 确保接受队列可以接受新的信息
            readResult = self.Read(chanel)
            if readResult[0] != PCAN_ERROR_QRCVEMPTY:
                #过滤返回的CAN报文，只取有数据的一组
                if readResult[1].ID == 0x511:
                    if readResult[1].DATA[0] == 0x60:
                        # print("id = %x data=[%x %x %x %x %x %x %x %x]"
                        #       % (readResult[1].ID,
                        #           readResult[1].DATA[0],
                        #           readResult[1].DATA[1],
                        #           readResult[1].DATA[2],
                        #           readResult[1].DATA[3],
                        #           readResult[1].DATA[4],
                        #           readResult[1].DATA[5],
                        #           readResult[1].DATA[6],
                        #           readResult[1].DATA[7]))
                        # Distance = readResult[1].DATA[2]
                        # print("Distance = %s" %(Distance,))
                        # ID = readResult[1].ID
                        # print("ID = %x" %(ID,))
                        SH200_DATA = readResult[3]<<8|readResult[2]   #20210519-BJW
                        # fillOperation(readResult[1].ID, readResult[1].DATA[2])
                        fillOperation(readResult[1].ID, SH200_DATA)
                        print("id = %x Distance = [%x]"
                               %(readResult[1].ID,
                                 SH200_DATA))
                    else:
                        pass
                    # self.can_processmessage(readResult[1])
                else:
                    pass
            else:
                # 错误发生，显示信息
                result = self.GetErrorText(readResult[0])
        else:
            pass

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
        global X_Origin_flag
        readResult = PCAN_ERROR_OK,
        timer = 0
        if (readResult[0] & PCAN_ERROR_QRCVEMPTY) != PCAN_ERROR_QRCVEMPTY:
            # 确保接受队列可以接受新的信息
            readResult = self.Read(chanel)
            if readResult[0] != PCAN_ERROR_QRCVEMPTY:
                if readResult[1].ID == 0x581 and readResult[1].DATA[4] == 0x37 and readResult[1].DATA[5] == 0xd6:
                    X_Origin_flag = 1
                    # Z_Origin_flag = 1
                    # R_Origin_flag = 1
                    #     print("id = %x data=[%s %s %s %s %s %s %s %s]"
                    #           % (readResult[1].ID,
                    #               readResult[1].DATA[0],
                    #               readResult[1].DATA[1],
                    #               readResult[1].DATA[2],
                    #               readResult[1].DATA[3],
                    #               readResult[1].DATA[4],
                    #               readResult[1].DATA[5],
                    #               readResult[1].DATA[6],
                    #               readResult[1].DATA[7]))
                    #     print("X_Origin_flag = %x" % (X_Origin_flag))
                elif readResult[1].ID == 0x582 and readResult[1].DATA[4] == 0x37 and readResult[1].DATA[5] == 0xd6:
                    Z_Origin_flag = 1
                elif readResult[1].ID == 0x583 and readResult[1].DATA[4] == 0x37 and readResult[1].DATA[5] == 0xd6:
                    R_Origin_flag = 1
            else:
                # 错误发生，显示信息
                result = self.GetErrorText(readResult[0])
        else:
            pass

#######################################################################################

    # def can_processmessage(self, Result):
    #     """CAN接受数据后的处理函数
    #     """
    #     if Result.ID == 0x601:
    #         print("id = %x data=[%x %x %x %x %x %x %x %x]"
    #               % (Result.ID,
    #                  Result.DATA[0],
    #                  Result.DATA[1],
    #                  Result.DATA[2],
    #                  Result.DATA[3],
    #                  Result.DATA[4],
    #                  Result.DATA[5],
    #                  Result.DATA[6],
    #                  Result.DATA[7]))
    #     else:
    #         pass

    # def can_write(self,
    #               chanel=PCAN_USBBUS1,
    #               msg_type=PCAN_MESSAGE_STANDARD,
    #               frame_id=0x100,
    #               send_data=[1, 2, 3, 4]):  # 默认参数：通道，帧类型（标准），帧id，发送数据（列表）
    #     """CAN的写数据函数
    #     """
    #     msg = TPCANMsg()
    #     msg.ID = frame_id
    #     msg.MSGTYPE = PCAN_MESSAGE_STANDARD
    #     msg.LEN = len(send_data)
    #     for i in range(0, msg.LEN):  # 从0到len-1,如果发送数据有5位，则是0到4
    #         msg.DATA[i] = send_data[i]
    #
    #     # 发送数据
    #     result = self.Write(chanel, msg)
    #     if result != PCAN_ERROR_OK:
    #         # 错误发生，显示错误信息
    #         result = self.GetErrorText(result)
    #         print(result)
    #         # mainwindow.ChildDialog.textBrowser.append(result)
    #     else:
    #         print("CAN数据发送成功")
    #         # mainwindow.ChildDialog.textBrowser.append("CAN数据发送成功")

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
    def can_SH200_open(self,
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
def runcan():
    """CAN线程执行的任务函数

    """
    global objPCAN
    global can_init_flag
    # while can_kinco_flag:
    #     objPCAN.can_knico_write_601(send_data=[0x2B,0x40,0x60,0x00,0x0F,0x00,0x00,0x00])
    #     can_kinco_flag = False
        # fillOperation(readResult[1].ID, readResult[1].DATA[2])
    # try:
    #     objPCAN.Uninitialize(PCAN_NONEBUS)
    #     print("成功关闭了CAN口")
    #     # mainwindow.ChildDialog.textBrowser.append("成功关闭了CAN口")
    # except:
    #     print("已经关闭CAN口，不用重新关闭")
    #     # mainwindow.ChildDialog.textBrowser.append("已经关闭CAN口，不用重新关闭")

if __name__ == "__main__":
    objPCAN = DriveCAN()
    objPCAN.can_init()
    objPCAN.can_filter()
    can_init_flag = True
    runcan()



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


# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> GUI_0520
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/20 10:20
=================================================='''

import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QFileDialog
from PyQt5 import uic
#对应父窗口py文件：SR-SToF100-test-system/Ui/MainUi.py
from Ui.MainUi import Ui_MainGUI
#对应子窗口py文件：SR-SToF100-test-system/Ui/MotorControl.py
from Ui.MotorControl import Ui_MotorControl
import time

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

        #读CAN数据
        self.CAN_data_read_Button.clicked.connect(self.CAN_data_read_test)



    #方法：打开子窗口--电机控制
    def OpenMotorControl(self):
        self.MotorControl_Window.show()

    # 方法：开启SH200测距
    def SH200_enable(self):
        DriveCAN().can_SH200_open(send_data=[0x2F, 0x01, 0x20, 0x01, 0x01, 0x00, 0x00, 0x00])
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
            #检查是否已回原点
            flag = 0
            while flag == 0:
                time.sleep(1)
                DriveCAN().can_knicoX_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
                DriveCAN().can_knicoZ_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
                DriveCAN().can_knicoR_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])
                DriveCAN().CAN_read()
                if global_value.X_Origin_flag == 1:
                    flag = 1
                    print("global_value.X_Origin_flag = %x" % (global_value.X_Origin_flag))
        else:
            print("初始化失败！！！")
            # flag = 1
            # DriveCAN().CAN_read()
            # if global_value.X_Origin_flag == 1 and global_value.Z_Origin_flag == 1 and global_value.R_Origin_flag == 1:
            #     flag = 1
            #     self.MainUi_textBrowser.append("【X轴】已回原点")

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

    def CAN_data_read_test(self):
        # self.SToF_CAN_init()
        DriveCAN().CAN_read()

    # def system_open(self):
    def CAN_dataRead(self):
        self.Return_Origin()
        DriveCAN().can_knicoX_write_manual(send_data=[0x4B, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00])





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