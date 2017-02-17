OS := $(shell uname)

SRC := jsdecode.c
OBJ := $(SRC:.c=.o)

CFLAGS := -Wall -fpic -O3

ifeq ($(OS), Darwin)
	SO := libjsdecode.dylib
else
	SO := libjsdecode.so
endif

.PHONY = all test clean

all: $(SO)

${OBJ}: ${SRC}
	$(CC) $(CFLAGS) -c $^

${SO}: ${OBJ}
	$(CC) $(CFLAGS) $^ -o $@ -shared

test: $(SO)
	gcc test.c -ljsdecode -L. -o runtest -Wl,-rpath=. && ./runtest
		
clean:
	rm -rf *.o *.so *.dylib runtest
