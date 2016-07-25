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
#include "message.h"

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
	
		//SLX_INFO_NOTICE_LOG("Client message : \"%s\"",recvBuff);
		//SLX_INFO_NOTICE_LOG("Respond to client : \"%s\"",sendBuff);
		//decode message
		SLX_INFO_NOTICE("Size pocket size : \"%d\" \"%d\"",read_size, sizeof(slx_req_header_t) + sizeof(slx_req_echo_t));
		slx_req_header_t header;
		
		memcpy(&header, &recvBuff, sizeof(slx_req_header_t));
		SLX_INFO_NOTICE("header.message_code\t%d", header.message_code);
		SLX_INFO_NOTICE("header.status      \t%d", header.status);
		
		if (header.message_code == SLX_ECHO)
		{
			slx_req_echo_t echo;
			memset(&echo, '\0',sizeof(slx_req_echo_t));
			memcpy(&echo, *(&recvBuff) + sizeof(slx_req_header_t), sizeof(slx_req_echo_t));
			
			SLX_INFO_NOTICE("echo.echo_req_text \t\"%s\"", echo.echo_req_text);
		}
		else
			SLX_ERROR_DONE(err, "Unable to determine message header");	
		
		//prepare data to reply
		slx_res_header_t header_res;
		slx_replay_echo_t echo_res;
		
		unsigned char *buff_send = NULL;
		
		memset(&echo_res, '\0',sizeof(slx_res_header_t));
		memset(&header_res, '\0',sizeof(slx_replay_echo_t));
		
		header_res.message_code = SLX_ECHO;
		header_res.status = SLX_OK;
		strcpy(echo_res.echo_replay_text, serverTestMessage);
		
		buff_send = malloc(sizeof(slx_res_header_t) + sizeof(slx_replay_echo_t));
		memcpy(buff_send, &header_res, sizeof(slx_res_header_t));
		memcpy(buff_send + sizeof(slx_res_header_t), &echo_res, sizeof(slx_replay_echo_t));
		
		//write_size = write(connfd, sendBuff, strlen(sendBuff));
		write_size = write(connfd, buff_send, sizeof(slx_res_header_t) + sizeof(slx_replay_echo_t));
		
		//if (write_size != (int)strlen(sendBuff))
		if (write_size != (int)sizeof(slx_res_header_t) + sizeof(slx_replay_echo_t))
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
