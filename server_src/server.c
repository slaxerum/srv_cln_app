#include <stdio.h> 
#include <netinet/in.h>
#include <time.h> 
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "global.h"
#include "log.h"

int g_port = 5556;
const char serverTestMessage[] = {"SLX Server Test Message"};

int main(int argc, char *argv[])
{	
	slx_error_t err = SLX_OK;
	struct sockaddr_in serv_addr; 
	int listenfd = 0;
	int connfd = 0;
	int read_size = 0;
	int write_size = 0;
	char sendBuff[1025];
	char recvBuff[1025];
	int flagON = 1;

	memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '\0', sizeof(sendBuff));
	memset(recvBuff, '\0', sizeof(recvBuff));
	strcpy(sendBuff, serverTestMessage);
	
	SLX_log_information_init();
	SLX_INFO_NOTICE("Begin - %s ver. %s", SLX_PROGNAME_SERVER, build_version());
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (listenfd == -1)
	{
		err = SLX_SET_ERRNO(errno);
		SLX_ERROR_DONE(err, "Unable to create client socket, socket: %d", listenfd);		
	}

	SLX_INFO_NOTICE_LOG("Server socket created, socket: %d", listenfd);
	
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flagON, sizeof(int)) == -1) 
	{
		err = SLX_SET_ERRNO(errno);
		SLX_INFO_WARN("Unable to set SO_REUSEADDR for socket %d", listenfd);
	}	
	
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(g_port); 	
	
	if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		err = SLX_SET_ERRNO(errno);
		SLX_ERROR_DONE(err, "Unable to bind server socket %d", listenfd);			
	}
	
	SLX_INFO_NOTICE_LOG("Server socket bind socket: %d", listenfd);
	listen(listenfd, 3);
	SLX_INFO_NOTICE_LOG("Waiting for incoming connections...");
	
    while(1)
    {
		struct sockaddr_in client_addr; 
		socklen_t clientaddr_size = sizeof(client_addr);
		memset(&client_addr, '0', sizeof(client_addr));
		char strClientAddress[INET_ADDRSTRLEN];
		
		connfd = accept(listenfd, (struct sockaddr*) &client_addr, &clientaddr_size); 

		if (connfd < 0)
		{
			err = SLX_SET_ERRNO(errno);
			SLX_ERROR_DONE(err, "New connections accept failed");			
		}
		
		if (inet_ntop(AF_INET, &client_addr.sin_addr, strClientAddress, INET_ADDRSTRLEN ) == NULL)
		{
			err = SLX_SET_ERRNO(errno);
			SLX_ERROR_DONE(err, "Unable to determine new connection host details");	
		}
		
		SLX_INFO_NOTICE("New connection accepted from %s", strClientAddress);		
		
		read_size = recv(connfd, recvBuff, 1025, 0);
		
		if (read_size < 0)
		{
			err = SLX_SET_ERRNO(errno);
			SLX_ERROR_DONE(err, "Unable to receive data from client");			
		}

		SLX_INFO_NOTICE_LOG("Client message : \"%s\"",recvBuff);
		SLX_INFO_NOTICE_LOG("Respond to client : \"%s\"",sendBuff);
        write_size = write(connfd, sendBuff, strlen(sendBuff));
		
		if (write_size != (int)strlen(sendBuff))
		{
			err = SLX_SET_ERRNO(errno);
			SLX_ERROR_DONE(err, "Unable to send message to client");	
		}
		
		SLX_INFO_NOTICE("Communication between server and client ended correctly");	
    }
	
done:
	close(listenfd);
	if (SLX_IS_ERROR(err))
		SLX_INFO_ERROR_CODE(err, "Error occurred during connection.");	
	SLX_INFO_NOTICE("End - Server BackMM", 0);
	
	return 0;
}
