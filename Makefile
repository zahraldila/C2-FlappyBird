CC = gcc
CFLAGS = -Iraylib/include
LDFLAGS = -Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
OUTPUT = build/game.exe
SRC = src/main.c src/bird.c  # Tambahkan bird.c

build: $(SRC)
	@if not exist build mkdir build
	$(CC) $(SRC) -o $(OUTPUT) $(CFLAGS) $(LDFLAGS)

run: build
	$(OUTPUT)

clean:
	@if exist build rmdir /s /q build
