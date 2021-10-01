import socket
import os
import time
from _thread import *

ServerSideSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
ServerSideSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
host = '0.0.0.0'
port = 1883
ThreadCount = 0
try:
    ServerSideSocket.bind((host, port))
except socket.error as e:
    print(str(e))

print('Start Listening on: ' + str(host) + ':' + str(port))
ServerSideSocket.listen(5)



def multi_threaded_client(connection, addr):
    # connection.send(str.encode('Server is working:'))
    print('>>> Connected!')
    print('>>> Address: ', addr[0])
    print('>>> Remote Port: ', addr[1])
    while True:
        data = connection.recv(3060)
        if not data:
            break
        else:
            print(data)
        
        #connection.sendall(str.encode(str(rtnData[2])))
    connection.close()

while True:
    Client, address = ServerSideSocket.accept()
    print('Connected to: ' + address[0] + ':' + str(address[1]))
    start_new_thread(multi_threaded_client, (Client, address))
    ThreadCount += 1
    print('Thread Number: ' + str(ThreadCount))
ServerSideSocket.close()
