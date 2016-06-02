CC = gcc
EXEC = window

.PHONY: all

all:
	@notify-send -i face-wink "Building the source files"
	$(CC) myWindow.c `pkg-config gtk+-2.0 --cflags --libs` -o $(EXEC)

clean:
	@notify-send -i face-cool "Cleaning up the files"
	@rm $(EXEC)
