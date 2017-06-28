#ifndef _EFUSE_STRUCT_H_
#define _EFUSE_STRUCT_H_

#include "type_define.h"

#ifndef EFUSE_ENABLE
#define EFUSE_ENABLE	0xA59914B3
#endif
#ifndef EFUSE_DISABLE
#define EFUSE_DISABLE    0x241C8F6D
#endif

//the same as PRELOADER, please not changed!!!
#define EFUSE_SBC_HASH_KEY_E  "010001"

enum efuse_extra_key
{
   EXTRA_C_LOCK,
   EXTRA_C_CTRL1_LOCK ,
   EXTRA_C_CTRL0_LOCK,
   EXTRA_C_DATA0_LOCK,
   EXTRA_C_DATA1_LOCK,
   EXTRA_C_DATA2_LOCK,
   EXTRA_C_DATA3_LOCK,
   EXTRA_C_DATA4_LOCK,
   EXTRA_C_DATA5_LOCK,
   
   EXTRA_CTRL1,
   EXTRA_SEC_CTRL1,

   EXTRA_C_CTRL_0,
   EXTRA_C_CTRL_1,
   EXTRA_C_DATA_0,
   EXTRA_C_DATA_1,
   EXTRA_C_DATA_2,
   EXTRA_C_DATA_3,
   EXTRA_C_DATA_4,
   EXTRA_C_DATA_5,

   EXTRA_SEC_CAP,
   EXTRA_PROD_EN,

   EXTRA_CUSTK,

   EXTRA_END_KEY
};

enum efuse_data_type
{
   TYPE_BOOLEAN = 1,
   TYPE_INT,
   TYPE_BUF,
   TYPE_PERCENT
};

struct _buffer_small
{
   unsigned int buf_len;
   char buf[64];
};

struct _buffer_large
{
   unsigned int buf_len;
   char buf[1024];
};

// Secure Key structure
// key string char array buffer with NULL character in the end
typedef struct _buffer_small key_buf_st_t;
typedef struct _buffer_small keyhash_value_t;
typedef struct _buffer_small spare_data_t;

struct publickey_string_st
{
   unsigned int key_type; //0, legacy  1, pss
   struct _buffer_large key_e;
   struct _buffer_large key_n; 
};

union efuse_extra_data
{
   struct
   {
      unsigned int key_blow;
      key_buf_st_t key;
   }key_pair;
   unsigned int enable;
   struct
   {
      unsigned int  value;
      unsigned int  blow;
   }ipair;
};

struct efuse_item
{
   unsigned int key;
   unsigned int type;
   union efuse_extra_data data;
};

// Argument for writing Efuse common register
struct common_ctrl
{
   unsigned int    emmc_boot_dis;
   unsigned int    nand_boot_dis;
   unsigned int    nand_boot_speedup_dis;

   unsigned int    usbdl_type_blow;
   unsigned int    usbdl_type; // valid value is only 0~3 for 2 bit

   unsigned int    usb_id_blow;
   unsigned short  usb_pid;    // 16 bit
   unsigned short  usb_vid;    // 16 bit

   unsigned int    spare_blow;
   unsigned int    disable_sbc_pub_hash; //-1
   unsigned int    disable_sbc_pub_hash1; //-1
};

typedef struct efuse_common_arg
{
   struct common_ctrl ctrl;
   spare_data_t       spare;
   unsigned int    pid_vid_custom_en;
   unsigned int    ufs_boot_dis; //-1
   unsigned int    ext_reserved[29]; //32-1-1-1
}efuse_common_arg_t;

// Argument for writing Efuse security register

struct secure_ctrl
{
   unsigned int    acc_en;
   unsigned int    ack_en;
   unsigned int    sbc_en;
   unsigned int    daa_en;
   unsigned int    sla_en;
   unsigned int    root_cert_en;
   unsigned int    jtag_dis;

   unsigned int    ac_key_blow;
   unsigned int    pl_ar_en; //-1
   unsigned int    pk_cus_en; //-1
};

#define SBC_PUBK_HASH_MAX_NUM 2

struct sbc_pubk_hash_info
{
   unsigned int    sbc_pubk_blow;
   union
   {
      struct publickey_string_st w_sbc_pub_key;  
      keyhash_value_t r_sbc_pub_key_hash;  // return sbc_public_key hash value when using Brom_WriteEfuseAll()
   }sbc_pub_key_u;
};

