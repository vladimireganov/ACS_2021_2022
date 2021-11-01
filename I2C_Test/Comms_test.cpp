//
// Created by jtc19 on 10/31/2021.
//


#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
//There's a i2c-dev header in the Linux userspace. I can't remember if this header is shipped with the lm-sensors package, or if it will need to be installed from source. I'd check your distro's package repository. xGoat has a nice article covering preparation & usage.
//Added i2c headers that may be used to reading and writing byte
#include <i2c_smbus_read_byte>
#include <i2c_smbus_write_byte>
/*
 * Other important headers that we may like to use
    #include <i2c_smbus_read_byte_data>
    #include <i2c_smbus_write_byte_data>
*/

/*
#include <linux/i2c-dev.h> //already in use
*************************
 * Important headers that may want to be used
  Including i2c-dev header will allow the following I2C SMBus functions
  - i2c_smbus_access
  - i2c_smbus_write_quick
  - i2c_smbus_read_byte
  - i2c_smbus_write_byte
  - i2c_smbus_read_byte_data
  - i2c_smbus_write_byte_data
  - i2c_smbus_read_word_data
  - i2c_smbus_write_word_data
  - i2c_smbus_process_call
  - i2c_smbus_read_block_data
  - i2c_smbus_write_block_data
  - i2c_smbus_read_i2c_block_data
  - i2c_smbus_write_i2c_block_data
  - i2c_smbus_block_process_call
*/

int file_i2c;
int length;
unsigned char buffer[60] = {0};

int main(void) {


//----- OPEN THE I2C BUS -----
    char *filename = (char *) "/dev/i2c-1";
    if ((file_i2c = open(filename, O_RDWR)) < 0) {
//ERROR HANDLING: you can check errno to see what went wrong
        printf("Failed to open the i2c bus");
        return;
    }

    int addr = 0x68;          //<<<<<The I2C address of the slave
    if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
//ERROR HANDLING; you can check errno to see what went wrong
        return;
    }


//----- READ BYTES -----
    length = 23;            //<<< Number of bytes to read
    if (read(file_i2c, buffer, length) !=
        length)        //read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
    {
//ERROR HANDLING: i2c transaction failed
        printf("Failed to read from the i2c bus.\n");
    } else {
        printf("Data read: %s\n", buffer);
    }


//----- WRITE BYTES -----
    buffer[0] = 0x01;
    buffer[1] = 0x02;
    length = 2;            //<<< Number of bytes to write
    if (write(file_i2c, buffer, length) !=
        length)        //write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
    {
/* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");

    }
return 0
}