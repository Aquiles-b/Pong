
CFLAGS = -Wall -Iinclude -Llib -static -static-libgcc -static-libstdc++ -lraylib -lopengl32 -lgdi32 -lwinmm

pong:
	g++ main.cpp -o pong $(CFLAGS)

run: pong
	.\pong
