CC = gcc
CFLAGS = -pthread -std=c99

UTILS = hashtable.c hashtable.h test/test_util.c test/test_util.h 

all : testsuit testsave test_conc

testsuit : test/test_runtime.c $(UTILS)
	${CC} ${CFLAGS} $^ -o $@ 

testsave : test_get test_set $(UTILS)

test_conc : test/test_concurrency.c $(UTILS)
	${CC} ${CFLAGS} $^ -o $@ 

test_get : test/test_get_values.c $(UTILS)
	${CC} ${CFLAGS} $^ -o $@ 


test_set : test/test_set_values.c $(UTILS)
	${CC} ${CFLAGS} $^ -o $@ 


clean: 
	rm testsuit *.o test_get test_set test_conc
