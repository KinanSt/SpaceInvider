CC = gcc

ifeq ($(OS),Windows_NT)
  EXE_EXT = .exe
  SDL_INC = -Ilibs/SDL3/include \
			-Ilibs/SDL3_ttf/include \
			-Ilibs/SDL3_image/include
  SDL_LIB = -Llibs/SDL3/lib -lSDL3 \
			-Llibs/SDL3_ttf/lib -lSDL3_ttf \
			-Llibs/SDL3_image/lib -lSDL3_image
  MKDIR = if not exist "$(subst /,\,$1)" mkdir "$(subst /,\,$1)"
  RM = if exist $(subst /,\,$1) del /Q $(subst /,\,$1)
else
  EXE_EXT =
  SDL_INC = $(shell pkg-config --cflags sdl3 SDL3_ttf SDL3_image)
  SDL_LIB = $(shell pkg-config --libs sdl3 SDL3_ttf SDL3_image)
  MKDIR = mkdir -p "$1"
  RM = rm -f "$1"
endif

OBJS = build/src/main.o build/src/visual.o build/src/invider.o build/src/bullet.o build/src/player.o build/src/list.o

programme: $(OBJS)
	$(call MKDIR,bin)
	$(CC) $(OBJS) -o bin/programme$(EXE_EXT) $(SDL_LIB)

build/src/%.o: src/%.c
	$(call MKDIR,build/src)
	$(CC) -c $< -o $@ $(SDL_INC)

build/libs/%.o: libs/%.c
	$(call MKDIR,build/libs)
	$(CC) -c $< -o $@ $(SDL_INC)

clean:
	$(call RM,build/src/*.o)
	$(call RM,build/libs/*.o)
	$(call RM,bin/programme$(EXE_EXT))

.PHONY: programme clean