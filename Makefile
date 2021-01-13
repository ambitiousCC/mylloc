# CC = clang
# FLAGS = -O0 -W -Wall -Wextra -g

# all: malloc.so test-0 test-1 test-2 test-3 test-4 wrapper

# malloc.so: malloc.c
# 	$(CC) $^ $(FLAGS) -o $@ -shared -fPIC

# test-0: test/test-0.c
# 	$(CC) $^ $(FLAGS) -o $@

# test-1: test/test-1.c
# 	$(CC) $^ $(FLAGS) -o $@

# test-2: test/test-2.c
# 	$(CC) $^ $(FLAGS) -o $@

# test-3: test/test-3.c
# 	$(CC) $^ $(FLAGS) -o $@

# test-4: test/test-4.c
# 	$(CC) $^ $(FLAGS) -o $@

# wrapper: wrapper.c
# 	$(CC) $^ $(FLAGS) -o $@

GXX := clang 
INCLUDE_FILE := ./%.h
SRC_FILES := $(wildcard *.c)
OBJ_FILES = $(SRC_FILES:%.c=./.objs/%.o)
TARGET := ./lib/libmymalloc.a
CFLAGS = -I./ -Wall -Werror -fPIC -ggdb 
AR := ar
ARFLAG := -rc

$(TARGET): $(SRC_FILES) $(OBJ_FILES)
	$(AR) $(ARFLAG) $(TARGET) $(OBJ_FILES)

$(OBJ_FILES): ./.objs/%.o : ./%.c
	$(GXX) -o $@ -c $< $(CFLAGS)

$(TESTS_FILES): 

clean:
	-rm $(TARGET) $(OBJ_FILES)

sinclude $(DEP_FILES) 
