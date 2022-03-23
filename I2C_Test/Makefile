
objects = main.o PWM.o LED.o

main : $(objects)
	g++ -o main $(objects) -lpigpio

main.o: main.cpp 
	g++ -c main.cpp

PWM.o: PWM.c PWM.h
	g++ -c PWM.c

LED.o: LED.cpp LED.h
	g++ -c LED.cpp