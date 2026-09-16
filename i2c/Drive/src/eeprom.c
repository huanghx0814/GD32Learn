#include "gd32f4xx.h" // Device header
#include "eeprom.h"

/*写一字节*/
void EEPROM_WriteByte(uint8_t address,uint8_t * data)
{
		/*等待I2C总线空闲*/
		while(SET == i2c_flag_get(I2C0,I2C_FLAG_I2CBSY));
	
		/*发送启动信号*/
		i2c_start_on_bus(I2C0);
	
		/*等待SBSEND置一，代表启动信号发送成功*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_SBSEND));
	
		/*写入从机地址，清SBSEND*/
		i2c_master_addressing(I2C0,EEPROM_SLAVE_ADDR,I2C_TRANSMITTER);
	
		/*等待ADDSEND置一，代表发送从机地址并收到ACK*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_ADDSEND));
	
		/*清待ADDSEND*/
		i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
		
		/*等待TBE置一，代表数据寄存器为空*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_TBE));
	
		/*发送要写的地址*/
		i2c_data_transmit(I2C0,address);
	
		/*等待BTC置一，代表发送的字节完成，数据和移位寄存器都为空*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_BTC));
	
		/*发送要写的数据*/
		i2c_data_transmit(I2C0,*data);
	
		/*等待BTC置一，代表发送的字节完成，数据和移位寄存器都为空*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_BTC));
		
		/*发送停止信号*/
		i2c_stop_on_bus(I2C0);
		
		/*等待停止信号发送完成*/
		while(I2C_CTL0(I2C0) & I2C_CTL0_STOP);
}

/*按页连续写，写完一个字节后，地址自动递增，地址范围是在当前地址所在页头到页尾，有32页，每页8个字节，所以地址范围0-255*/
void EEPROM_WritePage(uint8_t address,uint8_t * dataBuf,uint8_t len)
{
		/*等待I2C总线空闲*/
		while(SET == i2c_flag_get(I2C0,I2C_FLAG_I2CBSY));
	
		/*发送启动信号*/
		i2c_start_on_bus(I2C0);
	
		/*等待SBSEND置一，代表启动信号发送成功*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_SBSEND));
	
		/*写入从机地址，清SBSEND*/
		i2c_master_addressing(I2C0,EEPROM_SLAVE_ADDR,I2C_TRANSMITTER);
	
		/*等待ADDSEND置一，代表发送从机地址并收到ACK*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_ADDSEND));
	
		/*清待ADDSEND*/
		i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
		
		/*等待TBE置一，代表数据寄存器为空*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_TBE));
	
		/*发送要写的地址*/
		i2c_data_transmit(I2C0,address);
	
		/*等待BTC置一，代表发送的字节完成，数据和移位寄存器都为空*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_BTC));
		
		while(len--)
		{
			/*发送要写的数据*/
			i2c_data_transmit(I2C0,*dataBuf);
			
			dataBuf++;
			
			while(RESET == i2c_flag_get(I2C0,I2C_FLAG_BTC));
		}
		
		/*发送停止信号*/
		i2c_stop_on_bus(I2C0);
		
		/*等待停止信号发送完成*/
		while(I2C_CTL0(I2C0) & I2C_CTL0_STOP);
}

