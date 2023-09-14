CC=g++
CFLAGS=-c -Wall --std=c++20
LDFLAGS=
SOURCES=test.cpp task.cpp stat.cpp scheduler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=scheduler

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o hello *.txt *.out *.dSYM