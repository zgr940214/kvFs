#ifndef _FLASH_H_
#define _FLASH_H_
#include "nvs_config.h"

#if (NVS_USE_RAM_FLASH)
#define NVS_FLASH_DEV nvs_fs_ram_flash_dev;
#define NVS_FLASH_OPS nvs_fs_ram_flash_ops;
#endif

#endif
