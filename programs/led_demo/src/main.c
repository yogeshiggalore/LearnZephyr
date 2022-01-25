/*******************************************************************************
* Project Name: led_demo
*
* Version: 1.0
*
* Description:
* In this project zephyr, uses gpio to blink led
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

/* set led sleep time */
#define SLEEP_TIME_MS	1000

/* using port gpio0 */
const char *gpio0port = "GPIO_0";

/* led is connected to pin 20 of gpio0 */
gpio_pin_t ledpin = 20;

/* set led pin as out and active low drive */
gpio_flags_t ledflag = GPIO_OUTPUT | GPIO_ACTIVE_LOW;

/* register the log */
LOG_MODULE_REGISTER(led_demo, LOG_LEVEL_DBG);

/* device structure pointer for gpio0 port */
const struct device *dev_gpio0;

int ledstatus = 0;

void main(void)
{
	/* start delay for smooth log debug */
	k_sleep(K_SECONDS(5));

	/* print project name */
	LOG_INF("zephyr led demo");

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

	/* forever */
	while(1)
	{
		if(ledstatus == 0)
		{
			gpio_pin_set(dev_gpio0, ledpin, 1);
			ledstatus = 1;
			LOG_INF("led off");
		}
		else
		{
			gpio_pin_set(dev_gpio0, ledpin, 0);
			ledstatus = 0;
			LOG_INF("led on");
		}

		k_sleep(K_MSEC(SLEEP_TIME_MS));
	}
}
