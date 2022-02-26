#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev_smbus.h>      //Needed for I2C port
#include "DFRobot_BMX160.h"


DFRobot_BMX160::DFRobot_BMX160(int bus_file, char addr)
{

  my_addr = addr;
  i2c_bus = bus_file;
  Obmx160 = (struct bmx160Dev *)malloc(sizeof(struct bmx160Dev));
  Oaccel = (struct bmx160SensorData*)malloc(sizeof(struct bmx160SensorData));
  Ogyro = (struct bmx160SensorData*)malloc(sizeof(struct bmx160SensorData));
  Omagn = (struct bmx160SensorData*)malloc(sizeof(struct bmx160SensorData));
}

const uint8_t int_mask_lookup_table[13] = {
    BMX160_INT1_SLOPE_MASK,
    BMX160_INT1_SLOPE_MASK,
    BMX160_INT2_LOW_STEP_DETECT_MASK,
    BMX160_INT1_DOUBLE_TAP_MASK,
    BMX160_INT1_SINGLE_TAP_MASK,
    BMX160_INT1_ORIENT_MASK,
    BMX160_INT1_FLAT_MASK,
    BMX160_INT1_HIGH_G_MASK,
    BMX160_INT1_LOW_G_MASK,
    BMX160_INT1_NO_MOTION_MASK,
    BMX160_INT2_DATA_READY_MASK,
    BMX160_INT2_FIFO_FULL_MASK,
    BMX160_INT2_FIFO_WM_MASK
};

bool DFRobot_BMX160::begin()
{

    if (scan()){
        softReset();
        writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x11);
        usleep(50000);
        /* Set gyro to normal mode */
        writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x15);
        usleep(100000);
        /* Set mag to normal mode */
        writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x19);
        usleep(10000);
        setMagnConf();
        return true;
    }
    else
        return false;
}

void DFRobot_BMX160::setLowPower(){
    softReset();
    usleep(100000);
    setMagnConf();
    usleep(100000);
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x12);
    usleep(100000);
    /* Set gyro to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x17);
    usleep(100000);
    /* Set mag to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x1B);
    usleep(100000);
}

void DFRobot_BMX160::wakeUp(){
    softReset();
    usleep(100000);
    setMagnConf();
    usleep(100000);
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x11);
    usleep(100000);
    /* Set gyro to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x15);
    usleep(100000);
    /* Set mag to normal mode */
    writeBmxReg(BMX160_COMMAND_REG_ADDR, 0x19);
    usleep(100000);
}

bool DFRobot_BMX160::softReset()
{
  int8_t rslt=BMX160_OK;
  if (Obmx160 == NULL){
    rslt = BMX160_E_NULL_PTR;
  }  
  rslt = softReset(Obmx160);
  if (rslt == 0)
    return true;
  else
    return false;
}

int8_t DFRobot_BMX160::softReset(struct bmx160Dev *dev)
{
  int8_t rslt=BMX160_OK;
  uint8_t data = BMX160_SOFT_RESET_CMD;
  if (dev==NULL){
    rslt = BMX160_E_NULL_PTR;
  }
  writeBmxReg(BMX160_COMMAND_REG_ADDR, data);
  usleep(BMX160_SOFT_RESET_DELAY_MS*1000);
  if (rslt == BMX160_OK){
    DFRobot_BMX160::defaultParamSettg(dev);
  }  
  return rslt;
}

void DFRobot_BMX160::defaultParamSettg(struct bmx160Dev *dev)
{
  // Initializing accel and gyro params with
  dev->gyroCfg.bw = BMX160_GYRO_BW_NORMAL_MODE;
  dev->gyroCfg.odr = BMX160_GYRO_ODR_1600HZ;
  dev->gyroCfg.power = BMX160_GYRO_SUSPEND_MODE;
  dev->gyroCfg.range = BMX160_GYRO_RANGE_2000_DPS;
  dev->accelCfg.bw = BMX160_ACCEL_BW_NORMAL_AVG4;
  dev->accelCfg.odr = BMX160_ACCEL_ODR_1600HZ;
  dev->accelCfg.power = BMX160_ACCEL_SUSPEND_MODE;
  dev->accelCfg.range = BMX160_ACCEL_RANGE_16G;
  

  dev->prevMagnCfg = dev->magnCfg;
  dev->prevGyroCfg = dev->gyroCfg;
  dev->prevAccelCfg = dev->accelCfg;
}

