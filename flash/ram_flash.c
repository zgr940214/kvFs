#include "ram_flash.h"
#include <stdlib.h>
#include <string.h>

nvs_fs_flash_ops_t nvs_fs_ram_flash_ops = {
    .init = ram_flash_init,
    .flush = ram_flash_flush,
    .prog = ram_flash_program,
    .read = ram_flash_read,
    .erase = ram_flash_erase,
};

static int get_phys_address(nvs_fs_flash_config_t* cfg, uint32_t block_id);

int ram_flash_init (nvs_fs_flash_config_t* cfg) {
    memset(cfg->FLASHDEV, 0, cfg->n_blk * cfg->blk_size);
};

int ram_flash_flush(nvs_fs_flash_config_t* cfg, uint32_t scratch_id) {
    (void)cfg;
    (void)scratch_id;
    return 0;
}

int ram_flash_program (nvs_fs_flash_config_t* cfg, uint32_t block_id, uint8_t* data, uint32_t size) {
    memcpy(, );
};

int ram_flash_read (nvs_fs_flash_config_t* cfg, uint32_t block_id, uint8_t* data, uint32_t size) {

};

int ram_flash_erase (nvs_fs_flash_config_t* cfg, uint32_t block_id, uint32_t size) {

};