/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <drivers/flash.h>
#include <mgmt/mcumgr/zephyr_groups.h>

#include <flash_map_backend/flash_map_backend.h>
#include <sysflash/sysflash.h>

#include "bootutil/bootutil_log.h"
#include "../boot_serial/src/boot_serial_priv.h"
#include "../boot_serial/src/cbor_encode.h"

MCUBOOT_LOG_MODULE_DECLARE(mcuboot);

int bs_peruser_system_specific(const struct nmgr_hdr *hdr, const char *buffer,
                               int len, cbor_state_t *cs)
{
    int mgmt_rc = MGMT_ERR_ENOTSUP;
    int rc = -ENOTSUP;

    if (hdr->nh_group == ZEPHYR_MGMT_GRP_BASE &&
        hdr->nh_id == ZEPHYR_MGMT_GRP_BASIC_CMD_ERASE_STORAGE &&
        hdr->nh_op == NMGR_OP_WRITE) {

        const struct flash_area *fa;

        rc = flash_area_open(FLASH_AREA_ID(storage), &fa);

        if (rc < 0) {
            LOG_ERR("failed to open flash area");
        } else {
            rc = flash_area_erase(fa, 0, FLASH_AREA_SIZE(storage));
            if (rc < 0) {
                LOG_ERR("failed to erase flash area");
            }
            flash_area_close(fa);
        }
        if (rc == 0) {
            mgmt_rc = MGMT_ERR_OK;
        } else {
            mgmt_rc = MGMT_ERR_EUNKNOWN;
        }
    }

    map_start_encode(cs, 10);
    tstrx_put(cs, "rc");
    uintx32_put(cs, mgmt_rc);
    map_end_encode(cs, 10);

    return rc;
}
