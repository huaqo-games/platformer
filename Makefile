CC = gcc
CFLAGS = -Wall -std=c99
INCLUDE_PATH = -I/opt/homebrew/include
LIB_PATH = -L/opt/homebrew/Cellar/raylib/5.0/lib
LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit
TARGET = my_game
SRC = main.c utils.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $(TARGET) $(SRC) $(LIB_PATH) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: clean $(TARGET)
	./$(TARGET)