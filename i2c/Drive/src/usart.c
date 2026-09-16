#include "usart.h"

/*PA9 TX PA10 RX*/
GPIO_BASIC_CONFIG g_USART_GPIO_BasicConfig[USART_IO_NUM] = 
{
	{RCU_GPIOA,GPIOA,GPIO_PIN_9},/*tx*/
	{RCU_GPIOA,GPIOA,GPIO_PIN_10}/*rx*/
};

USART_GPIO_CONFIG g_USART_GPIO_Config[USART_IO_NUM] = {0};

USART_CONFIG g_USART_Config = {0};

/*填充句柄管脚配置*/
static void GPIO_ConfigInit()
{
	int i  = 0;
	
	USART_GPIO_CONFIG *USART_GPIO_Config = NULL;
	
	for(i=0;i<USART_IO_NUM;i++)
	{
		USART_GPIO_Config = &g_USART_GPIO_Config[i];
		
		USART_GPIO_Config->basicConfig = &g_USART_GPIO_BasicConfig[i];
	}
}

/*填充句柄配置信息*/
static void AllConfigInit()
{
	USART_CONFIG *pUSART_Config = NULL;
	
	GPIO_ConfigInit();
	
	pUSART_Config = &g_USART_Config;
	
	pUSART_Config->peripheral = USART0;
	
	pUSART_Config->clock = RCU_USART0;
	
	pUSART_Config->tx = &g_USART_GPIO_Config[0];
	
	pUSART_Config->rx = &g_USART_GPIO_Config[1];
	
	pUSART_Config->af = GPIO_AF_7;
	
	pUSART_Config->irq = USART0_IRQn;
}

/*串口GPIO初始化*/
void USART_GPIO_Config()
{
	USART_CONFIG *pUSART_Config = &g_USART_Config;
	
	rcu_periph_clock_enable(pUSART_Config->tx->basicConfig->clock);
	
	gpio_af_set(pUSART_Config->tx->basicConfig->port,pUSART_Config->af,pUSART_Config->tx->basicConfig->pin);
	gpio_mode_set(pUSART_Config->tx->basicConfig->port,GPIO_MODE_AF,GPIO_PUPD_NONE,pUSART_Config->tx->basicConfig->pin);
	gpio_output_options_set(pUSART_Config->tx->basicConfig->port,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,pUSART_Config->tx->basicConfig->pin);
	
	rcu_periph_clock_enable(pUSART_Config->rx->basicConfig->clock);
	
	gpio_af_set(pUSART_Config->rx->basicConfig->port,pUSART_Config->af,pUSART_Config->rx->basicConfig->pin);
	gpio_mode_set(pUSART_Config->rx->basicConfig->port,GPIO_MODE_AF,GPIO_PUPD_NONE,pUSART_Config->rx->basicConfig->pin);
	gpio_output_options_set(pUSART_Config->rx->basicConfig->port,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,pUSART_Config->rx->basicConfig->pin);
}

/*串口初始化*/
void USART_Init(int baudRate)
{
	USART_CONFIG *pUSART_Config = &g_USART_Config;
	
	AllConfigInit();
	
	USART_GPIO_Config();
	
	usart_deinit(pUSART_Config->peripheral);
	
	rcu_periph_clock_enable(pUSART_Config->clock);
	
	usart_baudrate_set(pUSART_Config->peripheral,baudRate);
	
	usart_transmit_config(pUSART_Config->peripheral,USART_TRANSMIT_ENABLE);
	
	usart_receive_config(pUSART_Config->peripheral,USART_RECEIVE_ENABLE);
	
	usart_enable(pUSART_Config->peripheral);
}

/*串口重定向*/
int fputc(int ch,FILE *f)
{
	USART_CONFIG *pUSART_Config = &g_USART_Config;
	
	usart_data_transmit(pUSART_Config->peripheral,(uint8_t)ch);
	
	while(RESET == usart_flag_get(pUSART_Config->peripheral,USART_FLAG_TBE));
	
	return ch;
}
