#ifndef _I2C_H_
#define _I2C_H_

#ifdef __cplusplus

 extern "C" {
#endif
	 
#include "gd32f4xx.h"                   // Device header
#include "io.h"
	 
#define I2C_SPEED 400000
#define SLAVE_ADDR 0x1

#define I2C_IO_NUM 2

typedef struct _I2C_GPIO_CONFIG
{
	GPIO_BASIC_CONFIG *basicConfig;
}I2C_GPIO_CONFIG;

typedef struct _I2C_CONFIG
{
	uint32_t peripheral;
	
	rcu_periph_enum clock;
	
	I2C_GPIO_CONFIG * scl;
	
	I2C_GPIO_CONFIG * sda;
	
	uint32_t af;
}I2C_CONFIG;
	
/*!
    \brief     	I2C≥ı ºªØ
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C_Init(void);
	 
#ifdef __cplusplus
}
#endif
#endif

