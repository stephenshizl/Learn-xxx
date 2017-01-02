from common import *
from datastruct import *
import re
import json

class AnsParse(object):
    def __init__(self):
        return

    def parseBaseinfo(self, data, user=UserInfo()):
        rel = re.compile('<script type=\"text\/json\" class=\"json-inline\" data-name=\"current_user\">\[.*\]<\/script>')
        rstList = rel.findall(data)
        rst = rstList[0]
        rel = re.compile('\[.*\]')
        rstList = rel.findall(rst)
        rst = rstList[0]
        rst = rst.strip('[]')
        rst = rst.replace('"', '')
        items = rst.split(',')
        user.setName(items[0])
        user.setUID(items[1])
        user.setSay(items[4])
        user.setEmail(items[8])
        hash_id = items[3]
        return user, hash_id
        
    def parseHomepage(self, data):
        writeLog2File(data,'parseHomepage.txt')
        user, hash_id = self.parseBaseinfo(data)
        return user._uid

    ## 泪奔： 正则表达式的模式部分，如果用()括起来，则findall的结果只有模式串代表的值，否则是规则匹配到的整个字符串
    ## re.compile('<span class=\"employment item\" title=.*?>(.*?)</span>') ： 返回 (.*?) 表示的内容
    ## re.compile('<span class=\"employment item\" title=.*?>.*?</span>') ： 返回匹配到的整个串
    def parsePersonPage(self, data, user):
        writeLog2File(data,'parsePersonPage.txt')
        rel = re.compile('<div class=\"title-section\">.*?<span class=\"name\">.*?</span><a class=\"icon-badge-wrapper\" .*?></a><div class=\"bio ellipsis\" title=.*?>(.*?)</div>.*?</div>', re.S)
        rstList = rel.findall(data)
        if not rstList:
            print(g_tagW, 'user : ' + user._uid + ' has no say item')
            rstList.append('')
        rst = rstList[0]
        user.setSay(rst)
        #print(rst)

        rel = re.compile('<script type=\"text/json\" class=\"json-inline\" data-name=\"current_people\">\[\"(.*?)\",\"(.*?)\",\"(.*?)\",\"(.*?)\"\]</script>')
        rstList = rel.findall(data)
        if not rstList:
            print(g_tagW, 'user : ' + user._uid + ' has no name and hash_id item')
            rstList.append(['','','',''])
        rst = rstList[0]
        user.setName(rst[0])
        userpic = rst[2].replace('\\','')
        hash_id = rst[3]
        #print(rst[0],userpic,hash_id)        
        
        rel = re.compile('<input type="radio" name="gender" value=.*? checked="checked" class=\"(.*?)\"/>')
        rstList = rel.findall(data)
        if not rstList:
            print(g_tagW, 'user : ' + user._uid + ' has no gender item')
            rstList.append('')
        rst = rstList[0].lower()
        user.setSex(rst == 'male')
        #print(rst)

        hascuritem = True
        rel = re.compile('<span class=\"employment item\" title=.*?>(.*?)</span>')
        rstList = rel.findall(data)
        if not rstList:
            rel = re.compile('<span class=\"employment item\" ><a .*?>(.*?)</a></span>')
            rstList = rel.findall(data)
            if not rstList:
                print(g_tagW, 'user : ' + user._uid + ' has no employment item')
                rstList.append('')
        rst = rstList[0]
        rel2 = re.compile('<a href=.*?>(.*?)</a>')
        rstList = rel2.findall(rst)
        if rstList:
            rst = rstList[0]
        if '填写公司信息' != rst:
            user.setEmployment(rst)
        #print(rst)

        hascuritem = True
        rel = re.compile('<span class=\"education item\" title=.*?>(.*?)</span>')
        rstList = rel.findall(data)
        if not rstList:
            print(g_tagW, 'user : ' + user._uid + ' has no education item')
            rstList.append('')
        rst = rstList[0]
        rel2 = re.compile('<a href=.*?>(.*?)</a>')
        rstList = rel2.findall(rst)
        if rstList:
            rst = rstList[0]
        user.setSchool(rst)
        #print(rst)

        hascuritem = True
        rel = re.compile('<span class=\"education-extra item\" title=.*?>(.*?)</span>')
        rstList = rel.findall(data)
        if not rstList:
            print(g_tagW, 'user : ' + user._uid + ' has no education-extra item')
            rstList.append('')
        rst = rstList[0]
        rel2 = re.compile('<a href=.*?>(.*?)</a>')
        rstList = rel2.findall(rst)
        if rstList:
            rst = rstList[0]
        user.setProfession(rst)
        #print(rst)

        hascuritem = True
        rel = re.compile('<span class=\"zg-gray-normal\">.*?</span>.*?<strong>(\d+)</strong>.*?<strong>(\d+)</strong><label>.*?</label>.*?</a>.*?</div>', re.S)
        rstList = rel.findall(data)
        if not rstList:
            print(g_tagW, 'user : ' + user._uid + ' has no followee_num and follower_num')
            rstList.append([-1, -1])
        rst = rstList[0]
        user.setFolloweeNum(rst[0])
        user.setFollowerNum(rst[1])
        #print(rst)
        #print(user.toString())
        
        return hash_id

    def parseFollowees(self, data, user):
        writeLog2File(data,'parseFollowees.txt')
        rel = re.compile('</div>\s*?<a title=.*\sdata-hovercard=.*\sclass=\"zm-item-link-avatar\"\shref=.*>')
        rstList = rel.findall(data)
        rel = re.compile('href=.*')
        
        cnt = 0
        for tmp in rstList:
            rlist = rel.findall(tmp)
            rst = rlist[0]
            rst = rst.replace("href=\"/people/", '')  # 个人
            rst = rst.replace("href=\"/org/", '')     # 公司
            rst = rst.strip('<>\"')
            #print(rst)
            info = g_datacenter.putTodo(rst)
            if info:
                user.addFollowee(info._id)
                info.addFollower(user._id)
                cnt = cnt + 1                
        return len(rstList),cnt

    def parseMoreFollowees(self, uid, data, user):
        jsonData = json.loads(data)
        cnt = 0
        for msg in jsonData['msg']:
            writeLog2File(msg,'parseMoreFollowees.txt')
            pos, tmp = self.parseFollowees(msg, user)
            cnt = cnt + pos
        return cnt
    
# for test        
##parser = AnsParse( )
##data = readFile('parseFollowees.txt').decode()
##print(parser.parseFollowees(data, UserInfo()))
##data = readFile('parseMoreFollowees.txt').decode()
##parser.parseMoreFollowees('', data, UserInfo())
##data = readFile('parsePersonPage.txt').decode()
##parser.parsePersonPage(data, UserInfo())
