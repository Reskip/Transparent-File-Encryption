# -*- coding: utf-8 -*- 
import requests
import rsa
import json
import base64
import sys

host = "http://127.0.0.1:4999/"
pk_URL = host + "publicKey"
login_URL = host + "login"

def get_pub_key():
    r = requests.post(pk_URL)
    param = json.loads(r.text)['key']
    #print(param)
    param = param[param.find('(') + 1: param.find(')')]
    #print(param)
    n, e = param.split(",")
    n = int(n)
    e = int(e)
    #print(n, e)
    return rsa.PublicKey(n, e)


def post(username, pwd):
    key = rsa.newkeys(1024)
    privateKey, publicKey = key[1], key[0]

    pubKey = get_pub_key()
    postData = dict()
    postData['username'] = username
    postData['pwd'] = pwd
    postData['n'] = publicKey.n
    postData['e'] = publicKey.e
    origin_data = json.dumps(postData).encode()
    
    realPost = dict()
    while len(origin_data) > 0: 
        # print(origin_data[:100])
        encrypt_data = rsa.encrypt(origin_data[:100], pubKey)
        # print(encrypt_data)
        realPost[len(realPost)] = base64.b64encode(encrypt_data).decode()
        origin_data = origin_data[100:]

    r = requests.post(login_URL, data = realPost)
    # print(r.text)
    rsp = json.loads(r.text)
    # print(rsp)
    i = 0
    origin_post = ""
    while str(i) in rsp:
        encrypt_data = rsp[str(i)]
        encrypt_data = base64.b64decode(encrypt_data.encode())
        origin_data = rsa.decrypt(encrypt_data, privateKey).decode()
        origin_post += origin_data
        i += 1
    data = json.loads(origin_post)
    
    print(data)
    if data['status'] == 'succeed':
        f = open("login_rsp", "w")
        f.write("%s\n%s\n" % (data['username'], data['level']))
        f.close()
    f = open("login_flag", "w")
    f.close()
    return True
    
if __name__ == "__main__":
    post(sys.argv[1], sys.argv[2])