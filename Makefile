PREFIX ?= /usr
PROGRAM = qf
OBJS = $(subst .c,.o,$(wildcard *.c))

$(PROGRAM): $(OBJS)

install: $(PROGRAM)
	install $(PROGRAM) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(PROGRAM)

$(OBJS): Makefile
$(PROGRAM).o: $(wildcard *.h)

.PHONY: clean uninstall
clean:
	rm -f qf *.o
