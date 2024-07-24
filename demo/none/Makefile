# Install
BIN = nuklear_gamepad_demo_none

# Flags

# TODO: Use C89
#CFLAGS += -std=c89 -pedantic -O0
CFLAGS += -std=c99 -pedantic -O0

CFLAGS += `sdl2-config --cflags`

SRC = main.c
OBJ = $(SRC:.c=.o)

ifeq ($(OS),Windows_NT)
#TODO
#BIN := $(BIN).exe
#LIBS = -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lm -lGLU32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
#TODO		LIBS = -lSDL2 -framework OpenGL -lm
	else
		LIBS += -lm -ldl `sdl2-config --libs`
	endif
endif

$(BIN): clean
	$(CC) $(SRC) $(CFLAGS) -o $(BIN) $(LIBS)

clean:
	rm -rf $(BIN) $(OBJ)

test: clean $(BIN)
	./$(BIN)