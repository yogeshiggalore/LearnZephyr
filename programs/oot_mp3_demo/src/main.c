/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>

int main(void)
{
	printk("code started\n");
	k_sleep(K_MSEC(5000));
	printk("code started\n");

	//static const struct device *mp3yx5300_dev = DEVICE_DT_GET_ANY(zephyr_mp3yx5300);
	static const struct device *mp3yx5300_dev = DEVICE_DT_GET(DT_NODELABEL(yx5300_1));
	//const struct device *const lsm6dsl_dev = DEVICE_DT_GET(DT_NODELABEL(mp3yx53001));

	if (mp3yx5300_dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		printk("\nError: no device found.\n");
		return 0;
	}

	if (!device_is_ready(mp3yx5300_dev)) {
		printk("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       mp3yx5300_dev->name);
		return 0;
	}

	printk("Found device \"%s\", getting sensor data\n", mp3yx5300_dev->name);

	if (mp3yx5300_dev == NULL) {
		return 0;
	}

	printk("calling after 5 sec\n");
	
	k_sleep(K_MSEC(5000));
	sensor_sample_fetch_chan(mp3yx5300_dev, SENSOR_CHAN_VOLTAGE);

	while (1) {

		//sensor_sample_fetch_chan(mp3yx5300_dev, SENSOR_CHAN_VOLTAGE);
		//sensor_channel_get(mp3yx5300_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		//sensor_channel_get(mp3yx5300_dev, SENSOR_CHAN_PRESS, &press);
		//sensor_channel_get(mp3yx5300_dev, SENSOR_CHAN_HUMIDITY, &humidity);
		//printk("ubaid it is working\n");
		k_sleep(K_MSEC(1000));
	}

	return 0;
}