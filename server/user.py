# -*- coding: utf-8 -*-
'''
Author  : Reskip
Date    : 2018.8.31
File    : user.py
'''
import json

class User(object):
    def __init__(self, username, pwd, level):
        self._username = username
        self._pwd = pwd
        self._level = level

    def __repr__(self):
        ret = dict()
        ret['username'] = self._username
        ret['pwd'] = self._pwd
        ret['level'] = self._level

        return json.dumps(ret)

class Server(object):
    def __init__(self):
        self._user = dict()
        f = open("user.info")

        for line in f.readlines():
            line = line.strip().split(",")
            name = line[0]
            self._user[name] = User(*line)

        print(self._user)
        print("init finished")

    def try_login(self, username, pwd):
        ret = dict()

        if username not in self._user:
            ret["status"] = "no_such_user"
        elif self._user[username]._pwd != pwd:
            ret["status"] = "pwd_error"
        else:
            ret["status"] = "succeed"
            ret["username"] = username
            ret["level"] = self._user[username]._level

        return json.dumps(ret)

if __name__ == "__main__":
    server = Server()
    print(server.try_login("1234", 33))
    print(server.try_login("admin", 33))
    print(server.try_login("admin", "123456"))