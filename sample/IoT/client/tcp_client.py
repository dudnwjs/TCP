import socket
import sys

class TCPClient :
    def __init__(self):
        self.sock = None
        self.bufMax = 4096

    def start(self, addr, port):
        if not self.sock is None :
            print "already started!\n"
            return None
        self.sock = socket.socket()
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.connect((addr,port))
        return self.sock 

    def read(self, data, sec):
        if self.sock is None:
            print "socket is empty!\n"
            return None
        self.sock.settimeout(sec)
        chunk = self.sock.recv(self.bufMax)
        data.append(chunk)
        return len(chunk)

    def write(self, data, sec):
        if self.sock is None:
            print "socket is empty!\n"
            return None
        self.sock.settimeout(sec)
        len = self.sock.send(data)
        return len

    def end(self):
        self.sock.close()


