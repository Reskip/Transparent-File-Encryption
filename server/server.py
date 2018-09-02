# -*- coding: utf-8 -*- 
'''
Author  : Reskip
Date    : 2018.8.31
File    : server.py
'''
from flask import Flask, request
import json
import rsa
from user import *

app = Flask(__name__)
server = Server()
key = rsa.newkeys(1024)
privateKey, publicKey = key[1], key[0]

@app.route('/login_test', methods=['POST', 'GET'])
def login_test():
    if 'username' not in request.form or 'pwd' not in request.form:
        return '''{"status": "bad_request_fomat", "request": "%s"}''' % (json.dumps(request.form))
    username = request.form['username']
    pwd = request.form['pwd']
    return server.try_login(username, pwd)

@app.route('/publicKey', methods=['POST', 'GET'])
def pubKey():
    return str(publicKey)

if __name__ == '__main__':
    app.run(host = "0.0.0.0", port = 4999)