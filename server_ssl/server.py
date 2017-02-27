import sys

from twisted.internet import reactor, ssl
from twisted.python import log
from twisted.web.server import Site
from twisted.web.static import File

import sqlite3 as lite
import sys
import random
con = lite.connect('User.db') 
cur = con.cursor()
import random, string

def randomword(length):
   return ''.join(random.choice(string.lowercase) for i in range(length))

def checkSession(sessionId,sId):
    x = str(sessionId).split('D')
    if str(sId) in x:
        return 1
    else:
        return 0
#Creating status Frame: S-SwitchCount-Statesofswitches
def createStatus(productId):
    cur.execute("SELECT * From "+productId)
    data = cur.fetchone()
    #print data
    reply = "S-" +str(data[1])
    for i in range(0,int(data[1])):
        reply = reply +"-"+str(data[i+2])
    reply += "-~"
    print reply
    return reply

def sendStatus(productId):
    reply = createStatus(productId)
    cur.execute("SELECT SessionId FROM " +productId)
    sessionId = str(cur.fetchone()[0])
    sessionId = sessionId.split('D')
    for sid in sessionId:
        if not(appToServer.get(str(productId)+str(sid)) == None):
            try:
                aself = appToServer.get(str(productId)+str(sid))
                aself.sendMessage(reply, isBinary = False)
            except:
                del appToServer[str(productId)+str(sid)]

from autobahn.twisted.websocket import WebSocketServerFactory, \
    WebSocketServerProtocol, \
    listenWS


appToServer = {None:None}
hardToServer = {None:None}

sensor_data = [];

class EchoServerProtocol(WebSocketServerProtocol):

    def onMessage(self, payload, isBinary):
