CC = gcc

KEYS = -Wall -Wextra -Werror

SRC_O = *.o

NAME = db_table.a

$(NAME):
	@$(CC) $(KEYS) -c vector.c
	@$(CC) $(KEYS) -c db_table.c
	@ar rcs $(NAME) $(SRC_O)
	@ranlib $(NAME)

all: $(NAME)

clean:
	@rm -rf $(SRC_O)

fclean: clean
	@rm -rf $(NAME) check

re: fclean all

