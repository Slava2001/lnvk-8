TARGET = asm
VERSION = 0.0.1

CC = gcc
CFLAGS = -std=c2x -Werror -Wall -Wextra  -Wshadow -Wconversion -Wformat=2 -Wcast-align     \
         -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls \
		 -Wundef -Wfloat-equal -Wpointer-arith -Wswitch-default -Wswitch-enum -Winit-self  \
		 -Wlogical-op -Wbad-function-cast -Wcast-qual -Wnested-externs -Wstrict-overflow=5 \
		 -O0 -g3 -D_DEFAULT_SOURCE -fmacro-prefix-map=$(pwd)=./
CFLAGS += -DTARGET_NAME=$(TARGET) -DTARGET_VERSION=$(VERSION)

LDFLAGS = -lm
BUILDDIR = build

ifeq ($(BUILD),release)
    CFLAGS += -O3 -DNDEBUG
endif

INCLUDE = include
SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c,$(BUILDDIR)/%.c.o,$(SRC))

CFLAGS += $(addprefix -I, $(INCLUDE))

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "[build] Link target: $@"
	@$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

$(BUILDDIR)/%.c.o: %.c
	@echo "[build] Compile $(@F)"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@

$(BUILDDIR)/src/stb_image_impl.c.o: src/stb_image_impl.c
	@echo "[build] Compile $(@F) (custom rules)"
	@mkdir -p $(@D)
	@$(CC) -I 3rdparty/stb -c $^ -o $@

clean:
	@rm -rf $(OBJ) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
