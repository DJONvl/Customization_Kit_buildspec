#ifndef __SPECAIL_SETTING_H__
#define __SPECAIL_SETTING_H__

#define SETUP_PACKET_LENGTH \
do{ \
    if(sysob_runtime_params.system_os == FT_SYSTEM_OS_LINUX) \
    { \
	    sysob_runtime_params.write_packet_length = 2*1024*1024; \
	    sysob_runtime_params.read_packet_length = 64*1024; \
    } \
	else \
	{ \
	    sysob_runtime_params.write_packet_length = 2*1024*1024; \
	    sysob_runtime_params.read_packet_length = 2*1024*1024; \
	} \
}while(0)
	
#endif	