HDR = table_new.h
SRC = table_new_demo.c
OBJ = $(SRC:.c=.o)
EXE = table_new_demo
CC = gcc
#-Wextra used to be -W
CFLAGS = -DDEBUG -g3 -Wall -Wextra  #-lm -Werror -std=c99
#CFLAGS = -DDEBUG -g3 -Wall -Wextra -pedantic  #-lm -Werror -std=c99

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -O2 -lph -lm -o $@ $(OBJ)  # -lpthread

r: clean $(EXE)
	./$(EXE)

.c.o:
	$(CC) $(CFLAGS) -O2 -c $<

clean:
	rm -f $(OBJ) $(EXE) *~
