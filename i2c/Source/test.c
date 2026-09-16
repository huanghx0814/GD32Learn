#include "gd32f4xx.h"                   // Device header
#include "test.h"

/*≤‚ ‘«∞Õ‚…Ë≥ı ºªØ*/
void All_Init(void)
{
	USART_Init(115200);
	
	I2C_Init();
}

/*µ•◊÷Ω⁄∂¡–¥≤‚ ‘*/
int EEPROM_Test_WriteReadOneByte(void)
{
	int ret = 0;
	uint8_t addr = 0x1;
	uint8_t writeData = 0x5;
	uint8_t readData = 0;
	
	EEPROM_WriteByte(addr,&writeData);
	
	//eeprom_wait_standby_state();
	delay_1ms(5);
	
	EEPROM_ReadByte(addr,&readData);
	
	if(readData != writeData)
	{
		printf("check error\n");
		printf("read data:0x%x write data:0x%x\n",readData,writeData);
		ret =  -1;
	}

	return ret;
}

/*“≥∂¡–¥≤‚ ‘*/
int EEPROM_Test_WriteReadOnePage(void)
{
	int i = 0;
	int ret = 0;
	int len = 8;
	
	uint8_t addr =  0x0;
	uint8_t writeBuf[9] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};
	uint8_t readBuf[9] = {0};
	
	EEPROM_WritePage(addr,writeBuf,len);
	
	delay_1ms(5);
	
	EEPROM_ReadData(addr,readBuf,len);
	
	for(i=0;i<len;i++)
	{
		if(readBuf[i] != writeBuf[i])
		{
			printf("check error\n");
			printf("readbuf[%d]=0x%x writeBuf[%d]=0x%x\n",i,readBuf[i],i,writeBuf[i]);
			ret = -1;
		}
	}

	return ret;
}

/*∂‡◊÷Ω⁄∂¡–¥≤‚ ‘*/
int	EEPROM_Test_WriteReadOverPage(void)
{
	int i  = 0;
	int ret = 0;
	int len = 50;
	
	uint8_t addr = 0x3;
	uint8_t writeBuf[50] = {0};
	uint8_t readBuf[50] = {0};
	
	for(i=0;i<len;i++)
	{
		writeBuf[i] = i;
	}
	
	EEPROM_WriteData(addr,writeBuf,len);
		
	delay_1ms(5);
	
	EEPROM_ReadData(addr,readBuf,len);
	
	for(i=0;i<len;i++)
	{
		if(readBuf[i] != writeBuf[i])
		{
			printf("check error\n");
			printf("readbuf[%d]=0x%x writeBuf[%d]=0x%x\n",i,readBuf[i],i,writeBuf[i]);
			ret = -1;
		}
	}

	return ret;
}
