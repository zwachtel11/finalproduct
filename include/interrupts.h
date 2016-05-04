#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <stdbool.h>
#include "driver_defines.h"
#include "TM4C123.h"

extern volatile bool ALERT_2_SEC;
extern volatile bool UP;
extern volatile bool DOWN;
extern volatile bool RIGHT;
extern volatile bool LEFT;
extern volatile bool BTN_A;
extern volatile bool BTN_B;
extern volatile bool BTN_START;
extern volatile bool BTN_Z;
extern volatile uint32_t result;
extern volatile bool PACKSENT;
extern volatile bool PACKREC;
extern volatile uint32_t stats;
extern volatile bool DRUPT;
extern volatile bool WATCHD;
extern volatile uint32_t count;


void Timer0A_Handler(void);

void ADC0SS0_Handler(void);
void Timer1A_Handler(void);
void UART0_Handler(void);
void SysTick_Handler(void);
void TIMER0A_Handler(void);
void WDT0_Handler(void);
void ADC0SS3_Handler(void);
void GPIOD_Handler(void);
void TIMER1_Handler(void);

#endif
