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
 
#ifndef _SLX_USAGE_H
#define _SLX_USAGE_H

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <getopt.h>
#include <stdarg.h>

struct usage_param_t
{
	int		log_level;
	char	org_file[SLX_PATH];
	char	new_file[SLX_PATH];
};

typedef struct usage_param_t usage_param_t;

void parse_command_line(int argc, char *argv[]);
void print_usage();

#endif