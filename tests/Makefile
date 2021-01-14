CC := gcc
RMRF := rm -rf
FLAGS = -I include/ -L lib/ -l mymalloc

all:tests/test-0.out tests/test-1.out tests/test-2.out tests/test-3.out tests/test-4.out 

tests/%.out: src/%.c
	$(CC) $^ $(FLAGS) -o $@
.PHONY:
clean:
	$(RMRF) tests/*.out
