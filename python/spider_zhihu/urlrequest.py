from common import *
from urlrequest import *
import json


class UrlRequest(object):
    def __init__(self, defaultHeader=None):
        self._oper = UrlOperProxy(defaultHeader)

    def _openUrl(self, url, postdata=None, header=None):
        #print(url)
        return self._oper.openUrl(url, postdata, header)

    def open(self, url, postdata=None, header=None):
        return self._openUrl(url, postdata, header)
    
    def getHomepage(self):
        data = self._openUrl(urlRoot)
        return data

    def getPersonPage(self, uid):
        urlUser = urlRoot + 'people/' + uid
        #print(g_tagI, 'getPersonPage --- url : ' + urlUser)
        data = self._openUrl(urlUser)
        return data

    def getFollowees(self, uid):
        url = urlRoot + 'people/' + uid + '/followees'
        data = self._openUrl(url)
        return data

    ## 这里很奇怪
    #  抓包时没发现 postDict 中有 _xsrf 参数，但是不带上会返回403错误
    #  初步怀疑是因为 keep-alive 失效，待验证
    def getMoreFollowees(self, uid, hash_id, xsrf, pos):
        params = {
            "offset":pos,
            "order_by":"created",
            "hash_id":hash_id
        }
        postDict = {
            '_xsrf':xsrf,
            'method':"next",
            'params':json.dumps(params)
        }
        url = urlRoot + 'node/ProfileFolloweesListV2'
        postData = urllib.parse.urlencode(postDict)
        postData = postData.encode('utf-8')
        data = self._openUrl(url, postData)
        return data
