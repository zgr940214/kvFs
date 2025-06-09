#include "nvs_fs.h"

#define FLIP_META_ID(mid) (~(mid))

static uint16_t fs_map_lo_to_phys_id(nvs_fs_context_t* ctx, uint16_t lblk_id) {
    ctx->ops->read(ctx->cfg,  );
}

/**
 * @brief We always passing relative offset to flash driver, since we have no idea where each flash backend's base address
 */
static uint32_t fs_get_phys_addr(nvs_fs_context_t* ctx, uint16_t phys_id, uint16_t offset) {
    return ctx->cfg->blk_size * phys_id + offset;
}

static int fs_compact_dblk(nvs_fs_context_t* ctx, uint16_t lblk) {

};

/**
 * @brief Update lblk meta in meta block
 */
static int fs_commit_mblk(nvs_fs_context_t* ctx) {
    int ret = 0;
    uint16_t meta_id = FLIP_META_ID(ctx->header.active_metablk);

    /* erase whole MetaBlock */
    ret = ctx->ops->erase(fs_get_phys_addr(ctx, meta_id, 0), );
    if (ret) {
        // error 
    }

    /* program Meta Header */
    
    /* program lblk meta */

    /* program file meta */
};

int nvs_fs_init(nvs_fs_context_t* ctx) {

};

int nvs_fs_get_file_info(nvs_fs_context_t* ctx, nvs_fs_fid_t fid, nvs_fs_file_info_t* file_info) {

};

int nvs_fs_write(nvs_fs_context_t* ctx, nvs_fs_fid_t fid, uint8_t* data, uint16_t size) {

};

int nvs_fs_read(nvs_fs_context_t* ctx, nvs_fs_fid_t fid, uint8_t* data, uint16_t size) {

};

int nvs_fs_delete(nvs_fs_context_t* ctx, nvs_fs_fid_t fid) {

};