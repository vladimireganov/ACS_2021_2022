
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
*/