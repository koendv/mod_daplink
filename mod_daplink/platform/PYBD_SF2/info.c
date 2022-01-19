/**
 * @file    info.c
 * @brief   Implementation of info.h
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2020 Arm Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "info.h"
#include <stdio.h>
#include "py/mphal.h"

// Strings
static char string_unique_id[16 + 1];
static char string_version[] = MICROPY_VERSION_STRING;

const char *info_get_unique_id(void) {
    uint8_t *id = (uint8_t *)MP_HAL_UNIQUE_ID_ADDRESS;
    snprintf(string_unique_id, sizeof(string_unique_id),
        "%02X%02X%02X%02X%02X%02X",
        id[11], id[10] + id[2], id[9], id[8] + id[0], id[7], id[6]);
    return string_unique_id;
}

const char *info_get_version(void) {
    return string_version;
}
