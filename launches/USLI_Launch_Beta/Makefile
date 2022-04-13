
TARGET = flight

CC = g++
CFLAGS = -g

# OBJ = main.o LogManager.o DataManager.o
# HW_OBJ = MS5607.o DFRobot_BMX160.o PWM.o SerialLinux.o LED.o
# HARDWARE_DIR = ./hardware

OUTDIR = ./bin
DATADIR = ./data
SUBDIR = hardware
DIR_OBJ = ./obj

INCS = $(wildcard *.h $(foreach fd, $(SUBDIR), $(fd)/*.h))
SRCS = $(wildcard *.cpp $(foreach fd, $(SUBDIR), $(fd)/*.cpp))
NODIR_SRC = $(notdir $(SRCS))
OBJS = $(addprefix $(DIR_OBJ)/, $(SRCS:cpp=o)) # obj/xxx.o obj/folder/xxx .o
INC_DIRS = $(addprefix -I, $(SUBDIR))

.PHONY: clean echoes

$(TARGET): $(OBJS)
	@echo "Creating the final flight code ..."
	mkdir -p $(OUTDIR)
	mkdir -p $(DATADIR)
	$(CC) -o $(OUTDIR)/$@ $(OBJS) -lpigpio

$(DIR_OBJ)/%.o: %.cpp $(INCS)
	mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CFLAGS) $(INC_DIRS)

clean:
	rm -rf $(OUTDIR)/* $(DATADIR)/* $(DIR_OBJ)/*

echoes:
	@echo "INC files: $(INCS)"  
	@echo "SRC files: $(SRCS)"
	@echo "OBJ files: $(OBJS)"