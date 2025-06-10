#ifndef _NVS_CONFIG_H_
#define _NVS_CONFIG_H_


/* Default Configuration */
#define FLASH_ERASE_SZ          (1u << 12) // 4KB
#define NVS_BLK_SIZE            (FLASH_ERASE_SZ) // same as FLASH_ERASE_SZ
#define NVS_BLK_MAX_NUM         64 
#define NVS_DBLK_MAX_NUM        (NVS_BLK_MAX_NUM - 3) // metablk 0/1 , scratch blk
#define NVS_ALIGNMENT           4
#define NVS_MAX_FILE_SIZE       NVS_BLK_SIZE
#define NVS_MAX_FILE_NUM        96
#define NVS_FLASH_SIZE          (1u << 18) // 256KB)
#define NVS_FLASH_MINIAML_PROGRAM_UNIT 256 // 256 Bytes
#define NVS_FLASH_ALIGNMENT     4  // bytes

// flash size check
static_assert(NVS_FLASH_SIZE >= (NVS_BLK_MAX_NUM * NVS_BLK_SIZE))


#define NVS_USE_RAM_FLASH   1
// #define NVS_USE_NOR_FLASH
// #define NVS_USE_NAND_FLASH

#endif