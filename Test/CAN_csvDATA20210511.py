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
            # mainwindow.ChildDialog.textBrowser.append(str(result[1]))
        else:
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

    def can_read(self, chanel=PCAN_USBBUS1):  # 默认参数：通道
        # 所有初始化的通道被释放
        readResult = PCAN_ERROR_OK,
        timer = 0
        if (readResult[0] & PCAN_ERROR_QRCVEMPTY) != PCAN_ERROR_QRCVEMPTY:
            # 确保接受队列可以接受新的信息
            readResult = self.Read(chanel)
            if readResult[0] != PCAN_ERROR_QRCVEMPTY:
                # print(readResult[1].DATA[0])
                #过滤返回的CAN报文，只取有数据的一组
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
                    fillOperation(readResult[1].ID, readResult[1].DATA[2])
                    print("id = %x Distance = [%x]"
                           %(readResult[1].ID,
                             readResult[1].DATA[2]))
                else:
                    pass
                # self.can_processmessage(readResult[1])
            else:
                # 错误发生，显示信息
                result = self.GetErrorText(readResult[0])
        else:
            pass

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

    def can_write(self,
                  chanel=PCAN_USBBUS1,
                  msg_type=PCAN_MESSAGE_STANDARD,
                  frame_id=0x100,
                  send_data=[1, 2, 3, 4]):  # 默认参数：通道，帧类型（标准），帧id，发送数据（列表）
        """CAN的写数据函数
        """
        msg = TPCANMsg()
        msg.ID = frame_id
        msg.MSGTYPE = PCAN_MESSAGE_STANDARD
        msg.LEN = len(send_data)
        for i in range(0, msg.LEN):  # 从0到len-1,如果发送数据有5位，则是0到4
            msg.DATA[i] = send_data[i]

        # 发送数据
        result = self.Write(chanel, msg)
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

    如果线程标识位为True，就调用读取函数不断读取数据，否则尝试关闭CAN
    """
    global objPCAN
    global can_thread_destroy_flag
    while can_thread_destroy_flag:
        objPCAN.can_read()
        # fillOperation(readResult[1].ID, readResult[1].DATA[2])
    try:
        objPCAN.Uninitialize(PCAN_NONEBUS)
        print("成功关闭了CAN口")
        # mainwindow.ChildDialog.textBrowser.append("成功关闭了CAN口")
    except:
        print("已经关闭CAN口，不用重新关闭")
        # mainwindow.ChildDialog.textBrowser.append("已经关闭CAN口，不用重新关闭")

if __name__ == "__main__":
    can_thread_destroy_flag = True
    objPCAN = DriveCAN()
    objPCAN.can_init()
    objPCAN.can_filter()
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