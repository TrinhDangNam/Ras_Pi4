CC=gcc
TARGET=bin
OBJ=buttonled.o
DEPS=.
CFLAGS= -Wall -Wextra -g
LDFLAGS= -lwiringPi
$(TARGET): $(OBJ)
	$(CC)	$(CFLAGS)	-o	$@	$(OBJ)	$(LDFLAGS)
%.o: %.c	(DEPS)
	$(CC)	$(CFLAGS)	-c	$<
clean: 
	rm	-f	$(TARGET)	$(OBJ)
	