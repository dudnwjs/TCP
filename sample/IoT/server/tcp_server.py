import socket
import sys

class TCPServer :
    def __init__(self):
        self.serverSock = None
        self.clientSock = None
        self.clientAddr = None
        self.bufMax = 4096

    def start(self, port):
        if not self.serverSock is None :
            print "already started!\n"
            return None
        self.serverSock = socket.socket()
        self.serverSock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.serverSock.bind(('0.0.0.0', port))
        self.serverSock.listen(1)
        self.clientSock,self.clientAddr = self.serverSock.accept()
        return self.clientAddr

    def read(self, data, sec):
        if self.clientSock is None:
            print "socket is empty!\n"
            return None
        self.clientSock.settimeout(sec)
        chunk = self.clientSock.recv(self.bufMax)
        data.append(chunk)
        return len(chunk)

    def write(self, data, sec):
        if self.clientSock is None:
            print "socket is empty!\n"
            return None
        self.clientSock.settimeout(sec)
        len = self.clientSock.send(data)
        return len

    def end(self):
        self.serverSock.close()
        self.clientSock.close()


