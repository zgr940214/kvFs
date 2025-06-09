#ifndef _NVS_FS_DATA_BLOCK_H_
#define _NVS_FS_DATA_BLOCK_H
#include <stdint.h>
#include "nvs_fs.h"


typedef struct {
    uint32_t phys_blk_id;
    uint32_t free_size;
    uint32_t data_offset;
} nvs_fs_dblock_t;

/**
 * @brief Converts logical block id to physical block id
 */
int dblk_lo_to_phys_id(nvs_fs_context_t* ctx, uint32_t id) {
    int ret = 0;
    nvs_fs_dblock_t blk_meta;

    ctx->ops->read(ctx->cfg, ctx->header.active_metablk, );
}

/**
 * @brief write phys_blk
 */
int compact_dblk();


#endif