

.PHONY: clean

clean:
	@rm a.out

.PHONY: all
all:
	@notify-send "Building source file"
	gcc myWindow.c `pkg-config --cflags --libs gtk+-3.0`
