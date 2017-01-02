import gzip
import base64
import os.path
import http.cookiejar
import urllib.request
from subprocess import Popen

g_tagE = '!!!error   : '
g_tagS = '!!!success : '
g_tagI = '!!!info    : '
g_tagW = '!!!warning : '
g_writefile_on = True

g_dbname = 'users.db'

urlRoot = 'https://www.zhihu.com/'
user = 'joyoushunter@yeah.net'

def ungzip(data):
    try:
        #print('ungzip ing ...')
        data = gzip.decompress(data)
        #print('ungzip ed ...')
    except:
        print('')
        #print('ungzip not work')
    return data

def decodeB64(data):
    data = data.encode(encoding='utf-8')
    rst = base64.b64decode(data)
    return rst

def encodeB64(data):
    data = data.encode(encoding='utf-8')
    rst = base64.b64encode(data)
    return rst

def getCurAbsPath(path):
    tmp = os.path.abspath(os.curdir)
    tmp = tmp + '/' + path
    return tmp

def writeLog2File(data, path='tmp.txt'):
    if not g_writefile_on:
        return
    write2File(data, path)
    
def write2File(data, path='tmp.txt'):
    #print(type(data))
    if isinstance(data, (str)):
        data = data.encode(encoding="utf-8")
        #print(g_tagI, 'data need write to ' + path + ' has been changed to bytes')
    f = open(path, 'wb+')
    f.write(data)
	
def readFile(path='tmp.txt'):
	if not os.path.exists(path):
		print(g_tagE, 'readFile error : ' + path + ' not exists')
		return ''
	f = open(path, 'rb')
	data = f.read()
	return data

def showImage(imgpath):
    path = ''
    if os.name == 'nt':
        path = ''
    elif os.name == 'posix':
        path = 'eog ' # ubuntu默认图片查看器是Eye of GNOME, 命令是"eog 图片名称"
    else :
        print(g_tagE, 'don\'t know current system')
        return None
    path = path + imgpath
    Popen(path, shell =True)
    return None

#########################################################################

class CreateUrlOperImpl(object):    
    @staticmethod 
    def create(head):
        cookjar = http.cookiejar.CookieJar()
        pro = urllib.request.HTTPCookieProcessor(cookjar)
        opener = urllib.request.build_opener(pro)
        header = []
        for key, value in head.items():
            elem = (key, value)
            header.append(elem)
        opener.addheaders = header
        return opener

    @staticmethod 
    def request(opener, url, postdata, header):
        if header:
            opener.addHeaders = header
        if None == postdata:
            ops = opener.open(url)
        else:
            ops = opener.open(url, postdata)
        data = ops.read()
        data = ungzip(data)
        try :
            tmp = data.decode()
            data = tmp
        except:
            None
            #print(g_tagE, 'openUrl() -> data.decode() failed')            
        return data

##import urllib.request
##class CreateUrlOperImplEx(object):
##    @staticmethod 
##    def create(head):
##        opener = requests.session()
##        if not head:
##            opener.header.update(head)
##        return opener
##    
##    @staticmethod 
##    def request(session, url, postdata, header):
##            if header:
##                session.header.update(header)
##            if None == postdata:
##                ops = session.get(url)
##            else:
##                ops = session.post(url, postdata)
##            data = ops.read()
##            data = ungzip(data)
##            data = data.decode()
##            return data

class UrlOperProxy(object):
    def __init__(self, defaultHeader=None):
        self._operImpl = CreateUrlOperImpl.create(defaultHeader)
        
    def openUrl(self, url, postdata=None, header=None):
        return CreateUrlOperImpl.request(self._operImpl, url, postdata, header)
    
#########################################################################
