#### GTThread Library Makefile

CFLAGS  = -Wall -pedantic
LFLAGS  =
CC      = mpicc
RM      = /bin/rm -rf
AR      = ar rc
RANLIB  = ranlib

LIBRARY = mpi_barriers.a

CEN = centralized_main
MCS = mcs_main
LIB_SRC = centralized_barrier.c mcs_tree_barrier.c

LIB_OBJ = $(patsubst %.c,%.o,$(LIB_SRC))

# pattern rule for object files
%.o: %.c
	$(CC) -c  $(CFLAGS) $< -o $@

all: $(LIBRARY) $(CEN) $(MCS) 

$(CEN): $(LIBRARY)
$(MCS): $(LIBRARY)


$(LIBRARY): $(LIB_OBJ)
	$(AR) $(LIBRARY) $(LIB_OBJ)
	$(RANLIB) $(LIBRARY)

clean:
	$(RM) $(LIBRARY) $(LIB_OBJ) $(MCS) $(CEN)

.PHONY: depend
depend:
	$(CFLAGS) -- $(LIB_SRC)  2>/dev/null
