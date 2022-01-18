/*******************************************************************************
* Project Name: i2c_demo 
*
* Version: 1.0
*
* Description:
* In this project device communicate with sensor board using zephyr framework
* 
********************************************************************************
* LearnZephyr
*******************************************************************************/

/* include headers */
#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/i2c.h>
#include <BMP180.h>
#include <stdio.h>

#define MAX_I2C_ADDRESS	127

#define ALTITUDE	763.0

/* device structure */
const struct device *i2c_dev;

double Temp, Pres, PresSealvl, Alt;

char status;

/* functions */
void i2c_scan_devices(void);

void main(void)
{
	/* initial delay */
	k_sleep(K_SECONDS(5));

	/* binding i2c_0 device */
	i2c_dev = device_get_binding("I2C_0");

	/* check if binding is sucess or failure */
	if(i2c_dev == NULL)
	{
		printk("Fail to bind I2C_0 device\n");
		return;
	}
	else
	{
		printk(" I2C_0 device bind sucess\n");
	}

	/* scan i2c devices */
	scan_i2c_devices();
	
	k_sleep(K_SECONDS(1));

	/* start bmp180 read */
	printf("setting up BMP180 \n");
	BMP180_Begin(i2c_dev);
	
	/* while loop */
	while(1)
	{

		status = BMP180_StartTemperature(i2c_dev);
		if(status !=0)
		{
			k_msleep(status);
		}
		
		status = BMP180_GetTemperature(i2c_dev,&Temp);
    	if (status != 0)
    	{
			k_msleep(status);
			status = BMP180_StartPressure(i2c_dev,3);
			if(status != 0 )
			{
				PresSealvl = BMP180_Sealevel(Pres,ALTITUDE);
				Alt = BMP180_Altitude(Pres, PresSealvl);
				printf("Temp:%.2fc Pres:%.2fmb Alt:%.2fm\n",Temp,(Pres*0.0295333727 *25.4),Alt);
			}
		}
	}

	k_sleep(K_SECONDS(1));
}

void scan_i2c_devices(void)
{
	uint8_t *ui8Data;
	uint8_t ui8I2CAddr;
	uint8_t ui8Status;
	uint8_t ui8NumberOfBytes;

	/* structure for i2c messages buffer, length and flags */
	struct i2c_msg i2cmsg;
	
	i2cmsg.buf   = &ui8Data;
	i2cmsg.len   = 0;
	i2cmsg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	ui8NumberOfBytes = 1;

	printk("started scanning i2c devices\n\n");
	printk("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n00:    ");
	for(ui8I2CAddr=1;ui8I2CAddr<=MAX_I2C_ADDRESS;ui8I2CAddr++)
	{
		if((ui8I2CAddr % 0x10) == 0)	
		{
			printk("\n%2x: ",ui8I2CAddr);
		}

		ui8Status = i2c_transfer(i2c_dev, &i2cmsg, ui8NumberOfBytes, ui8I2CAddr);

		if(ui8Status == 0)
		{
			printk("%2x ",ui8I2CAddr);
		}
		else
		{
			printk("-- ");
		}

		/* 100 ms sleep */
		k_sleep(K_MSEC(100));	
	}
	printk("\n\nstopped scanning i2c devices\n");

}