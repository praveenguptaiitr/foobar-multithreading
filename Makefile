LIBS = -lm
CC = gcc
CFLAGS = -g -pthread 
SOURCES = foobar.c

TARGET = foobar

default: all

all: clean $(TARGET) 

$(TARGET):
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

clean:
	rm -rf $(TARGET)
	
