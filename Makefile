####

CC = g++
CFLAGS = -c -O3
LDFLAGS =
SOURCES = main.cpp hash.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = Stack

all: $(SOURCES) $(EXECUTABLE) clean

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o


