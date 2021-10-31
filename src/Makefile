
objects = main.o  Serial.o SPI.o Wire.o WString.o Core.o MS5607.o DFRobot_BMX160.o

main : $(objects)
	g++ -o main $(objects)

main.o: main.cpp 
	g++ -c main.cpp


Serial.o: Serial.cpp Serial.h
	g++ -c Serial.cpp


SPI.o: SPI.cpp SPI.h
	g++ -c SPI.cpp

Wire.o: Wire.cpp Wire.h
	g++ -c Wire.cpp

WString.o: WString.cpp WString.h
	g++ -c WString.cpp

Core.o: Core.cpp Core.h
	g++ -c Core.cpp

MS5607.o: MS5607.cpp MS5607.h
	g++ -c MS5607.cpp

DFRobot_BMX160.o: DFRobot_BMX160.cpp DFRobot_BMX160.h
	g++ -c DFRobot_BMX160.cpp
