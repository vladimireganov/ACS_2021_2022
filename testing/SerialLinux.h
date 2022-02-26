/*
  SPI.cpp - LinuxDuino Serial (UART) library header

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

#ifndef SerialLinux_h
#define SerialLinux_h

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


/////////////////////////////////////////////
//          SerialLinux class (UART)      //
////////////////////////////////////////////

// Printing format options
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

// Defines for setting data, parity, and stop bits
// e.g SERIAL_ABC
// A= data (5 bits, 6 bits, 7 bits, 8 bits)
// B= parity (None, Even, Odd)
// C= stop bits (1 bit, 2 bits)
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06 // default
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

// A char not found in a valid ASCII numeric field
#define NO_IGNORE_CHAR  '\x01'

// Serial Driver name (user can change it)
extern char SERIAL_DRIVER_NAME[];


class SerialLinux {

private:
    int fd;
    FILE * fd_file;
    long timeOut;
    long timeDiffmillis(timespec start, timespec end);

public:
    SerialLinux();
    void begin(int baud, unsigned char config = SERIAL_8N1);
    void begin(const char *serialPort, int baud, unsigned char config = SERIAL_8N1);
    void end();
    int available();
    void flush();
    size_t print(const std::string &s);
    size_t print(const char str[]);
    size_t print(char c);
    size_t println(void);
    size_t println(const std::string &s);
    size_t println(const char c[]);
    size_t println(char c);
    size_t printf(const char *fmt, ... );
    int read();
    size_t readBytes(char buffer[], size_t length);
    std::string readString();
    void setTimeout(long millis);
    size_t write(uint8_t c);
    size_t write(const char *str);
    operator bool() { return (fd == -1) ? false : true; }

};


extern SerialLinux Serial;


#endif
