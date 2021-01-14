GXX := clang 
INCLUDE_FILE := ./include/%.h
SRC_FILES := $(wildcard src/*.c)
OBJ_FILES = $(SRC_FILES:%.c=.objs/%.o)
TARGET := lib/libmymalloc.a
CFLAGS = -I./ -Wall -Werror -fPIC -ggdb 
AR := ar
ARFLAG := -rc

$(TARGET): $(SRC_FILES) $(OBJ_FILES)
	$(AR) $(ARFLAG) $(TARGET) $(OBJ_FILES)

$(OBJ_FILES): .objs/%.o : src/%.c
	$(GXX) -o $@ -c $< $(CFLAGS)

$(TESTS_FILES): 

clean:
	-rm $(TARGET) $(OBJ_FILES)
