/*******************************************************************************
* Project Name: uart_interrupt_demo
*
* Version: 1.0
*
* Description:
* In this project zephyr, uses uart interrupt for demo
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
#include <logging/log.h>
#include <mp3.h>

/* register the log */
LOG_MODULE_REGISTER(mp3demo, LOG_LEVEL_NONE);

/* using port uart0 */
const char *uart0port = "UART_0";

/* device pointer to for uart 0 port */
const struct device *dev_uart0;

/* uart0 config structure */
struct uart_config cfg_uart0;

static void uart0_cb(const struct device *dev, void *user_data);

static uint8_t uart0_buf[256];

void write_to_uart(char array[], uint8_t len);

void main(void)
{
	/* start delay for smooth log */
	k_sleep(K_SECONDS(1));

	/* print project name */
	LOG_INF("zephyr uart interrupt demo");

	/* get uart0 device binding */
	dev_uart0 = device_get_binding(uart0port);

	if(dev_uart0 == NULL)
	{
		LOG_ERR("device binding to port uart0 failed");
		return;
	}
	else
	{
		LOG_INF("device binding to port uart0 sucess");
	}

	/* uart configuration */
	cfg_uart0.baudrate = 9600;
	cfg_uart0.data_bits = UART_CFG_DATA_BITS_8;
	cfg_uart0.flow_ctrl = UART_CFG_FLOW_CTRL_NONE;
	cfg_uart0.parity = UART_CFG_PARITY_NONE;
	cfg_uart0.stop_bits = UART_CFG_STOP_BITS_1;

	/* configure uart0 */
	int ret = uart_configure(dev_uart0, &cfg_uart0);
	if(ret != 0)
	{
		LOG_ERR("uart0 configuration failed");
		return;		
	}
	else
	{
		LOG_INF("uart0 configuration sucess");
	} 

	int uart1_data = 3;
    uart_irq_callback_user_data_set(dev_uart0, uart0_cb, &uart1_data);
	uart_irq_rx_enable(dev_uart0);

	k_sleep(K_SECONDS(1));
	LOG_INF("zephyr uart interrupt demo");

	uint8_t aui8tx[8] = {0x7E,0xFF,0x06,0x03,0x00,0x00,0x10,0xEF};
	write_to_uart(aui8tx,sizeof(aui8tx));
}

static void uart0_cb(const struct device *dev, void *user_data)
{
    //printk("*");
	uart_irq_update(dev);
	int data_length = 0;

	if (uart_irq_rx_ready(dev)) {
		data_length = uart_fifo_read(dev, uart0_buf, sizeof(uart0_buf));
		uart0_buf[data_length] = 0;
	}

	printk("%s\n", uart0_buf);
}

void write_to_uart(char array[], uint8_t len)
{
    
    for(uint8_t i=0; i < len;i++)
    {
        uart_poll_out(dev_uart0,array[i]);
    }
}
