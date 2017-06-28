
#ifndef __PROTOCOL_FORMAT__
#define __PROTOCOL_FORMAT__

#include "sys/types.h"

#define MAGIC_PROTOCOL 0xFEEEEEEF

typedef enum
{
   DT_PROTOCOL_FLOW = 1,
   DT_MESSAGE = 2,
}data_type_t;

typedef struct
{  
   uint32 guard_magic;     
   uint32 type;  //protocol or massage.   
}protocol_header_t;

typedef struct
{  
   protocol_header_t phdr;
   uint32 length;  //length of data
}protocol_header_wrapper_t;

typedef struct
{  
   protocol_header_t phdr;
   uint32 length; //length of data
   uint32 priority;
}message_header_wrapper_t;


//just for format depiction.
typedef struct
{  
   message_header_wrapper_t hdr;; 
   uint8  data[1]; 
}protocol_data_t;

typedef struct
{  
   message_header_wrapper_t hdr; 
   uint8  data[1]; 
}message_data_t;



#endif


