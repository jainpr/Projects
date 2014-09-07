OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

all: centralized tree_barrier

centralized: centralized_barrier.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

tree_barrier: tree_barrier.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o centralized tree_barrier
