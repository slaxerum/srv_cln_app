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
#include "global.h"
#include "usage.h"

const char* const short_options = "v:hf:n:b:"; 
static struct option long_options[] = {
	{"verbose",  	required_argument, 			0, 'v'},	
	{"help",     	no_argument,		      	0, 'h'},
	{"file",     	required_argument,      	0, 'f'},
	{"new_file", 	required_argument, 	 		0, 'n'},
	{"buffer_size", required_argument, 	 		0, 'b'},	
	{0,       0,	                			0, 0}
};

extern char *g_org_file;
extern char *g_new_file;
extern int g_log_level;
extern int g_buffer_size;

void print_usage()
{
	printf("Usage: %s options [ inputfile ... ]\n", SLX_PROGNAME_CLIENT);
	printf( " -h --help 		Display this usage information.\n"
			" -f --file 		File to proceed.\n"
			" -n --new_file		Output to file.\n"
			" -b --buffer_size	buffer size.\n"			
			" -v --verbose		Level [0-7]]\n");
}

void parse_command_line(int argc, char *argv[])
{	
	int next_option = 0;
	
	while (1)
	{
		int option_index = 0;		
		next_option = getopt_long(argc, argv, short_options, long_options, &option_index);
		
		if (next_option == -1)
			break;
		
		switch (next_option)
		{
			case 'v':
				g_log_level = atoi(optarg);
				break;
			case 'h':
				print_usage();
				exit(0);
				break;				
			case 'f':
				strcpy(g_org_file, optarg);
				break;
			case 'n':
				strcpy(g_new_file, optarg);
				break;				
			case 'b':
				g_buffer_size = atoi(optarg);
				break;					
			case '?':
				print_usage(); 
				exit(0);
				break;
			default:
				abort();
		}
	}
}