CC=gcc
CFLAGS=-c -o

all: test_assign1_1 run_test_assign1_1 memory_check_test_assign1_1

test_assign1_1: test_assign1_1.o storage_mgr.o dberror.o
	$(CC) test_assign1_1.o storage_mgr.o dberror.o -o test_assign1_1

test_assign1_1.o: test_assign1_1.c
	$(CC) $(CFLAGS) test_assign1_1.o test_assign1_1.c

storage_mgr.o: storage_mgr.c
	$(CC) $(CFLAGS) storage_mgr.o storage_mgr.c

dberror.o: dberror.c
	$(CC) $(CFLAGS) dberror.o dberror.c

run_test_assign1_1: test_assign1_1
	./test_assign1_1

memory_check_test_assign1_1: test_assign1_1
	valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --verbose \
              ./test_assign1_1

clean:
	rm -f *.o test_assign1_1