// COPYRIGHT: Larisa Florea

# compiler setup
CC = gcc
CFLAGS =-Wall -Wextra -std=c99

# define targets
TARGETS = image_editor

build: $(TARGETS)

image_editor: functions_image_editor.o image_editor.c
	$(CC) $(CFLAGS) functii_image_editor.o image_editor.c -o image_editor -lm

functii_image_editor.o: functions_image_editor.c
	$(CC) -c $(CFLAGS) functions_image_editor.c

pack:
	zip -FSr src.zip README Makefile *.c *.h

clean:
	rm -f *.o $(TARGETS)

.PHONY: pack clean