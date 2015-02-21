# -*- coding: utf-8 -*-
#Sicily Chan's Translate Module
import GoogleTrans
import BingDict

#Finally,return .encode("utf-8")
#GoogleTrans def Translate(text, target_language="zh", source_language=''):
#return str
#BingDict def BingDict(word):
#return a list[mean,type]

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
            0
            #return ""
            
    #return GoogleTrans.Translate(text,target_language,source_language)
    
    if target_language == 'en':
        return GoogleTrans.Translate(text,target_language,source_language)
        
    #Judge whether is a single word
    isSencence = False
    firstWord = False
    wordBlank = False
    for u in text:
        if u != ' ':
            firstWord = True
            if wordBlank:
                isSencence = True
                break
        else:
            if firstWord:
                wordBlank = True
                
    if isSencence:
        return GoogleTrans.Translate(text,target_language,source_language)
    else:
        return BingDict.BingDict(text)
    
#print Translate("hello","zh","en")
#print Translate('1+2')