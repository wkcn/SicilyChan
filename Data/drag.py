# -*- coding: utf-8 -*-
import math

def Drag(ax,ay,dragTime):
    #return ""
    #msg = str(ax) + " " +str(ay)
    #msg += " T " + str(dragTime)
    #return msg.encode("utf-8")
    msg = ''
    v = math.sqrt(ax * ax + ay * ay)
    if v > 800:
        msg = '● ﹏ ●'
    if msg == '':
        return ''
    return msg.encode("utf-8")
