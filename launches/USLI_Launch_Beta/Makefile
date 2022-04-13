
# objects = main.o

# main : $(objects)
# 	g++ -o main $(objects) -lpigpio

# main.o: main.cpp 
# 	g++ -c main.cpp

#see README for a manual compilation process
CC = g++
CFLAGS =
OBJ = main.o FileManager.o LogManager.o DataManager.o MS5607.o DFRobot_BMX160.o PWM.o SerialLinux.o LED.o

main: $(OBJ) main.cpp
	@echo "Creating the main ..."
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c %.h
	@echo "Creating object files ..."
	$(CC) -c $< -o $@

clean:
	@echo "Deleting object and binary files ..."
	rm -rvf *.o main