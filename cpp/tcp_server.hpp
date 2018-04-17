#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_

#include "tcp_errors.h"

namespace spt_tcp {

class TCPServer
{
	public:
		TCPServer(void);
		~TCPServer(void);
		int start(int port);
		int read(char *output, int size, int usec);
		int write(const char *input, int size, int usec);
		void end(void);

	private:
		int serverFd,clientFd;
		char *buffer;
		char *clientAddr;
};


} //namespace spt_tcp

#endif//_TCP_SERVER_HPP_
