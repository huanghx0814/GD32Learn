#ifndef _IO_H_20240929
#define _IO_H_20240929

#ifdef __cplusplus

 extern "C" {
#endif
	 
#include "gd32f4xx.h"                   // Device header

typedef struct _GPIO_BASIC_CONFIG
{
	rcu_periph_enum clock;
	uint32_t port;
	uint32_t pin;	
}GPIO_BASIC_CONFIG;

#ifdef __cplusplus
}
#endif
#endif
