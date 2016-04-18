/*------------------------------------------------------------------------------
 *
 *      SlaxerumBakcup INC. PROPRIETARY INFORMATION
 * This software is supplied under the terms of a license agreement or
 * nondisclosure agreement with SlaxerumBakcup Inc. and may not
 * be copied or disclosed except in accordance with the terms of that
 * agreement.
 *
 * Copyright (c) 2010-2011 SlaxerumBakcup Inc. All Rights Reserved.
 *------------------------------------------------------------------------------
 */

#include <stdio.h> 
#include <stdint.h>
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
#include "usage.h"
#include "log.h"

char *g_org_file = NULL;
char *g_new_file = NULL;
int g_buffer_size = SLX_BUFFER_SIZE;
struct backup_item_t backup_items[SLX_MAX_BACKUP_ITEMS];
int g_backup_item_count = 0;
int g_port = 5556;
const char clientTestMessage[] = {"SLX Client Test Message"};

/*void clean_the_mess()
{
	int i = 0;
	
	for(i = 0; i <= g_backup_item_count; i++)
	{
		free(backup_items[i].item_data);
	}
}

slx_error_t merge_split_file(char *path)
{
	FILE *pFile = NULL;
	
	SLX_NOTICE("Try open copy \"%s\" file ... ", path);
	pFile = fopen(path, "ab");
	
	if (pFile != NULL)
	{
		int i = 0;
		
		for(i = 0; i <= g_backup_item_count; i++)
		{
			SLX_INFO("Item [%d]=item_size %d", i, backup_items[i].item_size);
			fwrite(backup_items[i].item_data , sizeof(char), backup_items[i].item_size, pFile);
		}
		
		SLX_NOTICE("File \"%s\" merged successfully", path);
		fclose(pFile);
		return SLX_OK;
	}
	else
	{
		SLX_NOTICE("Unable open file %s", path);
		return SLX_EGENERIC;
	}
}

slx_error_t read_split_file(char *org_file_path)
{
	FILE *pFile = NULL;
	int index = 0;
	
	SLX_NOTICE("Try open \"%s\" file ... ", org_file_path);
	pFile = fopen(org_file_path, "rb");
	
	if (pFile != NULL)
	{
		long lSize = 0;
		long lSizeRead = 0;
		long ltmpRead = 0;
		char *buffer = NULL;
		SLX_INFO("File opened ... ");
		
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		
		SLX_INFO("File size = %ld", lSize);
		
		buffer = (char*) malloc(sizeof(char)*g_buffer_size);
		
		while(g_buffer_size == (ltmpRead = fread(buffer, 1, g_buffer_size, pFile)))
		{
			SLX_INFO("Data read = %ld", ltmpRead);
			lSizeRead += ltmpRead;
			backup_items[index].item_size = ltmpRead;
			backup_items[index].item_data = buffer;
			index++;
			buffer = (char*) malloc(sizeof(char)*g_buffer_size);
		}

		if (feof(pFile))
		{
			lSizeRead += ltmpRead;
			g_backup_item_count = index;
			SLX_INFO("Data read = %ld", ltmpRead);
			backup_items[index].item_size = ltmpRead;
			backup_items[index].item_data = buffer;			
			SLX_INFO("End-of-File reached.");
			SLX_INFO("Total number of bytes read: %ld", lSizeRead);
			SLX_INFO("Backup items = %ld", g_backup_item_count);
			fclose(pFile);
			if (lSizeRead == lSize)
			{
				SLX_NOTICE("File readed successfully");
				return SLX_OK;
			}
			else
			{
				SLX_NOTICE("File readed incorrectly");
				return SLX_EGENERIC;
			}
		}
		return SLX_OK;
	}
	else
	{
		SLX_NOTICE("Unable open file %s", org_file_path);
		return SLX_EGENERIC;
	}
}

slx_error_t merge_split_file_from_read(char *path, char *buffer, long buffer_size)
{
	FILE *pFile = NULL;
	
	SLX_INFO("Try open copy \"%s\" file ... ", path);
	pFile = fopen(path, "ab");
	if (pFile != NULL)
	{
		fwrite(buffer , sizeof(char), buffer_size, pFile);
		fclose(pFile);
		return SLX_OK;
	}
	else
	{
		SLX_INFO("Unable open file %s", path);
		return SLX_EGENERIC;
	}
}

slx_error_t read_split_file_and_merge(char *org_file_path, char *new_file_path)
{
	FILE *pFile = NULL;
	
	SLX_INFO("Try open \"%s\" file ... ", org_file_path);
	pFile = fopen(org_file_path, "rb");
	
	if (pFile != NULL)
	{
		long lSize = 0;
		long lSizeRead = 0;
		long ltmpRead = 0;
		char *buffer = NULL;
		SLX_INFO("File opened ... ");
		
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		
		SLX_INFO("File size = %ld", lSize);
		
		buffer = (char*) malloc(sizeof(char)*g_buffer_size);
		
		while(g_buffer_size == (ltmpRead = fread(buffer, 1, g_buffer_size, pFile)))
		{
			SLX_INFO("Data read = %ld", ltmpRead);
			lSizeRead += ltmpRead;
			merge_split_file_from_read(new_file_path, buffer, g_buffer_size);
			memset(buffer, 0, g_buffer_size);
		}

		if (feof(pFile))
		{
			lSizeRead += ltmpRead;
			merge_split_file_from_read(new_file_path, buffer, ltmpRead);
			SLX_INFO("End-of-File reached.");
			SLX_INFO("Total number of bytes read: %ld", lSizeRead);
			SLX_INFO_NO_NEW_LINE("Sanity check ... ");
			fclose(pFile);
			if (lSizeRead == lSize)
			{
				SLX_INFO("OK ");
				return SLX_OK;
			}
			else
			{
				SLX_INFO("File readed incorrectly");
				return SLX_EGENERIC;
			}
		}
		return SLX_OK;
	}
	else
	{
		SLX_INFO("Unable open file %s", org_file_path);
		return SLX_EGENERIC;
	}
}*/

