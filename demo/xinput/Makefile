BIN = nuklear_gamepad_demo_xinput

CFLAGS += -std=c89 -Wall -Wextra -pedantic -O2

SRC = main.c
OBJ = $(SRC:.c=.o)

ifeq ($(OS),Windows_NT)
BIN := $(BIN).exe
LIBS = -lglfw3 -lopengl32 -lm -lGLU32 -lGLEW32 -lxinput
else
$(error XInput is only supported on Windows)
endif

$(BIN): clean
	$(CC) $(SRC) $(CFLAGS) -o $(BIN) $(LIBS)

clean:
	rm -f $(BIN) $(OBJS)

test: clean $(BIN)
	./$(BIN)
