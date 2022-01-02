/*
	SFE_BMP180.cpp
	Bosch BMP180 pressure sensor library for the Arduino microcontroller
	Mike Grusin, SparkFun Electronics

	Uses floating-point equations from the Weather Station Data Logger project
	http://wmrx00.sourceforge.net/
	http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf

	Forked from BMP085 library by M.Grusin

	version 1.0 2013/09/20 initial version
	Verison 1.1.2 - Updated for Arduino 1.6.4 5/2015

	Our example code uses the "beerware" license. You can do anything
	you like with this code. No really, anything. If you find it useful,
	buy me a (root) beer someday.
*/

#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/i2c.h>
#include <SFE_BMP180.h>
#include <stdio.h>
#include <math.h>

int16_t AC1, AC2, AC3, VB1, VB2, MB, MC, MD;
uint16_t AC4, AC5, AC6;
double c5, c6, mc, md, x0, x1, x2, y0, y1, y2, p0, p1, p2;
char _error;

char i2c_begin(const struct device *i2c_dev)
// Initialize library for subsequent pressure measurements
{
	double c3,c4,b1;
	
	// Start up the Arduino's "wire" (I2C) library:
	
	// The BMP180 includes factory calibration data stored on the device.
	// Each device has different numbers, these must be retrieved and
	// used in the calculations when taking pressure measurements.

	// Retrieve calibration data from device:
	
	if (i2c_readInt(i2c_dev,0xAA,&AC1) &&
		i2c_readInt(i2c_dev,0xAC,&AC2) &&
		i2c_readInt(i2c_dev,0xAE,&AC3) &&
		i2c_readUInt(i2c_dev,0xB0,&AC4) &&
		i2c_readUInt(i2c_dev,0xB2,&AC5) &&
		i2c_readUInt(i2c_dev,0xB4,&AC6) &&
		i2c_readInt(i2c_dev,0xB6,&VB1) &&
		i2c_readInt(i2c_dev,0xB8,&VB2) &&
		i2c_readInt(i2c_dev,0xBA,&MB) &&
		i2c_readInt(i2c_dev,0xBC,&MC) &&
		i2c_readInt(i2c_dev,0xBE,&MD))
	{

		// All reads completed successfully!

		// If you need to check your math using known numbers,
		// you can uncomment one of these examples.
		// (The correct results are commented in the below functions.)

		// Example from Bosch datasheet
		// AC1 = 408; AC2 = -72; AC3 = -14383; AC4 = 32741; AC5 = 32757; AC6 = 23153;
		// B1 = 6190; B2 = 4; MB = -32768; MC = -8711; MD = 2868;

		// Example from http://wmrx00.sourceforge.net/Arduino/BMP180-Calcs.pdf
		// AC1 = 7911; AC2 = -934; AC3 = -14306; AC4 = 31567; AC5 = 25671; AC6 = 18974;
		// VB1 = 5498; VB2 = 46; MB = -32768; MC = -11075; MD = 2432;

		
		printk("AC1: "); printk("%x\n",AC1);
		printk("AC2: "); printk("%x\n",AC2);
		printk("AC3: "); printk("%x\n",AC3);
		printk("AC4: "); printk("%x\n",AC4);
		printk("AC5: "); printk("%x\n",AC5);
		printk("AC6: "); printk("%x\n",AC6);
		printk("VB1: "); printk("%x\n",VB1);
		printk("VB2: "); printk("%x\n",VB2);
		printk("MB: "); printk("%x\n",MB);
		printk("MC: "); printk("%x\n",MC);
		printk("MD: "); printk("%x\n",MD);
		
		
		// Compute floating-point polynominals:

		c3 = 160.0 * pow(2,-15) * AC3;
		c4 = pow(10,-3) * pow(2,-15) * AC4;
		b1 = pow(160,2) * pow(2,-30) * VB1;
		c5 = (pow(2,-15) / 160) * AC5;
		c6 = AC6;
		mc = (pow(2,11) / pow(160,2)) * MC;
		md = MD / 160.0;
		x0 = AC1;
		x1 = 160.0 * pow(2,-13) * AC2;
		x2 = pow(160,2) * pow(2,-25) * VB2;
		y0 = c4 * pow(2,15);
		y1 = c4 * c3;
		y2 = c4 * b1;
		p0 = (3791.0 - 8.0) / 1600.0;
		p1 = 1.0 - 7357.0 * pow(2,-20);
		p2 = 3038.0 * 100.0 * pow(2,-36);

		/*printk("c3: "); printk("%.2f\n",c3);
		printk("c4: "); printk("%f\n",c4);
		printk("c5: "); printk("%f\n",c5);
		printk("c6: "); printk("%f\n",c6);
		printk("b1: "); printk("%f\n",b1);
		printk("mc: "); printk("%f\n",mc);
		printk("md: "); printk("%f\n",md);
		printk("x0: "); printk("%f\n",x0);
		printk("x1: "); printk("%f\n",x1);
		printk("x2: "); printk("%f\n",x2);
		printk("y0: "); printk("%f\n",y0);
		printk("y1: "); printk("%f\n",y1);
		printk("y2: "); printk("%f\n",y2);
		printk("p0: "); printk("%f\n",p0);
		printk("p1: "); printk("%f\n",p1);
		printk("p2: "); printk("%f\n",p2);*/
		
		
		// Success!
		return(1);
	}
	else
	{
		// Error reading calibration data; bad component or connection?
		return(0);
	}
}


