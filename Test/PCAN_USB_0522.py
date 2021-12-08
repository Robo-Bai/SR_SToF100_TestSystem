# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> PCAN_USB_0522
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/22 9:42
=================================================='''

####################################################################
#                             CAN类                                #
####################################################################
import drivers.pcan.PCANBasic
from drivers.pcan.PCANBasic import *
from Test.test_csv import *
from functools import reduce #将列表里的数字合并成一个数字
from threading import Thread

class global_value():
    CAN_init_flag = 0


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
            while True:
                readResult = PCAN_ERROR_OK,
                if (readResult[0] & PCAN_ERROR_QRCVEMPTY) != PCAN_ERROR_QRCVEMPTY:
                    # 确保接受队列可以接受新的信息
                    readResult = self.Read(chanel)
                    if readResult[0] != PCAN_ERROR_QRCVEMPTY:
                        #过滤返回的CAN报文，只取有数据的一组
                        if readResult[1].ID == 0x511:
                            if readResult[1].DATA[0] == 0x60:
                                fillOperation(readResult[1].ID, readResult[1].DATA[2])
                                print("id = %x Distance = [%s]"
                                        % (readResult[1].ID,
                                           readResult[1].DATA[2]))
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