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
#ifndef _SLX_MESSAGE_H
#define _SLX_MESSAGE_H

#include "log.h"

#define SLX_ECHO_TEXT_LEN	100
#define SLX_NAME_AUTH		20
#define SLX_PASS_AUTH		20
#define SLX_AUTH_VERSION	20

enum slx_mscode {
	SLX_NO_OP = 0,
	SLX_ECHO = 0 + 1,
	SLX_AUTH = 0 + 2
};
typedef enum slx_mscode slx_mscode;

#pragma pack(1)

struct slx_req_header_t
{
	slx_mscode message_code;
	slx_error_t status;
};
typedef struct slx_req_header_t slx_req_header_t;

struct slx_res_header_t
{
	slx_mscode message_code;
	slx_error_t status;
};
typedef struct slx_res_header_t slx_res_header_t; 

//SLX_ECHO
struct slx_req_echo_t
{
	char echo_req_text[SLX_ECHO_TEXT_LEN];
};
typedef struct slx_req_echo_t slx_req_echo_t;

struct slx_replay_echo_t
{
	char echo_replay_text[SLX_ECHO_TEXT_LEN];
};
typedef struct slx_replay_echo_t slx_replay_echo_t;
//SLX_AUTH
struct slx_req_auth_t
{
	char name[SLX_NAME_AUTH];
	char pass[SLX_PASS_AUTH];
};
typedef struct slx_req_auth_t slx_req_auth_t;

struct slx_replay_auth_t
{
	char version[SLX_AUTH_VERSION];
};
typedef struct slx_replay_auth_t slx_replay_auth_t;

#pragma pack(0)

#endif