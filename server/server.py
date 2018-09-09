# -*- coding: utf-8 -*- 
'''
Author  : Reskip
Date    : 2018.8.31
File    : server.py
'''
from flask import Flask, request, jsonify
import json
import rsa
import base64
from user import *

app = Flask(__name__)
server = Server()
key = rsa.newkeys(1024)
privateKey, publicKey = key[1], key[0]

@app.route('/login_test', methods=['POST', 'GET'])
def login_test():
    if 'username' not in request.form or 'pwd' not in request.form:
        # return '''{"status": "bad_request_fomat", "request": "%s"}''' % (json.dumps(request.form))
        return jsonify(status='bad_request_format'), 400
    username = request.form['username']
    pwd = request.form['pwd']
    return server.try_login(username, pwd)

@app.route('/publicKey', methods=['POST', 'GET'])
def pubKey():
    return jsonify(key=str(publicKey)), 400

@app.route('/login', methods=['POST', 'GET'])
def login():
    i = 0
    origin_post = ""
    while str(i) in request.form:
        encrypt_data = request.form[str(i)]
        encrypt_data = base64.b64decode(encrypt_data.encode())
        origin_data = rsa.decrypt(encrypt_data, privateKey).decode()
        origin_post += origin_data
        i += 1
    data = json.loads(origin_post)

    pubKey = rsa.PublicKey(int(data['n']), int(data['e']))
    ret = ""
    if 'username' not in data or 'pwd' not in data:
        # return '''{"status": "bad_request_fomat", "request": "%s"}''' % (json.dumps(request.form))
        ret = jsonify(status='bad_request_format'), 400
    else:
        username = data['username']
        pwd = data['pwd']
        ret = server.try_login(username, pwd)
    
    ret = ret.encode()
    realPost = dict()
    while len(ret) > 0: 
        # print(origin_data[:100])
        encrypt_data = rsa.encrypt(ret[:100], pubKey)
        # print(encrypt_data)
        realPost[len(realPost)] = base64.b64encode(encrypt_data).decode()
        ret = ret[100:]
    
    return json.dumps(realPost)

if __name__ == '__main__':
    app.run(host = "0.0.0.0", port = 4999)