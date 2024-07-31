/* emade9000.c - Driver for energy meter */

/*
 * Copyright (c) 2024, ubaid
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/pm/device.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/device.h>

#include "mp3yx5300.h"

#define DT_DRV_COMPAT zephyr_mp3yx5300

struct uart_config uart_cfg_yx5300 = {
    .baudrate  = DT_INST_PROP(0, current_speed),
    .parity    = DT_INST_PROP(0, parity),
    .stop_bits = DT_INST_PROP(0, stop_bits),
    .data_bits = DT_INST_PROP(0, data_bits),
    /* Retrieve other parameters as needed */
};

LOG_MODULE_REGISTER(MP3YX5300, CONFIG_SENSOR_LOG_LEVEL);

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#warning "MP3 YX5300 serial mp3 driver enabled without any devices"
#endif

static void mp3yx5300_uart_isr(const struct device *uart_dev, void *user_data)
{
	const struct device *dev = user_data;
	struct mp3yx5300_data *data = dev->data;

	if (uart_dev == NULL) {
		printk("UART device is NULL");
		return;
	}

	if (!uart_irq_update(uart_dev)) {
		printk("Unable to start processing interrupts");
		return;
	}

	if (uart_irq_rx_ready(uart_dev))
    {
		//data->rx_pkt.rx_count += uart_fifo_read(uart_dev, &data->rx_pkt.rx_buf[data->rx_pkt.rx_count], mp3yx5300_RX_BUF_SIZE - data->rx_pkt.rx_count);
		//mp3yx5300_print_rx_data(data);
		//mp3yx5300_parse_rx_pkt(data);
		//data->rx_pkt.rx_count = 0;
	}
}

#ifdef CONFIG_PM_DEVICE
static int mp3yx5300_pm_action(const struct device *dev,
			    enum pm_device_action action)
{
	int ret = 0;
    /*
	switch (action) {
	case PM_DEVICE_ACTION_RESUME:
		ret = bme280_chip_init(dev);
		break;
	case PM_DEVICE_ACTION_SUSPEND:
		ret = bme280_reg_write(dev,
			BME280_REG_CTRL_MEAS,
			BME280_CTRL_MEAS_OFF_VAL);

		if (ret < 0) {
			LOG_DBG("CTRL_MEAS write failed: %d", ret);
		}
		break;
	default:
		return -ENOTSUP;
	}*/

	return ret;
}
#endif /* CONFIG_PM_DEVICE */

static void mp3yx5300_uart_flush(const struct device *uart_dev)
{
	uint8_t c;
	while (uart_fifo_read(uart_dev, &c, 1) > 0) {
		continue;
	}
}

static int mp3yx5300_init(const struct device *dev)
{
	struct mp3yx5300_config *mp3_cfg = dev->config;
	int ret = 0;

	if( mp3_cfg->uart_dev != NULL )
	{
		printk("Initializing mp3 yx5300 driver");
		uart_irq_rx_disable(mp3_cfg->uart_dev);
		uart_irq_tx_disable(mp3_cfg->uart_dev);
		mp3yx5300_uart_flush(mp3_cfg->uart_dev);

		ret = uart_configure(mp3_cfg->uart_dev, &uart_cfg_yx5300);
		if (ret == -ENOSYS) {
			printk("Unable to configure UART port");
			return -ENOSYS;
		}

		ret = uart_irq_callback_user_data_set(mp3_cfg->uart_dev, mp3_cfg->cb, (void *)dev);

		if (ret < 0) {
			if (ret == -ENOTSUP) {
				printk("Interrupt-driven UART API support not enabled");
			} else if (ret == -ENOSYS) {
				printk("UART device does not support interrupt-driven API");
			} else {
				printk("Error setting UART callback: %d", ret);
			}
			return ret;
		}

		uart_irq_rx_enable(mp3_cfg->uart_dev);
	
	}
	else
	{
		printk("failed to init mp3yx5300 driver\n");
	}

	return ret;
}

static int mp3yx5300_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	printk("mp3yx5300_sample_fetch ");
		
	return -ENOTSUP;
}

static int mp3yx5300_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
	struct mp3yx5300_data *data = dev->data;

    /*
	if (chan != SENSOR_CHAN_DISTANCE) {
		return -ENOTSUP;
	}
	val->val1 = (uint32_t) (data->data / (uint16_t) 1000);
	val->val2 = (uint32_t) ((data->data % 1000) * 1000);
	*/
    return 0;
}



static const struct sensor_driver_api mp3yx5300_api_funs = {
	.sample_fetch = mp3yx5300_sample_fetch,
	.channel_get = mp3yx5300_channel_get,
    //.attr_set = ;
	//.attr_get = ;
	//.trigger_set = ;
	//.get_decoder = ;
	//.submit = ;
};

/*
 * Main instantiation macro, which selects the correct bus-specific
 * instantiation macros for the instance.
 */
#define MP3YX5300_DEFINE(inst)						\
	static struct mp3yx5300_data mp3yx5300_data_##inst;			\
	static struct mp3yx5300_config mp3yx5300_config_##inst ={			\
		.uart_dev = DEVICE_DT_GET(DT_INST_BUS(inst)),				\
        .cb = mp3yx5300_uart_isr,	\
		.uart_cfg = uart_cfg_yx5300, \
	}; \
    PM_DEVICE_DT_DEFINE(DT_DRV_INST(0), mp3yx5300_pm_ctrl);\
    SENSOR_DEVICE_DT_INST_DEFINE(inst, mp3yx5300_init, NULL, \
                    &mp3yx5300_data_##inst, &mp3yx5300_config_##inst, POST_KERNEL, \
		      CONFIG_SENSOR_INIT_PRIORITY, &mp3yx5300_api_funs);

/* Create the struct device for every status "okay" node in the devicetree. */
DT_INST_FOREACH_STATUS_OKAY(MP3YX5300_DEFINE)

//.uart_cfg.baudrate = DT_INST_PROP(inst, current_speed), \
//		.uart_cfg.data_bits = DT_INST_PROP(inst, data_bits), \
//		.uart_cfg.parity = DT_INST_PROP(inst, parity), \
//		.uart_cfg.stop_bits = DT_INST_PROP(inst, stop_bits), \