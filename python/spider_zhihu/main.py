from common import *
from login import *
from urlrequest import *
from datastruct import *
from ansparse import *

def getUserFollowee(session, parser, userinfo, hash_id):
    uid = userinfo._uid   
    req = session._opener

    try:
        data = req.getFollowees(uid)
    except:
        print(g_tagE,  'important --- uid:' + uid + ' getFollowees failed')
        return
    nfound, nadded = parser.parseFollowees(data, userinfo)

    total_cnt = 0
    loop_cnt = 0
    while(1):
        hasExcept = False
        last_data = data
        last_found = nfound
        loop_cnt += 1
        try:
            data = req.getMoreFollowees(uid, hash_id, session._xsrf, loop_cnt * 20)
            nfound = parser.parseMoreFollowees(uid, data, userinfo)
            total_cnt = total_cnt + nfound
        except:
            hasExcept = True

        if hasExcept:
            writeLog2File(last_data, 'FolloweesEerror_'+uid+'_'+str(last_found)+'_'+str(loop_cnt * 20)+'.txt')
            break        
        if 0 == nfound:
            break
        if 0 != last_found % 20:
            writeLog2File(last_data, 'FolloweesEerror_'+uid+'_'+str(last_found)+'_'+str(loop_cnt * 20)+'.txt')
            
    return total_cnt

def getUserData(session, parser, userinfo):
    uid = userinfo._uid   
    req = session._opener
    
    try:
        data = req.getPersonPage(uid)
    except:
        print(g_tagE, 'important --- uid:' + uid + ' getPersonPage failed')
        return
    hash_id = parser.parsePersonPage(data, userinfo)
    userinfo.setHashID(hash_id)
    print(g_tagI, 'getUserData --- cur_info:'+userinfo.toString())
    return hash_id
     
def spiderZH(session, parser):
    cnt = 0
    skipGetFollowee = False
    while(True):
        num_todo = len(g_datacenter._todo)
        if 0 == num_todo:
            break

        while num_todo:
            userinfo = g_datacenter.popTodo()
            hash_id  = getUserData(session, parser, userinfo)
            num_todo = num_todo - 1
            g_datacenter.addItem(userinfo)
            g_datacenter.putUndo(userinfo)

        num_undo = len(g_datacenter._undo)
        while num_undo:
            userinfo = g_datacenter.popUndo()
            getUserFollowee(session, parser, userinfo, userinfo._hash_id)
            g_datacenter.putDone(userinfo)
            num_undo = num_undo - 1
            num_todo = len(g_datacenter._todo)
            if num_todo > 3000:
                break
           
        cnt += 1
    print(g_tagI, 'spiderZH END : has ' + str(len(g_datacenter._users)) + ' users, has got ' + str(cnt) + ' users\'s info')
    
def main():
    print(g_tagI, 'ready to login...')
    print(g_tagI, user)
    password = input('input --- password : ')

    ansparse = AnsParse()
    
    login = Login(urlRoot, user, password)
    session = login._in()
    if not session:
        return    
    print(g_tagI, 'ready to get homepage')
    data = session._opener.getHomepage()
    #write2File(data)
    #print(g_tagI, 'has write homepage to file')

    curuid = ansparse.parseHomepage(data)
    g_datacenter.putTodo(curuid)    
    
    spiderZH(session, ansparse)
    
    login._out()
    print(g_tagI, 'has logout')
    

if __name__ == "__main__":
    main()
