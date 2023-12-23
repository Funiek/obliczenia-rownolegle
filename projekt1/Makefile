CC = gcc
MPICC = mpicc
MPICPP = mpic++
LDFLAGS = -lm -O3
OMPFLAG = -fopenmp
TARGET_SEQ = main_seq
TARGET_MPI = main_mpi
TARGET_MPI_OMP = main_mpi_omp

SRCS_SEQ = main_seq.c seq/utils.c seq/array_conversion.c seq/image_processing.c
SRCS_MPI = main_mpi.c parallel/utils.c parallel/array_conversion.c parallel/image_processing.c
SRCS_MPI_OMP = main_mpi_omp.cpp parallel/utils_omp.cpp parallel/array_conversion_omp.cpp parallel/image_processing_omp.cpp

OBJS_SEQ = $(SRCS_SEQ:.c=.o)
OBJS_MPI = $(SRCS_MPI:.c=.o)
OBJS_MPI_OMP = $(SRCS_MPI_OMP:.c=.o)

seq: $(OBJS_SEQ)
	$(CC) -o $(TARGET_SEQ) $(OBJS_SEQ) $(LDFLAGS)

mpi: override CC=mpicc
mpi: $(OBJS_MPI)
	$(MPICC) -o $(TARGET_MPI) $(OBJS_MPI) $(LDFLAGS)

omp: override CC=mpic++
omp: $(OBJS_MPI_OMP)
	$(MPICPP) $(OMPFLAG) -o $(TARGET_MPI_OMP) $(OBJS_MPI_OMP) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS_SEQ) $(OBJS_MPI) $(OBJS_MPI_OMP) $(TARGET_SEQ) $(TARGET_MPI) $(TARGET_MPI_OMP)