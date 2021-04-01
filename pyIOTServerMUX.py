import socket
import os
import time
import mysql.connector
from _thread import *

ServerSideSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
ServerSideSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
host = '0.0.0.0'
port = 8120
ThreadCount = 0
try:
    ServerSideSocket.bind((host, port))
except socket.error as e:
    print(str(e))

print('Start Listening on: ' + str(host) + ':' + str(port))
ServerSideSocket.listen(5)


def insert2DB(tablename, fedData, srcIP, srcPort, hdrLength):
    try:
        if tablename:
            dbconn = mysql.connector.connect(
                host="mysql.lan.astarc.tk",
                user="",
                passwd="",
                database=""
            )
            NOW = time.strftime("%Y/%m/%d %H:%M:%S",time.localtime())
            dbCursor = dbconn.cursor()
            if(tablename != 'ErrorLogging'):
                sqlQuery = 'INSERT INTO '+tablename+' (TIMEMARK,SOURCEIP,SOURCEPORT,STATUS,VDATA) VALUES("'+str(NOW)+'","'+str(srcIP)+'","'+str(srcPort)+'",1,"'+str(fedData[hdrLength:])+'")'
                print(">>> [" + NOW + "] inserting: " + tablename)
                dbCursor.execute(sqlQuery)
                dbconn.commit()
            else:
                print("Error Logging:")
                chunks = [fedData[i:i+2] for i in range(0, len(fedData), 2)]
                
                if(chunks[1] == '00'):
                    sqlQuery = 'INSERT INTO vibrationTD (TIMEMARK,SOURCEIP,SOURCEPORT,STATUS,VDATA) VALUES("'+str(NOW)+'","'+str(srcIP)+'","'+str(srcPort)+'",0,"00")'
                    print(">>> [" + NOW + "] ErrorLogging: vibrationTD")
                    dbCursor.execute(sqlQuery)
                    dbconn.commit()
                if(chunks[2] == '00'):
                    sqlQuery = 'INSERT INTO vibrationFD (TIMEMARK,SOURCEIP,SOURCEPORT,STATUS,VDATA) VALUES("'+str(NOW)+'","'+str(srcIP)+'","'+str(srcPort)+'",0,"00")'
                    print(">>> [" + NOW + "] ErrorLogging: vibrationFD")
                    dbCursor.execute(sqlQuery)
                    dbconn.commit()
                if(chunks[3] == '00'):
                    sqlQuery = 'INSERT INTO EDS (TIMEMARK,SOURCEIP,SOURCEPORT,STATUS,VDATA) VALUES("'+str(NOW)+'","'+str(srcIP)+'","'+str(srcPort)+'",0,"00")'
                    print(">>> [" + NOW + "] ErrorLogging: EDS")
                    dbCursor.execute(sqlQuery)
                    dbconn.commit()
                if(chunks[4] == '00'):
                    sqlQuery = 'INSERT INTO temperature (TIMEMARK,SOURCEIP,SOURCEPORT,STATUS,VDATA) VALUES("'+str(NOW)+'","'+str(srcIP)+'","'+str(srcPort)+'",0,"00")'
                    print(">>> [" + NOW + "] ErrorLogging: temperature")
                    dbCursor.execute(sqlQuery)
                    dbconn.commit()
                
            dbCursor.close()
            dbconn.close()
            print("")
    except Exception as e:
        print("Error: " + str(e))

def dataAry2String(displacedData):
    cleanData = ''
    tablename = ''
    hdrLength = 0
    for idx,ch in enumerate(displacedData):
        cleanData += str('{:02x}'.format(ch))
    
    if displacedData:
        if displacedData[0] == 0xAA:
            tablename = 'vibrationTD'
            hdrLength = 2
        elif displacedData[0] == 0xBB:
            tablename = 'vibrationFD'
            hdrLength = 8
        elif displacedData[0] == 0xCC:
            tablename = 'EDS'
            hdrLength = 4
        elif displacedData[0] == 0xDD:
            tablename = 'temperature'
            hdrLength = 4
        elif displacedData[0] == 0xEE:
            tablename = 'ErrorLogging'
        else:
            tablename = ''
            
        if tablename:
            print('Received Data>>> ' + tablename)
            # print('original data length>>> ' + str(len(displacedData)))
            # print('clean data length>>> ' + str(len(cleanData)))
        else:
            print('Unlabeled Data>>> ')
            print('Header = ', end='')
            print(displacedData[0])
            print('Raw data length>>> ' + str(len(displacedData)))
        
    return [tablename, cleanData, hdrLength]


def multi_threaded_client(connection, addr):
    # connection.send(str.encode('Server is working:'))
    print('>>> Connected!')
    print('>>> Address: ', addr[0])
    print('>>> Remote Port: ', addr[1])
    while True:
        data = connection.recv(2048)
        rtnData = dataAry2String(data)
        if not data:
            break
        else:
            if rtnData[0]:
                insert2DB(rtnData[0], rtnData[1], addr[0], addr[1], rtnData[2])
        
        # connection.sendall(str.encode(str(rtnData[2])))
    connection.close()

while True:
    Client, address = ServerSideSocket.accept()
    print('Connected to: ' + address[0] + ':' + str(address[1]))
    start_new_thread(multi_threaded_client, (Client, address))
    ThreadCount += 1
    print('Thread Number: ' + str(ThreadCount))
ServerSideSocket.close()
