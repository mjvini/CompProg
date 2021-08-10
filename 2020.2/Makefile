#compiler: gcc
CC = gcc

#compiler flags:
#-g 
#-Wall
CFLAGS = -g -Wall

#the build target executable
TARGET = Lab4

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean: 
	$(RM) $(TARGET)