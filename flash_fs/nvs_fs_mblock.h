#ifndef _NVS_FS_META_BLOCK_H_
#define _NVS_FS_META_BLOCK_H_
#include <stdint.h>

#define NVS_META_BLOCK0 0
#define NVS_META_BLOCK0 1

typedef struct nvs_fs_context_t;

/**
 * MetaBlock Header
 */
typedef struct {
    uint16_t scratch_blk_id;
    uint8_t active_metablk;
    uint8_t padding;
} nvs_fs_mblk_header_t;

/**
 * File MetaBlock
 */
typedef struct {
    nvs_fs_fid_t fid;
    uint16_t lblk_id;
    uint16_t lblk_offset;
    uint16_t curr_size;
    uint16_t max_size;
} nvs_fs_file_meta_t;

/**
 * @brief commit Meta Block, we have 2 MetaBlock do A/B atomic swap each time we modify MetaBlock
 */
int nvs_commit_meta_block(nvs_fs_context_t* ctx);

#endif