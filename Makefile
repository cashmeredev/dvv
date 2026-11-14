CC			= clang
VENDOR      = -isystem vendor -DEPOLL -D_DEFAULT_SOURCE -Itemplates
CFLAGS		= -Wall -Wextra -O2 -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wvla -std=c99 -Werror
DBGFLAGS	= -Wall -Wextra -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer
SRC			= src/main.c
TPL			= $(patsubst %.tt,%,$(wildcard templates/*.h.tt))
HDR			= $(wildcard src/*.h vendor/*.h)
BIN			= build/main
VENDOR_SRC	= vendor/monocypher.c vendor/monocypher-ed25519.c
VENDOR_OBJ	= $(patsubst vendor/%.c,build/%.o,$(VENDOR_SRC))

.PHONY: run check asan valgrind clean

run: $(BIN)
	./$(BIN)

$(BIN): $(SRC) $(HDR) $(TPL) $(VENDOR_OBJ) Makefile
	mkdir -p build
	$(CC) $(CFLAGS) $(VENDOR) $(SRC) $(VENDOR_OBJ) -o $@

check: $(TPL)
	clang-tidy --header-filter='src/.*' $(SRC) -- -Wpedantic -Wall -Wextra $(VENDOR)

$(BIN)-asan: $(SRC) $(HDR) $(TPL) $(VENDOR_OBJ) Makefile
	mkdir -p build
	$(CC) $(DBGFLAGS) $(VENDOR) $(SRC) $(VENDOR_OBJ) -o $@

asan: $(BIN)-asan
	./$(BIN)-asan

$(BIN)-dbg: $(SRC) $(HDR) $(TPL) $(VENDOR_OBJ) Makefile
	mkdir -p build
	$(CC) -Wall -Wextra -g -O0 $(VENDOR) $(SRC) $(VENDOR_OBJ) -o $@

valgrind: $(BIN)-dbg
	valgrind --track-fds=yes ./$(BIN)-dbg

clean:
	rm -f $(BIN) $(BIN)-asan $(BIN)-dbg build/tt $(TPL) $(VENDOR_OBJ) src/*.o

build/tt: vendor/tt.c vendor/s.h vendor/memory.h
	mkdir -p build
	$(CC) -Wall -Wextra -std=c11 vendor/tt.c -o $@

templates/%.h: templates/%.h.tt build/tt
	./build/tt $< > $@

build/%.o: vendor/%.c vendor/monocypher.h
	mkdir -p build
	$(CC) -O2 -std=c99 -isystem vendor -c $< -o $@
