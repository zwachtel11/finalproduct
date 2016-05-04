#include "timers.h"
#include "TM4C123.h"

// ADD CODE
// Use the macro in driver_defines.h that sets a general purpose in one shot mode
#define ONE_SHOT    TIMER_TAMR_TAMR_1_SHOT
#define PERIODIC		TIMER_TAMR_TAMR_PERIOD

TIMER0_Type *gp_timer0;
TIMER0_Type *gp_timer1;
//*****************************************************************************
// Verifies that the base address is a valid GPIO base address
//*****************************************************************************
static bool verify_base_addr(uint32_t base_addr)
{
   switch( base_addr )
   {
     case TIMER0_BASE:
     case TIMER1_BASE:
     case TIMER2_BASE:
     case TIMER3_BASE:
     case TIMER4_BASE:
     case TIMER5_BASE:
     {
       return true;
     }
     default:
     {
       return false;
     }
   }
}

//*****************************************************************************
// Returns the RCGC and PR masks for a given TIMER base address
//*****************************************************************************
static bool get_clock_masks(uint32_t base_addr, uint32_t *timer_rcgc_mask, uint32_t *timer_pr_mask)
{
  // Set the timer_rcgc_mask and timer_pr_mask using the appropriate
  // #defines in ../include/sysctrl.h
  switch(base_addr)
  {
    case TIMER0_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R0;
      *timer_pr_mask = SYSCTL_PRTIMER_R0;
      break;
    }
    case TIMER1_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R1;
      *timer_pr_mask = SYSCTL_PRTIMER_R1;
      break;
    }
    case TIMER2_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R2;
      *timer_pr_mask = SYSCTL_PRTIMER_R2;
      break;
    }
    case TIMER3_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R3;
      *timer_pr_mask = SYSCTL_PRTIMER_R3;
      break;
    }
    case TIMER4_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R4;
      *timer_pr_mask = SYSCTL_PRTIMER_R4;
      break;
    }
    case TIMER5_BASE:
    {
      *timer_rcgc_mask = SYSCTL_RCGCTIMER_R5;
      *timer_pr_mask = SYSCTL_PRTIMER_R5;
      break;
    }
    default:
    {
      return false;
    }
  }
  return true;
}


//*****************************************************************************
// Waits for 'ticks' number of clock cycles and then returns.
//
//The function returns true if the base_addr is a valid general purpose timer
//*****************************************************************************
bool gp_timer_wait(uint32_t base_addr, uint32_t ticks)
{
  TIMER0_Type *gp_timer;
  
  // Verify the base address.
  if ( ! verify_base_addr(base_addr) )
  {
    return false;
  }

  // Type cast the base address to a TIMER0_Type struct
  gp_timer = (TIMER0_Type *)base_addr;

  // Stop the timers
	gp_timer ->CTL &= ~TIMER_CTL_TAEN;	
	gp_timer ->CTL &= ~TIMER_CTL_TBEN;
  
  // Set the Interval Load Register
	gp_timer ->TAILR = ticks;

  // Clear any timeout interrupts before we wait
	gp_timer -> ICR |= TIMER_ICR_TATOCINT;
  
  // Enable the Timer
  gp_timer -> CTL |= TIMER_CTL_TAEN;
  // wait for the timer to expire.
	while( (TIMER0 ->RIS & TIMER_RIS_TATORIS) == 0) {}
		
    
  return true;
}


