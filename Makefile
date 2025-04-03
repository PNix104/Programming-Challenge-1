# Makefile for ThreadedMatrixMultiply

CC = gcc
CFLAGS = -Wall -pthread
TARGET = ThreadedMatrixMultiply

all: $(TARGET)

$(TARGET): ThreadedMatrixMultiply.c
	$(CC) $(CFLAGS) -o $(TARGET) ThreadedMatrixMultiply.c

clean:
	rm -f $(TARGET)

