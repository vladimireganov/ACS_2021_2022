
objects = main.o  MS5607.o DFRobot_BMX160.o PWM.o SerialLinux.o LED.o

main : $(objects)
	g++ -o main $(objects) -lpigpio

main.o: main.cpp 
	g++ -c main.cpp