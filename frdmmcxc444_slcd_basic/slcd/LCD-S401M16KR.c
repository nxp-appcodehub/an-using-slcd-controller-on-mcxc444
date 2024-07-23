/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "LCD-S401M16KR.h"

/* Define the pins for sync bus and data bus. */
const uint8_t SLCD_PIN_COMx[SLCD_COMx_COUNT] =
{
    59, /* COM0. */
    60, /* COM1. */
    14, /* COM2. */
    15  /* COM3. */
};
const uint8_t SLCD_PIN_DATA[SLCD_DATA_BUS_WIDTH] =
{
    20, /* D0. */
    24, /* D1. */
    26, /* D2. */
    27, /* D3. */
    40, /* D4. */
    42, /* D5. */
    43, /* D6. */
    44  /* D7. */
};

/* Define the on-show code on SLCD.
 * The four codes for one on-show number are data for each scan phase.
 */
const uint8_t SLCD_NUMBER_TABLE[][SLCD_COMx_COUNT] =
{
    { 0x1, 0x3, 0x2, 0x3 }, /* SLCD_ON_SHOW_NUMBER_0    */
    { 0x0, 0x2, 0x2, 0x0 }, /* SLCD_ON_SHOW_NUMBER_1    */
    { 0x1, 0x1, 0x3, 0x2 }, /* SLCD_ON_SHOW_NUMBER_2    */
    { 0x1, 0x2, 0x3, 0x2 }, /* SLCD_ON_SHOW_NUMBER_3    */
    { 0x0, 0x2, 0x3, 0x1 }, /* SLCD_ON_SHOW_NUMBER_4    */
    { 0x1, 0x2, 0x1, 0x3 }, /* SLCD_ON_SHOW_NUMBER_5    */
    { 0x1, 0x3, 0x1, 0x3 }, /* SLCD_ON_SHOW_NUMBER_6    */
    { 0x0, 0x2, 0x2, 0x2 }, /* SLCD_ON_SHOW_NUMBER_7    */
    { 0x1, 0x3, 0x3, 0x3 }, /* SLCD_ON_SHOW_NUMBER_8    */
    { 0x1, 0x2, 0x3, 0x3 }, /* SLCD_ON_SHOW_NUMBER_9    */
    { 0x0, 0x0, 0x0, 0x0 }, /* SLCD_ON_SHOW_NUMBER_NONE */
    { 0x2, 0x0, 0x0, 0x0 }, /* SLCD_ON_SHOW_NUMBER_DP   */

};

static void slcd_init_hardware_pins(void);
static void slcd_init_controller_basic(void);
static void slcd_set_bus_data(uint8_t com_idx, uint8_t show_dat);



void slcd_init(void)
{
    /* Enable the clock to SLCD module. */
    SIM->SCGC5 |= SIM_SCGC5_SLCD_MASK;

	slcd_init_hardware_pins();
	slcd_init_controller_basic();
}

/*
| Functional ID | SLCD Pin | MCU Pin | Altx | Comment |
| ------------- | -------- | ------- | ---- |         |
| LCD-01        |  P59     | PTE20   | ALT7 |         |
| LCD-02        |  P60     | PTE21   | ALT7 |         |
| LCD-03        |  P14     | PTB18   | ALT7 |         |
| LCD-04        |  P15     | PTB19   | ALT7 |         |
| LCD-05        |  P20     | PTC0    | ALT7 |         |
| LCD-06        |  P24     | PTC4    | ALT7 |         |
| LCD-07        |  P26     | PTC5    | ALT7 |         |
| LCD-08        |  P27     | PTC6    | ALT7 |         |
| LCD-09        |  P40     | PTD0    | ALT7 |         |
| LCD-10        |  P42     | PTD2    | ALT7 |         |
| LCD-11        |  P43     | PTD3    | ALT7 |         |
| LCD-12        |  P44     | PTD4    | ALT7 |         |
*/
static void slcd_init_hardware_pins(void)
{
    /* make sure the clock to PORT are enabled. */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK
                | SIM_SCGC5_PORTC_MASK
                | SIM_SCGC5_PORTD_MASK
                | SIM_SCGC5_PORTE_MASK
                ;

    PORTE->PCR[20] = PORT_PCR_MUX(7); /* LCD-01, P59, COM0. */
    PORTE->PCR[21] = PORT_PCR_MUX(7); /* LCD-02, P60, COM1. */
    PORTB->PCR[18] = PORT_PCR_MUX(7); /* LCD-03, P14, COM2. */
    PORTB->PCR[19] = PORT_PCR_MUX(7); /* LCD-04, P15, COM3. */
    PORTC->PCR[0 ] = PORT_PCR_MUX(7); /* LCD-05, P20, . */
    PORTC->PCR[4 ] = PORT_PCR_MUX(7); /* LCD-06, P24, . */
    PORTC->PCR[5 ] = PORT_PCR_MUX(7); /* LCD-07, P26, . */
    PORTC->PCR[6 ] = PORT_PCR_MUX(7); /* LCD-08, P27, . */
    PORTD->PCR[0 ] = PORT_PCR_MUX(7); /* LCD-09, P40, . */
    PORTD->PCR[2 ] = PORT_PCR_MUX(7); /* LCD-10, P42, . */
    PORTD->PCR[3 ] = PORT_PCR_MUX(7); /* LCD-11, P43, . */
    PORTD->PCR[4 ] = PORT_PCR_MUX(7); /* LCD-12, P44, . */
}

