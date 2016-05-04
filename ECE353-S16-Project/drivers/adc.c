#include "adc.h"

#include "TM4C123.h"





#define   PS2_ADC_BASE     ADC0_BASE






/******************************************************************************
 * Initializes ADC to use Sample Sequencer #3, triggered by the processor,
 * no IRQs
 *****************************************************************************/
bool initializeADC(  uint32_t adc_base )
{
  ADC0_Type  *myADC;
  uint32_t rcgc_adc_mask;
  uint32_t pr_mask;
  

  // examine the adc_base.  Verify that it is either ADC0 or ADC1
  // Set the rcgc_adc_mask and pr_mask  
  switch (adc_base) 
  {
    case ADC0_BASE :
    {
      
      // ADD CODE
      // set rcgc_adc_mask
			rcgc_adc_mask = SYSCTL_RCGCADC_R0;
      
      // Set pr_mask 
			pr_mask = SYSCTL_PRADC_R0 ;
      break;
    }
    case ADC1_BASE :
    {
        // ADD CODE
      // set rcgc_adc_mask
			rcgc_adc_mask = SYSCTL_RCGCADC_R1;
      
      // Set pr_mask 
			pr_mask = SYSCTL_PRADC_R1 ;
      break;
    }
    
    default:
      return false;
  }
  
  // Turn on the ADC Clock
  SYSCTL->RCGCADC |= rcgc_adc_mask;
  
  // Wait for ADCx to become ready
  while( (pr_mask & SYSCTL->PRADC) != pr_mask){}
    
  // Type Cast adc_base and set it to myADC
  myADC = (ADC0_Type *)adc_base;
  
		
	myADC ->ACTSS &= ~(ADC_ACTSS_ASEN0);
  // ADD CODE
  // disable sample sequencer #0 by writing a 0 to the 
  // corresponding ASENn bit in the ADCACTSS register 
	//	myADC -> EMUX |= ADC_EMUX_EM0_M;
		
  // ADD CODE
  // Set the event multiplexer to trigger conversion on a processor trigger
  // for sample sequencer #0.
		myADC->EMUX = 0;

  // ADD CODE
  // Set IE0 and END0 in SSCTL3
		myADC -> SSCTL0 |= ADC_SSCTL0_END4;
	//	myADC -> SSCTL0 |= ADC_SSCTL0_IE0 |ADC_SSCTL0_IE1 |ADC_SSCTL0_IE2|ADC_SSCTL0_IE3|ADC_SSCTL0_IE4;		
		
		
		
		
		
		//Set necessary ADC registers
		myADC->SSMUX0 = 0x00011100;
	
	myADC -> SSOP0 |= ADC_SSOP0_S0DCOP;
	myADC -> SSOP0 |= ADC_SSOP0_S1DCOP;
	myADC -> SSOP0 |= ADC_SSOP0_S2DCOP;
	myADC -> SSOP0 |= ADC_SSOP0_S3DCOP;
	myADC -> SSOP0 |= ADC_SSOP0_S4DCOP;
	
	myADC -> SSDC0 = 0x00043210;

	
	myADC->DCCTL0 |= ADC_DCCTL0_CIE;
	myADC->DCCTL1 |= ADC_DCCTL1_CIE;
	myADC->DCCTL2 |= ADC_DCCTL2_CIE;
	myADC->DCCTL3 |= ADC_DCCTL3_CIE;
	myADC->DCCTL4 |= ADC_DCCTL4_CIE;
	
	myADC->DCCTL0 |= ADC_DCCTL0_CIM_ONCE | ADC_DCCTL0_CIC_LOW;
	myADC->DCCTL1 |= ADC_DCCTL1_CIM_ONCE | ADC_DCCTL1_CIC_LOW;
	myADC->DCCTL2 |= ADC_DCCTL2_CIM_ONCE | ADC_DCCTL2_CIC_HIGH;
	myADC->DCCTL3 |= ADC_DCCTL3_CIM_ONCE | ADC_DCCTL3_CIC_LOW;
	myADC->DCCTL4 |= ADC_DCCTL4_CIM_ONCE | ADC_DCCTL4_CIC_HIGH;
	
	
	myADC->IM |= ADC_IM_DCONSS0;
	
	//Thresholds for pins
	myADC ->DCCMP0 = 0x0BFF03FF;
	myADC ->DCCMP1 = 0x0BFF03FF;
	myADC ->DCCMP2 = 0x0BFF03FF;
	myADC ->DCCMP3 = 0x0BFF03FF;
	myADC ->DCCMP4 = 0x0BFF03FF;
	
	myADC ->ISC |=ADC_ISC_DCINSS0;
	
	myADC ->ACTSS |= ADC_ACTSS_ASEN0;
	 
	
	NVIC_SetPriority (ADC0SS0_IRQn, 1);
	NVIC_EnableIRQ(ADC0SS0_IRQn);
	
  return true;
}
bool initializeADC1(  uint32_t adc_base ){
	  ADC0_Type  *myADC;
  uint32_t rcgc_adc_mask;
  uint32_t pr_mask;
  

  // examine the adc_base.  Verify that it is either ADC0 or ADC1
  // Set the rcgc_adc_mask and pr_mask  
  switch (adc_base) 
  {
    case ADC0_BASE :
    {
      
      // ADD CODE
      // set rcgc_adc_mask
			rcgc_adc_mask = SYSCTL_RCGCADC_R0;
      
      // Set pr_mask 
			pr_mask = SYSCTL_PRADC_R0 ;
      break;
    }
    case ADC1_BASE :
    {
        // ADD CODE
      // set rcgc_adc_mask
			rcgc_adc_mask = SYSCTL_RCGCADC_R1;
      
      // Set pr_mask 
			pr_mask = SYSCTL_PRADC_R1 ;
      break;
    }
    
    default:
      return false;
  }
  
  // Turn on the ADC Clock
  SYSCTL->RCGCADC |= rcgc_adc_mask;
  
  // Wait for ADCx to become ready
  while( (pr_mask & SYSCTL->PRADC) != pr_mask){}
    
  // Type Cast adc_base and set it to myADC
  myADC = (ADC0_Type *)adc_base;
		
		
		myADC->ACTSS &= ~ADC_ACTSS_ASEN3;
		
			myADC->EMUX = 0;
		
		myADC->IM |= ADC_IM_MASK3;
		myADC -> SSCTL3 |= ADC_SSCTL3_END0 | ADC_SSCTL3_IE0;
		
		myADC->SSMUX3 = 2;
		
		myADC ->ACTSS |= 8;
		
		NVIC_EnableIRQ(ADC1SS3_IRQn);
		
		NVIC_SetPriority (ADC1SS3_IRQn, 1);
}




/******************************************************************************
 * Reads SSMUX3 for the given ADC.  Busy waits until completion
 *****************************************************************************/
uint32_t getADCValue( uint32_t adc_base, uint8_t channel)
{
  ADC0_Type  *myADC;
  uint32_t result;
  
  if( adc_base == 0)
  {
    return false;
  }
  
	//Possible registers to change EMUX, SSCTL0, SSMUX3, ACTSS. look at what they do and modify any values
	
  myADC = (ADC0_Type *)adc_base;
  
  myADC->SSMUX3 = channel;          // Set the Channel
  
  myADC->ACTSS |= ADC_ACTSS_ASEN0;  // Enable SS0
  
  while( (myADC->RIS & ADC_RIS_INR3)  == 0)
  {
    // wait
  }
  
  result = myADC->SSFIFO3 & 0xFFF;    // Read 12-bit data
  
  myADC->ISC = ADC_ISC_IN3;          // Ack the conversion
  
  return result;
}

/******************************************************************

 * Initializes ADC to use Sample Sequencer #3, triggered by

 * the processor,no IRQs

 ******************************************************************/



void hw3_adc_init(void) {
	
	initializeADC(ADC0_BASE);
	initializeADC1(ADC1_BASE);
	

}
	






