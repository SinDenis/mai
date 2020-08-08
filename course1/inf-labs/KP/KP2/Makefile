CC = gcc
NAME = sparse_matrix
FLAGS = -Wall -Wextra -Werror

VECTOR = vector/vector.c

SRC_DIR = srcs

SRC = create.c\
	  read.c\
	  print.c\
	  operation.c\
	  supporting_functions.c\

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))

all:
	@$(CC) $(KEYS) main.c $(SRCS) $(VECTOR) -o $(NAME)

clean:
	@rm -rf $(NAME)

fclean: clean

re: fclean all