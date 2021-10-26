SOURCES = main.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)

TARGET = myExecutable

$(TARGET): $(OBJECTS)
	g++ $^ -o $@

%.o: %.cpp
	g++ -c $< -o $@
