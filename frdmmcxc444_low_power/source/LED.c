/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "LED.h"


/* LED1 - PTD5.
 * LED2 - PTE31.
 */
void led_init(void)
{
    gpio_pin_config_t gpio_pin_config;

    /* enable the clock. */
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK
                | SIM_SCGC5_PORTE_MASK
                ;
    /* pin mux. */
    PORTD->PCR[5]  = PORT_PCR_MUX(1);
    PORTE->PCR[31] = PORT_PCR_MUX(1);

    gpio_pin_config.pinDirection = kGPIO_DigitalOutput;
    gpio_pin_config.outputLogic  = 1;

    GPIO_PinInit(GPIOD,  5, &gpio_pin_config);
    GPIO_PinInit(GPIOE, 31, &gpio_pin_config);
}

void led_deinit(void)
{
    /* pin mux. */
    PORTD->PCR[5]  = PORT_PCR_MUX(0);
    PORTE->PCR[31] = PORT_PCR_MUX(0);
}

void led_on(uint32_t mask)
{
    if (LED_0_MASK & mask)
    {
        GPIO_PinWrite(GPIOD, 5, 0);
    }
    if (LED_1_MASK & mask)
    {
        GPIO_PinWrite(GPIOE, 31, 0);
    }
}

void led_off(uint32_t mask)
{
    if (LED_0_MASK & mask)
    {
        GPIO_PinWrite(GPIOD, 5, 1);
    }
    if (LED_1_MASK & mask)
    {
        GPIO_PinWrite(GPIOE, 31, 1);
    }
}

/* EOF. */

