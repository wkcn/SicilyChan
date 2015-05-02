# -*- coding: utf-8 -*-
#Sicily Chan's Translate Module
#import goslate
import MyDict
from math import *

#gs = goslate.Goslate()	

def Translate(text, target_language="zh", source_language='en'):
    Code = False

    if text[0] == '!':
        Code = True
        text = text[1:]
        
    if Code:
        try:
            res = eval(text)
            return str(res)
        except:
            pass

    ans = MyDict.FindWord(text)
    return ans

#print Translate("hello I'm sicily","zh","en")
#print Translate('1+2')

  #  try:
   #     if target_language == 'en':
    #        return gs.translate(text,target_language).encode("utf-8")
   ## except:
      #  return ""
