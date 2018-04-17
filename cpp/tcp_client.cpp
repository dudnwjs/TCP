#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcp_client.hpp"

#define TCP_CLIENT_BUFFER_MAX 4096

namespace spt_tcp {

TCPClient::TCPClient(void)
{
	buffer = (char*)calloc(TCP_CLIENT_BUFFER_MAX, sizeof(char));
	socketFd = -1;
	port = 0;
}

TCPClient::~TCPClient(void)
{
	free(buffer);
}

int TCPClient::start(const char* addr, int port)
{
	struct sockaddr_in address;
	int opt = 1;
	if(socketFd != -1) return NR_ALREADY_STARTED;	
	if( (socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		return NR_SOCKET_FAIL;
	}
	
	if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
	{
		perror("setsockopt");
		return NR_SOCKETOPT_FAIL;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(port);

	if(connect(socketFd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("connect failed. Error");
		return NR_CONNECT_FAIL;
	}

  	return NR_SUCCESS;
}

int TCPClient::read(char *output, int size, int usec= 0)
{
	if(socketFd == -1) return NR_EMPTY_CONNECTION;

	struct timeval timeout;      
	timeout.tv_sec = usec/1000;
	timeout.tv_usec = usec%1000;

	if(setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
	{
		perror("setsockopt failed\n");
		return NR_SOCKETOPT_FAIL;
	}

	memset(&buffer[0], 0, sizeof(buffer));

	int len;
	len = recv(socketFd, buffer, TCP_CLIENT_BUFFER_MAX, 0);
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

int TCPClient::write(const char *input, int size, int usec= 0)
{
	if(socketFd == -1) return NR_EMPTY_CONNECTION;

	struct timeval timeout;      
	timeout.tv_sec = usec/1000;
	timeout.tv_usec = usec%1000;

	if(setsockopt(socketFd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
	{
		perror("setsockopt failed\n");
		return NR_SOCKETOPT_FAIL;
	}

	int len;	
	len = send(socketFd, input, size, 0);
	// TODO : retry send
	if(len != size) return NR_SND_FAIL;
	return NR_SUCCESS;
}

void TCPClient::end(void)
{
	close(socketFd);
	socketFd = -1;
}

}//namespace spt_tcp

