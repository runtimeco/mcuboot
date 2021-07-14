/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * Original license:
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/**
 * @file
 * @brief Hooks definition implementation API
 *
 * This file contains API interface definition for hooks which can be
 * implemented for overide some of MCUboot's native routines.
 */

#ifndef H_BOOTUTIL_HOOKS
#define H_BOOTUTIL_HOOKS

/** Hook for provide image header data.
 *
 * This Hook may be used to overide image header read implementation or doing
 * a custom action befor.
 *
 * @param img_index the index of the images pair
 * @param slot slot number
 * @param img_hed image header structure to be populated
 *
 * @retval 0: header was read/populated, skip direct header data read
 *         1: follow the normal execution path,
 *         otherwise an error-code value.
 */
int boot_read_image_header_hook(int img_index, int slot,
                                struct image_header *img_head);

/** Hook for Validate primary image hash/signature
 *
 * This Hook may be used to overide image validation procedure or doing
 * a custom action befor.
 *
 * @param img_index the index of the images pair
 * 
 * @retval FIH_SUCCESS: image is valid, skip direct validation
 *         FIH_FAILURE: image is invalid, skip direct validation
 *         fih encoded 1: follow the normal execution path.
 */
fih_int boot_image_check_hook(int img_index);

/** Hook for implement image update
 *
 * This hook is for for implementing an alternative mechanism of image update or
 * doing a custom action before.
 *
 * @param img_index the index of the images pair
 * @param img_hed the image header of the secondary image
 * @param area the flash area of the secondary image.
 *
 * @retval 0: update was done, skip performing the update
 *         1: follow the normal execution path,
 *         otherwise an error-code value.
 */
int boot_perform_update_hook(int img_index, struct image_header *img_head,
                             const struct flash_area *area);

/** Hook for implement image's post coping action
 *
 * This hook is for implement action which might be done right after image was
 * copied to the primary slot. This hook is called in MCUBOOT_OVERWRITE_ONLY
 * mode only.
 *
 * @param img_index the index of the images pair
 * @param area the flash area of the primary image.
 * @param size size of copied image.
 *
 * @retval 0: success, mcuboot will follow normal code execution flow after
 *            execution of this call.
 *         non-zero: an error, mcuboot will return from
 *         boot_copy_image() with error.
 *         Update will be undone so might be resume on the next boot.
 */
int boot_copy_region_post_hook(int img_index, const struct flash_area *area,
                               size_t size);

#endif /*H_BOOTUTIL_HOOKS*/
