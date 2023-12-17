CC = gcc
mpi : override CC=mpicc
MPICC = mpicc
LDFLAGS = -lm
MPIFLAGS = -I/usr/include/x86_64-linux-gnu/mpich
TARGET_SEQ = main_seq
TARGET_MPI = main_mpi

SRCS_SEQ = main_seq2.c seq/utils.c seq/array_conversion.c seq/image_processing.c
SRCS_MPI = main_mpi.c parallel/utils.c parallel/array_conversion.c parallel/image_processing.c

OBJS_SEQ = $(SRCS_SEQ:.c=.o)
OBJS_MPI = $(SRCS_MPI:.c=.o)

seq: $(OBJS_SEQ)
	$(CC) -o $(TARGET_SEQ) $(OBJS_SEQ) $(LDFLAGS)

mpi: $(OBJS_MPI)
	$(MPICC) -o $(TARGET_MPI) $(OBJS_MPI) $(LDFLAGS) $(MPIFLAGS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS_SEQ) $(OBJS_MPI) $(TARGET_SEQ) $(TARGET_MPI)