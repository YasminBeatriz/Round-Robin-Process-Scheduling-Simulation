IDIR =../include
CC = gcc
CFLAGS = -Wall -I$(IDIR) -g

ODIR = obj
LDIR = ../lib

LIBS = -lm

_DEPS = LinkedList.h Queue.h PriorityQueue.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main_program.o Queue.o PriorityQueue.o LinkedList.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main_program: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

#Link: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

