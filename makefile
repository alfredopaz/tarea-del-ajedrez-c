# C11, compilación separada. Ejemplo: make HW=examples/pieza
CC = gcc
CPPFLAGS += -I. -MMD -MP
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Wstrict-prototypes
HW ?= ejemplo
BUILD := build
# Con fuente: recompilar normalmente. Sin fuente: usar el objeto distribuido.
# Se acepta build/chess.o o chess.o (y lo mismo para gc).
.DEFAULT_GOAL := all
SOURCE_MODULES := $(foreach m,chess gc,$(if $(wildcard $(m).c),$(m)))
BINARY_MODULES := $(filter-out $(SOURCE_MODULES),chess gc)

# Una regla explícita sin requisitos evita buscar el .c mediante reglas implícitas.
define binary_module
$(1)_OBJECT := $(or $(firstword $(wildcard $(BUILD)/$(1).o $(1).o)),$(BUILD)/$(1).o)
$$($(1)_OBJECT):
	@echo "Error: falta $(1).c y su objeto. Proporcione $(BUILD)/$(1).o o $(1).o." >&2
	@exit 1
endef
$(foreach m,$(BINARY_MODULES),$(eval $(call binary_module,$(m))))
$(foreach m,$(SOURCE_MODULES),$(eval $(m)_OBJECT := $(BUILD)/$(m).o))
COMMON := $(chess_OBJECT) $(gc_OBJECT) $(BUILD)/figures.o
HAVE_SOURCES := $(if $(filter 2,$(words $(SOURCE_MODULES))),yes,no)
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

$(BUILD)/test_chess: $(BUILD)/tests/test_chess.o $(COMMON)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

ifeq ($(HAVE_SOURCES),yes)
$(BUILD)/test_alloc_failures: tests/test_alloc_failures.c gc.c gc.h
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

test: $(BUILD)/test_chess $(BUILD)/test_alloc_failures
	./$(BUILD)/test_chess
	./$(BUILD)/test_alloc_failures
else
test: $(BUILD)/test_chess
	./$(BUILD)/test_chess
	@echo "Prueba de fallos de reserva omitida: requiere los fuentes privados."
endif

ifeq ($(HAVE_SOURCES),yes)
sanitize:
	@mkdir -p $(BUILD)
	$(CC) -I. -std=c11 -g -O1 -Wall -Wextra -Wpedantic -Werror -fno-omit-frame-pointer -fsanitize=address,undefined tests/test_chess.c chess.c gc.c figures.c -o $(BUILD)/test_sanitize
	ASAN_OPTIONS='$(ASAN_OPTIONS)' ./$(BUILD)/test_sanitize
	$(CC) -I. -std=c11 -g -O1 -Wall -Wextra -Wpedantic -Werror -fno-omit-frame-pointer -fsanitize=address,undefined tests/test_alloc_failures.c -o $(BUILD)/test_alloc_sanitize
	ASAN_OPTIONS='$(ASAN_OPTIONS)' ./$(BUILD)/test_alloc_sanitize

else
sanitize:
	@echo "Error: sanitize requiere chess.c y gc.c para instrumentarlos." >&2
	@exit 1
endif

# Los objetos sin fuente son parte de la distribución: clean debe conservarlos.
clean:
	@if test -d $(BUILD); then \
	  find $(BUILD) -type f $(foreach m,$(BINARY_MODULES),! -path '$(BUILD)/$(m).o') -delete; \
	  find $(BUILD) -depth -type d -empty -delete; \
	fi
	rm -f -- $(HW) $(HW)-ppm

# Ignorar los .d de objetos privados: podrían mencionar fuentes ya retirados.
-include $(addprefix $(BUILD)/,$(addsuffix .d,$(SOURCE_MODULES)))
-include $(BUILD)/figures.d $(BUILD)/main.d $(BUILD)/main-headless.d
-include $(BUILD)/$(HW).d $(BUILD)/tests/test_chess.d
