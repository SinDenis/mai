CC = gcc

CCKEYS = -Wall -Wextra -Werror -pedantic

LD = gcc

LDKEYS = 

main: main.o table.o vector.o
	@$(LD) $(LDKEYS) main.o table.o vector.o -o main

main.o: main.c table.h
	@$(CC) $(CCKEYS) -c main.c

table.o: table.c table.h vector.h
	@$(CC) $(CCKEYS) -c table.c

vector.o: vector.c vector.h
	@$(CC) $(CCKEYS) -c vector.c

clean:
	@rm -rf main.o table.o vector.o main
