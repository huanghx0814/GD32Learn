#ifndef _USART_H_20241009
#define _USART_H_20241009

#ifdef __cplusplus

 extern "C" {
#endif
	 
#include <stdio.h>
#include "gd32f4xx.h" // Device header
#include "io.h"
	 
#define USART_IO_NUM 2

typedef struct _USART_GPIO_CONFIG
{
	GPIO_BASIC_CONFIG *basicConfig;
}USART_GPIO_CONFIG;

typedef struct _USART_CONFIG
{
	uint32_t peripheral;
	
	rcu_periph_enum clock;
	
	USART_GPIO_CONFIG *tx;
	
	USART_GPIO_CONFIG *rx;
	
	uint32_t af;
	
	uint8_t irq;
}USART_CONFIG;

/*!
    \brief     	串口初始化
    \param[in]  baudRate:波特率(例如115200)
    \param[out] none
    \retval     none
*/
void USART_Init(int baudRate);

#ifdef __cplusplus
}
#endif
#endif
