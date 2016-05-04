//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "TM4C123.h"
#include "wireless.h"
#include "interrupts.h"
#include "validate.h"
#include "pc_buffer.h"
#include "uart_irqs.h"
#include "eeprom.h"
#include "Io.h"
#include "gpio_port.h"
#include "adc.h"
#include "lcd.h"
#include "timers.h"


PC_Buffer UART0_Rx_Buffer;
PC_Buffer UART0_Tx_Buffer;

char Rx_Buff_Arr[UART0_BUFFER_SIZE];
char Tx_Buff_Arr[UART0_BUFFER_SIZE];


#define   IO_EXPANDER_I2C_BASE          I2C1_BASE

//*****************************************************************************
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
// 
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}
void serialDebugInit(void)
{
   gpio_enable_port(GPIOA_BASE);
   gpio_config_digital_enable( GPIOA_BASE, PA0 | PA1);
   gpio_config_alternate_function( GPIOA_BASE, PA0 | PA1);
   gpio_config_port_control( 
													GPIOA_BASE, 
													(GPIO_PCTL_PA0_M | GPIO_PCTL_PA1_M), 
													(GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX)
														);
  
	initialize_uart();
}
void i2c_init(void)
{
	gpio_enable_port(EEPROM_GPIO_BASE);

	// Configure SCL 
	gpio_config_digital_enable(EEPROM_GPIO_BASE, EEPROM_I2C_SCL_PIN);
	gpio_config_alternate_function(EEPROM_GPIO_BASE, EEPROM_I2C_SCL_PIN);
	gpio_config_port_control(EEPROM_GPIO_BASE, EEPROM_I2C_SCL_PCTL_M, EEPROM_I2C_SCL_PIN_PCTL);


	// Configure SDA 
	gpio_config_digital_enable(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN);
	gpio_config_open_drain(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN);
	gpio_config_alternate_function(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PIN);
	gpio_config_port_control(EEPROM_GPIO_BASE, EEPROM_I2C_SDA_PCTL_M, EEPROM_I2C_SDA_PIN_PCTL);

	//  Initialize the I2C peripheral
	initializeI2CMaster(EEPROM_I2C_BASE);
	
	
	gpio_enable_port(GPIOD_BASE);
	gpio_config_digital_enable(GPIOD_BASE, 0x80);
	gpio_config_enable_input(GPIOD_BASE, 0x80);
	gpio_config_falling_edge_irq(GPIOD_BASE, 0x80);
	
}
uint8_t myID[]      = { '3', '5', '3', '1', '0'};
uint8_t remoteID[]  = { '3', '5', '3', '1', '1'};

void initialize_board(void)
{
  
  DisableInterrupts();
	serialDebugInit();
	i2c_init();
	hw3_gpio_init();
	hw3_adc_init();
	initializeTimer0A();
	initializeTimer1A();
	initializeWatchDog();
	initializeSystickTimer();
	lcd_pins_init();
	lcd_screen_init();
	wireless_initialize();
//	initialize_spi();
  EnableInterrupts();
	ioExpanderInit(IO_EXPANDER_I2C_BASE);

}

void write_image(void){
	int i;

	ledMatrixWriteData(IO_EXPANDER_I2C_BASE, 2, 0xCC);
		ledMatrixWriteData(IO_EXPANDER_I2C_BASE, 1, 0x00);
	
	
	
}


void start_board(void){
	int i;
	char read_data1[80];
  char read_data2[80];
  char read_data3[2];
	
	

	  for(i = 0; i < 80; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i,&(read_data1[i]));
  }
  printf("Student1:");
	printf(read_data1);
	printf("\n");
	
  
  for(i = 0; i < 80; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i+80,&(read_data2[i]));
	
  }
  printf("Student2:");
	printf(read_data2);
  printf("\n");
  for(i = 0; i < 2; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i+160,&(read_data3[i]));
	
  }
	printf("Team Number:");
	printf(read_data3);
	printf("\n");
	write_image();
}




char data0[80];
char data1[80];
char nums2[2];

void prompt1(void){
		int i;
	  printf("new student?");
		printf("\n");
		scanf("%79[^\n]", &data0);
		if(data0[0] == '\n'){
			
		}else{
			
		
	for(i = 0; i < 80; i++)
	{
    eeprom_byte_write(EEPROM_I2C_BASE,i,data0[i]);
	}
}
	
}
void prompt2(void){
	int i;
	printf("new student?");
		printf("\n");
		scanf("%79[^\n]", &data1);
		if(data1[0] == '\n'){
			
		}else{
			
		
	for(i = 0; i < 80; i++)
	{
    eeprom_byte_write(EEPROM_I2C_BASE,i+80,data1[i]);
	}
		}
	
}
void prompt3(void){
		int i;
			printf("new group?");
		printf("\n");
		scanf("%79[^\n]", &nums2);
		if(nums2[0] == '\n'){
			
		}else{
			
		
	for(i = 0; i < 2; i++)
	{
    eeprom_byte_write(EEPROM_I2C_BASE,i+160,nums2[i]);
	}
		}
	}
