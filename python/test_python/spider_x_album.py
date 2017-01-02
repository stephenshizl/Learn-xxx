import urllib2
import re
import os
import urllib

#从相册中获取所有

cururl='http://image.baidu.com/detail/newindex?col=%E7%BE%8E%E5%A5%B3&tag=%E5%85%A8%E9%83%A8&pn=2&pid=9523264651&aid=&user_id=862713050&setid=-1&sort=0&newsPn=&fr=&from=1'
download_dir="E:\\baidu_image"

#parse picture item
def DownloadItem(item_con):
    re_desc=r'{(\"id\"\:\"(?P<id>.*?)\")([^{]*?)(\"owner\"\:{([^{]*?)})([^{]*?)(\"imageUrl\"\:\"(?P<url>.*?)\")([^{]*?)(\"title\"\:\"(?P<title>.*?)\")([^{]*?)},'
    item=re.compile(re_desc)
    items=item.finditer(item_con)
    for x in items:    
        print unicode('id:%s title:%s url:%s' %(x.group('id').decode('utf-8'),x.group('title').decode('utf-8'),x.group('url').decode('utf-8')))
        url=x.group('url')
        name='%s\\%s_%s.jpg' %(download_dir,x.group('title'),x.group('id'))
        urllib.urlretrieve(url, name.decode('utf-8'))

#dir is exist
if os.path.isdir(download_dir):
    #print('%s exist' %(download_dir))
    download_dir
else:
    print('%s no exist, so we made it ' %(download_dir))
    os.mkdir(download_dir)

#main
url = urllib2.urlopen(cururl)
con = url.read()

nospace = re.compile("(\t| |\n)")
con=nospace.sub('', con)

items=re.compile(r"app\.appendData\(.albumData.,{(\"coverId\")([^{]*?)(?P<pic>({([^{]*?){([^{]*?)}([^{]*?)},)+){}\]}\);.?}\);")
it=items.finditer(con)
for x in it:
    item_con=x.group('pic')
    DownloadItem(item_con)

print('download ok !! -_-')

