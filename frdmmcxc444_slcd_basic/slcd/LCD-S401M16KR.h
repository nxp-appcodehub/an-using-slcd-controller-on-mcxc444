/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __LCD_S401M16KR__
#define __LCD_S401M16KR__

#include "fsl_common.h"

/* Define the on-show number for SLCD. */
#define SLCD_ON_SHOW_NUMBER_0     0u
#define SLCD_ON_SHOW_NUMBER_1     1u
#define SLCD_ON_SHOW_NUMBER_2     2u
#define SLCD_ON_SHOW_NUMBER_3     3u
#define SLCD_ON_SHOW_NUMBER_4     4u
#define SLCD_ON_SHOW_NUMBER_5     5u
#define SLCD_ON_SHOW_NUMBER_6     6u
#define SLCD_ON_SHOW_NUMBER_7     7u
#define SLCD_ON_SHOW_NUMBER_8     8u
#define SLCD_ON_SHOW_NUMBER_9     9u
#define SLCD_ON_SHOW_NONE         10u
#define SLCD_ON_SHOW_DP           11u

#define SLCD_ON_SHOW_COUNT  11u

#define SLCD_ON_SHOW_NUMBER_COUNT 4u /* fixed 4 numbers' position. */

/* Define the sync bus and the data bus. */
#define SLCD_COMx_COUNT        4u
#define SLCD_DATA_BUS_WIDTH    8u

void slcd_init(void);
void slcd_start(void);
void slcd_stop(void);
void slcd_set_number(uint8_t index, uint8_t number, bool en_dp);

#endif /* __LCD_S401M16KR__ */

