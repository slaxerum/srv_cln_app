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

int g_enable_slx_log = 1;
int g_log_level = SLX_LOG_LVL_DEFAULT;
FILE *slx_log_fptr = NULL;
char *g_rofs_logfile_path = {SLX_LOG_FILE};

char mmb_version[64] = "";
char cmdline[2048] = "";
int process_id = 0;
char *process_name = cmdline;

char *build_version()
{
	return "1.0.0";
}

char *log_level_to_str(int lvl)
{
	char *lv_str = "";

	switch (lvl)
	{
		case SLX_LOG_LVL_ERROR  : lv_str = "ERROR:  ";   break;
		case SLX_LOG_LVL_WARN   : lv_str = "WARN:   ";    break;
		case SLX_LOG_LVL_NOTICE : lv_str = "NOTICE: ";  break;
		default:                  lv_str = " ";			break;
	}

	return lv_str;
} 


void SLX_log_information_init()
{
	FILE *fp = NULL;
	slx_log_fptr = fopen(g_rofs_logfile_path, "a");
	
	if (slx_log_fptr != NULL) 
	{
		setvbuf(slx_log_fptr, NULL, _IONBF, 0);
	} 
	else 
	{
		g_log_level = -1;
		g_enable_slx_log = 0;
	}

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

void SLX_log_information_fp(int only_log_file, slx_error_t err, int lvl, const char* format, ...)
{
    char msg[1024] = {0};
	int len = 0;
    va_list args;
    time_t cur_time;
    struct tm tmbuf;
    char timestr[100] = "";

    cur_time = time(NULL);
    localtime_r(&cur_time, &tmbuf);
    snprintf(timestr, sizeof(timestr), "%4d-%02d-%02d %02d:%02d:%02d ",
			1900+tmbuf.tm_year, (tmbuf.tm_mon + 1),
			tmbuf.tm_mday,
			tmbuf.tm_hour, tmbuf.tm_min, tmbuf.tm_sec);
	
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);

    if (SLX_IS_ERROR(err))
    {
        if (SLX_IF_ERRNO(err))
        {
            int myerrno = SLX_ERROR_TO_ERRNO(err);
            
			len = (int)strlen(msg);
            if (myerrno >= 0) {
                snprintf(msg + len, sizeof(msg) - len, " (%s)", strerror(myerrno));
            } else {
                snprintf(msg + len, sizeof(msg) - len, " (%ju)", err);
            }
        }
		else
		{
			len = (int) strlen(msg);
			snprintf(msg + len, sizeof(msg) - len, " (%ju)", (uintmax_t)err);
		}
    }

	len = (int)strlen(msg);
	snprintf(msg + len, sizeof(msg) - len, "\n");

    if (lvl <= g_log_level)
    {
        FILE* log = g_enable_slx_log ? slx_log_fptr : stderr;
		pid_t tid = (pid_t) syscall(SYS_gettid);
		
		fprintf(log, "%s%s[%u]: [%u] %s%s",
				timestr, process_name, process_id,
				(int)tid, log_level_to_str(lvl), msg);
		
		if (!only_log_file)
			printf("%s%s[%u]: [%u] %s%s",
				timestr, process_name, process_id,
				(int)tid, log_level_to_str(lvl), msg);
	}
}
