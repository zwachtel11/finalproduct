#include "Io.h"


#define MCP23017_DEV_ID    0x20

#define IODIRA    0x00
#define IODIRB    0x01
#define IO_EXPANDER_GPIOA   0x12
#define IO_EXPANDER_GPIOB   0x13

#define FIXME   0x00


void ioExpanderInit(uint32_t i2cBase)
{
	  
  i2cSetSlaveAddr(i2cBase, MCP23017_DEV_ID, I2C_WRITE);
  
 
  i2cSendByte( i2cBase, IODIRA, I2C_MCS_START | I2C_MCS_RUN);

  
  i2cSendByte( i2cBase, 0x00,  I2C_MCS_RUN | I2C_MCS_STOP); 

  
  i2cSendByte( i2cBase, IODIRB, I2C_MCS_START | I2C_MCS_RUN);

  
  i2cSendByte( i2cBase, 0x00, I2C_MCS_RUN | I2C_MCS_STOP); 
}



void ledMatrixWriteData( uint32_t i2cBase, uint8_t colNum, uint8_t data)
{
  if( i2cBase != 0 && colNum <= 5)
  {
    
    
    i2cSetSlaveAddr(i2cBase, MCP23017_DEV_ID, I2C_WRITE);
    
    
    i2cSendByte( i2cBase, IO_EXPANDER_GPIOA, I2C_MCS_START | I2C_MCS_RUN);

    
    i2cSendByte( i2cBase, 0xFF, I2C_MCS_RUN | I2C_MCS_STOP);
    

    
    i2cSendByte( i2cBase, IO_EXPANDER_GPIOB, I2C_MCS_START | I2C_MCS_RUN);

    
    i2cSendByte( i2cBase, (1 << colNum), I2C_MCS_RUN | I2C_MCS_STOP);
    
    
    
    i2cSendByte( i2cBase,  IO_EXPANDER_GPIOA, I2C_MCS_START | I2C_MCS_RUN);

    
    i2cSendByte( i2cBase, data, I2C_MCS_RUN | I2C_MCS_STOP);
  }
}