#encoding=utf-8

import json
import sys
import threading
import traceback
import os
from os import sep, curdir
import shutil
from unit.untfunction import TGobFun
from bottle import route, run,request
import pridict

Gunit=TGobFun();
Gunit.initlogger(1,1,"./log/")
Gunit.showlog("Init  NumRecServer  servering")


#NumRec
#params PicFile1
#params PicFile2
@route('/api/recnum')
def API_RecNum():
    #指定返回编码
    try:
        p1=request.query.PicFile1
        p2=request.query.PicFile2
        if not os.path.exists(p1):
            content="rec the number and resb"
        else:
            content = "file not Exist"

    except:
        content="error: "+traceback.format_exc()
        content="{\"%s\":\"%s\"}"%("result",content)
        Gunit.showlog(content)
    return  content


if __name__ == "__main__":
    pridict.initcnn();
    gpath="/root/"
    print "the Pic Path is",gpath
    print 'started NumRecServer...'
    run(port=18800)