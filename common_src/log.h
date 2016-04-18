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
#include <stdint.h>

typedef uint64_t slx_error_t;


#define SLX_LOG_LVL_ERROR	0
#define SLX_LOG_LVL_WARN	1
#define SLX_LOG_LVL_NOTICE	2

#define SLX_LOG_LVL_DEFAULT	SLX_LOG_LVL_NOTICE
#define SLX_LOG_FILE "/var/log/slx.log"

extern int g_log_level;

#define SLX_TRUE 1
#define SLX_FALSE 0
#define SLX_GENERIC 0x00
#define SLX_SYSTEM_ERRNO 0x01
#define SLX_SYSTEM 0x02

#define SLX_SET_STATUS(status, value, source) 											\
	((status == SLX_FALSE ? 															\
		(slx_error_t)(0xc000000000000000ULL) : (slx_error_t)(0x0000000000000000ULL)) | 	\
		(((uint64_t)(source)) << 32) |													\
		((uint64_t)(value) & 0x00000000ffffffffULL))

#define SLX_OK				SLX_SET_STATUS(SLX_TRUE,	0, SLX_GENERIC)
#define SLX_ERROR			SLX_SET_STATUS(SLX_FALSE, 	1, SLX_GENERIC)
#define SLX_INET_PTON_ERR	SLX_SET_STATUS(SLX_FALSE, 	2, SLX_GENERIC)

#define SLX_BACKUP		SLX_SET_STATUS(SLX_TRUE,	0, SLX_SYSTEM)
#define SLX_RESTORE		SLX_SET_STATUS(SLX_TRUE, 	1, SLX_SYSTEM)
#define SLX_BACKUP_ERR	SLX_SET_STATUS(SLX_FALSE,	2, SLX_SYSTEM)
#define SLX_RESTORE_ERR	SLX_SET_STATUS(SLX_FALSE, 	3, SLX_SYSTEM)

#define SLX_IS_OK(e)      	((e) == SLX_OK)
#define SLX_IS_ERROR(e)   	(((e) & 0xc000000000000000ULL) != 0)
#define SLX_IS_NO_ERROR(e)	(((e) & 0xc000000000000000ULL) == 0)

#define SLX_SET_ERRNO(value) SLX_SET_STATUS(SLX_FALSE, value, SLX_SYSTEM_ERRNO) 
#define SLX_ERROR_SYSTEM(value, source) \
	((slx_error_t)(value) & 0xc000000000000000ULL && \
	 (slx_error_t)(value) & (((uint64_t)(source)) << 32))
	 
#define SLX_IF_ERRNO(value) \
	SLX_ERROR_SYSTEM(value, SLX_SYSTEM_ERRNO)
	 
#define SLX_ERROR_TO_ERRNO(val) \
	((uint64_t)(val) & 0x00000000ffffffffULL)

//logging
#define SLX_INFO_LOG(level, format, ...)										\
	SLX_INFO_LOG_MSG_FP(0, 0, level, format, ##__VA_ARGS__)	
	
#define SLX_INFO_ONLY_LOG(level, format, ...)										\
	SLX_INFO_LOG_MSG_FP(1, 0, level, format, ##__VA_ARGS__)
	
#define SLX_INFO_LOG_ERROR_CODE(err, level, format, ...)										\
	SLX_INFO_LOG_MSG_FP(0, err, level, format, ##__VA_ARGS__)

#define SLX_INFO_ERROR_LOG(format, ...) SLX_INFO_ONLY_LOG(SLX_LOG_LVL_ERROR, format, ##__VA_ARGS__)
#define SLX_INFO_WARN_LOG(format, ...) SLX_INFO_ONLY_LOG(SLX_LOG_LVL_WARN, format, ##__VA_ARGS__)
#define SLX_INFO_NOTICE_LOG(format, ...) SLX_INFO_ONLY_LOG(SLX_LOG_LVL_NOTICE, format, ##__VA_ARGS__)

#define SLX_INFO_ERROR(format, ...) SLX_INFO_LOG(SLX_LOG_LVL_ERROR, format, ##__VA_ARGS__)
#define SLX_INFO_ERROR_CODE(err, format, ...) SLX_INFO_LOG_ERROR_CODE(err, SLX_LOG_LVL_ERROR, format, ##__VA_ARGS__)
#define SLX_INFO_WARN(format, ...) SLX_INFO_LOG(SLX_LOG_LVL_WARN, format, ##__VA_ARGS__)
#define SLX_INFO_NOTICE(format, ...) SLX_INFO_LOG(SLX_LOG_LVL_NOTICE, format, ##__VA_ARGS__)

#define SLX_INFO_LOG_MSG_FP(fp, err, level, format, ...)							\
	do {																\
		if (level <= g_log_level)											\
			SLX_log_information_fp(fp, err, level, format, ##__VA_ARGS__);		\
	} while(0)

#define SLX_INFO_LOG_MSG_DONE_FP(fp, err, level, format, ...)						\
    do {                                                                \
        if (SLX_IS_ERROR(err)) {                                        \
            SLX_INFO_LOG_MSG_FP(fp, err, level, format, ##__VA_ARGS__);          	\
            goto done;                                                  \
        }                                                               \
    } while (0)

#define SLX_INFO_LOG_MSG_DONE(err, level, format, ...)							\
	SLX_INFO_LOG_MSG_DONE_FP(0, err, level, format, ##__VA_ARGS__)

#define SLX_ERROR_DONE(err, format, ...) SLX_INFO_LOG_MSG_DONE(err, SLX_LOG_LVL_ERROR, format, ##__VA_ARGS__)

char *build_version();
char *log_level_to_str(int lvl);
void SLX_log_information_fp(int only_log_file, slx_error_t err, int lvl, const char* format, ...);
void SLX_log_information_init();
#endif