typedef struct efuse_secure_arg
{
   struct secure_ctrl ctrl;
   key_buf_st_t        ac_key;         // ACK length 128 bit = 16 byte

   struct sbc_pubk_hash_info hash[2];
   unsigned int 	jtag_sw_con;
   unsigned int    rom_cmd_dis;
   unsigned int    dbgport_lock_dis;
   unsigned int    md1_sbc_en; //-1
   unsigned int    c2k_sbc_en; //-1
   unsigned int    dxcc_kcst_en; //-1 
   unsigned int    reserved[58]; //64-1-1-1-1-1-1-1
}efuse_secure_arg_t;

// Argument for locking Efuse register
struct lock_ctrl
{
    unsigned int common_ctrl_lock;
    unsigned int usb_id_lock;
    unsigned int spare_lock;
    unsigned int sec_ctrl_lock;
    unsigned int ackey_lock;
    unsigned int sbc_pubk_hash_lock;
    unsigned int sec_msc_lock;  //-1
    unsigned int custk_lock; //-1
    unsigned int sbc_pubk_hash1_lock;  //-1
};

typedef struct efuse_lock_arg
{
   // lock =  0xA59914B3
   // no-action = 0x241C8F6D  (can't unlock)
   struct lock_ctrl ctrl;
   unsigned int reserved[29];//32-1-1-1
}efuse_lock_arg_t;

typedef struct stb_key_param
{
   unsigned int key_blow;
   char key_name[32];
   key_buf_st_t  stb_key;
}stb_key_param_t;

typedef struct stb_lock_param
{
   unsigned int stb_key_g7_lock;
   unsigned int stb_key_g6_lock;
   unsigned int stb_key_g5_lock;
   unsigned int stb_key_g4_lock;
   unsigned int stb_key_g3_lock;
   unsigned int stb_key_g2_lock;
   unsigned int stb_key_g1_lock;
   unsigned int stb_key_g0_lock;
   unsigned int stb_key_operatorid_lock;
   unsigned int stb_key_chipid_lock;
   unsigned int stb_key_sn_lock;
}stb_lock_param_t;

// Argument for STB Key Efuse register
struct stb_key_ctrl 
{
   unsigned int stb_key_chipid_blow;
   unsigned int stb_key_chipID;
   unsigned int stb_key_chipid_spr;
   unsigned int stb_key_operatorid_blow;
   unsigned int stb_key_operatorid;
   unsigned int stb_key_operatorid_spr;
};
typedef struct efuse_stb_key_arg
{
   struct stb_lock_param stb_lock;
   struct stb_key_ctrl ctrl;
   struct stb_key_param stb_blow_keys[16];
}efuse_stb_key_arg_t;

typedef struct efuse_extra_arg
{
   struct efuse_item  items[EXTRA_END_KEY];
   unsigned int item_count;
}efuse_extra_arg_t;

typedef struct vcore_item
{
   unsigned int v_value;
   enum efuse_data_type type;
}vcore_item_t;

typedef struct write_option
{
   unsigned int v_fsource;
   struct vcore_item v_core;
   unsigned int v_io18;
   uint16 enable_ecc;
}write_option_t;

typedef struct verify_option
{
   struct vcore_item v_core[8];
   uint32 item_count;
   uint16 flip_ecc;
}verify_option_t;

typedef struct extra_item
{
    uint32 address;
    uint32 value;
}extra_item_t;

typedef struct extra_option
{
   struct extra_item items[16];
   uint32 item_count;
}extra_option_t;

typedef struct read_option
{
   unsigned int v_fsource;
   unsigned int v_core;
   unsigned int v_io18;
}read_option_t;

typedef struct debug_option
{
   unsigned int lock_voltage;
   uint16 allow_empty_write;
   uint16 ignore_broken_mark;
   uint16 verify_only;
}debug_option_t;


//efuse arguments are for users
typedef struct efuse_argument
{
   struct efuse_common_arg common_arg;
   struct efuse_secure_arg  secure_arg;
   struct efuse_lock_arg  lock_arg;
   struct efuse_stb_key_arg stb_key_arg;
   struct efuse_extra_arg extra_arg;
}efuse_argument_t;

//following arguments are for yield test.
typedef struct yield_argument
{
   write_option_t write;
   read_option_t read;
   verify_option_t verify;
   debug_option_t debug;
   extra_option_t extra;
}yield_argument_t;



#endif
