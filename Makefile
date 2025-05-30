CC = gcc
CFLAGS = -Iraylib/include -Iinclude
LDFLAGS = -Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
OUTPUT = build/game.exe
SRC = src/main.c src/bird.c src/main_menu.c src/Pipa.c src/PipaLinkedList.c src/collision.c src/score.c src/sound.c src/background.c src/background_selector.c  src/game_state.c src/leaderboard.c src/help_menu.c

build: $(SRC)
	@if not exist build mkdir build
	$(CC) $(SRC) -o $(OUTPUT) $(CFLAGS) $(LDFLAGS)

run: build
	./$(OUTPUT)

clean:
	@if exist build rmdir /s /q build