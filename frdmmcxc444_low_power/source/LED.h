/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __LED_H__
#define __LED_H__

#define LED_0_MASK  (1u << 0u)
#define LED_1_MASK  (1u << 1u)

void led_init(void);
void led_deinit(void);

void led_on(uint32_t mask);
void led_off(uint32_t mask);

#endif /* __LED_H__ */

