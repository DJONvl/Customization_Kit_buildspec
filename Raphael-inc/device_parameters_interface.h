#pragma once
#include "type_define.h"

enum device_type_e
{
   DEVICE_CHARGER = 1,
   //DEVICE_EMMC,
};

#define DEVICE_PARAMETERS_MAGIC 0x4FADAAAA
#define TYPE_NAME_MAX_LEN 32

typedef struct device_charger
{
   char type_name[TYPE_NAME_MAX_LEN];
   uint32 i2c_id;
   uint32 i2c_address;

   uint32 wdt_reg_address;
   uint32 wdt_disable_value;
   uint32 wdt_value_mask;
   uint32 wdt_value_shift;

   uint32 enable_reg_address;
   uint32 enable_value;
   uint32 enable_value_mask;
   uint32 enable_value_shift;

   uint32 currentlimit_reg_address;
   uint32 currentlimit_value;
   uint32 currentlimit_value_mask;
   uint32 currentlimit_value_shift;

   uint32 vbatlimit_reg_address;
   uint32 vbatlimit_value;
   uint32 vbatlimit_value_mask;
   uint32 vbatlimit_value_shift;
}device_charger_t;

typedef struct device_parameters_hdr
{
   uint32 magic;
   uint32 total_length;
   uint32 device_type; //enum device_type_e
   uint32 reserved1;
   uint32 reserved2;
}device_parameters_hdr_t;

typedef struct device_charger_parameters
{
   device_parameters_hdr_t hdr;
   device_charger_t charger;
}device_charger_parameters_t;

//struct device_mmc_parameters
//{
//   device_parameters_hdr_t hdr;
//   device_mmc_t mmc;
//};