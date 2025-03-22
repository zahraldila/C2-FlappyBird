CC = gcc
CFLAGS = -Iraylib/include -Iinclude  # Tambahkan -Iinclude untuk folder header
LDFLAGS = -Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
OUTPUT = build/game.exe
SRC = src/main.c src/bird.c src/main_menu.c src/Pipa.c src/collision.c src/score.c src/sound.c src/background.c

build: $(SRC)
	@if not exist build mkdir build
	$(CC) $(SRC) -o $(OUTPUT) $(CFLAGS) $(LDFLAGS)

run: build
	$(OUTPUT)

clean:
	@if exist build rmdir /s /q build