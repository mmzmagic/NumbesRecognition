#encoding=utf-8

import json
import sys
import threading
import traceback
from os import sep, curdir
import shutil
from unit.untfunction import TGobFun
from bottle import route, run,request

Gunit=TGobFun();
Gunit.initlogger(1,1,"./log/")
Gunit.showlog("Init  API servering")

#Api For Init
#params list
#did  areaid
#cid   carmeid
#filename file.mp4
#stime  recordtime
#etime  tottime
@route('/api/BeginAnalysis')
def API_NumRec():
    #指定返回编码
    try:
        did=request.query.Did         #   areaid
        cid = request.query.Did       #    carmeid
        filename = request.query.filename  #   filename
        stime = request.query.stime  # recordtime
        etime = request.query.etime  # tottime

        Gunit.showlog("VideoClient BeginAnalysis An Video Record to DB")
    except:
        content="error: "+traceback.format_exc()
        content="{\"%s\":\"%s\"}"%("result",content)
        Gunit.showlog(content)
    return  content


#NumRec
#params PicFile1
#params PicFile2
@route('/api/numrec')
def API_NumRec():
    #指定返回编码
    try:
        p=request.query.p
        print p
        content="hello wd"
        if p<>"":
            ltp=p.split("=")[1]
            print ltp
    except:
        content="error: "+traceback.format_exc()
        content="{\"%s\":\"%s\"}"%("result",content)
        Gunit.showlog(content)
    return  content


def transDicts(params):
        dicts={}
        if len(params)==0:
            return
        params = params.split('&')
        for param in params:
            dicts[param.split('=')[0]]=param.split('=')[1]
        return dicts


if __name__ == "__main__":

    print 'started APIserver...'
    run(port=18900)