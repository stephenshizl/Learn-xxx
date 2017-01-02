import multiprocessing
import multiprocessing.queues
from common import *
from dbmgr import *

class UserInfo(object):
    def __init__(self, name='', uid=''):
        self._id = -1
        self._name = name
        self._uid = uid
        self._sex = 'male'
        self._says = ''
        self._follower = set()
        self._email = ''
        self._followee = set()
        self._employment = ''
        self._school = ''
        self._profession = ''
        self._follower_num = 0
        self._followee_num = 0
        self._done = 0
        self._hash_id = ''
    
    def setID(self, _id):
        self._id = _id
        
    def setName(self, name):
        self._name = name

    def setEmployment(self, employment):
        self._employment = employment

    def setProfession(self, profession):
        self._profession = profession
        
    def setSchool(self, school):
        self._school = school
        
    def setUID(self, uid):
        self._uid = uid
        
    def setSay(self, sentence):
        self._says = sentence

    def setSex(self, isMale):
        if isMale:
            self._sex = 'male'
        else:
            self._sex = 'female'
        
    def setEmail(self, email):
        self._email = email

    def setFollowerNum(self, num):
        self._follower_num = num

    def setFolloweeNum(self, num):
        self._followee_num = num
        
    def addFollower(self, follower):
        self._follower.add(follower)
        
    def addFollowee(self, followee):
        self._followee.add(followee)
        
    def addFollowees(self, followees):
        self._followee.union(followees)

    def setDone(self):
        self._done = 1

    def setHashID(self, hash_id):
        self._hash_id = hash_id
        
    def toString(self):
        ret = '\n\t'
        ret = ret + "id   : " + str(self._id) + "\n\t"
        #ret = ret + "name : " + self._name + "\n\t"
        ret = ret + "done : " + str(self._done) + "\n\t" 
        ret = ret + "uid  : " + self._uid + "\n\t"
        ret = ret + "sex  : " + self._sex + "\n\t"
        ret = ret + "say  : " + self._says + "\n\t"
        ret = ret + "employment  : " + self._employment + "\n\t"
        ret = ret + "school      : " + self._school + "\n\t"
        ret = ret + "profession  : " + self._profession + "\n\t"
        ret = ret + "follower_num: " + str(self._follower_num) + "\n\t"
        ret = ret + "followee_num: " + str(self._followee_num) + "\n\t"
        #ret = ret + "email : " + self._email + "\n\t"        
        #ret = ret + "follower : " + str(self._follower) + "\n\t"
        #ret = ret + "followee : " + str(self._followee) + "\n\t"
        #ret = ret + "follower(found) : " + str(len(self._follower)) + "\n\t"
        #ret = ret + "followee(found) : " + str(len(self._followee)) + "\n\t"
        return ret

# _todo = set() : 表示新发现的UID集合
# _undo = {}    : 表示正在处理的 UID 到 ID 的映射
# _done = {}    : 表示已处理完的 UID 到 ID 的映射
class DataCenter(object):
    def __init__(self):
        self._users = {}
        self._todo = set()
        self._undo = {}
        self._done = {}
        self._nextID = 1

    def _getNewID(self):
        ret = self._nextID
        self._nextID = self._nextID + 1
        return ret
    
    def getID(self, uid):
        if uid in self._undo.keys():
            return self._undo[uid]
        elif uid in self._done.keys():
            return self._done[uid]

    def getByID(self, _id):
        if _id in self._users.keys():
            return self._users[_id]
    
    def hasFound(self, uid):
        if uid in self._todo:
            return True
        elif uid in self._undo.keys():
            return True
        elif uid in self._done.keys():
            return True
        return False

    def addItem(self, userinfo):
        if not userinfo:
            return -1
        uid = userinfo._uid
        _id = userinfo._id
        if _id < 0:
            _id = self._getNewID()
            userinfo.setID(_id)
        self._users[_id] = userinfo
        #print(g_tagI, 'addItem --- ' + str(_id) + ' uid: '+ userinfo._uid)
        return _id
        
    def putTodo(self, uid):
        if not self.hasFound(uid):
            self._todo.add(uid)
            print(g_tagI, 'putTodo --- cnt: ' + str(len(self._todo)) + ' uid: '+ uid)

    def popTodo(self):
        if len(self._todo):
            uid = self._todo.pop()
            userinfo = UserInfo()
            userinfo.setUID(uid)            
            userinfo.setID(self._getNewID())
            return userinfo
        
    def putUndo(self, userinfo):
        if not userinfo:
            return -1
        uid = userinfo._uid
        _id = userinfo._id
        if uid in self._done.keys():
            return -1
        self._undo[uid] = _id

    def popUndo(self):
        if len(self._undo):
            (uid, _id) = self._undo.popitem()
            userinfo = self.getByID(_id)
            return userinfo
        
    def putDone(self, userinfo):
        if not userinfo:
            return -1
        uid = userinfo._uid
        _id = userinfo._id
        userinfo.setDone()
        if uid in self._undo.keys():
            del self._undo[uid]
        self._done[uid] = _id        
                
        
class SharedCounter(object):
    def __init__(self, n = 0):
        self.count = multiprocessing.Value('i', n)

    def increment(self, n = 1):
        with self.count.get_lock():
            self.count.value += n

    @property
    def value(self):
        return self.count.value

class Queue(multiprocessing.queues.Queue):
    """ Implemention of multiprocessing.Queue
    Solve the NotImplementedError in multiprocessing.queues on Mac OS
    """
    def __init__(self, *args, **kwargs):
        super(Queue, self).__init__(ctx=multiprocessing.get_context(), *args, **kwargs)
        self.size = SharedCounter(0)

    def put(self, *args, **kwargs):
        self.size.increment(1)
        super(Queue, self).put(*args, **kwargs)

    def get(self, *args, **kwargs):
        self.size.increment(-1)
        return super(Queue, self).get(*args, **kwargs)

    def qsize(self):
        return self.size.value

    def empty(self):
        return not self.qsize()

    def clear(self):
        while not self.empty():
            self.get()


g_datacenter = DataCenter()
g_dbMgr = DbMgr(g_dbname)

##g_datacenter.putUndo('shui-lin-feng-hai', 1)
##print(g_datacenter._users)
##print(len(g_datacenter._users))
