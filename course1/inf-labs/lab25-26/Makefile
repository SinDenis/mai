CC = gcc

LD = gcc

CCKEYS = -Wall -Werror -Wextra -pedantic

LDKEYS = 

OBJ = main.o sort.o stack.o

.SUFFIXES: .c .o

main: $(OBJ)
	@$(LD) $(LDKEYS) -o main $(OBJ)

main.o: main.c sort.h stack.h
	@$(CC) $(CCKEYS) -c main.c

.c.o:
	@$(CC) $(CCKEYS) -c $< -o $@

sort.c: sort.h

stack.c: stack.h

fclean: clean
	@rm -rf main

clean:
	@rm -rf *.o
