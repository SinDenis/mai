CC = gcc

FLAGS = -g -Wall -Wextra -Werror -pedantic

DB = data_base/db_table.a

all:
	@$(CC) $(FLAGS) input_db.c $(DB) -o input_db
	@$(CC) $(FLAGS) input_txt.c $(DB) -o input_txt
	@$(CC) $(FLAGS) generate.c $(DB) -o generate
	@$(CC) $(FLAGS) execute.c $(DB) -o execute
	@$(CC) $(FLAGS) student_add.c $(DB) -o student_add
	@$(CC) $(FLAGS) student_del.c $(DB) -o student_del

clean:
	@rm -rf *.dSYM
fclean: clean
	@rm -rf generate input_db input_txt execute student_add student_del
	@rm -rf *.bin *.txt

re: fclean all
