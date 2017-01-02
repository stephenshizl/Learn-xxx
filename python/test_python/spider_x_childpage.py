import urllib2
import re
import os
import urllib

#从子标签页中获取所有分类

cururl='http://image.baidu.com/channel?c=%E7%BE%8E%E5%A5%B3#%E7%BE%8E%E5%A5%B3'
download_dir="E:\\baidu_image"
con_index=[]
url_choose=''
index_choose=0

#parse picture item
def ParseIndex(item_con):
    re_desc=r"<li([^<]*?)><adata-nsclick=\"([^<]*?)\"([^<]*?)href=\"(?P<href>([^<]*?))\">(?P<desc>(.*?))</a></li>"
    item=re.compile(re_desc)
    items=item.finditer(item_con)
    for x in items:
        index={}
        index['href']=x.group('href').decode('utf-8')
        index['desc']=x.group('desc').decode('utf-8')
        con_index.append(index)
        #print unicode('href:%s desc:%s' %(x.group('href').decode('utf-8'),x.group('desc').decode('utf-8')))

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

items=re.compile(r"<div.?class=\"rowclearfixboxsec-con\"><div([^<]*?)><div([^<]*?)><div([^<]*?)>(<ul([^<]*?)>.*?</ul>)")
it=items.finditer(con)
for x in it:
    item_con=x.group()
    ParseIndex(item_con)
    #print(item_con)

ShowIndex(con_index)

GetUserChoose()

print('download ok !! -_-')