#if isBinary:
      #   print("Binary message received: {0} bytes".format(len(payload)))
      #else:
      #   print("Text message received: {0}".format(payload.decode('utf8')))
        print payload
        a = payload.split('-')  #using - as Delimiter for messages
        
        #self_dumped = json
        #Android to Server Communication
        if(a[0] == 'A'):
            #Registration of new user
            #Frame: A-R-Roll-Room-PID-Password
            if (a[1] == 'R'):       #Connection Info
                with con:
                    cur.execute("SELECT Room FROM Users WHERE Roll=?", (a[2],))
                    data = cur.fetchone()
                    if data is None:
                        key = randomword(10)
                         #In case of collision trouble can be here
                        cur.execute("INSERT INTO Users VALUES (?, ?, ?, ?, ?);", (a[2], a[3], a[4], a[5],key))
                        print a[2] + " added as a new User"
                        reply = "S-R-1-New user added-~"
                        
                        self.sendMessage(reply, isBinary = False)
                        cur.execute("CREATE TABLE IF NOT EXISTS "+str(a[4])+" (SessionId NUMERIC ,switchCount INT , SW1 INT , SW2 INT, SW3 INT , SW4 INT, SW5 INT, SW6 INT);")
                        cur.execute("INSERT INTO "+str(a[4])+"  VALUES (?, ?, ?, ?, ?, ?, ?, ?);", ("",6, 0, 0, 0, 0, 0, 0))
                        #print key
                    else:
                        reply = "S-R-0-You are already registered-~"
                        self.sendMessage(reply, isBinary = False)
            #Logging in 
            #Frame: A-L-Roll-Password
            elif(a[1] == 'L' ):
                with con:
                    cur.execute("SELECT Password,Product FROM Users WHERE Roll=?", (a[2],))
                    data = cur.fetchone()
                    #print data[0]
                    if data is None:
                        reply = "S-L-0-User does not exist-~"
                        self.sendMessage(reply, isBinary = False)
                    #Assigning a sessionId if credentials are correct
                    elif (data[0] == a[3]):
                        productId = data[1]
                        cur.execute("SELECT Key FROM Users WHERE Product =?", (productId,))
                        key = cur.fetchone()[0]
                        #print key
                        sId = random.getrandbits(8)
                        cur.execute("SELECT SessionId FROM " +productId)

                        sessionId = str(cur.fetchone()[0]) + str(sId) +"D" #D as Delimiter
                        
                        print sessionId
                        #storing current state of all devices in the room
                        smdcmd = "Update "+productId+" SET SessionId='"+ str(sessionId) + "'"
                        print smdcmd
                        cur.execute(smdcmd)
                        reply = "I-"+key+"-" +str(sId)
                        print reply
                        #cur.execute("SELECT * FROM "+key+" ")
                        #print cur.fetchall()

                        self.sendMessage(str(reply), isBinary = False)
                    elif(data[0] != a[3]):
                        reply = "S-L-0-Incorrect Password-~"
                        self.sendMessage(reply, isBinary = False)

            #Controlling Switches
            #Frame: A-C-key-SessionID-Switch-State
            elif (a[1] == 'C'):
                cur.execute("SELECT Product FROM Users WHERE Key = ?",(a[2],))
                productId = cur.fetchone()[0]
                cur.execute("SELECT SessionId FROM "+productId)
                sessionId = cur.fetchone()[0]
                print sessionId
                print checkSession(sessionId,a[3])
                if(checkSession(sessionId,a[3])==1):
                    #sqlcmd = "UPDATE "+a[2]+" SET SW" + str(a[4]) +"=" + str(a[5])
                    #print sqlcmd
                    #cur.execute(sqlcmd)
                    print a 
                    eself = hardToServer.get(productId)
                    reply = "S-C-"+str(a[4])+"-" + str(a[5])+"-~"
                    print reply
                    eself.sendMessage(reply, isBinary = False)
                    if(int(a[4]>4)):
                        sqlcmd = "UPDATE "+productId+" SET SW" + str(a[4]) +"=" + str(a[5])
                        cur.execute(sqlcmd)
                        
            #To send current status of the switches to the application
            #Frame: A-S-Key-SessionID
            elif (a[1] == 'S'):
                cur.execute("SELECT Product FROM Users WHERE Key = ?",(a[2],))
                productId = cur.fetchone()[0]
                cur.execute("SELECT SessionId FROM "+productId)
                sessionId = cur.fetchone()[0]
                print sessionId
                print checkSession(sessionId,a[3])
                if(checkSession(sessionId,a[3])==1):
                    reply = createStatus(productId)
                    self.sendMessage((reply), isBinary = False)
                    appToServer[str(productId)+str(a[3])] = self
                    #print str(appToServer)
            #Logging out
            #Frame: A-Z-Key-SessionID
            elif (a[1] == 'Z'):
                cur.execute("SELECT Product FROM Users WHERE Key = ?",(a[2],))
                productId = cur.fetchone()[0]
                cur.execute("SELECT SessionId FROM "+productId)
                sessionId = cur.fetchone()[0]
                x = sessionId.split("D")
                x.remove(str(a[3]))
                sessionId = ""
                for i in x:
                    sessionId = sessionId + "D" +i
                print sessionId
                srr = sessionId[0]
                for i in range(0,len(sessionId)-1):
                    if sessionId[i] =='D' and sessionId[i+1] =='D':
                        pass
                    else:
                        srr += sessionId[i+1]
                sessionId = srr
                print sessionId

                smdcmd = "Update "+productId+" SET SessionId='"+ str(sessionId) + "'"
                print smdcmd
                cur.execute(smdcmd)

        #Electronics to server communication
        elif(a[0] == 'E'):
            productId = a[2]
            #Registration of Electronic side
            #Frame: E-R-ProductID
            if(a[1]=='R'):
                hardToServer[productId] = self
                print str(hardToServer)
            #Electronic side requesting status
            #Frame: E-S-ProductID
            elif(a[1]=='S'):
                reply = createStatus(productId)
                self.sendMessage((reply), isBinary = False)
            #Controlling switch using electronics sending status to server
            #Frame: E-C-ProductID-Swith-state
            elif(a[1] == 'C'):
                cur.execute("Update "+ productId +" SET SW" + str(a[3]) + "=" + str(a[4]))
                sendStatus(productId)
            #Response from electronic side when switch is controlled using application
            #E-U-PID-Switch-State
            elif(a[1] == 'U'):
                sqlcmd = "UPDATE "+productId+" SET SW" + str(a[3]) +"=" + str(a[4])
                #print sqlcmd
                cur.execute(sqlcmd)
                sendStatus(productId)
            elif(a[1] == 'P'):
                reply = "S-P"
                self.sendMessage((reply), isBinary = False)


if __name__ == '__main__':

    log.startLogging(sys.stdout)

    # SSL server context: load server key and certificate
    # We use this for both WS and Web!
    contextFactory = ssl.DefaultOpenSSLContextFactory('keys/server.key',
                                                      'keys/server.crt')

    factory = WebSocketServerFactory(u"wss://127.0.0.1:9000")

    factory.protocol = EchoServerProtocol
    listenWS(factory, contextFactory)

    webdir = File(".")
    webdir.contentTypes['.crt'] = 'application/x-x509-ca-cert'
    web = Site(webdir)
    reactor.listenSSL(8080, web, contextFactory)
    #reactor.listenTCP(8080, web)

    reactor.run()
