from common import *
from urlrequest import *
import urllib.request
import urllib.parse
import re
import json
import time

def parseLoginResponse(data):
    errorcode = data['r']
    if 0 != errorcode:
        print(g_tagE,data['msg'])
    else:
        print(g_tagS, 'login ok : user-',user)
    return (0==errorcode)

def hasCaptcha(data):
    n = data.find('<div class=\"captcha-container\">')
    return n > -1

def getCaptchaImage(opener, path):
    captcha_url = 'http://www.zhihu.com/captcha.gif?r='
    captcha_url = captcha_url + str(int(time.time()*1000))
    captcha_url = captcha_url + '&type=login'
    data = opener.open(captcha_url)
    #print(data)
    write2File(data, path)

class ZHSession(object):
    def __init__(self, opener, xsrf):
        self._opener = opener
        self._xsrf = xsrf
    def _getOpener(self):
        return self._opener

class Login(object):    
    _header = {
        'Host': 'www.zhihu.com',
        'Connection': 'keep-alive',
        'Upgrade-Insecure-Requests': ' 1',
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36',
        'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
        'Accept-Encoding': 'gzip, deflate, sdch',
        'Accept-Language': 'zh-CN,zh;q=0.8'
    }

    def __init__(self, url, user, pwd):
        self._user = user
        self._pwd  = pwd #decodeB64(pwd)
        self._url  = url
        self._opener = UrlRequest(self._header)
        self._xsrf = self.getXSRF()
    
    def getXSRF(self):
        data = self._opener.open(self._url)
        cer = re.compile('name=\"_xsrf\" value=\"(.*)\"')
        strlist = cer.findall(data)
        return strlist[0]

    def getCaptcha(self):
        path_captcha = getCurAbsPath('captchaCode.gif')
        getCaptchaImage(self._opener, path_captcha)
        showImage(path_captcha)
        captcha = input('input --- captcha: ')
        return captcha

    def makeDict(self):       
        postDict = {
            '_xsrf':self._xsrf,
            'email':self._user,
            'password':self._pwd,
            'remember_me':'true'
        }
        captcha = self.getCaptcha()
        postDict['captcha'] = captcha
        return postDict
                       
    def _in(self):
        urlLogin = self._url + 'login/email'
        postDict = self.makeDict()
        postData = urllib.parse.urlencode(postDict)
        postData = postData.encode('utf-8')
        #print(postData, url, opener.addheaders)
        try:
            data = self._opener.open(urlLogin, postData)
        except:
            print(g_tagE, urlLogin, ' failed')
            return None
        respDict = json.loads(data)
        #print(respDict)
        success = parseLoginResponse(respDict)
        if not success:
            return None
        return ZHSession(self._opener, self._xsrf)

    def _out(self):
        urlLogout = self._url + 'logout'
        try:
            data = self._opener.open(urlLogout)
        except:
            print(g_tagE, urlLogout, 'failed')
            return None 
        #print(data)

#login = Login(urlRoot, user, password)
#session = login._in()
