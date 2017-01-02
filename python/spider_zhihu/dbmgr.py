import sqlite3
from common import *

sql_createUser = '''CREATE TABLE IF NOT EXISTS users
                    (id integer primary key, name text, 
                     uid text, sex integer, says text,
                     school text, status integer)'''

sql_updateUser = '''REPLACE INTO users
                    (id, name, uid, sex ,says ,school,
                     status) VALUES(?,?,?,?,?,?,?)'''

sql_selectAllUsers = '''SELECT * FROM users'''

sql_changeUserStatus = '''UPDATE users SET status=? where id=?'''

sql_getStatusUser = '''SELECT * FROM users where status=%d LIMIT 1'''

######################################################################

class DbMgr(object):
    
    def __init__(self, dbname):
        self._dbname = dbname
        self._conn = None

    def _exec(self, sql, arg=None):
        ret = None
        cursor = self._conn.cursor()
        if arg:
            ret = cursor.execute(sql, arg)
        else:
            ret = cursor.execute(sql)
        self._conn.commit()
        return ret

    def update(self, index, name, uid, sex, says, school, status):
        arg = (index, name, uid, (1 if sex=='male' else 2), says, school, status)
        self._exec(sql_updateUser, arg)

    def changeStatus(self, index, status):
        arg = (status, index)
        self._exec(sql_changeUserStatus, arg)

    def getStatusUser(self, status):
        sql = sql_getStatusUser % status
        cur = self._exec(sql)
        data = cur.fetchone()
        return data
    
    def openDB(self):
        if (self._conn):
            self.closeDB()
        self._conn = sqlite3.connect(self._dbname)
        
    def closeDB(self):
        self._conn.close()
        self._conn = None
        
if __name__ == '__main__':
   sqlite = DbMgr('test.db')
   sqlite.openDB()
   sqlite._exec(sql_createUser)
   sqlite.update(1,'a','x',1,'m','',1)
   sqlite.update(2,'a','x',1,'m','',1)
   sqlite.changeStatus(2, 2)
   print(sqlite.getStatusUser(2))
   
