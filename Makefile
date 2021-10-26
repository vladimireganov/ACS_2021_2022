SRC_DIR := .../src
OBJ_DIR := .../obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := ...
CPPFLAGS := ...
CXXFLAGS := ...

TARGET = myExecutable

$(TARGET): $(OBJ_FILES)
	g++ $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
   g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<