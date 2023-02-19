CC = g++
OPT = -O3
#OPT = -g

INC = -I.
LIB = -L.

CFLAGS = $(OPT) $(INC) $(LIB)

LIBS = -l721sim -lpthread -lz -ldl 

SOURCES := $(wildcard *.cc)
OBJECTS := $(patsubst %.cc, %.o, $(SOURCES))

#################################

all: $(OBJECTS)
	$(CC) $(CFLAGS) -o 721sim $^ $(LIBS)
	@echo "-----------DONE WITH 721sim-----------"

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o 721sim core
clobber:
	rm -f *.o
