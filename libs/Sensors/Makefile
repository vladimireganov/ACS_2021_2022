
objects = util_test.o pi_util.o MS5607.o DFRobot_BMX160.o

util_test : $(objects)
	g++ -o util_test $(objects) -lpigpio

util_test.o: util_test.cpp
	g++ -c util_test.cpp

pi_util.o: pi_util.cpp pi_util.h
	g++ -c pi_util.cpp

MS5607.o: MS5607.cpp MS5607.h
	g++ -c MS5607.cpp

DFRobot_BMX160.o: DFRobot_BMX160.cpp DFRobot_BMX160.h
	g++ -c DFRobot_BMX160.cpp


