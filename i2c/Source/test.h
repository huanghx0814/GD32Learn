#ifndef _TEST_H_20241007
#define _TEST_H_20241007

#include <stdio.h>
#include "systick.h"
#include "eeprom.h"
#include "usart.h"

/*!
    \brief     	测试前外设初始化
    \param[in]  none
    \param[out] none
    \retval     none
*/
void All_Init(void);

/*!
    \brief     	单字节读写测试
    \param[in]  none
    \param[out] none
    \retval     none
*/
int EEPROM_Test_WriteReadOneByte(void);

/*!
    \brief     	页读写测试
    \param[in]  none
    \param[out] none
    \retval     none
*/
int EEPROM_Test_WriteReadOnePage(void);

/*!
    \brief     	多字节读写测试
    \param[in]  none
    \param[out] none
    \retval     none
*/
int	EEPROM_Test_WriteReadOverPage(void);

#endif
