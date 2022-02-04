//
// Created by jtc19 on 2/3/2022.
//

#ifndef ACS_2021_2022_PI_UTIL_H
#define ACS_2021_2022_PI_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fstream>
#include <fcntl.h>
#include <pigpio.h>
#include "DFRobot_BMX160.h"
#include "MS5607.h"

#define DELAY 1000
#define SPI_PIN 27
#define BMX_ADDR1 0x68
#define BMX_ADDR2 0x69
#define MS_ADDR1 0x76
#define MS_ADDR1 0x76


void shift_bmx_Addr();
int init_Bus();
int init_GPIO();
int begin_Sensors();


#endif //ACS_2021_2022_PI_UTIL_H
