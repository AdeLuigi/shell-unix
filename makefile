CC = gcc
CFLAGS = -m32

all: 
	${CC} ${CFLAGS} shell.c -o shell -lreadline
	./shell


	

	
	