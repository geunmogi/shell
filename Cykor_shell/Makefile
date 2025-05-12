CC = gcc
CFLAGS = -Wall -g
OBJS = main.o execute.o pipe.o
TARGET = mysh

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c shell.h
execute.o: execute.c shell.h
pipe.o: pipe.c shell.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)