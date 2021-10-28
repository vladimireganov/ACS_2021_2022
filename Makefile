SRC_DIR := .../src
OBJ_DIR := .../obj
SRC_FILES := $(wildcard $(SRC_DIR)/%.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -c
CPPFLAGS := ...
CXXFLAGS += -MMD

TARGET = myExecutable

$(TARGET): $(OBJ_FILES)
        g++ $(LDFLAGS) src/main.cpp -o $@ $^
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
        g++ $(CXXFLAGS) -c -o $@ $<
