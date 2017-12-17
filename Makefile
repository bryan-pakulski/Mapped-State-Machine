MODE := C++
# Compiler to be used
CC := g++ -std=c++11
# Cflags to be used
CFLAGS := -Wall -g
# Binary folder location
BIN := bin
# Source folder location
SRC := src
# Object folder location
OBJ := obj
# Run file after compilation?
RUN := ON
# Terminal to use + arguments
TERM := bash
TERMARGS := -c
# Libraries to include
LDFLAGS := 
# Program name
EXECUTABLE := main
# Program arguments
EXECUTABLE_ARGUMENTS := mglist.txt
# Source files
SOURCES := $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/*.c)
# Dependancies
DEPS := $(wildcard $(SRC)/*.hpp) $(wildcard $(SRC)/*.h)
# Object generation
ifeq ($(MODE), C++)
	OBJECTS := $(SOURCES:.cpp=.o)
else
	OBJECTS := $(SOURCES:.c=.o)
endif


# Main target
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

	# Create binary folder if it doesn't exist
	if [ ! -d $(BIN) ]; then \
		mkdir $(BIN); \
	fi

	# Create obj folder if it doesn't exist
	if [ ! -d $(OBJ) ]; then \
		mkdir $(OBJ); \
	fi

	# Move .o files if they exist
	mv $(SRC)/*.o $(OBJ)/ || echo No .o files to move from $(SRC) directory

	# Move binary file
	if [ -f $(EXECUTABLE) ]; then \
		mv $(EXECUTABLE) $(BIN)/$(EXECUTABLE); \
	fi

	# Run file if run is set to true
	if [ "$(RUN)" = "ON" ]; then \
		$(TERM) $(TERMARGS) "./$(BIN)/$(EXECUTABLE) $(EXECUTABLE_ARGUMENTS)"; \
	fi

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CFLAGS) $< $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $(CFLAFS) $< $(LDFLAGS) -o $@

# To remove generated files
clean:
	rm -rf $(BIN)/$(EXECUTABLE)
	rm -rf $(OBJ)/*.o
	rm -rf $(SRC)/*.o