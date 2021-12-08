# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> test_global
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/18 17:32
=================================================='''
import sys
def test_global():
    global CAN_init
    CAN_init = 1

def print_global():
    print(CAN_init)

if __name__ == "__main__":
    print_global()