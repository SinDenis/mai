/* Integer multiproccessing calculator */
/* Using file mapping and semaphores */
/* Author: Sin Denis */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>

typedef unsigned char uc_t;

typedef struct {
	int lhs;
	int rhs;
	char op;
} expr_t;

int expr_read(expr_t* expr)
{
	return scanf("%d %c %d", &expr->lhs, &expr->op, &expr->rhs) == 3;
}

void expr_write(expr_t* expr)
{
	printf("%d %c %d\n", expr->lhs, expr->op, expr->rhs);
}

int expr_sum(expr_t* expr)
{
	return expr->lhs + expr->rhs;
}

int expr_sub(expr_t* expr)
{
	return expr->lhs - expr->rhs;
}

void expr_share(expr_t* expr, void* memory, int start)
{
	int* lhs = (int*)((uc_t*)(memory) + start);
	int* rhs = (int*)((uc_t*)(memory) + start + 5);
	char* op = (char*)((uc_t*)(memory) + start + 4);
	*lhs = expr->lhs;
	*rhs = expr->rhs;
	*op = expr->op;
}

void expr_get_from_mem(expr_t* expr, void* memory, int pos)
{
	int* lhs = (int*)((uc_t *)(memory) + pos);
	int* rhs = (int*)((uc_t *)(memory) + pos + 5);
	char* op = (char*)((uc_t *)(memory) + pos + 4);
	expr->lhs = *lhs;
	expr->rhs = *rhs;
	expr->op = *op;
}

void expr_write_res(void* memory, int pos, int res)
{
	int *place = (int*)((uc_t*)(memory) + pos);
	*place = res;
}

int expr_get_res(void* memory, int pos)
{
	int* res = (int*)((uc_t*)(memory) + pos);
	return *res;
}

int get_temp_file(void)
{
	char *tempFileName = strdup("/tmp/tmpFile.XXXXXX");
	int fd = mkstemp(tempFileName);
	unlink(tempFileName);
	free(tempFileName);
	write(fd, "\0\0\0\0\0\0\0\0\0\0\0\0\0", 13);
	return fd;
}

void* get_map_memory(int fd, int start, int size)
{
	uc_t* mem = (uc_t*)mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, start);
	if (mem == MAP_FAILED) {
		perror("Memmory mapping failed");
		exit(1);
	}
	return mem;
}

int main()
{
	int fd = get_temp_file();
	uc_t* mem = get_map_memory(fd, 0, 13);
	sem_t* sem_calc = sem_open("/calculus", O_CREAT, 777, 0);
	sem_t* sem_output = sem_open("/output", O_CREAT, 777, 0);
	if (sem_calc == SEM_FAILED || sem_output == SEM_FAILED) {
		perror("Semaphores doesn't create");
		exit(1);
	}
	sem_unlink("/calculus");
	sem_unlink("/output");
	int err = fork();
	if (err == -1) {
		perror("Can't fork child");
		exit(1);
	} else if (err > 0) {
		expr_t expr;
		int res;
		printf("> ");
		int read_flag;
		while ((read_flag = expr_read(&expr))) {
			if (expr.op != '+' && expr.op != '-') {
				printf("This operation doesn't support\n> ");
				continue ;
			}
			expr_share(&expr, mem, 0);
			sem_post(sem_calc);
			sem_wait(sem_output);
			res = expr_get_res(mem, 9);
			printf("%d\n> ", res);
		}
		mem[12] = 1;
		sem_post(sem_calc);
		sem_close(sem_calc);
		sem_close(sem_output);
		close(fd);
	} else {
		expr_t expr;
		int res;
		while (1) {
			sem_wait(sem_calc);
			if (mem[12] == 1)
				break;
			expr_get_from_mem(&expr, mem, 0);
			switch (expr.op) {
				case '+':
					res = expr_sum(&expr);
					break;
				case '-':
					res = expr_sub(&expr);
					break;
			}
			expr_write_res(mem, 9, res);
			sem_post(sem_output);
		}
		sem_close(sem_calc);
		sem_close(sem_output);
		munmap(mem, 13);
		close(fd);
	}
	return 0;
}
