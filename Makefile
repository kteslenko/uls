CC = clang
CFLAGS = -O2 -std=c11 -Wall -Wextra -Werror -Wpedantic -Iinc -Ilibmx/inc
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:src/%.c=obj/%.o)
LIB = libmx/libmx.a
OUT = uls

all: $(OUT)

$(OUT): $(OBJECTS) $(LIB)
	$(CC) -o $@ $^

$(LIB):
	make -C libmx

obj/%.o: src/%.c | obj
	$(CC) -c $(CFLAGS) -o $@ $<

obj:
	mkdir -p $@

uninstall:
	rm -rf obj
	rm -f $(OUT)

clean:
	rm -f $(OBJECTS)

reinstall: uninstall all
