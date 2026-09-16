#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <stdint.h>
#include "i2c.h"

#define EEPROM_SLAVE_ADDR 0xa0  
#define PAGE_SIZE 8

/*!
    \brief     	写一字节
    \param[in]  address：写入的地址（0-255）
		\param[in]  data：要写入的数据
    \param[out] none
    \retval     none
*/
void EEPROM_WriteByte(uint8_t address,uint8_t * data);

/*!
    \brief     	按页写，写完一个字节后，地址自动递增，地址范围是在当前地址所在页头到页尾，有32页，每页8个字节，所以地址范围0-255
    \param[in]  address：写入的地址（0-255）
		\param[in]  dataBuf：要写入的数据缓冲区
		\param[in]  len：长度最大不超过8，一页8个字节，超过从该页第一个字节重新写
    \param[out] none
    \retval     none
*/
void EEPROM_WritePage(uint8_t address,uint8_t * dataBuf,uint8_t len);

/*!
    \brief     	写多个字节
    \param[in]  address：写入的地址（0-255）
		\param[in]  dataBuf：要写入的数据缓冲区
		\param[in]  len：长度不超过256，总共8个字节，超过从第0页第一个字节重新写
    \param[out] none
    \retval     none
*/
void EEPROM_WriteData(uint8_t address,uint8_t *dataBuf,uint8_t len);

/*!
    \brief     	读一字节
    \param[in]  address：要读的地址（0-255）
		\param[in]  data：读出数据存放的地址
    \param[out] none
    \retval     none
*/
void EEPROM_ReadByte(uint8_t address,uint8_t * data);

/*!
    \brief     	读多个字节
    \param[in]  address：要读的地址（0-255）
		\param[in]  dataBuf：读出数据存放的缓冲区
		\param[in]  len：要读的长度
    \param[out] none
    \retval     none
*/
void EEPROM_ReadData(uint8_t address,uint8_t * dataBuf,uint8_t len);


/*等待eeprom内部操作完成*/
void eeprom_wait_standby_state(void);
#endif
