
CC=g++
parallel : override CC=mpicxx

ifeq ($(OPT),y)
LOPT=-lm  -O3 -flto
COPT=-O3 
else
LOPT=-lm -g
COPT=-g
endif


ifeq ($(OPENMP),y)
CC :=$(CC) -fopenmp 
endif



%.o : %.cpp
		$(CC) $(COPT) $(GCCOPT) -c  $<


serial: mandel.o serial.o
		$(CC) $(LOPT) $^ -lrt -o $@

parallel: parallel.o mandel.o
		$(CC) $(LOPT) $^ -o $@
clean:
		rm -f *.o parallel serial
