CFLAGS = -g
INCLUDES = -lncurses -lXm -lXt -lX11
LFLAGS = 
MAIN = main
PROGRAM = xsessionselector


all: $(MAIN)

$(MAIN): src/$(MAIN).c
	$(CC) $< $(CFLAGS) $(INCLUDES) $(LFLAGS) -o $(PROGRAM)

format:
	astyle \
		--fill-empty-lines \
		--indent=tab \
		--indent-switches \
		--suffix=none \
		src/*.c

install:
	cp $(PROGRAM) /usr/bin/$(PROGRAM)
	chmod 755 /usr/bin/$(PROGRAM)

uninstall:
	$(RM) /usr/bin/$(PROGRAM)

clean:
	$(RM) $(PROGRAM)

