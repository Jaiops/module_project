CC = gcc
CFLAGS = -pthread -std=c99

testsuit : writeToHash.c hashtable.c hashtable.h
	${CC} ${CFLAGS} $^ -o $@ 

clean: 
	rm testsuit *.o