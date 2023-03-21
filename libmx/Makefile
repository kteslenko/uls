CC = clang
CFLAGS = -c -std=c11 -Wall -Wextra -Werror -Wpedantic -Iinc
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:src/%.c=obj/%.o)
OUT = libmx.a

all: $(OUT)

$(OUT): $(OBJECTS)
	ar rsc $@ $^

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -o $@ $<

obj:
	mkdir -p $@

uninstall:
	rm -rf obj
	rm -f $(OUT)

clean:
	rm -f $(OBJECTS)

reinstall: uninstall all
