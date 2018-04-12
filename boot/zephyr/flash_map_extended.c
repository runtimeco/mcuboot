/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2015 Runtime Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <flash.h>

#include "target.h"

#include <flash_map_backend/flash_map_backend.h>
#include <hal/hal_flash.h>
#include <sysflash/sysflash.h>

#define BOOT_LOG_LEVEL BOOT_LOG_LEVEL_INFO
#include "bootutil/bootutil_log.h"

/*
 * For now, we only support one flash device.
 *
 * Pick the SoC Flash driver ID.
 */
#define FLASH_DEVICE_ID SOC_FLASH_0_ID
#define FLASH_DEVICE_BASE CONFIG_FLASH_BASE_ADDRESS

int flash_device_base(uint8_t fd_id, uintptr_t *ret)
{
    if (fd_id != FLASH_DEVICE_ID) {
        BOOT_LOG_ERR("invalid flash ID %d; expected %d",
                     fd_id, FLASH_DEVICE_ID);
        return -EINVAL;
    }
    *ret = FLASH_DEVICE_BASE;
    return 0;
}

/*
 * This depends on the mappings defined in sysflash.h, and assumes
 * that slot 0, slot 1, and the scratch areas are contiguous.
 */
int flash_area_id_from_image_slot(int slot)
{
    return slot + FLASH_AREA_IMAGE_0;
}
