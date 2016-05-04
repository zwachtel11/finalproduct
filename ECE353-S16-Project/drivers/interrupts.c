#include "interrupts.h"
#include "timers.h"
#include "driver_defines.h"
#include "TM4C123.h"
#include "pc_buffer.h"
#include "uart_irqs.h"
#include "validate.h"
// Add required Interrupt Service Routines Below.

extern PC_Buffer UART0_Rx_Buffer;
extern PC_Buffer UART0_Tx_Buffer;
extern void boardRX(void);
#define   PS2_ADC_BASE     ADC0_BASE
//volatile bool AlertSysTick = false;
//volatile bool DisableTone = false;

volatile bool ALERT_2_SEC = false;
volatile bool UP = false;
volatile bool DOWN = false;
volatile bool RIGHT = false;
volatile bool LEFT = false;
volatile bool BTN_A = false;
volatile bool BTN_B = false;
volatile bool BTN_START = false;
volatile bool BTN_Z = false;
volatile uint32_t result;
volatile uint32_t stats;
volatile bool PACKSENT;
volatile bool PACKREC;
volatile bool DRUPT;
volatile bool WATCHD;
#define B_PUSHED (1<<2)
#define A_PUSHED (1<<3)
#define SRT_PUSHED (1<<1)
#define Z_PUSHED (1<<4)
	uint32_t numb = 0;
	uint32_t numa = 0;
	uint32_t nums = 0;
	uint32_t numz = 0;
	
//*****************************************************************************
// Rx Portion of the UART ISR Handler
//*****************************************************************************
__INLINE static void UART0_Rx_Flow(PC_Buffer *rx_buffer)
{
  // Loop until all characters in the RX FIFO have been removed
		while ((UART0->FR & UART_FR_RXFE) == 0){
      // Inside Loop: Add the character to the circular buffer
				if (!pc_buffer_full(rx_buffer)) {
					pc_buffer_add(rx_buffer, UART0->DR);
		}
	}
  // Clear the RX interrupts so it can trigger again when the hardware 
  // FIFO becomes full
			UART0->ICR |= 0x50;
	
}


//*****************************************************************************
// Tx Portion of the UART ISR Handler
//*****************************************************************************
__INLINE static void UART0_Tx_Flow(PC_Buffer *tx_buffer)
{
 char c;
 
 // Check to see if we have any data in the circular queue
 if (!pc_buffer_empty(tx_buffer)){
	 
	 
 // Move data from the circular queue to the hardware FIFO
 // until the hardware FIFO is full or the circular buffer
 // is empty.
 /*ADD CODE*/
	 while((!(UART0->FR & UART_FR_TXFF)) && ((!(pc_buffer_empty(tx_buffer))))){
			pc_buffer_remove(tx_buffer, &c);
			UART0->DR = c;
	 }
 }
 else
 {
			
     // Any data in the hardware FIFO will continue to be transmitted
     // but the TX empty interrupt will not be enabled since there
     // is no data in the circular buffer.
 
     // Disable the TX interrupts.
     /*ADD CODE */
			UART0->IM &= ~UART_IM_TXIM;
			
  }

  // Clear the TX interrupt so it can trigger again when the hardware
  // FIFO is empty
  /*ADD CODE */
	UART0->ICR |= UART_ICR_TXIC;
	
}
//*****************************************************************************
// UART0 Interrupt Service handler
//*****************************************************************************
void UART0_Handler(void)
{
    uint32_t  status;

    // Check to see if RXMIS or RTMIS is active
    status = UART0->MIS;

    if ( status & (UART_MIS_RXMIS | UART_MIS_RTMIS ) )
    {
       UART0_Rx_Flow(&UART0_Rx_Buffer);
    }
		
		if((status & UART_MIS_TXMIS)){
			UART0_Tx_Flow(&UART0_Tx_Buffer);
		}
		
    return;
}

