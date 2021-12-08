# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> test_timer
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/19 18:05
=================================================='''
import time

def timer_1():
    print("start: %s" % time.ctime())
    time.sleep( 0.5 )
    print("End: %s" % time.ctime())

if __name__ == "__main__":
    timer_1()