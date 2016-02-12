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
#include "global.h"
#include "log.h"
#include <time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

extern int g_log_level;
char mmb_version[64] = "";
char cmdline[2048] = "";
int process_id = 0;
char *process_name = cmdline;

char *build_version()
{
	return "1.0.0";
}
 
char *log_level_str(int lvl)
{
	char *lvl_str = "";

	switch (lvl)
	{
		case SLX_LOG_LVL_EMERG  : lvl_str = "EMERG:  ";	break;
		case SLX_LOG_LVL_ALERT  : lvl_str = "ALERT:  ";   break;
		case SLX_LOG_LVL_CRIT   : lvl_str = "CRIT:   ";    break;
		case SLX_LOG_LVL_ERROR  : lvl_str = "ERROR:  ";   break;
		case SLX_LOG_LVL_WARN   : lvl_str = "WARN:   ";    break;
		case SLX_LOG_LVL_NOTICE : lvl_str = "NOTICE: ";  break;
		case SLX_LOG_LVL_INFO   : lvl_str = "INFO:   ";    break;
		case SLX_LOG_LVL_DEBUG  : lvl_str = "DEBUG:  ";   break;
		default:                  lvl_str = "";			break;
	}

	return lvl_str;
} 

void SLX_log_message(int new_line, int lvl, const char* format, ...)
{
	char msg[1024] = {0};
    va_list args;
    time_t cur_time;
    struct tm tmbuf;
    char timestr[100] = "";
	int len = 0;

	cur_time = time(NULL);
	localtime_r(&cur_time, &tmbuf);
	snprintf(timestr, sizeof(timestr), "%4d-%02d-%02d %02d:%02d:%02d ",
				1900+tmbuf.tm_year, (tmbuf.tm_mon + 1),
				tmbuf.tm_mday,
				tmbuf.tm_hour, tmbuf.tm_min, tmbuf.tm_sec);	
	
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);
	
	if (new_line)
	{
		len = (int)strlen(msg);
		snprintf(msg + len, sizeof(msg) - len, "\n");
	}
	
	if (lvl <= g_log_level && lvl != -1)
	{
		pid_t tid = (pid_t) syscall(SYS_gettid);
		fprintf(stdout, "%s%s[%u]: [%u] %s%s", timestr, process_name, process_id, tid, log_level_str(lvl), msg);
	}
	else if (lvl == -1)
	{
		fprintf(stdout, "%s", msg);
	}
}

void SLX_log_message_init()
{
	FILE *fp = NULL;

	snprintf(mmb_version, sizeof(mmb_version), "%s", build_version());

	process_id = getpid();

	fp = fopen("/proc/self/cmdline", "r");
	if (fp != NULL) 
	{
		fgets(cmdline, sizeof(cmdline), fp);
		if (strrchr(cmdline,'/')) process_name = strrchr(cmdline,'/') + 1;
		fclose(fp);
	}	
}