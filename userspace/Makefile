CC = gcc
CFLAGS = -pthread -std=c99

UTILS = test_util.c test_util.h hashtable.c hashtable.h

testsuit : test_runtime.c $(UTILS)
	${CC} ${CFLAGS} $^ -o $@ 

testsave : test_get test_set 

test_get : test_get_values.c $(UTILS)
	${CC} ${CFLAGS} $^ -o $@ 


test_set : test_set_values.c $(UTILS)
	${CC} ${CFLAGS} $^ -o $@ 


clean: 
	rm testsuit *.o