/*File   : MS5607.cpp
  Author : Amit Ate
  Email  : amit@uravulabs.com
  Company: Uravu Labs
*/

#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <stdint.h>
#include <math.h>
#include "MS5607.h"
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev_smbus.h>		//Needed for I2C port



MS5607::MS5607(){

}

MS5607::MS5607(int bus_file, short address){
    this->MS5607_ADDR = address;
    i2c_bus = bus_file;
}

// Initialise coefficient by reading calibration data
char MS5607::begin(){


    return(readCalibration());
}

char MS5607::resetDevice(void){
    //Wire.beginTransmission(MS5607_ADDR);
    //Wire.write(RESET);
    //char error = Wire.endTransmission();

    if (ioctl(i2c_bus, I2C_SLAVE, MS5607_ADDR) < 0) {
        fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror (errno));
        return 0;
    }

    if (i2c_smbus_write_byte(i2c_bus, RESET) < 0){
        return 0;
    }
    else{usleep(3000);
        return(1);}

}

// read calibration data from PROM
char MS5607::readCalibration(){
    if(resetDevice() &&
       readUInt_16(PROM_READ+2, C1) &&
       readUInt_16(PROM_READ+4, C2) &&
       readUInt_16(PROM_READ+6, C3) &&
       readUInt_16(PROM_READ+8, C4) &&
       readUInt_16(PROM_READ+10, C5) &&
       readUInt_16(PROM_READ+12, C6)
            ){
        return (1);
    }else{return(0);}
}
/*
char MS5607::readPublicConfig(unsigned int &add0,unsigned int &add1,unsigned int &add2,unsigned int &add3,unsigned int &add4,unsigned int &add5) {
    if(resetDevice() &&
       readUInt_16(PROM_READ+2, add0) &&
       readUInt_16(PROM_READ+4, add1) &&
       readUInt_16(PROM_READ+6, add2) &&
       readUInt_16(PROM_READ+8, add3) &&
       readUInt_16(PROM_READ+10, add4) &&
       readUInt_16(PROM_READ+12, add5)
            ){
        return (1);
    }else{return(0);}
}*/

// convert raw data into unsigned int
char MS5607::readUInt_16(char address, unsigned int &value){
    unsigned char data[2];	//4bit
    data[0] = address;
    if (readBytes(data,2))
    {
        value = (((unsigned int)data[0]*(1<<8))|(unsigned int)data[1]);
        return(1);
    }
    value = 0;
    return(0);
}

// read number of bytes over i2c
char MS5607::readBytes(unsigned char *values, char length){
    char x;
    char reg = values[0];
    if (ioctl(i2c_bus, I2C_SLAVE, MS5607_ADDR) < 0) {
        fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror (errno));
        return 0;
    }
    //Wire.beginTransmission(MS5607_ADDR);
    //Wire.write(values[0]);
    //char error = Wire.endTransmission();
    //if (error == 0)
    //{
    //	Wire.requestFrom(MS5607_ADDR,length);
    //while(!Wire.available()) ; // wait until bytes are read



    for(x=0;x<length;x++)
    {
        values[x] = i2c_smbus_read_byte_data(i2c_bus,reg);
        //		values[x] = Wire.read();
    }
    return(1);
}


// send command to start measurement
char MS5607::startMeasurment(void){
    //Wire.beginTransmission(MS5607_ADDR);
    //Wire.write(R_ADC);
    //char error = Wire.endTransmission();
    if (ioctl(i2c_bus, I2C_SLAVE, MS5607_ADDR) < 0) {
        fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror (errno));
        return 0;
    }

    if (i2c_smbus_write_byte(i2c_bus, R_ADC ) < 0){
        return 0;
    }
    usleep(3000);
    return(1);
    /*
    if(error == 0){
      usleep(3000);
      return(1);
    }else{return(0);} */
}

