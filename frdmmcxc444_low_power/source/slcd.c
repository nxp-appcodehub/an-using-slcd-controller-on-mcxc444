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

#include "fsl_rtc.h"
#include "fsl_gpio.h"

//#include "fsl_slcd.h"
#include "LCD-S401M16KR.h"
#include "LED.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//typedef void (*callback_func_0_t)(void);
//typedef void (*callback_func_1_t)(void *param1);


/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool rtc_alarm_done;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void low_power_init(void);
void low_power_enter_vlls3(void);

void rtc_init(void);
void rtc_setup_wakeup_in_seconds(uint32_t seconds);

void llwu_init(void);


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t ch;

    /* unlock all the low power modes */
    low_power_init();

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("slcd low power example.\r\n");

    rtc_init();

    PRINTF("input a number to show in SLCD: \r\n");
    ch = GETCHAR();
    PRINTF("%c\r\n");

    if ( (ch < '1') || (ch > '9') )
    {
        PRINTF("unavailable option.\r\n");
        while (1);
    }
    ch -= '0'; /* convert char to number. */

#if 1
    /* init the slcd ans show the number on it. */
    slcd_init();
    /* show the option on LCD. */
    slcd_set_number(0, ch, false);
    slcd_set_number(1, ch, false);
    slcd_set_number(2, ch, false);
    slcd_set_number(3, ch, false);
    slcd_start();
#endif

    /* setup up wakeup source. */
    llwu_init();
    rtc_setup_wakeup_in_seconds(ch);
    low_power_enter_vlls3();


    /* code would never run to here. */
    while (1)
    {
        rtc_alarm_done = false;

        while (!rtc_alarm_done)
        {}

        PRINTF("\r\nrtc_alarm_done.\r\n");
    }
}

void low_power_init(void)
{
    /* unlock all the low power modes. */
    SMC->PMPROT = SMC_PMPROT_AVLP_MASK
                | SMC_PMPROT_ALLS_MASK
                | SMC_PMPROT_AVLLS_MASK
                ;

    /* disable the low voltage reset. */
    PMC->LVDSC1 &= ~(PMC_LVDSC1_LVDRE_MASK);

    /* clear the pin lock. */
    PMC->REGSC |= PMC_REGSC_ACKISO_MASK;
}

/*!
 * @brief Waitting for the OSC clock steady.
 *
 * Due to the oscillator startup time is depending on the hardware design and usually
 * take hundreds of milliseconds to make the oscillator stable. Here, we just delay a certain
 * time to ensure the socillator stable, please use the suitable delay time to adapt
 * to your real usage if needed.
 */
static void rtc_wait_osc_ready(uint32_t delay_ms)
{
    uint32_t ticks = 0UL;
    uint32_t count = delay_ms;

    /* Make a 1 milliseconds counter. */
    ticks = SystemCoreClock / 1000UL;
    assert((ticks - 1UL) <= SysTick_LOAD_RELOAD_Msk);

    /* Enable the SysTick for counting. */
    SysTick->LOAD = (uint32_t)(ticks - 1UL);
    SysTick->VAL  = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (; count > 0U; count--)
    {
        /* Wait for the SysTick counter reach 0. */
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
        {
        }
    }

    /* Disable SysTick. */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    SysTick->LOAD = 0UL;
    SysTick->VAL  = 0UL;
}

void rtc_init(void)
{
    rtc_config_t rtcConfig;

    RTC_GetDefaultConfig(&rtcConfig);
    RTC_Init(RTC, &rtcConfig); /* including the operation to enable clock. */

    RTC_SetClockSource(RTC);

    rtc_wait_osc_ready(1000u);
    //PRINTF("OSC Ready.\r\n");

   /* Set a start date time and start RT */
    RTC_StopTimer(RTC); /* RTC time counter has to be stopped before setting register */

    RTC->TSR = 1;
    RTC->TAR = 0; /* make sure the alarm time is earlier than current time. */

    RTC_EnableInterrupts(RTC, kRTC_AlarmInterruptEnable);
    NVIC_EnableIRQ(RTC_IRQn);

    RTC_StartTimer(RTC); /* RTC runs freely. */
}

void rtc_setup_wakeup_in_seconds(uint32_t seconds)
{
    //RTC_SetDatetime(RTC, &date);
    RTC->TAR = RTC->TSR + seconds;
}

/* rtc_irq_handler(). */
void RTC_IRQHandler(void)
{
    uint32_t flags = RTC_GetStatusFlags(RTC);
    RTC_ClearStatusFlags(RTC, flags);

    if (flags & kRTC_AlarmFlag)
    {
        rtc_alarm_done = true;
        //rtc_setup_wakeup_in_seconds(1);
    }
}

/*
* setup the wakeup source from LLS or VLLSx modes.
*/
void llwu_init(void)
{
    LLWU->ME = LLWU_ME_WUME5_MASK; /* LLWU_M5IF for RTC Alarm. */

    /* no need to clear internal module's flags in llwu. clear them in each peripheral.  */

    NVIC_EnableIRQ(LLWU_IRQn);
}

void low_power_enter_vlls3(void)
{
#if 0
    /* disable all the unnecesary clock. */
    SIM->SCGC4 = 0u;
    SIM->SCGC5 = 0u; //SIM_SCGC5_SLCD_MASK; /* only the slcd is available. */
    SIM->SCGC6 = SIM_SCGC6_RTC_MASK | SIM_SCGC6_FTF_MASK; /* only the rtc and flash are available. */
    SIM->SCGC7 = 0u;
#endif

    /* keep OSC alive in low power modes. */
    OSC0->CR = OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;

    /* setup the target stop mode. */
    SMC->PMCTRL = SMC_PMCTRL_STOPM(0x4); /* VLLSx. */
    SMC->STOPCTRL = SMC_STOPCTRL_VLLSM(0x3); /* VLLS3. */

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; /* STOP modes. */

    asm("wfi");
}

void LLWU_IRQHandler(void)
{

}

/* EOF. */

