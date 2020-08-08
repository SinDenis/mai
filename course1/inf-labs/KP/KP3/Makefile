CC = gcc

CCKYES = -Wall -Wextra -Werror -pedantic

LD = gcc

LDKEYS = 

main: main.o list.o list.h
	@$(LD) -g $(CCKYES) -o main main.o list.o

main.o: main.c list.h
	@$(CC) $(CCKYES) -c main.c

list.o: list.c list.h
	@$(CC) $(CCKYES) -c list.c

fclean: clean
	@rm -rf main

clean:
	@rm -rf *.o
