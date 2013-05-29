# Compile and link flags
CC=g++
CFLAGS=-c -Wall

# Compilation (add flags as needed)
CXXFLAGS    += `pkg-config opencv --cflags`

# Linking (add flags as needed)
LDFLAGS     += `pkg-config opencv --libs`

SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

.PHONY: all clean
all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(CXXFLAGS)

clean:
	rm *.o
