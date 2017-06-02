#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys,os
import time,datetime
import random

import re
try:
    from influxdb import InfluxDBClient
except:
   pass

class TGobFun(object):

   def initinfluxdb(self,iwho,ishost,ireg,ihost,iport,iacc,ipsd,idb):
       self.who=iwho
       self.shost=ishost
       self.reg=ireg
       self.fluxclient=InfluxDBClient(ihost, iport, iacc,ipsd, idb)
       #self.fluxclient.write_points("lg_events", [{"email": "paul@influxdb.org", "state": "1", "type": "click"}]);

   def getnow(self):
      return datetime.datetime.now()
    
   #for time
   def getformatdate(self):
      return  time.strftime('%Y-%m-%d',time.localtime(time.time()))

   def getformattime(self):
      return  time.strftime('%H:%M:%S',time.localtime(time.time()))

   def getdocdate(self):
      return  time.strftime('%Y%m%d',time.localtime(time.time()))

   def getdoctime(self):
      return  time.strftime('%H%M%S',time.localtime(time.time()))

   def getformatdatetime(self):
      return  time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))

   def getdoctimesec(self):
       return  time.strftime('%H%M%S',time.localtime(time.time()))[4:6]

   def getdoctimemin(self):
       return  time.strftime('%H%M%S',time.localtime(time.time()))[2:4]

   def getdoctimehour(self):
       return  time.strftime('%H%M%S',time.localtime(time.time()))[0:2]

   def getthesec(self,idatetime):
       return  idatetime[12:14]

   def getthemin(self,idatetime):
       return  idatetime[10:12]

   def getthehour(self,idatetime):
       return  idatetime[8:10]

    #获取脚本文件的当前路径
   def getcurrdir(self):
         #获取脚本路径
         path = sys.path[0]
         #判断为脚本文件还是py2exe编译后的文件，如果是脚本文件，则返回的是脚本的目录，如果是py2exe编译后的文件，则返回的是编译后的文件路径
         if os.path.isdir(path):
             return path
         elif os.path.isfile(path):
             return os.path.dirname(path)

   def Str_Encry(self,ISrc,Key= 'mmz'):
      KeyLen=0;
      KeyPos=0;
      offset=0;
      dest='';
      SrcPos=0;
      SrcAsc=0;
      Range=0;

      KeyLen = len(Key)-1;
      KeyPos = -1;
      Range = 256;
      offset = random.randint(1,Range);
      dest = '%1.2x'%offset;  
      for SrcPos  in range(len(ISrc)) :    
        SrcAsc = (ord(ISrc[SrcPos]) + offset) % 255;
        if KeyPos < KeyLen :
          KeyPos = KeyPos + 1
        else:
          KeyPos = 0;
        SrcAsc = SrcAsc ^ ord(Key[KeyPos]);
        dest = dest + '%1.2x'%SrcAsc;
        offset = SrcAsc;
      
      return dest;


   def Str_Decry(self,ISrc,Key='mmz'): 
      KeyLen=0
      KeyPos=0
      offset=0
      dest=''
      SrcPos=0
      SrcAsc=0
      TmpSrcAsc=0
      KeyLen = len(Key)-1;
      KeyPos = -1;  
      offset = int(ISrc[0:2],16);
      SrcPos = 2;
      SrcAsc = 0;

      while True:
        try:
          SrcAsc = int(ISrc[SrcPos:SrcPos+2],16);
        except:
          pass
        
        if KeyPos < KeyLen:
          KeyPos = KeyPos + 1
        else:
          KeyPos = 0;
        TmpSrcAsc = SrcAsc ^ ord(Key[KeyPos]);
        if TmpSrcAsc <= offset:
          TmpSrcAsc = 255 + TmpSrcAsc - offset
        else:
          TmpSrcAsc = TmpSrcAsc - offset;
        dest = dest + chr(TmpSrcAsc);
        offset = SrcAsc;
        SrcPos = SrcPos + 2;
        if SrcPos >= len(ISrc):
           break;

      return dest;


   #Public Funnctions
   def get_mac_address(self):
       import uuid
       node = uuid.getnode()
       mac = uuid.UUID(int = node).hex[-12:]
       return mac

   #get the ip with ethx:x
   def get_ip_address(self,ifname):
       s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
       return socket.inet_ntoa(fcntl.ioctl(
           s.fileno(),
           0x8915,  # SIOCGIFADDR
           struct.pack('256s', ifname[:15])
       )[20:24])


   def getALLIpAddresses(self):
        addrList = socket.getaddrinfo(socket.gethostname(), None)
        ipList=[]
        for item in addrList:        
            ipList.append(item[4][0])
        return ipList


    #get the netpart
   def getipnetpart(self,ip):
        ls=ip.split(".")
        return ls[0]+'.'+ls[1]+'.'+ls[2]+'.'

    #is leage Ip
   def islegalip(self,ip_str):
       pattern = r"\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b"
       if re.match(pattern, ip_str):
          return True
       else:
          return False


   def getiplast(self,ip_str):
        iplst=ip_str.split('.')
        return int(iplst[len(iplst)-1])

   def __init__(self):
      self.gwritefile=1
      self.gprintlog=1
      self.gmylogerpath=""
      self.gfilename=""
      self.gfileobj=None
      self.wcount=0
      self.fluxclient=None


   def initlogger(self,iprintlog,iwritefile,ifilepath):
        self.gwritefile=iwritefile
        self.gprintlog=iprintlog
        if os.path.exists(ifilepath)==False:
           os.makedirs(ifilepath)
           self.gmylogerpath=ifilepath
           print "mkdir: "+self.gmylogerpath
            
        self.gfilename=self.getdocdate()+'.txt'
        print ifilepath+self.gfilename
        if iwritefile:
           self.gfileobj=open(ifilepath+self.gfilename,"a") 
           print "open file"


   def wlog(self,iconn):
        if(self.gwritefile==1):
            self.gfileobj.write(iconn+"\r\n")
            self.checknew()
        
   def slog(self,iconn):
        print iconn

   def wslog(self,iconn):
        if(self.gwritefile==1):
            self.gfileobj.write(iconn+"\r\n")
            self.checknew()
        print iconn



   def showlog(self,iconn,ilevel=0):
       iconn=self.getformattime()+" "+iconn;
       if(self.gwritefile==1):
           if self.fluxclient<>None:
                json_body = [{
                        "measurement": self.who,
                        "tags": {
                            "host": self.shost,
                            "region": self.reg
                        },
                        "fields": {
                            "lv": str(ilevel),
                            "conn": iconn
                         }
                        }]
                self.fluxclient.write_points(json_body)
           else:
               self.gfileobj.write(iconn+"\r\n")
               self.checknew()
       print iconn


   def printlog(self,iconn):
       self.showlog(iconn)
        
   def checknew(self):
       self.wcount= self.wcount+1
       #if self.wcount % 10==0:
       self.gfileobj.flush()
       if self.wcount>=2000:
          self.wcount=0
          if self.gfilename<>self.getdocdate()+'.txt':
             self.gfileobj.close()
             self.gfilename=self.getdocdate()+'.txt'
             self.gfileobj=open(ifilepath+self.gfilename,"a")  

   def random_str(self,randomlength):
             str = ''
             chars = 'AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789'
             length = len(chars) - 1
             for i in range(randomlength):
                 str+=chars[random.randint(0, length)]
             return str

gobfuns=TGobFun()

if __name__ == '__main__' :
         
    lp=TGobFun()     
    lp.initlogger(1,1,os.path.curdir+"mylogfile\\")
    
    lp.wslog("hello mylog 1.0")
    print "getthesec",lp.getthesec("20140511150122")
    print "getthemin",lp.getthemin("20140511150122")
    print "getthehour",lp.getthehour("20140511150122")

    print "getdoctimesec",lp.getdoctimesec()
    print "getdoctimemin",lp.getdoctimemin()
    print "getdoctimehour",lp.getdoctimehour()