// send command to start conversion of temp/pressure
char MS5607::startConversion(char CMD){
    //Wire.beginTransmission(MS5607_ADDR);
    //Wire.write(CMD);
    //char error = Wire.endTransmission();

    if (ioctl(i2c_bus, I2C_SLAVE, MS5607_ADDR) < 0) {
        fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror (errno));
        return 0;
    }

    if (i2c_smbus_write_byte(i2c_bus, CMD ) < 0){
        return 0;
    }
    usleep(1000*Conv_Delay);
    return(1);
    /*
    if(error == 0){
      delay(Conv_Delay);
      return(1);
    }else{return(0);}
     */
}

// read raw digital values of temp & pressure from MS5607
char MS5607::readDigitalValue(void){
    if(startConversion(CONV_D1)){
        if(startMeasurment()){
            if(getDigitalValue(DP)) {
                //std::cout<<"Got pressure value: " << DP << std::endl;
            }
        }
    }
    else {
        return 0;
    }
    if(startConversion(CONV_D2)){
        if(startMeasurment()){
            if(getDigitalValue(DT)) {
                //std::cout<<"Got Temperature value: " << DT << std::endl;
            }
        }
    }
    else{
        return 0;
    }
    return 1;
}

char MS5607::getDigitalValue(unsigned long &value){
    char x, length = 3;
    unsigned char data[3];
    //  Wire.requestFrom(MS5607_ADDR,length);
    //  while(!Wire.available()) ; // wait until bytes are ready

    if (ioctl(i2c_bus, I2C_SLAVE, MS5607_ADDR) < 0) {
        fprintf(stderr, "%s(): ioctl error: %s\n", __func__, strerror (errno));
        return 0;
    }
    if ( read(i2c_bus,data,3) !=3 ) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to read from the i2c_bus.\n");
        //buffer = strerror(errno);
        //printf(buffer);
        printf("\n\n");
    } else {
        value = (unsigned long)data[0]*1<<16|(unsigned long)data[1]*1<<8|(unsigned long)data[2];
    }

    //data[x] = i2c_smbus_read_byte(i2c_bus);

    return(1);
}

float MS5607::getTemperature(void){       //return Celsius
    dT = (float)DT - ((float)C5)*((int)1<<8);
    TEMP = 2000.0 + dT * ((float)C6)/(float)((long)1<<23);
    return TEMP/100 ;
}

float MS5607::getPressure(void){      //return mbar
    dT = (float)DT - ((float)C5)*((int)1<<8);
    TEMP = 2000.0 + dT * ((float)C6)/(float)((long)1<<23);
    OFF = (((int64_t)C2)*((long)1<<17)) + dT * ((float)C4)/((int)1<<6);
    SENS = ((float)C1)*((long)1<<16) + dT * ((float)C3)/((int)1<<7);
    float pa = (float)((float)DP/((long)1<<15));
    float pb = (float)(SENS/((float)((long)1<<21)));
    float pc = pa*pb;
    float pd = (float)(OFF/((float)((long)1<<15)));
    P = pc - pd;
    return P/100;
}

// set OSR and select corresponding values for conversion commands & delay
void MS5607::setOSR(short OSR_U){
    this->OSR = OSR_U;
    switch (OSR) {
        case 256:
            CONV_D1 = 0x40;
            CONV_D2 = 0x50;
            Conv_Delay = 1;
            break;
        case 512:
            CONV_D1 = 0x42;
            CONV_D2 = 0x52;
            Conv_Delay = 2;
            break;
        case 1024:
            CONV_D1 = 0x44;
            CONV_D2 = 0x54;
            Conv_Delay = 3;
            break;
        case 2048:
            CONV_D1 = 0x46;
            CONV_D2 = 0x56;
            Conv_Delay = 5;
            break;
        case 4096:
            CONV_D1 = 0x48;
            CONV_D2 = 0x58;
            Conv_Delay = 10;
            break;
        default:
            CONV_D1 = 0x40;
            CONV_D2 = 0x50;
            Conv_Delay = 1;
            break;
    }
}

float MS5607::getAltitude(void){
    float h,t,p;
    t = getTemperature();
    p = getPressure();
    p = P0/p;
    h = 153.84615*(pow(p,0.19) - 1)*(t+273.15);
    return h;
}