/* The LCD module would be disabled after init function.
 * Please start it again after each init.
 */
static void slcd_init_controller_basic(void)
{
    uint64_t pins_mask = 0u;

    /* Disable the controller during setting. */
    slcd_stop();

    /* Setup slcd controller. */
    LCD->GCR = LCD_GCR_DUTY(3)      /* Selects the duty cycle of the LCD controller driver. 3: 4 COMx lines. */
             | LCD_GCR_LCLK(7)      /* Clock divider for clock source. 0-7*/
             | LCD_GCR_SOURCE(1)    /* LCD clock source. 1:use MCGIRCLK. */
             | LCD_GCR_LCDEN(0)     /* Disable the controller during setting. */
             | LCD_GCR_LCDSTP(1)    /* Keep LCD module alive in STOP mode. */
             | LCD_GCR_LCDDOZE(1)   /* Keep LCD module alive in DOZE mode. */
             | LCD_GCR_FFR(0)       /* Select the frame rate mode. 0:standard frame rate. */
             | LCD_GCR_ALTSOURCE(0) /* Select the alternate clock source. */
             | LCD_GCR_ALTDIV(2)    /* Clock divider for the alternate clock source. 0-3. */
             | LCD_GCR_FDCIEN(0)    /* Enables an LCD interrupt event when fault detection is completed. */
             | LCD_GCR_PADSAFE(0)   /* Force safe state on LCD pad controls (all LCD frontplane and backplane functions disabled) regardless of other LCD control bits. */
             | LCD_GCR_VSUPPLY(0)   /* Select the power voltage supply. */
             | LCD_GCR_LADJ(2)      /* Configures SLCD to handle different LCD glass capacitance. */
             | LCD_GCR_CPSEL(0)     /* Selects the LCD controller charge pump or a resistor network  to supply the LCD voltages V_LL1, V_LL2, and V_LL3. */
             | LCD_GCR_RVTRIM(8)    /* Regulated Voltage Trim. */
             | LCD_GCR_RVEN(0)      /* Regulated Voltage Enable. */
             ;

    /* Enable all pins used for SLCD. */
    for (uint8_t i = 0u; i < SLCD_COMx_COUNT; i++)
    {
        pins_mask |= (((uint64_t)1u) << SLCD_PIN_COMx[i]);
    }
    /* for comx (back panel) pins. */
    LCD->BPEN[0] = (uint32_t)(pins_mask);        /* LCD_BPENL. */
    LCD->BPEN[1] = (uint32_t)(pins_mask >> 32u); /* LCD_BPENH. */
    /* with additional data bus pins. */
    for (uint8_t i = 0u; i < SLCD_DATA_BUS_WIDTH; i++)
    {
        pins_mask |= (((uint64_t)1u) << SLCD_PIN_DATA[i]);
    }
    LCD->PEN[0] = (uint32_t)(pins_mask);        /* LCD_BENL. */
    LCD->PEN[1] = (uint32_t)(pins_mask >> 32u); /* LCD_BENH. */

    /* Setup the phase of COMx for their pins. */
    for (uint8_t i = 0u; i < SLCD_COMx_COUNT; i++)
    {
        LCD->WF8B[SLCD_PIN_COMx[i]] = (0x1 << i);
    }

}

#if 0
/* Add some additional functions. */
void slcd_setup_controller_auxiliary(void)
{
}

/* Add the fault detection. */
void slcd_setup_fault_detection(void)
{
}
#endif

void slcd_start(void)
{
    LCD->GCR |= LCD_GCR_LCDEN_MASK;
}

void slcd_stop(void)
{
    LCD->GCR &= ~LCD_GCR_LCDEN_MASK;
}

#if 0
void slcd_set_comx(void)
{
    for (uint8_t i = 0u; i < SLCD_COMx_COUNT; i++)
    {
        LCD->WF8B[SLCD_PIN_COMx[i]] = (0x1 << i)
    }
}
#endif


static void slcd_set_bus_data(uint8_t com_idx, uint8_t show_dat)
{
    uint8_t bit_mask = (1u << com_idx);

    for (uint8_t i = 0u; i < SLCD_DATA_BUS_WIDTH; i++)
    {
        if (show_dat & 0x1)
        {
            LCD->WF8B[SLCD_PIN_DATA[i]] |= bit_mask;
        }
        else
        {
            LCD->WF8B[SLCD_PIN_DATA[i]] &= ~bit_mask;
        }
        show_dat >>= 1u;
    }
}


static uint8_t slcd_on_show_numbers[SLCD_COMx_COUNT];

void slcd_set_number(uint8_t index, uint8_t number, bool en_dp)
{
    uint8_t tmp8 = 0u;
    for (uint8_t i = 0u; i < SLCD_COMx_COUNT; i++)
    {
        tmp8  = slcd_on_show_numbers[i] & (~(0x3 << (2 * index))); /* clear old setting. */
        tmp8 |= (SLCD_NUMBER_TABLE[number][i] << (2 * index)); /* add new setting. */

        if (en_dp)
        {
            tmp8 |= SLCD_NUMBER_TABLE[SLCD_ON_SHOW_DP][i] << (2 * index); /* add new setting. */
        }
        slcd_on_show_numbers[i] = tmp8;
        slcd_set_bus_data(i, slcd_on_show_numbers[i]);
    }
}




/* EOF. */