/*写多个字节*/
void EEPROM_WriteData(uint8_t address,uint8_t *dataBuf,uint8_t len)
{
	/*写入地址位于当前页位置 addressLocOfCurrentPage、当前页剩下字节数leftByteSizeOfCurrentPage*/
	/*写入长度要写满几页 writeCostPageCount，剩下未写完字节数 leftByteCount*/
	/*地址范围0-255，长度范围256个字节，有32页，每页8字节*/
	uint8_t addressLocOfCurrentPage = address % PAGE_SIZE;
	uint8_t leftByteSizeOfCurrentPage = PAGE_SIZE - addressLocOfCurrentPage;
	uint8_t writeCostPageCount = len / PAGE_SIZE;
	uint8_t leftByteCount = len % PAGE_SIZE;
	
	/*地址与页大小对齐*/
	/*循环从当前地址按整页写，直到剩余要写长度不满一页大小*/
	/*地址不与页大小对齐*/
	/*写入字节数小于当前页剩余字节数则直接写入，否则先将当前地址所在页剩余字节写满使地址与页大小重新对齐，然后再按照以上处理*/
	if(0 == addressLocOfCurrentPage)
	{
		while(writeCostPageCount --)
		{
			EEPROM_WritePage(address,dataBuf,PAGE_SIZE);
			eeprom_wait_standby_state();
			address += PAGE_SIZE;
			dataBuf += PAGE_SIZE;
		}
		
		if(0 != leftByteCount)
		{
			EEPROM_WritePage(address,dataBuf,leftByteCount);
			eeprom_wait_standby_state();
		}
	}
	else
	{
		if(len <= leftByteSizeOfCurrentPage)
		{
			EEPROM_WritePage(address,dataBuf,len);
			eeprom_wait_standby_state();
		}
		else
		{
			EEPROM_WritePage(address,dataBuf,leftByteSizeOfCurrentPage);
			eeprom_wait_standby_state();
			
			len -= leftByteSizeOfCurrentPage;
			address += leftByteSizeOfCurrentPage;
			dataBuf += leftByteSizeOfCurrentPage;
		
			writeCostPageCount = len / PAGE_SIZE;
			leftByteCount = len % PAGE_SIZE;
			
			while(writeCostPageCount --)
			{
				EEPROM_WritePage(address,dataBuf,PAGE_SIZE);
				eeprom_wait_standby_state();
				address += PAGE_SIZE;
				dataBuf += PAGE_SIZE;
			}
		
			if(0 != leftByteCount)
			{
			EEPROM_WritePage(address,dataBuf,leftByteCount);
			eeprom_wait_standby_state();
			}
		}	
	}
}

/*读一个字节*/
void EEPROM_ReadByte(uint8_t address,uint8_t * data)
{
		/*等待I2C总线空闲*/
		while(SET == i2c_flag_get(I2C0,I2C_FLAG_I2CBSY));
	
		/*发送启动信号*/
		i2c_start_on_bus(I2C0);
	
		/*等待SBSEND置一，代表启动信号发送成功*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_SBSEND));
	
		/*写入从机地址，清SBSEND*/
		i2c_master_addressing(I2C0,EEPROM_SLAVE_ADDR,I2C_TRANSMITTER);
	
		/*等待ADDSEND置一，代表发送从机地址并收到ACK*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_ADDSEND));
	
		/*清待ADDSEND*/
		i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
		
		/*等待TBE置一，代表数据寄存器为空*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_TBE));
	
		/*发送要读的地址*/
		i2c_data_transmit(I2C0,address);
	
		/*等待BTC置一，代表发送的字节完成，数据和移位寄存器都为空*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_BTC));
		
		/*发送启动信号*/
		i2c_start_on_bus(I2C0);
		
		/*等待SBSEND置一，代表启动信号发送成功*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_SBSEND));
	
		/*写入从机地址，清SBSEND*/
		i2c_master_addressing(I2C0,EEPROM_SLAVE_ADDR,I2C_RECEIVER);
		
		/*等待ADDSEND置一，代表发送从机地址并收到ACK*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_ADDSEND));
	
		i2c_ack_config(I2C0, I2C_ACK_DISABLE);	
	
		/*清待ADDSEND*/
		i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
		
		/*发送停止信号*/
		i2c_stop_on_bus(I2C0);
		
		/*等待接收缓冲区不为空*/
		while(RESET == i2c_flag_get(I2C0,I2C_FLAG_RBNE));
		
		*data = i2c_data_receive(I2C0);
		
		i2c_ack_config(I2C0, I2C_ACK_ENABLE);	
		
		/*等待停止信号发送完成*/
		while(I2C_CTL0(I2C0) & I2C_CTL0_STOP);
}

