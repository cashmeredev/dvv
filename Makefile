CC			= clang
VENDOR      = -isystem vendor -DEPOLL -D_DEFAULT_SOURCE -Itemplates
CFLAGS		= -Wall -Wextra -O2 -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wvla -std=c99 -Werror
DBGFLAGS	= -Wall -Wextra -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer
SRC			= src/main.c
TPL			= $(patsubst %.tt,%,$(wildcard templates/*.h.tt))
HDR			= $(wildcard src/*.h vendor/*.h)
BIN			= build/main

.PHONY: run check asan valgrind clean

run: $(BIN)
	./$(BIN)

$(BIN): $(SRC) $(HDR) Makefile
	mkdir -p build
	$(CC) $(CFLAGS) $(VENDOR) $(SRC) -o $@

check:
	clang-tidy --header-filter='src/.*' $(SRC) -- -Wall -Wextra $(VENDOR)

$(BIN)-asan: $(SRC) $(HDR) Makefile
	mkdir -p build
	$(CC) $(DBGFLAGS) $(VENDOR) $(SRC) -o $@

asan: $(BIN)-asan
	./$(BIN)-asan

$(BIN)-dbg: $(SRC) $(HDR) Makefile
	mkdir -p build
	$(CC) -Wall -Wextra -g -O0 $(VENDOR) $(SRC) -o $@

valgrind: $(BIN)-dbg
	valgrind --track-fds=yes ./$(BIN)-dbg

clean:
	rm -f $(BIN) $(BIN)-asan $(BIN)-dbg src/*.o

build/tt: vendor/tt.c vendor/s.h vendor/memory.h
	mkdir -p build
	$(CC) -Wall -Wextra -std=c11 vendor/tt.c -o $@

templates/%.h: templates/%.h.tt build/tt
	./build/tt $< > $@
