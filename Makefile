CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = main
SRC = main.c myers.c lines.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET)