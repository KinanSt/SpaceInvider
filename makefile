CC = gcc

SDL_INC = -Ilibs/SDL3/include \
					-Ilibs/SDL3_ttf/include \
					-Ilibs/SDL3_image/include

SDL_LIB = -Llibs/SDL3/lib -lSDL3 \
					-Llibs/SDL3_ttf/lib -lSDL3_ttf \
					-Llibs/SDL3_image/lib -lSDL3_image

OBJS = build/src/main.o build/src/visual.o build/src/invider.o build/src/bullet.o build/src/player.o build/src/list.o

programme: $(OBJS)
	$(CC) $(OBJS) -o build/programme $(SDL_LIB)

build/src/%.o: src/%.c
	if not exist "build\\src" mkdir "build\\src"
	$(CC) -c $< -o $@ $(SDL_INC)

build/libs/%.o: libs/%.c
	if not exist "build\\libs" mkdir "build\\libs"
	$(CC) -c $< -o $@ $(SDL_INC)

clean:
	del /Q build\src\*.o build\libs\*.o build\programme.exe 2>nul || true