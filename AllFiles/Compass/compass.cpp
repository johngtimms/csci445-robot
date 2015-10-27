#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <linux/i2c-dev.h> 
#include <math.h> 
const int HMC5883L_I2C_ADDR = 0x1E; 
void selectDevice(int fd, int addr, char * name) { 
	if (ioctl(fd, I2C_SLAVE, addr) < 0) { 
		fprintf(stderr, "%s not present\n", name); 
	} 
} 

void writeToDevice(int fd, int reg, int val) { 
	char buf[2]; buf[0]=reg; buf[1]=val; 
	if (write(fd, buf, 2) != 2) { 
		fprintf(stderr, "Can't write to ADXL345\n"); 
	} 
} 

int main(int argc, char **argv) { 
	int fd; 
	unsigned char buf[16]; 
	if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) { 
		fprintf(stderr, "Failed to open i2c bus\n"); 
		return 1; 
	} 
	selectDevice(fd, HMC5883L_I2C_ADDR, "HMC5883L"); 
	writeToDevice(fd, 0x01, 32); 
	writeToDevice(fd, 0x02, 0); 
	for (int i = 0; i < 10000; ++i) { 
		buf[0] = 0x03; 
		if ((write(fd, buf, 1)) != 1) { 
			fprintf(stderr, "Error writing to i2c slave\n"); 
		} 
		if (read(fd, buf, 6) != 6) { 
			fprintf(stderr, "Unable to read from HMC5883L\n"); 
		} 
		else { 
			short x = (buf[0] << 8) | buf[1]; 
			short y = (buf[4] << 8) | buf[5]; 
			short z = (buf[2] << 8) | buf[3]; 
			float angle = atan2(y, x) * 180 / M_PI; 
			printf("x=%d, y=%d, z=%d\n", x, y, z); 
			printf("angle = %0.1f\n\n", angle); 
		} 
		usleep(600 * 1000); 
	} 
	return 0; 
}
