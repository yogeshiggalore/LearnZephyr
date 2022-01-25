/*******************************************************************************
 * Project Name: spi_master_demo
 *
 * Version: 1.0
 *
 * Description:
 * In this project device uses spi peripheral
 *
 ********************************************************************************
 * LearnZephyr
 *******************************************************************************/

/* include headers */
#include <zephyr.h>
#include <logging/log.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include <device.h>
#include <sys/printk.h>

/* spi structures */
struct spi_config spi_cfg;
struct device *dev_spi;
struct device *dev_gpio0;
struct spi_cs_control spi_cs;
struct spi_buf spi_tx_buf;

/* register the log */
LOG_MODULE_REGISTER(spidemo, LOG_LEVEL_DBG);

void spi_test_write(void);

void main(void)
{
	/* start delay */
	k_sleep(K_SECONDS(5));

	LOG_INF("spi demo device restarted ");

	spi_cfg.frequency = 4000000;
	spi_cfg.operation = (SPI_WORD_SET(8) | SPI_TRANSFER_MSB);
	spi_cfg.slave = 0;
	spi_cs.delay = 0;
	spi_cs.gpio_dev = dev_gpio0;
	spi_cs.gpio_dt_flags = GPIO_ACTIVE_LOW;
	spi_cs.gpio_pin = 22;
	spi_cfg.cs = &spi_cs;

	/* get device binding */
 	dev_spi = device_get_binding("SPI_2");
	dev_gpio0 = device_get_binding("GPIO_0");

	if (dev_spi == NULL)
	{
		LOG_ERR("spi_2 device binding failed");
		return;
	}
	else
	{
		LOG_INF("spi_2 device binding sucess");
	}

	if (dev_gpio0 == NULL)
	{
		LOG_ERR("gpio0 device binding failed");
		return;
	}
	else
	{
		LOG_INF("gpio0 device binding sucess");
	}

	gpio_pin_configure(dev_gpio0,spi_cs.gpio_pin,(GPIO_OUTPUT | spi_cs.gpio_dt_flags));

	LOG_INF("pin:%d", spi_cfg.cs->gpio_pin);

	while (1)
	{
		k_sleep(K_MSEC(500));
		spi_test_write();
	}
}

void spi_test_write(void)
{
	uint8_t tx_buffer[5] = {0xA5,0xA4,0xA3,0xA2,0xA1};
	int err;

	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer)};
	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1};


	err = spi_write(dev_spi, &spi_cfg, &tx);

	if (err)
	{
		LOG_ERR("SPI error: %d", err);
	}
	else
	{
		/* Connect MISO to MOSI for loopback */
		LOG_INF("TX sent: %d", tx_buffer[0]);
	}
}