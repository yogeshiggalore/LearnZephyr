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
#include <string.h>
#include <errno.h>

/* spi structures */
struct spi_config cfg_spi;
const struct device *dev_spi;
const struct device *dev_gpio0;
struct spi_cs_control spi_cs;
struct spi_buf spi_tx_buf;

/* register the log */
LOG_MODULE_REGISTER(spidemo, LOG_LEVEL_DBG);

int spi2_master_write(struct device *spi, struct spi_config *cfg_spi, char *data);

char spi_data[10] = {'t','e','s','t','i','n','g','S','P','I'};

void main(void)
{
	/* startup delay */
	k_sleep(K_SECONDS(1));

	LOG_INF("spi demo device restarted ");

	cfg_spi.frequency = 1000000;
	cfg_spi.operation = (SPI_WORD_SET(8) | SPI_TRANSFER_MSB);
	cfg_spi.slave = 0;
	spi_cs.delay = 0;
	spi_cs.gpio_dev = dev_gpio0;
	spi_cs.gpio_dt_flags = GPIO_ACTIVE_LOW;
	spi_cs.gpio_pin = 22;
	cfg_spi.cs = &spi_cs;

	/* get device binding */
	dev_spi = device_get_binding("SPI_2");
	dev_gpio0 = device_get_binding("GPIO_0");

	LOG_INF("%s: master config @ %p:" " wordsize(%u), mode(%u/%u/%u)", __func__, &cfg_spi,
            SPI_WORD_SIZE_GET(cfg_spi.operation),
            (SPI_MODE_GET(cfg_spi.operation) & SPI_MODE_CPOL) ? 1 : 0,
            (SPI_MODE_GET(cfg_spi.operation) & SPI_MODE_CPHA) ? 1 : 0,
            (SPI_MODE_GET(cfg_spi.operation) & SPI_MODE_LOOP) ? 1 : 0);

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

	gpio_pin_configure(dev_gpio0, spi_cs.gpio_pin, (GPIO_OUTPUT | spi_cs.gpio_dt_flags));

	LOG_INF("pin:%d", cfg_spi.cs->gpio_pin);

	while (1)
	{
		k_sleep(K_MSEC(1000));
		spi2_master_write(dev_spi, &cfg_spi, spi_data);
		LOG_INF("spi2_master_write");
	}
}

int spi2_master_write(struct device *spi, struct spi_config *cfg_spi, char *data)
{
	struct spi_buf bufs = {
		.buf = data,
		.len = 10
	};

	struct spi_buf_set tx = {
		.buffers = &bufs
	};

	tx.count = 1;
	return spi_write(spi, cfg_spi, &tx);
}