char i2c_readInt(const struct device *i2c_dev,char address, int16_t *value)
// Read a signed integer (two bytes) from device
// address: register to start reading (plus subsequent register)
// value: external variable to store data (function modifies value)
{
	unsigned char data[2];

	data[0] = address;
	if (i2c_readBytes(i2c_dev,data,2))
	{
		*value = (int16_t)((data[0]<<8)|data[1]);
		//if (*value & 0x8000) *value |= 0xFFFF0000; // sign extend if negative
		return(1);
	}
	value = 0;
	return(0);
}


char i2c_readUInt(const struct device *i2c_dev,char address, uint16_t *value)
// Read an unsigned integer (two bytes) from device
// address: register to start reading (plus subsequent register)
// value: external variable to store data (function modifies value)
{
	unsigned char data[2];

	data[0] = address;
	if (i2c_readBytes(i2c_dev,data,2))
	{
		*value = (((uint16_t)data[0]<<8)|(uint16_t)data[1]);
		return(1);
	}
	value = 0;
	return(0);
}


char i2c_readBytes(const struct device *i2c_dev,unsigned char *values, char length)
// Read an array of bytes from device
// values: external array to hold data. Put starting register in values[0].
// length: number of bytes to read
{

	if(i2c_write_read(i2c_dev,BMP180_ADDR,&values[0],1,&values[0],length) == 0)
	{
		//printk("i2c_readBytes sucess\n");
		return 1;
	}else{
		printk("i2c_readBytes fail\n");
		return 0;
	}
	
	return 0;
}


char i2c_writeBytes(const struct device *i2c_dev,unsigned char *values, char length)
// Write an array of bytes to device
// values: external array of data to write. Put starting register in values[0].
// length: number of bytes to write
{
	if(i2c_write(i2c_dev,values,length,BMP180_ADDR) == 0)
	{
		//printk("i2c_writeBytes sucess\n");
		return 1;
	}
	else
	{
		printk("i2c_writeBytes fail\n");
		return 0;
	}
	
	return 0;
}


char startTemperature(const struct device *i2c_dev)
// Begin a temperature reading.
// Will return delay in ms to wait, or 0 if I2C error
{
	unsigned char data[2], result;
	
	data[0] = BMP180_REG_CONTROL;
	data[1] = BMP180_COMMAND_TEMPERATURE;
	result = i2c_writeBytes(i2c_dev,data, 2);
	if (result) // good write?
		return(5); // return the delay in ms (rounded up) to wait before retrieving data
	else
		return(0); // or return 0 if there was a problem communicating with the BMP
}


