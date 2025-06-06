#ifndef _NVS_FS_DATA_BLOCK_H_
#define _NVS_FS_DATA_BLOCK_H
#include <stdint.h>

typedef struct {
    uint32_t phys_blk_id;
    uint32_t curr_size;
    uint32_t max_size;
    uint32_t data_offset;
} nvs_fs_dblock_t;

/**
 * @brief write phys_blk
 */
int compact_dblk();


#endif