# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> parameter_call
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/6/2 10:48
=================================================='''

class A:
    def __init__(self):
        a = B()
        num = a.start()
        print("B 中的参数：%s" %(num,))
        # print("B 中的参数：%s" % (b,))
        print("22")

class B:
    def start(self):
        b = 10
        return b


if __name__ == "__main__":
    A()