//Handles buttons
void SysTick_Handler() {

	TIMER0_Type* tmr = (TIMER0_Type *)TIMER0_BASE;
	GPIOA_Type* PORTF = (GPIOA_Type *) GPIOF_BASE;
	ADC0_Type* myADC = (ADC0_Type *) ADC0_BASE;

	uint32_t val = SysTick->VAL;

		if ((PORTF->DATA & A_PUSHED) == 0)
		{
			numa++;
		} else {
			numa = 0;
			BTN_A = false;
		}
		if ((PORTF->DATA & Z_PUSHED) == 0)
		{
			numz++;
		} else {
			numz = 0;
			BTN_Z = false;
		}
	if ((PORTF->DATA & B_PUSHED) == 0)
		{
			numb++;
		} else {
			numb = 0;
			BTN_B = false;
		}
	if ((PORTF->DATA & SRT_PUSHED) == 0)
		{
			nums++;
		} else {
			nums = 0;
			BTN_START = false;
		}
	if ((numa >=3) && !BTN_A){
			
			BTN_A = true;
			numa = 0;
			
	}		
	if ((numb >=3) && !BTN_B){
		
			BTN_B = true;
			numb = 0;
			
	}	
	if ((nums >=3) && !BTN_START){

			BTN_START = true;
			nums = 0;
		
	}
	if ((numz >=3) && !BTN_Z){

			BTN_Z = true;
			numz = 0;
			
	}
	

	 
	
	
}


//Changes Alert2Sec to true every true seconds
void TIMER0A_Handler() {
	TIMER0_Type* tmr = (TIMER0_Type *)TIMER0_BASE;
    	ADC0_Type  *myADC;
	ADC0_Type *myADC1;
  myADC = (ADC0_Type *)ADC0_BASE;
  myADC1 = (ADC0_Type *)ADC1_BASE;
		

	

    myADC->PSSI |= ADC_PSSI_SS0;
		myADC1->PSSI |= ADC_PSSI_SS3;



		tmr ->ICR |= TIMER_ICR_TATOCINT;
	//reset clock

		
}
void WDT0_Handler(void) {

	
	WATCHDOG0->ICR = 0;
	printf("NO MOVEMENT_GAMEOVER\n");
	WATCHD = true;
	
	
}
volatile uint32_t count = 0 ;
void TIMER1A_Handler() {
	
    
		TIMER0_Type* tmr = (TIMER0_Type *)TIMER1_BASE;

		count++;
	
		tmr ->ICR |= TIMER_ICR_TATOCINT;
	//reset clock

		
}

//Handles joystick
void ADC0SS0_Handler() {

    uint32_t val;
    static uint32_t counter = 0;
	
	ADC0_Type  *myADC;
	ADC0_Type *myADC1;
  myADC = (ADC0_Type *)ADC0_BASE;

	val = myADC -> DCISC;
//printf("%d",val);
	
	switch(myADC->DCISC) {
			 
		case 1 : 
			
		case 2 : 
				DOWN =true;
		case 4 :
				UP = true;
		case 8 :
				RIGHT = true;
		case 16 :
			 LEFT = true ;
		
	}
	myADC -> DCISC |= ADC_DCISC_DCINT0 | ADC_DCISC_DCINT1 | ADC_DCISC_DCINT2 |ADC_DCISC_DCINT3 |ADC_DCISC_DCINT4;
	// how to declare up and down?
	//UP =true;
	
	//how to access adsc register.

	myADC ->ISC |=ADC_ISC_DCINSS0 |ADC_ISC_IN0 ;
	

	
}
void GPIOD_Handler(){

	GPIOD-> ICR |= 0x80;
	DRUPT = true;
	//boardRX();
	WATCHDOG0->ICR = 0;
	
}
volatile int lastmove = 0;
void ADC1SS3_Handler() {
	ADC0_Type* myADC = (ADC0_Type *)ADC1_BASE;
  
   //printf("in");
  result = myADC->SSFIFO3 & 0xFFF;    // Read 12-bit data

  myADC->ISC = ADC_ISC_IN3;  

	
}
