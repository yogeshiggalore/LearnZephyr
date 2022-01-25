/*******************************************************************************
* Project Name: uart_polling_demo
*
* Version: 1.0
*
* Description:
* In this project zephyr, uses UART to control led
* 
*
********************************************************************************
* Copyright (2022-23) , LearnZephyr
*******************************************************************************/

/* include headers */
#include <zephyr.h>
#include <logging/log.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/uart.h>

/* register the log */
LOG_MODULE_REGISTER(uartdemo, LOG_LEVEL_DBG);

/* using port gpio0 and uart0 */
const char *gpio0port = "GPIO_0";
const char *uart0port = "UART_0";

/* led is connected to pin 20 of gpio0 */
gpio_pin_t ledpin = 20;

/* set led pin as out and active low drive */
gpio_flags_t ledflag = GPIO_OUTPUT | GPIO_ACTIVE_LOW;

/* device structure pointer for gpio0 port and uart */
const struct device *dev_gpio0;
const struct device *dev_uart0;

/* uart config structure */
struct uart_config cfg_uart0;

/* uart functions */
void write_to_uart(char array[], uint8_t len);
void read_from_uart(void);

char sCMDString[] = "Use following commands to control LED \r\nA: Turn on led\r\nB: Turn off led\r\nC: Get current led status\r\n";
int  ledstatus;

void main(void)
{
    /* start delay for smooth log debug */
	k_sleep(K_SECONDS(5));

	/* print project name */
	LOG_INF("zephyr uart polling demo");
    
    /* get device binding to port gpio0 */
	dev_gpio0 = device_get_binding(gpio0port);

	if(dev_gpio0 == NULL)
	{
		LOG_ERR("device binding to port gpio0 failed ");
		return;
	}
	else
	{
		LOG_INF("device binding to port gpio0 sucess ");
	}

	/* configure pin as output */
	int ret;
	ret = gpio_pin_configure(dev_gpio0, ledpin, ledflag);
	if(ret != 0)
	{
		LOG_ERR("led pin configuration failed ");
		return;
	}
	else
	{
		LOG_INF("led pin configuration sucess ");
	}

    gpio_pin_set(dev_gpio0, ledpin, 0);
    ledstatus = 0;

    /* uart configuaration */
    cfg_uart0.baudrate  = 115200;
    cfg_uart0.data_bits = UART_CFG_DATA_BITS_8;
    cfg_uart0.flow_ctrl = UART_CFG_FLOW_CTRL_NONE;
    cfg_uart0.parity    = UART_CFG_PARITY_NONE;
    cfg_uart0.stop_bits = UART_CFG_STOP_BITS_1;

    /* get device binding to port uart0 */
	dev_uart0 = device_get_binding(uart0port);

	if(dev_uart0 == NULL)
	{
		LOG_ERR("device binding to port uart0 failed ");
		return;
	}
	else
	{
		LOG_INF("device binding to port uart0 sucess ");
	}

	/* configure uart0 */
	ret = uart_configure(dev_uart0, &cfg_uart0);
	if(ret != 0)
	{
		LOG_ERR("uart0 configuration failed ");
		return;
	}
	else
	{
		LOG_INF("uart0 configuration sucess ");
	}

    write_to_uart(sCMDString,strlen(sCMDString));

    while(1)
    {
        read_from_uart();
        k_sleep(K_MSEC(1));
    }
}

void write_to_uart(char array[], uint8_t len)
{
    
    for(uint8_t i=0; i < len;i++)
    {
        uart_poll_out(dev_uart0,array[i]);
    }
}

void read_from_uart(void)
{
    char recvch;
    if(uart_poll_in(dev_uart0,&recvch) == 0)
    {
        if((recvch == 'A') || (recvch == 'a'))
        {
            gpio_pin_set(dev_gpio0, ledpin, 0);
            ledstatus = 0;
            write_to_uart("led off\r\n",9);
        }
        else if((recvch == 'B') || (recvch == 'b'))
        {
            gpio_pin_set(dev_gpio0, ledpin, 1);
            ledstatus = 1;
            write_to_uart("led on\r\n",8);
        }
        else if((recvch == 'C') || (recvch == 'c'))
        {
            if(ledstatus == 0)
            {
                write_to_uart("led off\r\n",9);
            }
            else
            {
                write_to_uart("led on\r\n",8);
            }
        }
        else
        {
            write_to_uart("wrong cmd\r\n",11);
            write_to_uart(sCMDString,strlen(sCMDString));
        }
    }
}