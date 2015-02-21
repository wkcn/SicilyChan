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
                      }
    
    request = urllib2.Request(url = html,
                                                data = urllib.urlencode(postdata),
                                                headers = headers)
    wordHtml = urllib2.urlopen(request)
    return wordHtml.read().decode('utf-8')    
    
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


print BingDict('hello')
#http://cn.bing.com/dict/search?q=hello
