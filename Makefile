CFLAGS = 
INCLUDES = -lncurses
LFLAGS = 
MAIN = x-session-selector


all: $(MAIN)

$(MAIN): src/$(MAIN).c
	$(CC) $< $(CFLAGS) $(INCLUDES) $(LFLAGS) -o $@

format:
	astyle \
		--fill-empty-lines \
		--indent=tab \
		--indent-switches \
		--suffix=none \
		src/*.c

install:
	cp $(MAIN) /usr/bin/$(MAIN)
	chmod 755 /usr/bin/$(MAIN)

uninstall:
	$(RM) /usr/bin/$(MAIN)

clean:
	$(RM) $(MAIN)

