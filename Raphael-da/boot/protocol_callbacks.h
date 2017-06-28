#ifndef __PROTOCOL_CALLBACKS_H__
#define __PROTOCOL_CALLBACKS_H__

typedef unsigned int (*CB_OP_PROGRESS)(void* user_arg, unsigned int progress, unsigned int* stop_flag);
struct progress_cb
{
   void* user_arg;
   CB_OP_PROGRESS cb;
};

 extern unsigned int cb_op_progress(void* user_arg, unsigned int progress, unsigned int* stop_flag);

#endif