/*读多个字节*/
void EEPROM_ReadData(uint8_t address,uint8_t * dataBuf,uint8_t len)
{	
	/*只读取两字节时，需要在收到最后一个字节时，不发送ACK，当BTC置位时，此时已收到最后一个字节，所以需要设置ACK只作用在下一个字节*/
	if(len == 2)
	{
		i2c_ackpos_config(I2C0,I2C_ACKPOS_NEXT);
	}
	
	/*等待I2C总线空闲*/
	while(SET == i2c_flag_get(I2C0,I2C_FLAG_I2CBSY));

	/*发送启动信号*/
	i2c_start_on_bus(I2C0);

	/*等待SBSEND置一，代表启动信号发送成功*/
	while(RESET == i2c_flag_get(I2C0,I2C_FLAG_SBSEND));

	/*写入从机地址，清SBSEND*/
	i2c_master_addressing(I2C0,EEPROM_SLAVE_ADDR,I2C_TRANSMITTER);

	/*等待ADDSEND置一，代表发送从机地址并收到ACK*/
	while(RESET == i2c_flag_get(I2C0,I2C_FLAG_ADDSEND));

	/*清待ADDSEND*/
	i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
	
	/*等待TBE置一，代表数据寄存器为空*/
	while(RESET == i2c_flag_get(I2C0,I2C_FLAG_TBE));

	/*发送要读的地址*/
	i2c_data_transmit(I2C0,address);

	/*等待BTC置一，代表发送的字节完成，数据和移位寄存器都为空*/
	while(RESET == i2c_flag_get(I2C0,I2C_FLAG_BTC));
	
	/*发送启动信号*/
	i2c_start_on_bus(I2C0);
	
	/*等待SBSEND置一，代表启动信号发送成功*/
	while(RESET == i2c_flag_get(I2C0,I2C_FLAG_SBSEND));

	/*写入从机地址，清SBSEND*/
	i2c_master_addressing(I2C0,EEPROM_SLAVE_ADDR,I2C_RECEIVER);
	
	/*等待ADDSEND置一，代表发送从机地址并收到ACK*/
	while(RESET == i2c_flag_get(I2C0,I2C_FLAG_ADDSEND));
	
	/*只读取1、2字节时，需要失能ACK*/
	if(len < 3)
	{
		i2c_ack_config(I2C0, I2C_ACK_DISABLE);	
	}

	/*清待ADDSEND*/
	i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
	
	if(len == 1)
	{
		i2c_stop_on_bus(I2C0);
	}
	
	while(len)
	{
		/*数据寄存器收到倒数第三个字节，移位寄存器存放倒数第二个字节，SCL被拉低阻止继续通信，此时失能ACK*/
		if(len == 3)
		{
			while(RESET == i2c_flag_get(I2C0,I2C_FLAG_BTC));
			i2c_ack_config(I2C0, I2C_ACK_DISABLE);	
		}
		
		/*数据寄存器收到倒数第二个字节，移位寄存器存放倒数第一个字节，此时发送停止信号*/
		if(len == 2)
		{
			while(RESET == i2c_flag_get(I2C0,I2C_FLAG_BTC));
			i2c_stop_on_bus(I2C0);
		}
		
		if(SET == i2c_flag_get(I2C0,I2C_FLAG_RBNE))
		{
			*dataBuf = i2c_data_receive(I2C0);
			dataBuf++;
			len --;
		}
	}
	
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);	
	
	i2c_ackpos_config(I2C0,I2C_ACKPOS_CURRENT);
	
	while(I2C_CTL0(I2C0) & I2C_CTL0_STOP);
}

void eeprom_wait_standby_state(void)
{
    __IO uint32_t val = 0;
    
    while(1){
        /* wait until I2C bus is idle */
        while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
        
        /* send a start condition to I2C bus */
        i2c_start_on_bus(I2C0);
        
        /* wait until SBSEND bit is set */
        while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
        
        /* send slave address to I2C bus */
        i2c_master_addressing(I2C0, EEPROM_SLAVE_ADDR, I2C_TRANSMITTER);
        
        /* keep looping till the Address is acknowledged or the AE flag is set (address not acknowledged at time) */
        do{
            /* get the current value of the I2C_STAT0 register */
            val = I2C_STAT0(I2C0);
            
        }while(0 == (val & (I2C_STAT0_ADDSEND | I2C_STAT0_AERR)));
         
        /* check if the ADDSEND flag has been set */
        if(val & I2C_STAT0_ADDSEND){
            
            /* clear ADDSEND flag */
            i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
            
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C0);
            
            /* exit the function */
            return ;
        } else {
            /* clear the bit of AERR */
            i2c_flag_clear(I2C0, I2C_FLAG_AERR);
        }
        
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C0);
        /* wait until the stop condition is finished */
        while(I2C_CTL0(I2C0)&I2C_CTL0_STOP);
    }
}



