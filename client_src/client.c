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
#include <stdlib.h>
#include <stdint.h>
#include "global.h"
#include "usage.h"
#include "log.h"

typedef uint64_t slx_error_t;
char *g_org_file = NULL;
char *g_new_file = NULL;
int g_log_level = SLX_LOG_LVL_DEFAULT;
int g_buffer_size = SLX_BUFFER_SIZE;
struct backup_item_t backup_items[SLX_MAX_BACKUP_ITEMS];
int g_backup_item_count = 0;

void clean_the_mess()
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
}

int main(int argc, char *argv[])
{
	slx_error_t err = SLX_OK;
	
	//allocate memory 
	g_org_file = (char*) malloc(sizeof(char)*SLX_PATH);
	g_new_file = (char*) malloc(sizeof(char)*SLX_PATH);
	
	//parse command line
	parse_command_line(argc, argv);
	
	SLX_log_message_init();
	SLX_NOTICE("Begin - %s", SLX_PROGNAME);
	SLX_NOTICE("***************************************************");
	SLX_NOTICE("Original file	: %s", g_org_file);
	SLX_NOTICE("New file		: %s", g_new_file);
	SLX_NOTICE("Buffer size		: %d", g_buffer_size);
	
	err = read_split_file(g_org_file);
	err = merge_split_file(g_new_file);
	
	if (SLX_IS_OK(err))
		SLX_NOTICE("*** Successfully ***", 0);
	
	//clear
	free(g_org_file);
	free(g_new_file);
	clean_the_mess();
	
	SLX_NOTICE("End - Client BackMM", 0);
	
	return err;
}

