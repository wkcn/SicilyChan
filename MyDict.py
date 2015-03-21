# -*- coding: utf-8 -*-
import xlrd
import sys

reload(sys)
sys.setdefaultencoding('utf-8')
#import sys
#reload(sys)
#sys.path.append('./')
#sys.setdefaultencoding('utf-8')

dictData = range(26)
#f = open('dictData.txt','w')
def LoadData():
    data = xlrd.open_workbook('dictData.xls')
    for i in range(26):
        table = data.sheet_by_index(i)
        nrows = table.nrows
        ncols = table.ncols
        dictData[i] = {}
        for r in range(1,nrows):
            eng = table.cell_value(r,0)
            tr  = table.cell_value(r,1)
            #f.write(str(eng)+ '=' + str(tr)+ '\n')
            dictData[i][str(eng)] = str(tr)

LoadData()
def FindWord(word):
    word = word.strip(" !@#$%^&*()-_+={}[]|:;'<>?,./\"").lower()
    id = ord(word[0]) - ord('a')
    r = ''
    for i in range(len(word)):
        r = dictData[id].get(word,'')
        if r != '':
            break
        word = word[:-1]

    r = r.replace('|','\n')
    r = word + '\n' + r
    if (r == ''):
        return ''
    return r.encode('utf-8')

#print FindWord("table")
