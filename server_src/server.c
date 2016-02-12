#include <stdio.h> 
#include "global.h"
#include "log.h"

int g_log_level = SLX_LOG_LVL_DEFAULT;

int main(int argc, char *argv[])
{	
	SLX_log_message_init();
	SLX_NOTICE("Begin - %s", SLX_PROGNAME_SERVER);
	SLX_NOTICE("End - Server BackMM", 0);
	
	return 0;
}