int main(int argc, char *argv[])
{
	slx_error_t err = SLX_OK;
    int connfd = 0;
    char recvBuff[1025];
	char sendBuff[1025];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '\0',sizeof(recvBuff));
	memset(sendBuff, '\0',sizeof(sendBuff));
    memset(&serv_addr, '0', sizeof(serv_addr));	
	strcpy(sendBuff, clientTestMessage);
	
	SLX_log_information_init();
	SLX_INFO_NOTICE("Begin - %s ver. %s", SLX_PROGNAME_CLIENT, build_version());
	
    if((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
		err = SLX_SET_ERRNO(errno);
		SLX_ERROR_DONE(err, "Unable to create client socket, socket: %d", connfd);
    } 

	SLX_INFO_NOTICE_LOG("Client socket created, socket: %d", connfd);
	 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(g_port); 

    if((err = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)) <= 0)
    {
		if (err == 0)
		{
			err = SLX_INET_PTON_ERR;
			SLX_ERROR_DONE(err, "inet_pton - src does not contain a character string representing a valid network address in the specified address family");
		}
		else
		{
			err = SLX_SET_ERRNO(errno);
			SLX_ERROR_DONE(err, "Client inet_pton error occurred, cannot convert IPv4 and IPv6 addresses");
		}
    } 
	
	SLX_INFO_NOTICE_LOG("Client inet_pton convert IPv4/IPv6");

    if(connect(connfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
		err = SLX_SET_ERRNO(errno);
		SLX_ERROR_DONE(err, "Client connect failed");	   
    }
	
	SLX_INFO_NOTICE("Client connect successfully");

    while(1)
    {
        if(send(connfd, sendBuff, strlen(sendBuff), 0) < 0)
        {
			err = SLX_SET_ERRNO(errno);
			SLX_ERROR_DONE(err, "Unable to send data to server");
        }
         
		SLX_INFO_NOTICE_LOG("Data sended to server successfully");

        if( recv(connfd, sendBuff, 1025, 0) < 0)
        {
			err = SLX_SET_ERRNO(errno);
			SLX_ERROR_DONE(err, "Unable to receive respond from server");
        }
		
        SLX_INFO_NOTICE_LOG("Server reply : \"%s\"", sendBuff);
		break;
    }
	
done:
	close(connfd);
	if (SLX_IS_ERROR(err))
		SLX_INFO_ERROR_CODE(err, "Error occurred during connection.");
	SLX_INFO_NOTICE("End - Client BackMM", 0);
	
	return 0;
}