bool dwn_pressed(void){

	
	char read_data1[80];
	char read_data2[80];
	char read_data3[2];
	int i;
	
	for(i = 0; i < 80; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i,&(read_data1[i]));
  }
		

  printf("Student1:");
	printf(read_data1);
	printf("\n");

	prompt1();
	
	for(i = 0; i < 80; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i+80,&(read_data2[i]));
	
  }

	printf("Student2:");
	printf(read_data2);
	printf("\n");

	prompt2();
			 
	
	for(i = 0; i < 2; i++)
  {
    eeprom_byte_read(EEPROM_I2C_BASE,i+160,&(read_data3[i]));
  }

	printf("Team Number:");
	printf(read_data3);
	printf("\n");

	prompt3();
	
	return true;
	
}

void boardTX(int position){
  uint32_t status;
	int j;
	

			while ((status = wireless_send_32(false, false, position) ) != NRF24L01_TX_SUCCESS){}
				for( j = 0; j <5000;j++){}
			
			

			
	}



	

int boardRX(void){
	
	
	uint32_t	data;
	uint32_t status;
	int i = 0;
	status =  wireless_get_32(false, &data);
	//	while (((status =  wireless_get_32(false, &data)) != NRF24L01_RX_SUCCESS)) {};
	stats++;
			
		
	return data;
		
		
}


int main(void)
{
	
	initialize_board();
  // Infinite Loop
  setup_eeprom();
  start_board();
	
  wireless_configure_device(myID, remoteID ) ;
	lcd_clear();

	lcd_write_char_10pts( 0, '|', 0);
	
  lcd_write_char_10pts( 1, '$', 5);
	 
	lcd_write_char_10pts( 3, '|', 9);

 while(1){

	 start_game();
 }
 
}
volatile uint32_t y1;
volatile uint32_t y2;
volatile uint32_t y3;
volatile uint32_t x1;
volatile uint32_t x2;
volatile uint32_t x3;
volatile uint32_t player1;
volatile uint32_t player2;
volatile char c1;
volatile char c2;
volatile uint32_t lastvalue = 0;
void score(void){
	lcd_write_char_10pts( 0,c1, 3);
  lcd_write_char_10pts( 0, c2, 6);
}
void test_lcd(void)
{
	 lcd_clear();


	
  lcd_write_char_10pts( y1, '|', x1);
  lcd_write_char_10pts( y2, '|', x2);
  lcd_write_char_10pts( y3, '$', x3);
	score();
}

void end_game(void){
	printf("gameover");
	DisableInterrupts();
	while(1){
		
	}
	
}
void check_board(void){
	
			if(y1 != lastvalue){
					lastvalue = y1;
					boardTX(y1);
				
		}
			
		
		if(DRUPT){
				DRUPT = false;
			y2 = boardRX();
			
		}
}
bool right = true;
bool left = false;
int start_game(void){
	

	bool playin = true;
	int i;
	int count;
	y1 = 0;
	x1 = 0;
	y2 = 3;
	x2 = 9;
	y3 = 2;
	x3 = 5;
	lcd_clear();
	test_lcd();
	while(playin){
		count++;
		if(BTN_A == true){
			if(y2 <3){
				y2++;
			}
			BTN_A = false;
		}
		if(BTN_B == true){
			if(y2 > 0){
				y2--;
			}
			BTN_B = false;
		}
		if(result >= 3072){
			y1 = 0;
			
		}else if (result < 3072 && result>= 2048){
			y1 = 1;
			
		}else if(result < 2048 && result>= 1024){
			y1 = 2;
			
		}else {
			y1= 3;
		}
		
		
		check_board();
		
		if(x3 == 0){
			left = false;
			right = true;
			if(y3 != y1){
				player2++;
				playin = false;
				//boardTX(2);
			}
		}else if (x3 == 9){
			right = false;
			left = true;
			if(y3 != y2){
				player1++;
				playin = false;
				//boardTX(1);
			}
		}
		if(right && count%3 == 1){
			x3++;
			if (x3%2 == 0){
				if(y3 > 0)
					y3--;
				else y3++;
			}
		}else if(count%3 == 1){ x3 --;
			if (x3%2 == 0){
				if(y3 < 3) y3++;
				else y3 --;
				
			}}
	c1 = player1 + '0';
	c2 = player2 + '0';
		test_lcd();
			if(DOWN){
				DOWN = false;
				dwn_pressed();
			}
		for (i = 0; i < 4000000; i++){}	
			if(WATCHD){
				end_game();
			}
			if (player1 ==9 || player2 ==9){
				printf("you win\n");
				end_game();
			}
		if(count > 5000){
		printf("Packet Stats: ");
		printf("%d", stats);
		printf("\n");
		count = 0;
	}
	}
}

