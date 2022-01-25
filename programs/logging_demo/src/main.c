/*******************************************************************************
* Project Name: logging_demo 
*
* Version: 1.0
*
* Description:
* In this project device zephyr log module for demonstration
* 
********************************************************************************
* LearnZephyr
*******************************************************************************/

/* include headers */
#include <zephyr.h>
#include <sys/printk.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(logdemo,LOG_LEVEL_DBG);

void main(void)
{
	k_sleep(K_SECONDS(5));
	printk("Hello World! %s\n", CONFIG_BOARD);
	LOG_INF("Hello World!");
	LOG_ERR("Hello World!");
	LOG_DBG("Hello World!");
	LOG_WRN("Hello World!");
}
