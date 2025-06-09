#ifndef NVS_FILESYSTEM_H_
#define NVS_FILESYSTEM_H_
#include <stdint.h>

#include "nvs_config.h"

#include "nvs_fs_mblock.h"
#include "nvs_fs_dblock.h"

/**
 * Flash Driver layer context
 */
typedef struct {
    const void* FLASHDEV;
    uint32_t blk_size;
    uint16_t n_blk;
    uint16_t dblk_start;
    
    uint16_t file_max_num;
    uint16_t file_max_size;
} nvs_fs_flash_config_t;

/**
 *  Flash Driver ops
 */
typedef struct {
    int (*init) (nvs_fs_flash_config_t* cfg);
    int (*flush) (nvs_fs_flash_config_t* cfg, uint32_t scratch_id);
    int (*prog) (nvs_fs_flash_config_t* cfg, uint32_t block_id, uint32_t offset, uint8_t* data, uint32_t size);
    int (*read) (nvs_fs_flash_config_t* cfg, uint32_t block_id, uint32_t offset, uint8_t* data, uint32_t size);
    int (*erase) (nvs_fs_flash_config_t* cfg, uint32_t block_id, uint32_t size);
} nvs_fs_flash_ops_t;


/* exposed to application layer */
/**
 *  NVS FS context
 */
typedef struct {
    nvs_fs_flash_config_t* cfg;
    nvs_fs_flash_ops_t* ops;

    nvs_fs_mblk_header_t header;
    uint16_t activated_fid;
    uint16_t used_lblk;
 } nvs_fs_context_t;

 /**
  * File ID
  */
typedef uint64_t nvs_fs_fid_t;

/**
 * NVS FS File Info
 */
typedef struct {
    uint32_t curr_size;
    uint32_t max_size;
} nvs_fs_file_info_t;

/**
 *  File System public APIs
 */
int nvs_fs_init(nvs_fs_context_t* ctx);
int nvs_fs_get_file_info(nvs_fs_context_t* ctx, nvs_fs_fid_t fid, nvs_fs_file_info_t* file_info);
int nvs_fs_write(nvs_fs_context_t* ctx, nvs_fs_fid_t fid, uint8_t* data, uint16_t size);
int nvs_fs_read(nvs_fs_context_t* ctx, nvs_fs_fid_t fid, uint8_t* data, uint16_t size);
int nvs_fs_delete(nvs_fs_context_t* ctx, nvs_fs_fid_t fid);

#endif