
Important functions to use when adding i2c-dev.h header

#include <linux/i2c-dev.h> //already in use
*************************
  *These are all functions that can now be used
  - i2c_smbus_access
        *
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
  
*******************************************
        FUNCTIONALITY CONSTANTS
*******************************************
I2C_FUNC_I2C
Plain i2c-level commands (Pure SMBus adapters typically can not do these)

I2C_FUNC_10BIT_ADDR
Handles the 10-bit address extensions

I2C_FUNC_PROTOCOL_MANGLING
Knows about the I2C_M_IGNORE_NAK, I2C_M_REV_DIR_ADDR and I2C_M_NO_RD_ACK flags (which modify the I2C protocol!)

I2C_FUNC_NOSTART
Can skip repeated start sequence

I2C_FUNC_SMBUS_QUICK
Handles the SMBus write_quick command

I2C_FUNC_SMBUS_READ_BYTE
Handles the SMBus read_byte command

I2C_FUNC_SMBUS_WRITE_BYTE
Handles the SMBus write_byte command

I2C_FUNC_SMBUS_READ_BYTE_DATA
Handles the SMBus read_byte_data command

I2C_FUNC_SMBUS_WRITE_BYTE_DATA
Handles the SMBus write_byte_data command

I2C_FUNC_SMBUS_READ_WORD_DATA
Handles the SMBus read_word_data command

I2C_FUNC_SMBUS_WRITE_WORD_DATA
Handles the SMBus write_byte_data command

I2C_FUNC_SMBUS_PROC_CALL
Handles the SMBus process_call command

I2C_FUNC_SMBUS_READ_BLOCK_DATA
Handles the SMBus read_block_data command

I2C_FUNC_SMBUS_WRITE_BLOCK_DATA
Handles the SMBus write_block_data command

I2C_FUNC_SMBUS_READ_I2C_BLOCK
Handles the SMBus read_i2c_block_data command

I2C_FUNC_SMBUS_WRITE_I2C_BLOCK
Handles the SMBus write_i2c_block_data command




*******************************************