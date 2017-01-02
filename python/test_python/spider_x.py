import re
import os
import urllib
import urllib2

class HTML_TOOL:
    # �÷� ̰��ģʽ ƥ�� \t ���� \n ���� �ո� ���� ������ ���� ͼƬ
    BgnCharToNoneRex=re.compile("(\t|\n| |<a.*?>|<img.*?>)")

    # �÷� ̰��ģʽ ƥ�� ����<>��ǩ 
    EndCharToNoneRex=re.compile("<.*?>")

     # �÷� ̰��ģʽ ƥ�� ����<p>��ǩ  
    BgnPartRex = re.compile("<p.*?>")  
    CharToNewLineRex = re.compile("(<br/>|</p>|<tr>|<div>|</div>)")  
    CharToNextTabRex = re.compile("<td>")

    # ��һЩhtml�ķ���ʵ��ת��Ϊԭʼ����  
    replaceTab = [("<","<"),(">",">"),("&","&"),("&","\""),(" "," ")]

    def Replace_Char(self,x):  
        x = self.BgnCharToNoneRex.sub("",x)  
        x = self.BgnPartRex.sub("\n    ",x)  
        x = self.CharToNewLineRex.sub("\n",x)  
        x = self.CharToNextTabRex.sub("\t",x)  
        x = self.EndCharToNoneRex.sub("",x)  
  
        for t in self.replaceTab:    
            x = x.replace(t[0],t[1])    
        return x    


download_dir="D:\\baidu_image"
if os.path.isdir(download_dir):
    #print('%s exist' %(download_dir))
    download_dir
else:
    print('%s no exist, so we made it ' %(download_dir))
    os.mkdir(download_dir)

url= urllib2.urlopen('http://image.baidu.com/channel?c=%E7%BE%8E%E5%A5%B3#%E7%BE%8E%E5%A5%B3')
con=url.read()

nospace = re.compile("(\t| |\n)")
con=nospace.sub('', con)
#print con+'\n'

items=re.compile(r"{(\"id\"\:\"(?P<id>.*?)\")([^{]*?)(\"tags\"\:\[\"(?P<tags>.*?)\"\])([^{]*?)(\"downloadUrl\"\:\"(?P<url>.*?)\")([^{]*?)(\"albumObjNum\"\:\"(?P<picnum>.*?)\")([^{]*?)},")
it=items.finditer(con)
for x in it:
    print unicode('id:%s tag:%s url:%s' %(x.group('id').decode('utf-8'),x.group('tags').decode('utf-8'),x.group('url').decode('utf-8')))
    url=x.group('url')
    name='%s\\%s_%s.jpg' %(download_dir,x.group('tags'),x.group('id'))
    urllib.urlretrieve(url, name.decode('utf-8'))

print('download ok !! -_-')

    
