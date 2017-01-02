import urllib2
import re
import os
import urllib

cururl='http://image.baidu.com'
download_dir="E:\\baidu_image"
con_index=[]
url_choose=''
index_choose=0

def WriteCon(path, con):
    fp=open(path, 'a+') 
    fp.write(con)
    fp.close

def ParseIndex(con):
    items=re.compile(r"<li([^<]*?)><ahref=\"(?P<href>((/|http)([^<]*?)))\"([^<]*?)>((<([^<]*?)></([^<]*?)>)*)(?P<desc>(.*?))</a></li>")
    it=items.finditer(con)
    for x in it:
        index={}
        index['href']=x.group('href').decode('utf-8')
        index['desc']=x.group('desc').decode('utf-8')
        con_index.append(index)
        #print unicode('href:%s desc:%s' %(x.group('href').decode('utf-8'),x.group('desc').decode('utf-8')))
        #print(index)

def ShowIndex(con_index_arg):
    cnt=len(con_index_arg)
    for i in range(cnt):
        print('%02d    %s' %(i, con_index_arg[i]['desc']))

def GetUserChoose():
    index = int(raw_input("\ninput your choose : "))
    if index < 0 or index > len(con_index):
        print(r'choose error!')
    else:
        index_choose=index
        url_choose=con_index[index]['href']        
        print(r'your choose is %d, desc:%s, href:%s'%(index, con_index[index]['desc'], url_choose))
        print(r'working...')
        print(r'')
    

###########################################################################

#main
url = urllib2.urlopen(cururl)
con = url.read()

nospace = re.compile("(\t| |\n|\r)")
con=nospace.sub('', con)
#print(con)

#WriteCon(r"C:\test.txt", con)

items=re.compile(r"<div.?class=\"mod-channel-navresp-channel-nav\"><div([^<]*?)><div([^<]*?)>(<ul([^<]*?)>.*?</ul>){2}</div></div></div>")
it=items.finditer(con)
for x in it:
    item_con=x.group()
    ParseIndex(item_con)

ShowIndex(con_index)

GetUserChoose()

if 0 == cmp(url_choose[0:8], '/channel'):
    url_choose=cururl+url_choose
else:
    url_choose
print('url is %s '% url_choose)

if index_choose==0:
    print('get things from homepage') #读取首页中的图片以及相册
else:
    print('get things from childpage') #跳转到子页面读取图片以及相册

print('parse ok !! -_-')