//*****************************************************************************
// Configure a general purpose timer to be a 32-bit timer.  
//
// Paramters
//  base_address          The base address of a general purpose timer
//
//  mode                  bit mask for Periodic, One-Shot, or Capture
//
//  count_up              When true, the timer counts up.  When false, it counts
//                        down
//
//  enable_interrupts     When set to true, the timer generates and interrupt
//                        when the timer expires.  When set to false, the timer
//                        does not generate interrupts.
//
//The function returns true if the base_addr is a valid general purpose timer
//*****************************************************************************
bool gp_timer_config_32(uint32_t base_addr, uint32_t mode, bool count_up, bool enable_interrupts)
{
  uint32_t timer_rcgc_mask;
  uint32_t timer_pr_mask;
  TIMER0_Type *gp_timer;
  
  // Verify the base address.
  if ( ! verify_base_addr(base_addr) )
  {
    return false;
  }
  
  // get the correct RCGC and PR masks for the base address
  get_clock_masks(base_addr, &timer_rcgc_mask, &timer_pr_mask);
  
  // Turn on the clock for the timer
  SYSCTL->RCGCTIMER |= timer_rcgc_mask;

  // Wait for the timer to turn on
  while( (SYSCTL->PRTIMER & timer_pr_mask) == 0) {};
  
  //*********************    
  // ADD CODE
  //*********************

  // Type cast the base address to a TIMER0_Type struct
  gp_timer = (TIMER0_Type *)base_addr;
    
		
	
  // Stop the timers
	gp_timer ->CTL &= ~TIMER_CTL_TAEN;	
	gp_timer ->CTL &= ~TIMER_CTL_TBEN;
  // Set the timer to be a 32-bit timer
	
			gp_timer -> CFG = TIMER_CFG_32_BIT_TIMER;
		

  // Clear the timer mode 
		gp_timer -> TAMR  &= TIMER_TAMR_TAMR_M;
  // Set the mode
		gp_timer -> TAMR |= mode;

  // Set the timer direction.  count_up: 0 for down, 1 for up.
  
  if( count_up )
  {
    // Set the direction bit
		gp_timer -> TAMR |= TIMER_TAMR_TACDIR;
  }
	else
	{
		gp_timer ->TAMR &= ~TIMER_TAMR_TACDIR;
	}
  
  // Disable Timer Interrupts  
if (enable_interrupts) {
	gp_timer -> IMR |=TIMER_IMR_TATOIM;
	
}
else {
	gp_timer -> IMR &= ~TIMER_IMR_TATOIM;
}

  return true;  
}
//*****************************************************************************
// Configure a general purpose timer to be a 16-bit timer.  
//
// Paramters
//  base_address          The base address of a general purpose timer
//
//  mode                  bit mask for Periodic, One-Shot, or Capture
//
//  count_up              When true, the timer counts up.  When false, it counts
//                        down
//
//  enable_interrupts     When set to true, the timer generates and interrupt
//                        when the timer expires.  When set to false, the timer
//                        does not generate interrupts.
//
//The function returns true if the base_addr is a valid general purpose timer
//*****************************************************************************
bool gp_timer_config_16(uint32_t base_addr, uint32_t mode, bool count_up, bool enable_interrupts)
{
  uint32_t timer_rcgc_mask;
  uint32_t timer_pr_mask;
  TIMER0_Type *gp_timer;
  
  // Verify the base address.
  if ( ! verify_base_addr(base_addr) )
  {
    return false;
  }

  // get the correct RCGC and PR masks for the base address
  get_clock_masks(base_addr, &timer_rcgc_mask, &timer_pr_mask);
  
  // Turn on the clock for the timer
  SYSCTL->RCGCTIMER |= timer_rcgc_mask;

  // Wait for the timer to turn on
  while( (SYSCTL->PRTIMER & timer_pr_mask) == 0) {};

  //*********************    
  // ADD CODE
  //*********************

  // Type cast the base address to a TIMER0_Type struct
  gp_timer = (TIMER0_Type *)base_addr;
    
  // Stop the timers
    gp_timer ->CTL &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN);
  // Set the timer to be a 32-bit timer
    gp_timer ->CFG = TIMER_CFG_16_BIT;
  // Clear the timer mode
    gp_timer ->TAMR &= ~(TIMER_TAMR_TAMR_M);
  // Set the mode
    gp_timer ->TAMR = mode;
  // Set the timer direction.  count_up: 0 for down, 1 for up.

  if( count_up )
  {
    // Set the direction bit
        gp_timer ->TAMR |= TIMER_TAMR_TACDIR;
  }
  
  // Disable Timer Interrupts  
    gp_timer ->IMR = enable_interrupts;
    
  return true;
}



void initializeTimer0A(void) {
	gp_timer_config_16(TIMER0_BASE,TIMER_TAMR_TAMR_PERIOD,false, true);
			gp_timer0 = (TIMER0_Type *)TIMER0_BASE;

	//Does this line divide the clock frequency by 2 (50 mhz becomes 25 mhz)
	TIMER0 -> TAPR= 2;
	//.0005 *25000000 = 12500
	gp_timer0->TAILR = 12500;
	
			gp_timer0->ICR |= TIMER_ICR_CAMCINT;
		gp_timer0 ->CTL |= 1;
		NVIC_EnableIRQ(TIMER0A_IRQn);
    NVIC_SetPriority (TIMER0A_IRQn, 1);
	
	
}

void initializeTimer1A(void) {
	 gp_timer_config_16(TIMER1_BASE,TIMER_TAMR_TAMR_PERIOD,false, true);
	 gp_timer1 = (TIMER0_Type *)TIMER1_BASE;
	//set for 5 seconds
	gp_timer1 ->TAILR = 250000000;
	
			gp_timer1->ICR |= TIMER_ICR_CAMCINT;
		gp_timer1 ->CTL |= 1;
		NVIC_EnableIRQ(TIMER1A_IRQn);
    NVIC_SetPriority (TIMER1A_IRQn, 1);
}


void initializeWatchDog(void) {
	//turn on watchdog timer for timer0
	SYSCTL ->RCGCWD |= 0x00000001;
	//for 50mHz clock, this value sets off every 10 secs 
	
	while (((SYSCTL->RCGCWD & 0x00000001 )== 0)){} 
	//finish only step 5 of the data sheet
		WATCHDOG0 ->CTL  |= 0x00000001;
		WATCHDOG0 ->LOAD = 500000000;
		NVIC_EnableIRQ(WATCHDOG0_IRQn);
		NVIC_SetPriority (WATCHDOG0_IRQn, 1);
}

void initializeSystickTimer(void) {
	
	//Set systick to go off every 5ms
	SysTick_Config(250000);
	
}

/*

void hw3_timer0_init(void) {
	
		gp_timer_config_32(TIMER0_BASE, PERIODIC, false, true);
		gp_timer0 = (TIMER0_Type *)TIMER0_BASE;
		gp_timer0->TAILR = 2500000;
		gp_timer0->ICR |= TIMER_ICR_CAMCINT;
		gp_timer0 ->CTL |= 1;
		NVIC_EnableIRQ(TIMER0A_IRQn);
    NVIC_SetPriority (TIMER0A_IRQn, 1);
		
}

void hw3_timer1_init(void) {
	
	 gp_timer_config_32(TIMER1_BASE, ONE_SHOT, true, true);
		gp_timer1 = (TIMER0_Type *)TIMER1_BASE;
		gp_timer1->TAILR = 100000000;
		gp_timer1->ICR |= TIMER_ICR_CAMCINT;
		gp_timer1 ->CTL |= 1;
		NVIC_EnableIRQ(TIMER1A_IRQn);
    NVIC_SetPriority (TIMER1A_IRQn, 1);

}*/