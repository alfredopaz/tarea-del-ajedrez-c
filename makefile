# C11, compilación separada. Ejemplo: make HW=examples/pieza
CC = gcc
CPPFLAGS += -I. -MMD -MP
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Wstrict-prototypes
HW ?= ejemplo
BUILD := build
COMMON := $(BUILD)/chess.o $(BUILD)/gc.o $(BUILD)/figures.o
ASAN_OPTIONS ?= detect_leaks=1
GUI_LIBS := -lglut -lGL
ifeq ($(shell uname -s),Darwin)
GUI_LIBS := -framework GLUT -framework OpenGL
endif

.PHONY: all headless test sanitize clean
all: $(HW)

$(HW): $(BUILD)/main.o $(COMMON) $(BUILD)/$(HW).o
	$(CC) $(LDFLAGS) $^ $(GUI_LIBS) $(LDLIBS) -o $@

headless: $(HW)-ppm

$(HW)-ppm: $(BUILD)/main-headless.o $(COMMON) $(BUILD)/$(HW).o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD)/main-headless.o: main.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCHESS_HEADLESS -c $< -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/test_chess: tests/test_chess.c chess.c gc.c figures.c chess.h gc.h chess_internal.h figures.h
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) tests/test_chess.c chess.c gc.c figures.c $(LDLIBS) -o $@

$(BUILD)/test_alloc_failures: tests/test_alloc_failures.c gc.c gc.h
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

test: $(BUILD)/test_chess $(BUILD)/test_alloc_failures
	./$(BUILD)/test_chess
	./$(BUILD)/test_alloc_failures

sanitize:
	@mkdir -p $(BUILD)
	$(CC) -I. -std=c11 -g -O1 -Wall -Wextra -Wpedantic -Werror -fno-omit-frame-pointer -fsanitize=address,undefined tests/test_chess.c chess.c gc.c figures.c -o $(BUILD)/test_sanitize
	ASAN_OPTIONS='$(ASAN_OPTIONS)' ./$(BUILD)/test_sanitize
	$(CC) -I. -std=c11 -g -O1 -Wall -Wextra -Wpedantic -Werror -fno-omit-frame-pointer -fsanitize=address,undefined tests/test_alloc_failures.c -o $(BUILD)/test_alloc_sanitize
	ASAN_OPTIONS='$(ASAN_OPTIONS)' ./$(BUILD)/test_alloc_sanitize

clean:
	rm -rf $(BUILD)
	rm -f -- $(HW) $(HW)-ppm

-include $(COMMON:.o=.d) $(BUILD)/main.d $(BUILD)/main-headless.d $(BUILD)/$(HW).d
