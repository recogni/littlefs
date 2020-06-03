/*
 * SDHCI controller front end
 *
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef LFS_SDHCIBD_H
#define LFS_SDHCIBD_H

#include "lfs.h"
#include "lfs_util.h"

#ifdef __cplusplus
extern "C"
{
#endif


// Block device specific tracing
#ifndef LFS_SDHCIBD_YES_TRACE
#define LFS_SDHCIBD_TRACE(...) LFS_TRACE(__VA_ARGS__)
#else
#define LFS_SDHCIBD_TRACE(...)
#endif

// sdhcibd config (optional)
struct lfs_sdhcibd_config {
};

// sdhcibd state
typedef struct lfs_sdhcibd {
    uint8_t *buffer;
    const struct lfs_sdhcibd_config *cfg;
} lfs_sdhcibd_t;

//externally visible init routne
int xsdps_init();

// Create a RAM block device using the geometry in lfs_config
int lfs_sdhcibd_create(const struct lfs_config *cfg);
int lfs_sdhcibd_createcfg(const struct lfs_config *cfg,
        const struct lfs_sdhcibd_config *bdcfg);

// Clean up memory associated with block device
int lfs_sdhcibd_destroy(const struct lfs_config *cfg);

// Read a block
int lfs_sdhcibd_read(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size);

// Program a block
//
// The block must have previously been erased.
int lfs_sdhcibd_prog(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size);

// Erase a block
//
// A block must be erased before being programmed. The
// state of an erased block is undefined.
int lfs_sdhcibd_erase(const struct lfs_config *cfg, lfs_block_t block);

// Sync the block device
int lfs_sdhcibd_sync(const struct lfs_config *cfg);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
