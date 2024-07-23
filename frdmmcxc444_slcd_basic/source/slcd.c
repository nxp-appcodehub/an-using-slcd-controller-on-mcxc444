/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"

#include "fsl_slcd.h"
#include "LCD-S401M16KR.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void slcd_init_port_pins(void);
void slcd_setup_panel(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    bool en_dp;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("slcd basic example.\r\n");

    slcd_init();

    en_dp = false;
    while (1)
    {
        for (uint8_t i = 0u; i < SLCD_ON_SHOW_COUNT; i++)
        {
            GETCHAR();
            slcd_stop();
            slcd_set_number(0, i                       , en_dp);
            slcd_set_number(1, (i+1)%SLCD_ON_SHOW_COUNT, en_dp);
            slcd_set_number(2, (i+2)%SLCD_ON_SHOW_COUNT, en_dp);
            slcd_set_number(3, (i+3)%SLCD_ON_SHOW_COUNT, en_dp);
            slcd_start();
        }
        en_dp = !en_dp;
    }
}
/* EOF. */

