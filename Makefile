#######################
### FOLDER SETTINGS ###
#######################

# Binary folder location
BIN := bin
# Source folder location
SRC := src
# Object folder location
OBJ := obj

########################
### Include headers  ###
########################

INCDEF := 

######################
### COMPILER FLAGS ###
######################

# Compiler to be used, g++ only supported currently
CC := g++
CFLAGS := -std=c++11 -Wall -g3 $(INCDEF)

#########################
### EXECUTION OPTIONS ###
#########################

# Terminal to use + arguments
TERM := bash
TERMARGS := -c
# Program name
EXECUTABLE := main
# Program arguments
EXECUTABLE_ARGUMENTS := 

####################
### Source files ###
####################

SOURCES := $(shell find $(SRC) -type f -name "*.cpp")

#########################
### Object generation ###
#########################

OBJECTS := $(SOURCES:.cpp=.o)

# Generate objects based on source files
%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

###################
### Main target ###
###################

$(EXECUTABLE): $(OBJECTS)
	echo $(SOURCES)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

	# Move .o files if they exist
	find $(SRC) -type f -name "*.o" -exec mv {} $(OBJ)/  \;

	# Move binary file
	if [ -f $(EXECUTABLE) ]; then \
		mv $(EXECUTABLE) $(BIN)/$(EXECUTABLE); \
	fi

##############
###	 Init  ###
##############
init:
	# Create binary folder if it doesn't exist
	if [ ! -d $(BIN) ]; then \
		mkdir $(BIN); \
	fi

	# Create obj folder if it doesn't exist
	if [ ! -d $(OBJ) ]; then \
		mkdir $(OBJ); \
	fi

	# Create source folder if it doesn't exist
	if [ ! -d $(SRC) ]; then \
		mkdir $(SRC); \
	fi

###################
###  Run target  ##
###################
run:
	cd $(BIN); \
	$(TERM) $(TERMARGS) "./$(EXECUTABLE) $(EXECUTABLE_ARGUMENTS)"; \
	cd ..;

###################
###  Mem check  ###
###################
mem_check:
	cd $(BIN); \
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./$(EXECUTABLE) $(EXECUTABLE_ARGUMENTS); \
	cd ..;

####################
### Clean target ###
####################
clean:
	find $(BIN) -type f -name "*" -exec rm -f {} \;
	find $(SRC) -type f -name "*.o" -exec rm -f {} \;
	find $(OBJ) -type f -name "*.o" -exec rm -f {} \;