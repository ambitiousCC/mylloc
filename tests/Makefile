CC := gcc
CPP := g++
RMRF := rm -rf
FLAGS = -I include/ -L lib/ -l mymalloc
LIB = lib/libmymalloc.a
OBJ = test/*.out

all:tests/testforc.out tests/testforcpp.out

tests/%.out: src/%.c
	$(CC) $^ $(FLAGS) -o $@

tests/%.out: src/%.cpp
	$(CPP) -o $@ $^ $(LIB)
.PHONY:
clean:
	$(RMRF) $(OBJ)
