
objects = main.o DFRobot_BMP388.o

main : $(objects)
	g++ -o main $(objects)

main.o: main.cpp 
	g++ -c main.cpp

PWM.o: DFRobot_BMP388.cpp DFRobot_BMP388.h bmp3_defs.h
	g++ -c DFRobot_BMP388.cpp

