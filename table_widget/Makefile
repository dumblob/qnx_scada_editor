# make [  | all | debug | submit | submit_zip | clean ]

HDR = table.h main_testing.h
SRC = main_testing.c table.c
EXE ?= demo_table
SUB ?= $(EXE)
CC ?= cc
CFLAGS ?= -W -Wall -Wextra -pedantic -lph #-lm -Werror -std=c99

OBJ = $(SRC:.c=.o)

all: $(EXE)

debug: CFLAGS += -DDEBUG -g -ggdb3
debug: CC += -DDEBUG -g
debug: all

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -O2 -o $@ $(OBJ)

submit:
	tar cvzf $(SUB).tar.gz $(SRC) $(HDR) Makefile

submit_zip:
	zip -q $(SUB).zip -9 $(SRC) $(HDR) Makefile

clean:
	rm -f $(OBJ) $(EXE) $(SUB).tar.gz *~

.c.o:
	$(CC) $(CFLAGS) -c $<
