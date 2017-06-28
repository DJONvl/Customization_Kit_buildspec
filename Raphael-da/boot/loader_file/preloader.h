#pragma once

#include "sys/types.h"

#define MAX_CELL_COUNT 6
typedef struct data_cell_t
{
   uint64 at_address;
   uint8* data;
   uint32 length;
}data_cell_t;

typedef struct scatter_data_t
{
   uint32 count;
   data_cell_t cell[MAX_CELL_COUNT];
}scatter_data_t;

/*
input:  in/ data: origin preloader data,
in/ psz: data size, origin preloader size
in out/ scatter: new preloader data ptr list.
return: STATUS_PRELOADER_INVALID: not a valid preloader. or STATUS_OK .
*/
status_t create_preloader_mmc_hdr(uint8* data, uint32 size, /*IN OUT*/scatter_data_t* scatter);
status_t create_preloader_nand_hdr(uint8* data, uint32 size, /*IN OUT*/scatter_data_t* scatter);
status_t create_preloader_ufs_hdr (uint8* data, uint32 size, /*IN OUT*/scatter_data_t* scatter);


/*
input:  void
return: the correct address of preloader data.
*/
uint32 get_correct_mmc_preloader_address();
uint32 get_correct_ufs_preloader_address();


/*
Because SRAM do not has much memory to contain whole preloader. so provide this function.
input:  in/ how: true, preloader data itself and ext(if have) is also in 'scatter' returned by create_preloader_mmc_hdr() ,
false, scatter do not contain preloader data and ext.
return: void
*/
void set_only_preloader_hdr(bool how);
void set_only_ufs_preloader_hdr(bool how);


/*
input:  in/ preloader_ptr:  preloader data buffer to release. 
the value is 'scatter' that returned by process_preloader();

return: void.
*/
void release_scatter_data(scatter_data_t* scatter);



