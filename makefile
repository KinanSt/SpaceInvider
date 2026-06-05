programme: build/src/main.o build/libs/list.o
	gcc build/src/main.o build/libs/list.o -o build/programme

build/src/main.o: src/main.c libs/list.h
	gcc -c src/main.c -o build/src/main.o

build/libs/list.o: libs/list.c libs/list.h
	gcc -c libs/list.c -o build/libs/list.o