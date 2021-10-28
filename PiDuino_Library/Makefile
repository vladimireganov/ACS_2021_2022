objects = main.o DFRobot_BMX160.o MS5607.o Core.o Serial.o SPI.o Wire.o WString.o
piduino_dep = core.h serial.h SPI.h Wire.h Wstring.h


sensor_test : $(objects)
	cc -o sensor_test $(objects)
main.o : main.cpp Piduino.h DFRobot_BMX160.h MS5607.h $(piduino_dep)
	cc -c main.cpp
DFRobot_BMX160.o : DFRobrot_BMX160.cpp Piduino.h DFRobot_BMX160.h $(piduino_dep)
	cc -c DFRobot_BMX160.cpp
MS5607.o : MS5607.cpp Piduino.h MS5607.h $(piduino_dep)
	cc -c MS5607.cpp

Core.o : Core.cpp Core.h
	cc -c Core.cpp
SPI.o : SPI.cpp SPI.h
	cc -c SPI.cpp
Wire.o : Wire.cpp Wire.h
	cc -c Wire.cpp
WString.o : WString.cpp WString.h
	cc -c WString.cpp
Serial.o : Serial.cpp Serial.h
	cc -c Serial.cpp
clean :
        rm edit $(objects)
