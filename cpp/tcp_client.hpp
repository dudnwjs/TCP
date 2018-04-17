#ifndef _TCP_CLIENT_HPP_
#define _TCP_CLIENT_HPP_

#include "tcp_errors.h"

namespace spt_tcp {

class TCPClient
{
	private:
		int socketFd;
		int port;
		char *buffer;

	public:
		TCPClient(void);
		~TCPClient(void);
		int start(const char* addr, int port);
		int read(char *output, int size, int usec);
		int write(const char *input, int size, int usec);
		void end(void);
};

} //namespace spt_tcp

#endif//_TCP_CLIENT_HPP_
