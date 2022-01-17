/**
 * @file    IO_Config.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

// Override all defines if IO_CONFIG_OVERRIDE is defined
#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

// DAP LED
#define PIN_LED                 (25)

// SWCLK/TCK Pin
#define PIN_SWCLK               (2)
#define PIN_TCK                 (2)

// SWDIO/TMS In/Out Pin
#define PIN_SWDIO               (3)
#define PIN_TMS                 (3)

// TDI Pin
#define PIN_TDI                 (4)

// SWO/TDO Pin
#define PIN_TDO                 (5)

// nRESET Pin
#define PIN_nRESET              (6)

#endif
