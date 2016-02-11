/*------------------------------------------------------------------------------
 *
 *      MMBackup INC. PROPRIETARY INFORMATION
 * This software is supplied under the terms of a license agreement or
 * nondisclosure agreement with MMBackup Inc. and may not
 * be copied or disclosed except in accordance with the terms of that
 * agreement.
 *
 * Copyright (c) 2010-2011 MMBackup Inc. All Rights Reserved.
 *------------------------------------------------------------------------------
 */

#ifndef _SLX_LOG_H
#define _SLX_LOG_H
#include <stdio.h>
 
#define SLX_LOG_LVL_EMERG	0
#define SLX_LOG_LVL_ALERT	1
#define SLX_LOG_LVL_CRIT	2
#define SLX_LOG_LVL_ERROR	3
#define SLX_LOG_LVL_WARN	4
#define SLX_LOG_LVL_NOTICE	5
#define SLX_LOG_LVL_INFO	6
#define SLX_LOG_LVL_DEBUG	7

#define SLX_OK				0
#define SLX_EGENERIC		1
#define SLX_IS_OK(e)      	((e) == SLX_OK)
#define SLX_IS_ERROR(e)   	(((e) & 0xc000000000000000ULL) != 0)

#define SLX_LOG_MSG(lvl, fmt, ...)										\
	SLX_log_message(lvl, fmt, ##__VA_ARGS__)
		
#define SLX_EMERG(fmt, ...) SLX_LOG_MSG(SLX_LOG_LVL_EMERG, fmt, ##__VA_ARGS__)
#define SLX_ALERT(fmt, ...) SLX_LOG_MSG(SLX_LOG_LVL_ALERT, fmt, ##__VA_ARGS__)
#define SLX_CRIT(fmt, ...) SLX_LOG_MSG(SLX_LOG_LVL_CRIT, fmt, ##__VA_ARGS__)
#define SLX_ERROR(fmt, ...) SLX_LOG_MSG(SLX_LOG_LVL_ERROR, fmt, ##__VA_ARGS__)
#define SLX_WARN(fmt, ...) SLX_LOG_MSG(SLX_LOG_LVL_WARN, fmt, ##__VA_ARGS__)
#define SLX_NOTICE(fmt, ...) SLX_LOG_MSG(SLX_LOG_LVL_NOTICE, fmt, ##__VA_ARGS__)
#define SLX_INFO(fmt, ...) SLX_LOG_MSG(SLX_LOG_LVL_INFO, fmt, ##__VA_ARGS__)
#define SLX_DEBUG(fmt, ...) SLX_LOG_MSG(SLX_LOG_LVL_DEBUG, fmt, ##__VA_ARGS__)

#define SLX_LOG_LVL_DEFAULT	SLX_LOG_LVL_NOTICE

char *log_level_str(int lvl);
void SLX_log_message(int lvl, const char* format, ...);
void SLX_log_message_init();
#endif