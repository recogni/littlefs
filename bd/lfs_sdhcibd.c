/*
 * sdhci controller front end
 *
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "bd/lfs_sdhcibd.h"
#include "xsdps.h"


static XSdPs Instance;  //Global state config for SD/emmc device

int xsdps_init()
{
    XSdPs_Config *SdConfig = XSdPs_LookupConfig(SD_0_DEVICE_ID);
    LFS_ASSERT(SdConfig!= NULL);

/***
    FAT Driver has this before XSdPs_CfgInitialize(), in which case Instance isn't setup yet.
    If we end up calling it after XSdPs_CfgInitialize(), use InstancePtr->Config.BaseAddress instead.
***/
/**
    u32 status = XSdPs_GetPresentStatusReg(SD_0_BASEADDR);
    if ((status & XSDPS_PSR_CARD_INSRT_MASK) == 0) {
        printf("%s: No Card present, status 0x%x, BaseAddr %p\n", __func__, status, SdConfig->BaseAddress);
        return SD_CARD_NOT_PRESENT;
    }
*/

    if (XSdPs_CfgInitialize(&Instance, SdConfig, SD_0_BASEADDR) != XST_SUCCESS) {
        printf("%s: Unable to initialize SD controller @ %p\n", __func__, ARIANE_SDHCIBase);
        return SD_CONTROLLER_INIT_FAIL;
    }
    if (XSdPs_CardInitialize(&Instance) != XST_SUCCESS) {
        printf("%s: Unable to initialize eEMMC card, Base Addr %p\n", __func__, Instance.Config.BaseAddress);
        return SD_CARD_INIT_FAIL;
    }
    return 0;
}

int lfs_sdhcibd_createcfg(const struct lfs_config *cfg,
        const struct lfs_sdhcibd_config *bdcfg) {

    LFS_SDHCIBD_TRACE();

    lfs_sdhcibd_t *bd = cfg->context;

    bd->cfg = bdcfg;

    LFS_SDHCIBD_TRACE("-> %d", 0);
    return 0;
}


int lfs_sdhcibd_create(const struct lfs_config *cfg) {

    static const struct lfs_sdhcibd_config defaults;
    int err = lfs_sdhcibd_createcfg(cfg, &defaults);

    return err;
}

int lfs_sdhcibd_destroy(const struct lfs_config *cfg) {
    LFS_SDHCIBD_TRACE();
    // clean up 
    lfs_sdhcibd_t *bd = cfg->context;

    LFS_SDHCIBD_TRACE("-> %d", 0);
    return 0;
}

int lfs_sdhcibd_read(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size) {
    lfs_sdhcibd_t *bd = cfg->context;

    LFS_SDHCIBD_TRACE();
    LFS_ASSERT(off  % cfg->read_size == 0);
    LFS_ASSERT(size % cfg->read_size == 0);
    LFS_ASSERT(block < cfg->block_count);

    //Brett: XSdPs_ReadPolled(XSdPs *InstancePtr, u32 Arg, u32 BlkCnt, u8 *Buff);
    //Brett: What is Arg??  FAT passes in block number & block count
    XSdPs_ReadPolled(&Instance, 0, block, buffer);

    LFS_SDHCIBD_TRACE("-> %d", 0);
    return 0;
}

int lfs_sdhcibd_prog(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size) {

    lfs_sdhcibd_t *bd = cfg->context;

    LFS_SDHCIBD_TRACE();

    // check if write is valid
    LFS_ASSERT(off  % cfg->prog_size == 0);
    LFS_ASSERT(size % cfg->prog_size == 0);
    LFS_ASSERT(block < cfg->block_count);


    //s32 XSdPs_WritePolled(XSdPs *InstancePtr, u32 Arg, u32 BlkCnt, const u8 *Buff);
    XSdPs_WritePolled(NULL, 0, 0, buffer);
    LFS_SDHCIBD_TRACE("-> %d", 0);
    return 0;
}

int lfs_sdhcibd_erase(const struct lfs_config *cfg, lfs_block_t block) {
    lfs_sdhcibd_t *bd = cfg->context;

    LFS_SDHCIBD_TRACE();
    // check if erase is valid
    LFS_ASSERT(block < cfg->block_count);

    LFS_SDHCIBD_TRACE("-> %d", 0);
    return 0;
}

int lfs_sdhcibd_sync(const struct lfs_config *cfg) {
    LFS_SDHCIBD_TRACE();
    // sync does nothing because we aren't backed by anything real
    (void)cfg;
    LFS_SDHCIBD_TRACE("-> %d", 0);
    return 0;
}
