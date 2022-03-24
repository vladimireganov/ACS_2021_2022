
objects = main.o  MS5607.o DFRobot_BMX160.o PWM.o SerialLinux.o LED.o

main : $(objects)
	g++ -o main $(objects) -lpigpio

main.o: main.cpp 
	g++ -c main.cpp

MS5607.o: MS5607.cpp MS5607.h
	g++ -c MS5607.cpp

DFRobot_BMX160.o: DFRobot_BMX160.cpp DFRobot_BMX160.h
	g++ -c DFRobot_BMX160.cpp

PWM.o: PWM.c PWM.h
	g++ -c PWM.c

SerialLinux.o: SerialLinux.cpp SerialLinux.h
	g++ -c SerialLinux.cpp

LED.o: LED.cpp LED.h
	g++ -c LED.cpp