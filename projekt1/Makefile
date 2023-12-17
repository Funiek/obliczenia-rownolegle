CC = gcc
LDFLAGS = -lm
TARGET = main

SRCS = main_seq2.c seq/utils_seq.c seq/array_conversion_seq.c seq/image_processing_seq.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
