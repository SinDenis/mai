CC = gcc
LIB = str_array
FLAGS = -Wall -Werror -Wextra
FILES = str_array.c

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(addprefix $(SRC_DIR)/, $(FILES))
OBJS = $(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))

RT_NAME = rt_main
CT_NAME = ct_main

all: $(CT_NAME) $(RT_NAME)

$(CT_NAME): lib$(LIB).so ct_main.c
	@$(CC) ct_main.c -L. -lstr_array -o $(CT_NAME)
	@echo $(CT_NAME) is builded

$(RT_NAME): lib$(LIB).so rt_main.c
	@$(CC) $(FLAGS) -ldl rt_main.c -o $(RT_NAME)
	@echo $(RT_NAME) is builded

lib$(LIB).so: $(OBJ_DIR) $(OBJS)
	@$(CC) -shared $(OBJS) -o lib$(LIB).so
	@echo lib$(LIB).so is builded

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(FLAGS) -fPIC -c $^ -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf lib$(LIB).so
	@rm -rf $(OBJ_DIR)
	@rm -rf $(RT_NAME)
	@rm -rf $(CT_NAME)
