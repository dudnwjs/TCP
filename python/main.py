import threading
from tcp_server import TCPServer
from tcp_client import TCPClient

def func1(port):
    data = []
    s = TCPServer()
    clientAddr = s.start(port)
    print clientAddr
    len = s.read(data,None);
    print 'server read len =' + str(len)
    print ''.join(data)
    len = s.write("server write",None)
    print 'server write len =' + str(len)
    s.end()

def func2(addr,port):
    data2 = []
    s = TCPClient()
    s.start(addr,port)
    len = s.write("client write",None)
    print 'client write len =' + str(len)
    len = s.read(data2, None);
    print 'client read len = ' + str(len)
    print ''.join(data2)
    s.end()

t1 = threading.Thread(target=func1, args=(9989,))
t2 = threading.Thread(target=func2, args=('127.0.0.1', 9989,))
t1.start()
t2.start()

t1.join()
t2.join()
