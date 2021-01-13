CC := gcc
RMRF := rm -rf
FLAGS = -I include/ -L lib/ -l mymalloc

all:test-0.out test-1.out test-2.out test-3.out test-4.out 

%.out: src/%.c
	$(CC) $^ $(FLAGS) -o $@

clean:
	$(RMRF) *.out
