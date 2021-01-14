CC := gcc
CPP := g++
RMRF := rm -rf
FLAGS = -I include/ -L lib/ -l mymalloc
LIB = lib/libmymalloc.a
OBJ = tests/*

all: tests/testforc tests/testforcpp

.PHONY:all

tests/testforc: src/testforc.c
	$(CC) $^ $(FLAGS) -o $@

tests/testforcpp: src/testforcpp.cpp
	$(CPP) -o $@ $^ $(LIB)


clean:
	$(RMRF) $(OBJ)
