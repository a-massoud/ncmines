CC = gcc
CFLAGS = -Wall -Wpedantic `pkg-config --cflags ncurses`
LDFLAGS = 
LIB = `pkg-config --libs ncurses`
DSRC = src
SRC = $(wildcard $(DSRC)/*.c)
DBUILD = build
OBJ = $(patsubst $(DSRC)/%.c, $(DBUILD)/%.o, $(SRC))
DBIN = bin
NAME = ncmines
TARGET = $(DBIN)/$(NAME)
INCLUDE = include
PREFIX = /usr

all: $(TARGET)

$(TARGET) : $(OBJ)
	@mkdir -p $(DBIN)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIB)

$(DBUILD)/%.o : $(DSRC)/%.c
	@mkdir -p $(DBUILD)
	$(CC) -c -o $@ $(CFLAGS) -I$(INCLUDE) $<

.PHONY: install clean .depend

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man6
	cp $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(NAME)
	gzip -c ncmines.6 > $(DESTDIR)$(PREFIX)/share/man/man6/ncmines.6.gz

clean :
	$(RM) $(OBJ)
	$(RM) $(TARGET)

.depend : $(SRC)
	@$(RM) ./.depend
	@$(CC) -M $(CFLAGS) -I$(INCLUDE) $^ > ./.depend
	@sed -i '/^ /! s/^/$(DBUILD)\//' ./.depend

include .depend
