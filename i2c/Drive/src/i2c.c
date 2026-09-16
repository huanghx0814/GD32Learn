#include "i2c.h"

/*PB6 SCL PB7 SDA*/
GPIO_BASIC_CONFIG g_I2C_GPIO_BasicConfig[I2C_IO_NUM] = 
{
	{RCU_GPIOB,GPIOB,GPIO_PIN_6},/*SCL*/
	{RCU_GPIOB,GPIOB,GPIO_PIN_7}/*SDA*/
};

I2C_GPIO_CONFIG g_I2C_GPIO_Config[I2C_IO_NUM] = {0};

I2C_CONFIG g_I2C_Config = {0};

/*填充句柄管脚配置*/
static void GPIO_ConfigInit()
{
	int i  = 0;
	
	I2C_GPIO_CONFIG *pI2C_GPIO_Config = NULL;
	
	for(i=0;i<I2C_IO_NUM;i++)
	{
		pI2C_GPIO_Config = &g_I2C_GPIO_Config[i];
		
		pI2C_GPIO_Config->basicConfig = &g_I2C_GPIO_BasicConfig[i];
	}
}

/*填充句柄配置信息*/
static void AllConfigInit()
{
	I2C_CONFIG * pI2C_Config = NULL;
	
	GPIO_ConfigInit();
	
	pI2C_Config = &g_I2C_Config;
	
	pI2C_Config->peripheral = I2C0;
	
	pI2C_Config->clock = RCU_I2C0;
	
	pI2C_Config->scl = &g_I2C_GPIO_Config[0];
	
	pI2C_Config->sda = &g_I2C_GPIO_Config[1];
	
	pI2C_Config->af = GPIO_AF_4;
	
}

/*I2C GPIO初始化*/
void I2C_GPIO_Config()
{
	I2C_CONFIG * pI2C_Config = &g_I2C_Config;
	
	rcu_periph_clock_enable(pI2C_Config->scl->basicConfig->clock);
	
	gpio_af_set(pI2C_Config->scl->basicConfig->port,pI2C_Config->af,pI2C_Config->scl->basicConfig->pin);
	gpio_mode_set(pI2C_Config->scl->basicConfig->port,GPIO_MODE_AF,GPIO_PUPD_NONE,pI2C_Config->scl->basicConfig->pin);
	gpio_output_options_set(pI2C_Config->scl->basicConfig->port,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,pI2C_Config->scl->basicConfig->pin);
	
	rcu_periph_clock_enable(pI2C_Config->sda->basicConfig->clock);
	
	gpio_af_set(pI2C_Config->sda->basicConfig->port,pI2C_Config->af,pI2C_Config->sda->basicConfig->pin);
	gpio_mode_set(pI2C_Config->sda->basicConfig->port,GPIO_MODE_AF,GPIO_PUPD_NONE,pI2C_Config->sda->basicConfig->pin);
	gpio_output_options_set(pI2C_Config->sda->basicConfig->port,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,pI2C_Config->sda->basicConfig->pin);
}

/*初始化I2C*/
void I2C_Init(void)
{
	I2C_CONFIG * pI2C_Config = &g_I2C_Config;
	
	AllConfigInit();
	
	/*配置GPIO*/
	I2C_GPIO_Config();
	
	/*I2C时钟初始化*/
	rcu_periph_clock_enable(pI2C_Config->clock);
	
	/*配置I2C速率，400kb/s，占空比为50%*/
	i2c_clock_config(pI2C_Config->peripheral,I2C_SPEED,I2C_DTCY_2);
	
	/*设置I2C模式、默认是从机模式，选择从机地址*/
	i2c_mode_addr_config(pI2C_Config->peripheral,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,SLAVE_ADDR<<1);
	
	/*使能I2C*/
	i2c_enable(pI2C_Config->peripheral);
	
	i2c_ack_config(pI2C_Config->peripheral,I2C_ACK_ENABLE);
}
