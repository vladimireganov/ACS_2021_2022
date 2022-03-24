
objects = testmain.o ver3.o

testmain : $(objects)
	g++ -o testmain $(objects) -lpigpio

testmain.o: testmain.c 
	g++ -c testmain.c 

ver3.o: PWM.c PWM.h
	g++ -c ver3.c 

