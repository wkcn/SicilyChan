# -*- coding: utf-8 -*-
#Sicily Chan's Translate Module
import goslate
import MyDict

gs = goslate.Goslate()	

def Translate(text, target_language="zh", source_language=''):
    Code = False
    
    for c in ['+','-','*','/','^']:
        if c in text:
            Code = True
            text = text.replace('^','**')
            break
            
    if Code:
        try:
            res = eval(text)
            return str(res)
        except:
            pass
            
    try:
        if target_language == 'en':
            return gs.translate(text,target_language).encode("utf-8")
    except:
        return ""
        
    ans = MyDict.FindWord(text)
    try:
    	if ans == "":
        	ans = gs.translate(text,target_language).encode("utf-8")
    except:
        pass

    return ans
    
#print Translate("hello I'm sicily","zh","en")
#print Translate('1+2')
