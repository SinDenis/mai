CC = gcc

LD = gcc

CCKEYS = -Wall -Werror -Wextra -pedantic

LDKEYS = 

OBJ = main.o bintree.o

.SUFFIXES: .c .o

main: $(OBJ)
	@$(LD) $(LDKEYS) -o main $(OBJ)

main.o: main.c bintree.h
	@$(CC) $(CCKEYS) -c main.c

.c.o:
	@$(CC) $(CCKEYS) -c $< -o $@

bintree.c: bintree.h

fclean: clean
	@rm -rf main

clean:
	@rm -rf *.o