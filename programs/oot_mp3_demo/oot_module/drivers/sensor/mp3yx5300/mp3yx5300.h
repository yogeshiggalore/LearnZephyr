#ifndef ZEPHYR_DRIVERS_SENSOR_MP3YX5300_MP3YX5300_H_
#define ZEPHYR_DRIVERS_SENSOR_MP3YX5300_MP3YX5300_H_

#include <zephyr/types.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>

struct mp3yx5300_data
{
    const struct device *dev;
    uint8_t test1;
};

struct mp3yx5300_config
{
    const struct device *uart_dev;
    struct uart_config uart_cfg;
    uart_irq_callback_user_data_t cb;
    uint8_t test2;
};

#endif /* ZEPHYR_DRIVERS_SENSOR_EMADE9000_EMADE9000_H_ */

