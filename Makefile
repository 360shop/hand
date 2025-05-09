CC = gcc
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags opencv)
LIBS = $(shell pkg-config --libs opencv)

TARGET = hand_gesture
SRC = hand_gesture.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

.PHONY: clean
clean:
	rm -f $(TARGET)