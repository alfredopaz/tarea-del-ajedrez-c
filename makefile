# Proyecto original: C11 y GCC, sin alterar el arbol de directorios.
# MODE=reference (por defecto): biblioteca docente build/chess.o + build/gc.o.
# MODE=student: chess.c del alumno, SOLO malloc/free, sin recolector.
CC = gcc
CPPFLAGS += -I. -MMD -MP
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Wstrict-prototypes
HW ?= ejemplo
MODE ?= reference
BUILD := build
FUNCS := reverse join up repeatH repeatV superImpose flipH flipV rotateL rotateR
ASAN_OPTIONS ?= detect_leaks=1:halt_on_error=1
GUI_LIBS := -lglut -lGL
ifeq ($(shell uname -s),Darwin)
GUI_LIBS := -framework GLUT -framework OpenGL
endif

ifeq ($(MODE),student)
TARGET := $(HW)-student
CHESS_OBJ := $(BUILD)/chess-student.o
MAIN_OBJ := $(BUILD)/main-student.o
MAIN_PPM_OBJ := $(BUILD)/main-student-headless.o
GC_OBJ :=
else ifeq ($(MODE),reference)
TARGET := $(HW)
CHESS_OBJ := $(BUILD)/chess.o
MAIN_OBJ := $(BUILD)/main.o
MAIN_PPM_OBJ := $(BUILD)/main-headless.o
GC_OBJ := $(BUILD)/gc.o
else
$(error MODE debe ser reference o student)
endif

.DEFAULT_GOAL := all
.PHONY: all headless test test-reference test-func test-student test-integration test-failures \
        sanitize-func sanitize-student sanitize-integration valgrind-func \
        valgrind-student clean check-func

all: $(TARGET)
headless: $(TARGET)-ppm

$(TARGET): $(MAIN_OBJ) $(CHESS_OBJ) $(GC_OBJ) $(BUILD)/figures.o $(BUILD)/$(HW).o
	$(CC) $(LDFLAGS) $^ $(GUI_LIBS) $(LDLIBS) -o $@

$(TARGET)-ppm: $(MAIN_PPM_OBJ) $(CHESS_OBJ) $(GC_OBJ) $(BUILD)/figures.o $(BUILD)/$(HW).o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Los dos objetos docentes vienen en el ZIP. Nunca se recompilan desde chess.c.
$(BUILD)/chess.o $(BUILD)/gc.o:
	@test -f "$@" || { echo "Falta el objeto docente: $@" >&2; exit 1; }

$(BUILD)/chess-student.o: chess.c chess.h chess_internal.h
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/main-student.o: main.c chess.h chess_internal.h
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCHESS_MANUAL_MEMORY -c $< -o $@

$(BUILD)/main-student-headless.o: main.c chess.h chess_internal.h
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCHESS_MANUAL_MEMORY -DCHESS_HEADLESS -c $< -o $@

$(BUILD)/main-headless.o: main.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCHESS_HEADLESS -c $< -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Pruebas originales de la primera etapa: conservadas sin modificar.
$(BUILD)/test_chess: tests/test_chess.c chess.h $(BUILD)/chess.o $(BUILD)/gc.o $(BUILD)/figures.o
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_chess.c $(BUILD)/chess.o $(BUILD)/gc.o $(BUILD)/figures.o -o $@

$(BUILD)/test_alloc_failures: tests/test_alloc_failures.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

test test-reference: $(BUILD)/test_chess
	./$(BUILD)/test_chess

# Cada prueba enlaza chess.c del estudiante, nunca build/chess.o ni gc.o.
$(BUILD)/tests/test_%: tests/test_%.c tests/test_helpers.h chess.h chess_internal.h $(BUILD)/chess-student.o
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(BUILD)/chess-student.o -o $@

check-func:
	@case " $(FUNCS) " in *" $(FUNC) "*) ;; *) \
	  echo "FUNC invalida: $(FUNC). Opciones: $(FUNCS)" >&2; exit 2;; esac

test-func: check-func
	@$(MAKE) --no-print-directory $(BUILD)/tests/test_$(FUNC)
	./$(BUILD)/tests/test_$(FUNC)

test-student: $(addprefix $(BUILD)/tests/test_,$(FUNCS))
	@set -e; for f in $(FUNCS); do ./$(BUILD)/tests/test_$$f; done

$(BUILD)/tests/test_composition_manual: tests/test_composition_manual.c tests/test_helpers.h chess.h figures.h $(BUILD)/chess-student.o $(BUILD)/figures.o
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_composition_manual.c $(BUILD)/chess-student.o $(BUILD)/figures.o -o $@

test-integration: $(BUILD)/tests/test_composition_manual
	./$(BUILD)/tests/test_composition_manual

# Inyeccion de errores de malloc (solo GCC/Ubuntu): prueba liberaciones parciales.
$(BUILD)/tests/test_malloc_failures_manual: tests/test_malloc_failures_manual.c tests/test_helpers.h chess.c chess.h chess_internal.h
	@mkdir -p $(@D)
	$(CC) -I. -std=c11 -O0 -g -Wall -Wextra -Wpedantic \
	  tests/test_malloc_failures_manual.c chess.c \
	  -Wl,--wrap=malloc -Wl,--wrap=free -o $@

test-failures: $(BUILD)/tests/test_malloc_failures_manual
	./$(BUILD)/tests/test_malloc_failures_manual

sanitize-func: check-func
	@mkdir -p $(BUILD)/tests
	$(CC) -I. -std=c11 -O1 -g -Wall -Wextra -Wpedantic -fno-omit-frame-pointer \
	  -fsanitize=address,undefined tests/test_$(FUNC).c chess.c -o $(BUILD)/tests/asan_$(FUNC)
	ASAN_OPTIONS='$(ASAN_OPTIONS)' ./$(BUILD)/tests/asan_$(FUNC)

sanitize-student:
	@set -e; for f in $(FUNCS); do \
	  $(MAKE) --no-print-directory sanitize-func FUNC=$$f; \
	done

sanitize-integration:
	@mkdir -p $(BUILD)/tests
	$(CC) -I. -std=c11 -O1 -g -Wall -Wextra -Wpedantic -fno-omit-frame-pointer \
	  -fsanitize=address,undefined tests/test_composition_manual.c chess.c figures.c \
	  -o $(BUILD)/tests/asan_composition_manual
	ASAN_OPTIONS='$(ASAN_OPTIONS)' ./$(BUILD)/tests/asan_composition_manual

valgrind-func: check-func
	@command -v valgrind >/dev/null || { echo 'Instale valgrind: sudo apt install valgrind' >&2; exit 1; }
	@mkdir -p $(BUILD)/tests
	$(CC) -I. -std=c11 -O0 -g -Wall -Wextra -Wpedantic \
	  tests/test_$(FUNC).c chess.c -o $(BUILD)/tests/valgrind_$(FUNC)
	valgrind --leak-check=full --show-leak-kinds=all \
	  --errors-for-leak-kinds=definite,indirect,possible --error-exitcode=1 \
	  ./$(BUILD)/tests/valgrind_$(FUNC)

valgrind-student:
	@set -e; for f in $(FUNCS); do \
	  $(MAKE) --no-print-directory valgrind-func FUNC=$$f; \
	done

# Conservar los objetos binarios de referencia al limpiar.
clean:
	@if test -d $(BUILD); then \
	  find $(BUILD) -type f ! -path '$(BUILD)/chess.o' ! -path '$(BUILD)/gc.o' -delete; \
	  find $(BUILD) -depth -type d -empty -delete; \
	fi
	rm -f -- $(HW) $(HW)-ppm $(HW)-student $(HW)-student-ppm

-include $(wildcard $(BUILD)/*.d $(BUILD)/examples/*.d $(BUILD)/tests/*.d)
