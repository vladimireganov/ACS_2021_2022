/*
  SPI.cpp - LinuxDuino Serial (UART) library

  Copyright (c) 2016 Jorge Garza <jgarzagu@ucsd.edu>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <time.h>
#include <termios.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <stdarg.h>
//#include <linux/types.h>

#include "SerialLinux.h"

// All functions of unistd.h must be called like this: unistd::the_function()
namespace unistd {
    #include <unistd.h>
};



/////////////////////////////////////////////
//          SerialLinux class (UART)       //
////////////////////////////////////////////

char SERIAL_DRIVER_NAME[128] = "/dev/ttyS0";

////  Public methods ////

//Constructor
SerialLinux::SerialLinux()
{
    // Default serial driver and timeout
    timeOut = 1000;     //in milliseconds
    fd = -1;
    fd_file = NULL;
}

void SerialLinux::begin(int baud, unsigned char config)
{
     begin((const char *)SERIAL_DRIVER_NAME, baud, config);
}

// Sets the data rate in bits per second (baud) for serial data transmission
void SerialLinux::begin(const char *serialPort, int baud, unsigned char config)
{

    int speed;
    int DataSize, ParityEN, Parity, StopBits;
    struct termios options;
    int flags;

    // Open Serial port
    if ((fd = open(serialPort, O_RDWR | O_NOCTTY | O_NONBLOCK)) == -1) {
        fprintf(stderr,"%s(): Unable to open the serial port %s: %s\n",
            __func__, serialPort, strerror (errno));
        exit(1);
    }

    // We obtain a pointer to FILE structure (fd_file) from the file descriptor fd
    // and set it to be non-blocking
    fd_file = fdopen(fd,"r+");
    flags = fcntl( fileno(fd_file), F_GETFL );
    fcntl(fileno(fd_file), F_SETFL, flags | O_NONBLOCK);
    

    // Set Serial options: baudRate/speed, data size and parity.

    switch (baud) {
        case      50:   speed =      50 ; break ;
        case      75:   speed =      75 ; break ;
        case     110:   speed =     110 ; break ;
        case     134:   speed =     134 ; break ;
        case     150:   speed =     150 ; break ;
        case     200:   speed =     200 ; break ;
        case     300:   speed =     300 ; break ;
        case     600:   speed =     600 ; break ;
        case    1200:   speed =    1200 ; break ;
        case    1800:   speed =    1800 ; break ;
        case    2400:   speed =    2400 ; break ;
        case    9600:   speed =    9600 ; break ;
        case   19200:   speed =   19200 ; break ;
        case   38400:   speed =   38400 ; break ;
        case   57600:   speed =   57600 ; break ;
        case  115200:   speed =  115200 ; break ;
        case  230400:   speed =  230400 ; break ;
        case  460800:   speed =  460800 ; break ;
        case  500000:   speed =  500000 ; break ;
        case  576000:   speed =  576000 ; break ;
        case  921600:   speed =  921600 ; break ;
        case 1000000:   speed = 1000000 ; break ;
        case 1152000:   speed = 1152000 ; break ;
        case 1500000:   speed = 1500000 ; break ;
        case 2000000:   speed = 2000000 ; break ;
        case 2500000:   speed = 2500000 ; break ;
        case 3000000:   speed = 3000000 ; break ;
        case 3500000:   speed = 3500000 ; break ;
        case 4000000:   speed = 4000000 ; break ;
        default:        speed =    9600 ; break ;
    }

    tcgetattr(fd, &options);
    cfmakeraw(&options);
    cfsetispeed (&options, speed);
    cfsetospeed (&options, speed);

    switch (config) {
        case SERIAL_5N1: DataSize = CS5; ParityEN = 0; Parity = 0; StopBits = 0; break;
        case SERIAL_6N1: DataSize = CS6; ParityEN = 0; Parity = 0; StopBits = 0; break;
        case SERIAL_7N1: DataSize = CS7; ParityEN = 0; Parity = 0; StopBits = 0; break;
        case SERIAL_8N1: DataSize = CS8; ParityEN = 0; Parity = 0; StopBits = 0; break;
        case SERIAL_5N2: DataSize = CS5; ParityEN = 0; Parity = 0; StopBits = 1; break;
        case SERIAL_6N2: DataSize = CS6; ParityEN = 0; Parity = 0; StopBits = 1; break;
        case SERIAL_7N2: DataSize = CS7; ParityEN = 0; Parity = 0; StopBits = 1; break;
        case SERIAL_8N2: DataSize = CS8; ParityEN = 0; Parity = 0; StopBits = 1; break;
        case SERIAL_5E1: DataSize = CS5; ParityEN = 1; Parity = 0; StopBits = 0; break;
        case SERIAL_6E1: DataSize = CS6; ParityEN = 1; Parity = 0; StopBits = 0; break;
        case SERIAL_7E1: DataSize = CS7; ParityEN = 1; Parity = 0; StopBits = 0; break;
        case SERIAL_8E1: DataSize = CS8; ParityEN = 1; Parity = 0; StopBits = 0; break;
        case SERIAL_5E2: DataSize = CS5; ParityEN = 1; Parity = 0; StopBits = 1; break;
        case SERIAL_6E2: DataSize = CS6; ParityEN = 1; Parity = 0; StopBits = 1; break;
        case SERIAL_7E2: DataSize = CS7; ParityEN = 1; Parity = 0; StopBits = 1; break;
        case SERIAL_8E2: DataSize = CS8; ParityEN = 1; Parity = 0; StopBits = 1; break;
        case SERIAL_5O1: DataSize = CS5; ParityEN = 1; Parity = 1; StopBits = 0; break;
        case SERIAL_6O1: DataSize = CS6; ParityEN = 1; Parity = 1; StopBits = 0; break;
        case SERIAL_7O1: DataSize = CS7; ParityEN = 1; Parity = 1; StopBits = 0; break;
        case SERIAL_8O1: DataSize = CS8; ParityEN = 1; Parity = 1; StopBits = 0; break;
        case SERIAL_5O2: DataSize = CS5; ParityEN = 1; Parity = 1; StopBits = 1; break;
        case SERIAL_6O2: DataSize = CS6; ParityEN = 1; Parity = 1; StopBits = 1; break;
        case SERIAL_7O2: DataSize = CS7; ParityEN = 1; Parity = 1; StopBits = 1; break;
        case SERIAL_8O2: DataSize = CS8; ParityEN = 1; Parity = 1; StopBits = 1; break;
        default: DataSize = CS8; ParityEN = 0; Parity = 0; StopBits = 0; break; // SERIAL_8N1
    }

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &=  ~CSIZE;                                             // Enable set data size
    options.c_cflag |= DataSize;                                            // Data size
    (ParityEN) ? options.c_cflag |= PARENB : options.c_cflag &= ~PARENB;    // Parity enable ? YES : NO
    (Parity) ? options.c_cflag |= PARODD : options.c_cflag &= ~PARODD;      // Parity ? Odd : Even
    (StopBits) ? options.c_cflag |= CSTOPB : options.c_cflag &= ~CSTOPB;    // Stop bits ? 2 bits: 1 bit
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;

    tcsetattr (fd, TCSANOW, &options);
    
}


//disables serial communication
void SerialLinux::end() {
    unistd::close(fd);
    fd = -1;
}


// Get the numberof bytes (characters) available for reading from
// the serial port.
// Return: number of bytes avalable to read
int SerialLinux::available()
{
    int nbytes = 0;
    if (ioctl(fd, FIONREAD, &nbytes) < 0)  {
        fprintf(stderr, "%s(): serial get available bytes error: %s \n",
            __func__, strerror (errno));
        exit(1);
    }
    return nbytes;
}

// Remove any data remaining on the serial buffer
void SerialLinux::flush()
{
    tcflush(fd,TCIOFLUSH);
}




//------ PRINTS --------//

// Prints like a normal C language printf() but to the Serial port
size_t SerialLinux::printf(const char *fmt, ... ) {
    char *buf = NULL;
    va_list args;

    // Copy arguments to buf
    va_start (args, fmt);
    vasprintf(&buf, (const char *)fmt, args);
    va_end (args);

    return unistd::write(fd,buf,strlen(buf));
}

// Prints one character to the serial port as human-readable ASCII text.
size_t SerialLinux::print(char c)
{
    return unistd::write(fd,&c,1);
}

// Prints data to the serial port as human-readable ASCII text.
size_t SerialLinux::print(const std::string &s)
{
    return unistd::write(fd,s.c_str(), s.length());
}

// Prints data to the serial port as human-readable ASCII text.
size_t SerialLinux::print(const char str[])
{
    return unistd::write(fd,str,strlen(str));
}

// Prints a new line
size_t SerialLinux::println(void)
{
    char * msg;
    asprintf(&msg,"\r\n");
    return unistd::write(fd,msg,strlen(msg));
}

// Prints data to the serial port as human-readable ASCII text
// Followed by a new line
size_t SerialLinux::println(const std::string &s)
{
    size_t n = print(s);
    n += println();
    return n;
}


// Prints data to the serial port as human-readable ASCII text
// Followed by a new line
size_t SerialLinux::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

// Prints one character to the serial port as human-readable ASCII text.
// Followed by a new line
size_t SerialLinux::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}

//-------READS---------//


// Reads 1 byte of incoming serial data
// Returns: first byte of incoming serial data available
int SerialLinux::read()
{
    int8_t c;
    unistd::read(fd,&c,1);
    return c;
}

// Reads characters from the serial port into a buffer. The function
// terminates if the determined length has been read, or it times out
// Returns: number of bytes readed
size_t SerialLinux::readBytes(char buffer[], size_t length)
{
    timespec time1, time2;
    clock_gettime(CLOCK_REALTIME, &time1);
    size_t count = 0;
    while (count < length) {
        if (available()) {
            unistd::read(fd,&buffer[count],1);
            count ++;
        }
        clock_gettime(CLOCK_REALTIME, &time2);
        if (timeDiffmillis(time1,time2) > timeOut) {
            std::printf("readBytes timed out!\n");
            break;
        }
    }
    return count;
}

std::string SerialLinux::readString() {
    
    std::string ret = "";
    timespec time1, time2;
    char c = 0;
        clock_gettime(CLOCK_REALTIME, &time1);
        do {
            if (available()) {
                unistd::read(fd,&c,1);
                ret += (char)c;
            }
            clock_gettime(CLOCK_REALTIME, &time2);
            if (timeDiffmillis(time1,time2) > timeOut) {
                std:printf("readBytes timed out!\n");
                break;
            }
        } while (c >= 0);
        return ret;
    
}


//-------WRITES-------//



// Writes binary data to the serial port. This data is sent as a byte
// Returns: number of bytes written
size_t SerialLinux::write(uint8_t c)
{
    unistd::write(fd,&c,1);
    return 1;
}

// Writes binary data to the serial port. This data is sent as a series
// of bytes
// Returns: number of bytes written
size_t SerialLinux::write(const char *str)
{
    if (str == NULL) return 0;
    return unistd::write(fd,str,strlen(str));
}



// Sets the maximum milliseconds to wait for serial data when using
// readBytes(), readBytesUntil(), parseInt(), parseFloat(), findUnitl(), ...
// The default value is set to 1000
void SerialLinux::setTimeout(long millis)
{
    timeOut = millis;
}

// Returns the difference of two times in miiliseconds
long SerialLinux::timeDiffmillis(timespec start, timespec end)
{
    return ((end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) * 1e-6);
}

SerialLinux Serial = SerialLinux();
