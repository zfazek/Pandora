CC=g++
CFLAGS=-Wall -O2
LDFLAGS=
SOURCES=util.cc \
        record.cc \
        pgw.cc \
        pandora.cc 
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=pandora

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@ 

.PHONY: clean
clean:
	rm -f *.o $(EXECUTABLE)
