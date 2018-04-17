#include <iostream>
#include<thread>
#include <unistd.h>
#include "tcp_server.hpp"
#include "tcp_client.hpp"

using namespace std;
using namespace spt_tcp;

void func1(int val) { 
	TCPServer s;
	char buf[128];
	cout << 6 << endl;
	s.start(9989);
	cout << 7 << endl;
	s.read(buf, 128, 0);
	cout << 8 << endl;
	cout << "buf <" << buf << ">" << endl;
	cout << 9 << endl;
	s.write((char*)"server - write",sizeof("server - write"),0);
	s.end();
} 

void func2(int val) { 
	TCPClient c;
	char buf[128];
	cout << 1 << endl;
	c.start((char*)"127.0.0.1", 9989);
	cout << 2 << endl;
	c.write((char*)"client-write",sizeof("client-write"),0);
	cout << 3 << endl;
	c.read(buf, 128, 0);
	cout << 4 << endl;
	cout << "buf <" << buf << ">" << endl;
	cout << 5 << endl;
	c.end();
} 

int main() { 
	thread t1(&func1,1); 
	sleep(1);
	thread t2(&func2,2); 
	t1.join(); 
	t2.join(); 
	return 0; 
}

