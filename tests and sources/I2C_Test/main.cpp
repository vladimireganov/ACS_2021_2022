

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <i2c-dev_smbus.h>
#include "bmp3_defs.h"
#include "DFRobot_BMP388.h"





using namespace std;




int main() {

    int RPI_I2C_BUS;
    int adapter_nr = 1; /* default for raspberry pi */
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); // creating file path to open bus during init


    RPI_I2C_BUS = open(filename, O_RDWR); // open bus
    if (RPI_I2C_BUS < 0) {

        return 0;
    }
#define CALIBRATE_Altitude

/*Create a bmp388 object to communicate with IIC.*/
    DFRobot_BMP388 bmp388(RPI_I2C_BUS);

    float seaLevel;


        bmp388.set_iic_addr(0x76);
        /* Initialize bmp388*/
        while(bmp388.begin()){
            cout << "initialization error \n";
            sleep(1);
        }
        /*You can use an accurate altitude to calibrate sea level air pressure.
         *And then use this calibrated sea level pressure as a reference to obtain the calibrated altitude.
         *In this case,525.0m is chendu accurate altitude.
         */
        usleep(100000);
        seaLevel = bmp388.readSeaLevel(525.0);
        cout << "seaLevel : ";
        cout << seaLevel;
        cout << " Pa \n";
    }

#ifdef CALIBRATE_Altitude
    /* Read the calibrated altitude */
  float altitude = bmp388.readCalibratedAltitude(seaLevel);
  cout << "calibrate Altitude : ";
  cout << altitude;
  cout << " m \n";
#else
    /* Read the altitude */
    float altitude = bmp388.readAltitude();
    cout << "Altitude : ";
    cout << altitude;
    cout << " m \n";
#endif











        }
