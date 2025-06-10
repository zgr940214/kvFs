#ifndef _NVS_FS_DATA_BLOCK_H_
#define _NVS_FS_DATA_BLOCK_H
#include <stdint.h>
#include "nvs_fs.h"
#include "utils.h"


typedef struct {
    uint32_t phys_blk_id;
    uint32_t free_size;
    uint32_t data_offset;
} nvs_fs_dblock_t;

static char buf[NVS_FLASH_MINIAML_PROGRAM_UNIT];

static uint32_t dblk_meta_offset(uint32_t lo) {
    return sizeof(nvs_fs_mblk_header_t) + NVS_MAX_FILE_NUM * sizeof(nvs_fs_file_meta_t) + (2 + lo) * sizeof(nvs_fs_dblock_t);
}

static uint32_t file_meta_offset(uint32_t idx) {
    return sizeof(nvs_fs_mblk_header_t) + idx * sizeof(nvs_fs_file_meta_t);
}

static inline int read_file_meta(nvs_fs_context_t *ctx, uint32_t idx, nvs_fs_file_meta_t* file_meta) {
    int ret = 0;

    ret = ctx->ops->read(ctx->cfg, ctx->header.active_metablk, file_meta_offset(idx), file_meta, sizeof(nvs_fs_file_meta_t));
    if (ret) {
        // error read
    }
    return ret;
}

static inline int read_dblk_meta(nvs_fs_context_t *ctx, uint32_t phys_id, nvs_fs_dblock_t* dblk_meta) {
    int ret = 0;

    ret = ctx->ops->read(ctx->cfg, ctx->header.active_metablk, dblk_meta_offset(phys_id), dblk_meta, sizeof(nvs_fs_dblock_t));
    if (ret) {
        // error read
    }
    return ret;
}

static inline int write_dblk_meta(nvs_fs_context_t *ctx, uint32_t phys_id, nvs_fs_dblock_t *dblk_meta) {
    int ret = 0;

    ret = ctx->ops->prog(ctx->cfg, ctx->header.active_metablk, dblk_meta_offset(phys_id), dblk_meta, sizeof(nvs_fs_dblock_t));
};

/**
 * @brief Converts logical block id to physical block id
 */
static uint32_t dblk_lo_to_phys_id(nvs_fs_context_t* ctx, uint32_t lo) {
    int ret = 0;
    nvs_fs_dblock_t blk_meta;

    ctx->ops->read(ctx->cfg, ctx->header.active_metablk, dblk_meta_offset(lo), &blk_meta, sizeof(nvs_fs_dblock_t));
    return blk_meta.phys_blk_id;
}

static int dblk_move_data(nvs_fs_context_t* ctx, uint32_t dblk_from, uint32_t dblk_to, uint32_t offset, uint32_t size) {
    int ret = 0;
    uint32_t size_remain = size;
    uint32_t size_curr = 0;
    uint32_t size_moved = 0;
    uint32_t dblk_id0 = dblk_lo_to_phys_id(ctx, dblk_from);
    uint32_t dblk_id1 = dblk_lo_to_phys_id(ctx, dblk_to);
    
    ret = ctx->ops->erase(ctx->cfg, dblk_id1, FLASH_ERASE_SZ);
    if (ret) {
        // erase error
    }

    while(size_remain > 0) {
        size = size_remain >= NVS_FLASH_MINIAML_PROGRAM_UNIT ? NVS_FLASH_MINIAML_PROGRAM_UNIT : size_remain;
        size_remain -= NVS_FLASH_MINIAML_PROGRAM_UNIT;
        
        ret = ctx->ops->read(ctx->cfg, dblk_id0, size_moved, buf, size_curr);
        if (ret) {
            // read error
        }

        ret = ctx->ops->prog(ctx->cfg, dblk_id1, size_moved, buf, size_curr);
        if (ret) {
            // prog error
        }
        size_moved += size_curr;
    }
    
    return size_moved;
}


static int metablk_copy_rest_to_scratch(nvs_fs_context_t *ctx, uint32_t dblk_id) {
    int ret = 0;
    nvs_fs_dblock_t dblk_meta;
    
    for (uint16_t i = 0; i < dblk_id; i++) {
        if (i == dblk_id) { // copy rest --- skip target
            continue;
        }

        ret = read_dblk_meta(ctx, ctx->header.active_metablk, &dblk_meta);
        if (ret) {
            // read error
        }

        ret = write_dblk_meta(ctx, ctx->header.scratch_meta_blk_id, &dblk_meta);
        if (ret) {
            // prog error
        }
    }

    return 0;
}

/**
 * update one data block meta & copy result to scratch meta block :
 * (write modified block meta -> scratch meta block & copy rest of -> scratch meta block)
 */
int metablk_update_scratch_dblk_meta(nvs_fs_context_t *ctx, nvs_fs_dblock_t *dblk_meta, uint32_t dblk_id) {
    int ret = 0;

    // prog target block meta to scratch meta block
    ret = write_dblk_meta(ctx, ctx->header.scratch_meta_blk_id, dblk_meta);
    if (ret) {
        // error write
    }

    // prog rest of block meta to scratch meta block
    return metablk_copy_rest_to_scratch(ctx, dblk_id);
}

/**
 * @brief each time we delete an exist file or write new val to the tail of that blk, we need to compact the dblk.
 */
int compact_dblk(nvs_fs_context_t *ctx, uint32_t dblk_id, uint32_t offset, uint32_t size) {
    int ret = 0;
    nvs_fs_file_meta_t file_meta;
    nvs_fs_dblock_t dblk_meta;

    ret = read_dblk_meta(ctx, dblk_id, &dblk_meta);
    if (ret) {
        // read error
    }
    dblk_meta.free_size += size;
    
    // data block compaction
    for (int i = 0; i < NVS_MAX_FILE_NUM; i++) {
        ret = read_file_meta(ctx, i, &file_meta);
        if (ret) {
            // error read;
        }

        if (file_meta.lblk_id == dblk_id) {
            if (file_meta.lblk_offset < offset) { // before that hole
                ret = dblk_move_data(ctx, dblk_id, ctx->header.scratch_blk_id, file_meta.lblk_offset, file_meta.curr_size);
                if (ret) {
                    // error move data;
                }
            } else {
                ret = dblk_move_data(ctx, dblk_id, ctx->header.scratch_blk_id, file_meta.lblk_offset - offset, file_meta.curr_size);
                if (ret) {
                    // error move data;
                }
            }
        }
    }

};


#endif