char getTemperature(const struct device *i2c_dev,double *T)
// Retrieve a previously-started temperature reading.
// Requires begin() to be called once prior to retrieve calibration parameters.
// Requires startTemperature() to have been called prior and sufficient time elapsed.
// T: external variable to hold result.
// Returns 1 if successful, 0 if I2C error.
{
	unsigned char data[2];
	char result;
	double tu, a;
	
	data[0] = BMP180_REG_RESULT;

	result = i2c_readBytes(i2c_dev,data, 2);
	if (result) // good read, calculate temperature
	{
		tu = (data[0] * 256.0) + data[1];

		//example from Bosch datasheet
		//tu = 27898;

		//example from http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf
		//tu = 0x69EC;
		
		a = c5 * (tu - c6);
		*T = a + (mc / (a + md));

		/*		
		printk();
		printk("tu: "); printk(tu);
		printk("a: "); printk(a);
		printk("T: "); printk(*T);
		*/
	}
	return(result);
}


char startPressure(const struct device *i2c_dev,char oversampling)
// Begin a pressure reading.
// Oversampling: 0 to 3, higher numbers are slower, higher-res outputs.
// Will return delay in ms to wait, or 0 if I2C error.
{
	unsigned char data[2], result, delay;
	
	data[0] = BMP180_REG_CONTROL;

	switch (oversampling)
	{
		case 0:
			data[1] = BMP180_COMMAND_PRESSURE0;
			delay = 5;
		break;
		case 1:
			data[1] = BMP180_COMMAND_PRESSURE1;
			delay = 8;
		break;
		case 2:
			data[1] = BMP180_COMMAND_PRESSURE2;
			delay = 14;
		break;
		case 3:
			data[1] = BMP180_COMMAND_PRESSURE3;
			delay = 26;
		break;
		default:
			data[1] = BMP180_COMMAND_PRESSURE0;
			delay = 5;
		break;
	}
	result = i2c_writeBytes(i2c_dev,data, 2);
	if (result) // good write?
		return(delay); // return the delay in ms (rounded up) to wait before retrieving data
	else
		return(0); // or return 0 if there was a problem communicating with the BMP
}


char getPressure(const struct device *i2c_dev,double *P, double *T)
// Retrieve a previously started pressure reading, calculate abolute pressure in mbars.
// Requires begin() to be called once prior to retrieve calibration parameters.
// Requires startPressure() to have been called prior and sufficient time elapsed.
// Requires recent temperature reading to accurately calculate pressure.

// P: external variable to hold pressure.
// T: previously-calculated temperature.
// Returns 1 for success, 0 for I2C error.

// Note that calculated pressure value is absolute mbars, to compensate for altitude call sealevel().
{
	unsigned char data[3];
	char result;
	double pu,s,x,y,z;
	
	data[0] = BMP180_REG_RESULT;

	result = i2c_readBytes(i2c_dev,data, 3);
	if (result) // good read, calculate pressure
	{
		pu = (data[0] * 256.0) + data[1] + (data[2]/256.0);

		//example from Bosch datasheet
		//pu = 23843;

		//example from http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf, pu = 0x982FC0;	
		//pu = (0x98 * 256.0) + 0x2F + (0xC0/256.0);
		
		s = *T - 25.0;
		x = (x2 * pow(s,2)) + (x1 * s) + x0;
		y = (y2 * pow(s,2)) + (y1 * s) + y0;
		z = (pu - x) / y;
		*P = (p2 * pow(z,2)) + (p1 * z) + p0;

		/*
		printk();
		printk("pu: "); printk(pu);
		printk("T: "); printk(*T);
		printk("s: "); printk(s);
		printk("x: "); printk(x);
		printk("y: "); printk(y);
		printk("z: "); printk(z);
		printk("P: "); printk(*P);
		*/
	}
	return(result);
}


double sealevel(double P, double A)
// Given a pressure P (mb) taken at a specific altitude (meters),
// return the equivalent pressure (mb) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
	return(P/pow(1-(A/44330.0),5.255));
}


double altitude(double P, double P0)
// Given a pressure measurement P (mb) and the pressure at a baseline P0 (mb),
// return altitude (meters) above baseline.
{
	return(44330.0*(1-pow(P/P0,1/5.255)));
}


char getError(void)
	// If any library command fails, you can retrieve an extended
	// error code using this command. Errors are from the wire library: 
	// 0 = Success
	// 1 = Data too long to fit in transmit buffer
	// 2 = Received NACK on transmit of address
	// 3 = Received NACK on transmit of data
	// 4 = Other error
{
	return(_error);
}

