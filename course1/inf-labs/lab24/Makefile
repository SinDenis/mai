CC = gcc

LD = gcc

CCKEYS = -Wall -Wextra -Werror -pedantic

LDKEYS = 

OBJ = main.o deq.o tree.o opn.o

.SUFFIXES: .c .o

main: $(OBJ)
	@$(LD) $(LDKEYS) -o main $(OBJ)

main.o: main.c opn.h
	@$(CC) $(CCKEYS) -c main.c

.c.o:
	@$(CC) $(CCKEYS) -c $< -o $@

opn.c: opn.h deq.h tree.h

tree.c: tree.h

deq.c: deq.h

fclean: clean
	@rm -rf main

clean:
	@rm -rf *.o

