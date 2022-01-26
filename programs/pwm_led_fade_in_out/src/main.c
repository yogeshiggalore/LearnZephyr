/*******************************************************************************
* Project Name: pwm_led_fade_in_out
*
* Version: 1.0
*
* Description:
* In this project zephyr, uses pwm to fade in and fade out 
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
#include <drivers/pwm.h>

#define PERIOD	100
#define STEP	10

#define FORWARD		0
#define BACKWARD	1

/* pwm device structure */
const struct device *dev_pwm0;

/* using pwm0 port */
const char *pwm0port = "PWM_0";

/* register the log */
LOG_MODULE_REGISTER(led_pwm, LOG_LEVEL_DBG);

/* pwm pin */
int pwm_pin = 17;

int cycle=0;
int direction=FORWARD;

void main(void)
{
	/* start delay */
	k_sleep(K_SECONDS(1));

	/* project begin print */
	LOG_INF("project led_pwm demo started ");

	/* binding pwm0 device */
	dev_pwm0 = device_get_binding(pwm0port);
	if(dev_pwm0 == NULL)
	{
		LOG_ERR("device pwm0 binding failed");
		return;
	}
	else
	{
		LOG_INF("device pwm0 binding sucess");
	}

	while(1)
	{
		pwm_pin_set_usec(dev_pwm0,pwm_pin,PERIOD,cycle,0);
		
		if(direction == FORWARD)
		{
			if(cycle < PERIOD)
			{
				cycle += STEP; 
			}
			else
			{
				direction = BACKWARD;
			}
		}
		else
		{
			if(cycle > 0)
			{
				cycle -= STEP; 
			}
			else
			{
				direction = FORWARD;
			}
		}

		k_sleep(K_MSEC(PERIOD));
	}
}