void DFRobot_BMX160::setMagnConf()
{
    writeBmxReg(BMX160_MAGN_IF_0_ADDR, 0x80);
    usleep(50000);
    // Sleep mode
    writeBmxReg(BMX160_MAGN_IF_3_ADDR, 0x01);
    writeBmxReg(BMX160_MAGN_IF_2_ADDR, 0x4B);
    // REPXY regular preset
    writeBmxReg(BMX160_MAGN_IF_3_ADDR, 0x04);
    writeBmxReg(BMX160_MAGN_IF_2_ADDR, 0x51);
    // REPZ regular preset
    writeBmxReg(BMX160_MAGN_IF_3_ADDR, 0x0E);
    writeBmxReg(BMX160_MAGN_IF_2_ADDR, 0x52);
    
    writeBmxReg(BMX160_MAGN_IF_3_ADDR, 0x02);
    writeBmxReg(BMX160_MAGN_IF_2_ADDR, 0x4C);
    writeBmxReg(BMX160_MAGN_IF_1_ADDR, 0x42);
    writeBmxReg(BMX160_MAGN_CONFIG_ADDR, 0x0B);
    writeBmxReg(BMX160_MAGN_IF_0_ADDR, 0x03);
    usleep(50000);
}

void DFRobot_BMX160::setGyroODR(eGyroODR_t bits) {
    switch (bits){
        case eGyroODR_25Hz:
            writeBmxReg(BMX160_GYRO_CONFIG_ADDR,BMX160_GYRO_ODR_25HZ);
            break;
        case eGyroODR_100Hz:
            writeBmxReg(BMX160_GYRO_CONFIG_ADDR,BMX160_GYRO_ODR_100HZ);
            break;
        case eGyroODR_1600Hz:
            writeBmxReg(BMX160_GYRO_CONFIG_ADDR,BMX160_GYRO_ODR_1600HZ);
            break;
        case eGyroODR_3200Hz:
            writeBmxReg(BMX160_GYRO_CONFIG_ADDR,BMX160_GYRO_ODR_3200HZ);
            break;
        default:
            writeBmxReg(BMX160_GYRO_CONFIG_ADDR,BMX160_GYRO_ODR_100HZ);
            break;
    }
}
void DFRobot_BMX160::setAccelODR(eAccelODR_t bits) {
    switch (bits){
        case eAccelODR_100Hz:
            writeBmxReg(BMX160_ACCEL_CONFIG_ADDR,BMX160_ACCEL_ODR_100HZ);
            break;
        case eAccelODR_800Hz:
            writeBmxReg(BMX160_ACCEL_CONFIG_ADDR,BMX160_ACCEL_ODR_800HZ);
            break;
        case eAccelODR_1600Hz:
            writeBmxReg(BMX160_ACCEL_CONFIG_ADDR,BMX160_ACCEL_ODR_1600HZ);
            break;
        default:
            writeBmxReg(BMX160_ACCEL_CONFIG_ADDR,BMX160_ACCEL_ODR_12_5HZ);
            break;
    }

}

void DFRobot_BMX160::setGyroRange(eGyroRange_t bits){
    switch (bits){
        case eGyroRange_125DPS:
            gyroRange = BMX160_GYRO_SENSITIVITY_125DPS;
            writeBmxReg(BMX160_GYRO_RANGE_ADDR,BMX160_GYRO_RANGE_125_DPS);
            break;
        case eGyroRange_250DPS:
            gyroRange = BMX160_GYRO_SENSITIVITY_250DPS;
            writeBmxReg(BMX160_GYRO_RANGE_ADDR,BMX160_GYRO_RANGE_250_DPS);
            break;
        case eGyroRange_500DPS:
            gyroRange = BMX160_GYRO_SENSITIVITY_500DPS;
            writeBmxReg(BMX160_GYRO_RANGE_ADDR,BMX160_GYRO_RANGE_500_DPS);
            break;
        case eGyroRange_1000DPS:
            gyroRange = BMX160_GYRO_SENSITIVITY_1000DPS;
            writeBmxReg(BMX160_GYRO_RANGE_ADDR,BMX160_GYRO_RANGE_1000_DPS);
            break;
        case eGyroRange_2000DPS:
            gyroRange = BMX160_GYRO_SENSITIVITY_2000DPS;
            writeBmxReg(BMX160_GYRO_RANGE_ADDR,BMX160_GYRO_RANGE_2000_DPS);
            break;
        default:
            gyroRange = BMX160_GYRO_SENSITIVITY_250DPS;
            writeBmxReg(BMX160_GYRO_RANGE_ADDR,BMX160_GYRO_RANGE_250_DPS);
            break;
    }
}

