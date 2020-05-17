CC = gcc
MKDIR_P = mkdir -p
CFLAGS = -Wall -Wpedantic `pkg-config --cflags ncurses`
LDFLAGS = 
LIB = `pkg-config --libs ncurses`
DSRC = src
SRC = $(wildcard $(DSRC)/*.c)
DBUILD = build
OBJ = $(patsubst $(DSRC)/%.c, $(DBUILD)/%.o, $(SRC))
DBIN = bin
TARGET = $(DBIN)/ncmines
INCLUDE = include

$(TARGET) : $(OBJ)
	@$(MKDIR_P) $(DBIN)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIB)

$(DBUILD)/%.o : $(DSRC)/%.c
	@$(MKDIR_P) $(DBUILD)
	$(CC) -c -o $@ $(CFLAGS) -I$(INCLUDE) $<

.PHONY: clean .depend

clean :
	$(RM) $(OBJ)
	$(RM) $(TARGET)

.depend : $(SRC)
	@$(RM) ./.depend
	@$(CC) -M $(CFLAGS) -I$(INCLUDE) $^ > ./.depend
	@sed -i '/^ /! s/^/$(DBUILD)\//' ./.depend

include .depend
