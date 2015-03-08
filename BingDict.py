# -*- coding: utf-8 -*-

import urllib,urllib2
from bs4 import BeautifulSoup
#import sys

#inputDecode = sys.stdin.encoding
#outputDecode = sys.stdout.encoding

def getHtml(word):
    html = 'http://cn.bing.com/dict/search?&q=' + word
    postdata = {'qs':'bs', 'form':'CM'}
    headers = {
                      'User-Agent':'Mozilla/5.0 (Windows NT 6.3; WOW64; rv:35.0) Gecko/20100101 Firefox/35.0',
                      'Host':'cn.bing.com',
                      'Referer':'http://cn.bing.com/dict/search',
                      'Accept':'*/*',
                      'Accept-Language':'zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3',
                      'Cookie':'SRCHUID=V=2&GUID=9119740BFE5C469A9E78CC585AB25723; MUID=26597751AE2C6F781FE271C2AA2C6EE2; SRCHD=SM=1&D=3477526&MS=3477526&AF=IE8SSC; SRCHUSR=AUTOREDIR=0&GEOVAR=&DOB=20140811; _EDGE_V=1; SRCHHPGUSR=CW=1903&CH=293&DPR=1; _U=1mG_1C0ZcQwdxz4xTam4UAL7IEJmpg3nC-EkBrCppu_5sqBdi4sFXY_LFQkAlG-JTYo0c6fevyXXiIpdKGKJeXsM6k0Xcc_Ckv107TdSi83H7fYlPtE08zQ7brAQI5Usf; ANON=A=CF6857FF5F4EFA5B5B51CB2CFFFFFFFF&E=ff1&W=3; NAP=V=1.9&E=f97&C=bFj3iZj4yzBnRBCbeZRoLnp7vypRY_35aYU86WBeerWl_P2JSyG2zg&W=3; s_vnum=1410447513415%26vn%3D1; s_nr=1407855539358; MUIDB=26597751AE2C6F781FE271C2AA2C6EE2; SRCHHPGUSR=CW=1534&CH=802&DPR=1.25; _EDGE_E=O=muidflt40-qfn&O=muidflt39-qfn; _RwBf=s=70&o=16; _HOP=; _SS=SID=75F6F482B66F4EF0810236F758878361&bIm=289289; _EDGE_S=SID=0F37FCDD0AC66BC6169AFB8B0B676A20; SCRHDN=ASD=0&DURL=#; WLS=TS=63560162506; RMS=F=GCAAI'
                      }
    try:
        request = urllib2.Request(url = html,
                                                    data = urllib.urlencode(postdata),
                                                    headers = headers)
        wordHtml = urllib2.urlopen(request,timeout=5)
        return wordHtml.read().decode('utf-8')    
    except:
        return ""
    
def BingDict(word):
    try:
        soup = BeautifulSoup(getHtml(word));
        text = soup.find_all('span',{'class','def'})
        ty = soup.find_all('span',{'class','pos'})
        #res = []
        #for i in range(len(text)):
        #    res.append((text[i].string,ty[i].string))
        #return res
        ans = ""
        first = True
        for i in range(len(text)):
            if not first:
                ans += '\n'
            first = False
            ans += ty[i].string+' '+text[i].string
        #print type(ans)
        return ans.encode('utf-8')#.decode('utf-8')
        #for u in BingDict.BingDict(text):
         #   if not first:
         #       ans += '\n'
         #   first = False
        #    ans += u[1] + "  " + u[0]
            
        #.encode("utf-8")
    except:
        return ""


#print BingDict('hello')
#http://cn.bing.com/dict/search?q=hello
