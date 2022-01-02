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
#include <SFE_BMP180.h>

#define MAX_I2C_ADDRESS	127

#define TSL2561_ADDRESS	0x39
#define BMP180_ADDRESS	0x77

/* device structure */
const struct device *i2c_dev;

/* functions */
void i2c_scan_devices(void);
static int read_bytes(const struct device *i2c_dev, uint16_t addr,uint8_t *data, uint32_t num_bytes);


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
	
	/*uint8_t write_buf[16];
	uint8_t read_buf[16];
	int ret;

	write_buf[0] = 0xAA;
	ret = i2c_write_read(i2c_dev,BMP180_ADDRESS,&write_buf,1,&read_buf,2);
	printk("%2x\n",ret);
	printk("%2x\n",read_buf[0]);
	printk("%2x\n",read_buf[1]);

	k_sleep(K_SECONDS(1));

	write_buf[0] = 0;
	write_buf[1] = 0xAA;
	read_buf[0] = 0;
	read_buf[1] = 0;
	ret = i2c_write_read(i2c_dev,BMP180_ADDRESS,&write_buf,2,&read_buf,2);
	printk("%2x\n",ret);
	printk("%2x\n",read_buf[0]);
	printk("%2x\n",read_buf[1]);

	k_sleep(K_SECONDS(1));*/

	i2c_begin(i2c_dev);
	
	/* while loop */
	while(1)
	{
		k_sleep(K_SECONDS(1));
		double T;
		char status;
		status = getTemperature(i2c_dev,&T);
    	if (status != 0)
    	{
      		// Print out the measurement:
      		printk("temperature: ");
      		printk("%d\n",T*10);
		}
	}
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

//void i2c_read_byte(const struct device *i2c_dev, uint16_t addr,uint8_t *data, uint32_t num_bytes, uint8_t *error)
//{

//}

static int read_bytes(const struct device *i2c_dev, uint16_t addr,uint8_t *data, uint32_t num_bytes)
{
	uint8_t wr_addr[2];
	struct i2c_msg msgs[2];

	/* Now try to read back from FRAM */

	/* FRAM address */
	wr_addr[0] = (addr >> 8) & 0xFF;
	wr_addr[1] = addr & 0xFF;

	/* Setup I2C messages */

	/* Send the address to read from */
	msgs[0].buf = wr_addr;
	msgs[0].len = 2U;
	msgs[0].flags = I2C_MSG_WRITE;

	/* Read from device. STOP after this. */
	msgs[1].buf = data;
	msgs[1].len = num_bytes;
	msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

	return i2c_transfer(i2c_dev, &msgs[0], 2, BMP180_ADDRESS);
}