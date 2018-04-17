
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcp_server.hpp"

namespace spt_tcp {

#define TCP_SERVER_BUFFER_MAX 4096
TCPServer::TCPServer(void)
{
	buffer = (char*)calloc(TCP_SERVER_BUFFER_MAX, sizeof(char));
	serverFd = -1;
	clientFd = -1;
}

TCPServer::~TCPServer(void)
{
	free(buffer);
}

int TCPServer::start(int port)
{
	struct sockaddr_in address;
	int opt = 1;
	if(serverFd != -1) return NR_ALREADY_STARTED;	
	if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{
		perror("socket failed");
		return NR_SOCKET_FAIL;
	}

	if(setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
	{
		perror("setsockopt");
		return NR_SOCKETOPT_FAIL;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);

	if(bind(serverFd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		return NR_BIND_FAIL;
	}

	if(listen(serverFd, 5) < 0)
	{
		perror("listen");
		return NR_LISTEN_FAIL;
	}

	struct sockaddr_in clientAddress;
	socklen_t sosize  = sizeof(clientAddress);
	clientFd = accept(serverFd,(struct sockaddr*)&clientAddress,&sosize);
	clientAddr = inet_ntoa(clientAddress.sin_addr);

	return NR_SUCCESS;
}

int TCPServer::read(char *output, int size, int usec= 0)
{
	if(clientFd == -1) return NR_EMPTY_CONNECTION;

	struct timeval timeout;      
	timeout.tv_sec = usec/1000;
	timeout.tv_usec = usec%1000;

	if(setsockopt(clientFd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
	{
		perror("setsockopt failed\n");
		return NR_SOCKETOPT_FAIL;
	}
	
	int len;
	len = recv(clientFd, buffer, TCP_SERVER_BUFFER_MAX, 0);
	if(len <= 0) {
		return NR_RCV_FAIL;	
	}

	if(output == NULL) return NR_PARAM_ERROR;
	memset(output, 0, size);
	memcpy(output, buffer, len);
	// TODO : retry receive
	if(len != size) return NR_RCV_FAIL;
	return NR_SUCCESS;
}

int TCPServer::write(const char *input, int size, int usec= 0)
{
	if(clientFd == -1) return NR_EMPTY_CONNECTION;
	
	struct timeval timeout;      
	timeout.tv_sec = usec/1000;
	timeout.tv_usec = usec%1000;

	if(setsockopt(clientFd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
	{
		perror("setsockopt failed\n");
		return NR_SOCKETOPT_FAIL;
	}

	int len;	
	len = send(clientFd, input, size, 0);
	// TODO : retry send
	if(len != size) return NR_SND_FAIL;
	return NR_SUCCESS;
}


void TCPServer::end(void)
{
	close(clientFd);
	clientFd = -1;
	close(serverFd);
	serverFd = -1;
}

} //namespace spt_tcp