void DFRobot_BMX160::setAccelRange(eAccelRange_t bits){
    switch (bits){
        case eAccelRange_2G:
            accelRange = BMX160_ACCEL_MG_LSB_2G * 9.8;
            writeBmxReg(BMX160_ACCEL_RANGE_ADDR, BMX160_ACCEL_RANGE_2G);
            break;
        case eAccelRange_4G:
            accelRange = BMX160_ACCEL_MG_LSB_4G * 9.8;
            writeBmxReg(BMX160_ACCEL_RANGE_ADDR, BMX160_ACCEL_RANGE_4G);
            break;
        case eAccelRange_8G:
            accelRange = BMX160_ACCEL_MG_LSB_8G * 9.8;
            writeBmxReg(BMX160_ACCEL_RANGE_ADDR, BMX160_ACCEL_RANGE_8G);
            break;
        case eAccelRange_16G:
            accelRange = BMX160_ACCEL_MG_LSB_16G * 9.8;
            writeBmxReg(BMX160_ACCEL_RANGE_ADDR, BMX160_ACCEL_RANGE_16G);
            break;
        default:
            accelRange = BMX160_ACCEL_MG_LSB_2G * 9.8;
            writeBmxReg(BMX160_ACCEL_RANGE_ADDR, BMX160_ACCEL_RANGE_2G);
            break;
    }
}

void DFRobot_BMX160::getAllData(struct bmx160SensorData *magn, struct bmx160SensorData *gyro, struct bmx160SensorData *accel){

    uint8_t data[23] = {0};
    // put your main code here, to run repeatedly:
    readReg(BMX160_MAG_DATA_ADDR, data, 23);
    if(magn){
        magn->x = (int16_t) ((data[1] << 8) | data[0]);
        magn->y = (int16_t) ((data[3] << 8) | data[2]);
        magn->z = (int16_t) ((data[5] << 8) | data[4]);
        magn->x *= BMX160_MAGN_UT_LSB;
        magn->y *= BMX160_MAGN_UT_LSB;
        magn->z *= BMX160_MAGN_UT_LSB;
    }
    if(gyro){
        gyro->x = (int16_t) ((data[9] << 8) | data[8]);
        gyro->y = (int16_t) ((data[11] << 8) | data[10]);
        gyro->z = (int16_t) ((data[13] << 8) | data[12]);
        gyro->x *= gyroRange;
        gyro->y *= gyroRange;
        gyro->z *= gyroRange;
    }
    if(accel){
        accel->x = (int16_t) ((data[15] << 8) | data[14]);
        accel->y = (int16_t) ((data[17] << 8) | data[16]);
        accel->z = (int16_t) ((data[19] << 8) | data[18]);
        accel->x *= accelRange;
        accel->y *= accelRange;
        accel->z *= accelRange;
    }
}

int8_t DFRobot_BMX160::readBmxReg(uint8_t reg)
{
    uint8_t buf[1] = {0};
    
    readReg(reg, buf, sizeof(buf));
    return buf[0];
}

void DFRobot_BMX160::writeBmxReg(uint8_t reg, uint8_t value)
{
    uint8_t buffer[1] = {value};
    writeReg(reg, buffer, 1);
}

void DFRobot_BMX160::writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len)
{
    if (ioctl(i2c_bus, I2C_SLAVE, my_addr) < 0) {
        fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror (errno));
        exit(1);
    }

    for(uint16_t i = 0; i < len; i ++) {

        if (i2c_smbus_write_byte_data(i2c_bus, reg, pBuf[i]) < 0) {

            fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror(errno));
            exit(1);
        } else {
            reg++;
        }
    }



}

void DFRobot_BMX160::readReg(uint8_t reg, uint8_t *pBuf, uint16_t len)
{
    if (ioctl(i2c_bus, I2C_SLAVE, my_addr) < 0) {
        fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror (errno));

    }
    for(uint16_t i = 0; i < len; i ++) {

        pBuf[i] = i2c_smbus_read_byte_data(i2c_bus, reg);

        if (pBuf[i] < 0) {

            fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror (errno));

        }

        else {
            reg++;
        }
    }


}

bool DFRobot_BMX160::scan()
{

    if (ioctl(i2c_bus, I2C_SLAVE, my_addr) < 0) {
        fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror (errno));
    }
    if (i2c_smbus_read_byte(i2c_bus) < 0){

        return false;
    }
    return true;


}

bool DFRobot_BMX160::pubScan() {

    return scan();


}

bool DFRobot_BMX160::drdy_acc(){
    return readBmxReg(0x1b) &&  64;
}