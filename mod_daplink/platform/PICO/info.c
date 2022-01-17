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
#include "pico/unique_id.h"
#include "py/mpconfig.h"
#include "py/misc.h"

// Strings
static char string_unique_id[48 + 1];
static char string_version[] = MICROPY_VERSION_STRING;
static const char hexdigits[] = "0123456789ABCDEF";

static char * hexify(char *hex, const void *buf, size_t size) {
    char *tmp = hex;
    const uint8_t *b = buf;

    while (size--) {
        *tmp++ = hexdigits[*b >> 4];
        *tmp++ = hexdigits[*b++ & 0xF];
    }
    *tmp++ = 0;

    return hex;
}

const char *info_get_unique_id(void) {
    pico_unique_board_id_t id;
    pico_get_unique_board_id(&id);
    uint32_t id_len = MIN(sizeof(id), sizeof(string_unique_id) / 2 - 1);
    hexify(string_unique_id, &id, id_len);
    return string_unique_id;
}

const char *info_get_version(void) {
    return string_version;
}
