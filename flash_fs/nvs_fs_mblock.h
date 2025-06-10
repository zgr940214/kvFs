#ifndef _NVS_FS_META_BLOCK_H_
#define _NVS_FS_META_BLOCK_H_
#include <stdint.h>

#define NVS_META_BLOCK0 0
#define NVS_META_BLOCK0 1

typedef struct nvs_fs_context_t;

/**
 * MetaBlock Header 4 bytes alignment
 */

#define T1  \
    uint32_t crc32_val;             \
    uint32_t version;               \
    uint8_t scratch_meta_blk_id;    \
    uint8_t active_metablk;         \
    uint16_t scratch_blk_id;        

typedef struct {
    T1
#if (NVS_FLASH_ALIGNMENT > 4)
    uint8_t padding[sizeof(__attribute__(__aligned__(NVS_FLASH_ALIGNMENT)) struct{T1}) - 
                    sizeof(struct{T1})];
#endif
} nvs_fs_mblk_header_t;
#undef T1

/**
 * File MetaBlock
 */
#define T2  \
    nvs_fs_fid_t fid;       \
    uint16_t lblk_id;       \
    uint16_t lblk_offset;   \
    uint16_t curr_size;     \
    uint16_t max_size;


typedef struct {
    T2
#if (NVS_FLASH_ALIGNMENT > 4)
    uint8_t padding[sizeof(__attribute__(__aligned__(NVS_FLASH_ALIGNMENT)) struct{T2}) - 
                    sizeof(struct{T2})];
#endif
} nvs_fs_file_meta_t;
#undef T2

/**
 * nvs FileSystem Commit Operation
 */
static int meta_header_update_meta_scratch(nvs_fs_context_t *ctx) {
    int ret = 0;
    uint32_t block_id = ctx->header.scratch_meta_blk_id;

    ctx->header.version++;
    ctx->header.scratch_meta_blk_id = ctx->header.active_metablk; 
    ctx->header.active_metablk = block_id;
    // leave scratch data block id to dblk operation

    ctx->header.crc32_val = crc32(&(ctx->header.version), sizeof(nvs_fs_mblk_header_t) - sizeof(uint32_t));

    // commit 
    ret = ctx->ops->prog(ctx->cfg, block_id, 0, &ctx->header, sizeof(nvs_fs_mblk_header_t));
    if (ret) {
        // fatal : program failed
    }
    return 0;
};


#endif