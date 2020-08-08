#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PIPE_FD_SIZE 2

void DataWritePipe(int *lhs, int *rhs, char *op, int fd) {
	write(fd, lhs, sizeof(int));
	write(fd, op, sizeof(char));
	write(fd, rhs, sizeof(int));
}

void DataReadPipe(int *res, int fd) {
	read(fd, res, sizeof(int));
}

int Sum(int lhs, int rhs) {
	return lhs + rhs;
}

int Sub(int lhs, int rhs) {
	return lhs - rhs;
}

int main(void) {
	int fd1[PIPE_FD_SIZE];
	int fd2[PIPE_FD_SIZE];
	int pr;
	if (pipe(fd1) == -1 || pipe(fd2) == -1) {
		printf("Can\'t create pipe\n");
		exit(0);
	}
	pr = fork();
	if (pr == -1) {
		printf("Can\'t fork child\n");
		exit(0);
	} else if (pr > 0) {
		close(fd1[0]);
		close(fd2[1]);
		int res,
			lhs,
			rhs;
		char op;
		while (scanf("%d %c %d", &lhs, &op, &rhs) == 3) {
			if (op != '+' &&  op != '-') {
				printf("Unknow operation\n");
				continue;
			}
			DataWritePipe(&lhs, &rhs, &op, fd1[1]);
			DataReadPipe(&res, fd2[0]);
			printf("%d\n", res);
		}
		close(fd1[1]);
		close(fd2[0]);
	} else {
		close(fd1[1]);
		close(fd2[0]);
		int res,
			lhs,
			rhs;
		char op;
		while (read(fd1[0], &lhs, sizeof(int))) {
			read(fd1[0], &op, sizeof(char));
			read(fd1[0], &rhs, sizeof(int));
			res = (op == '+') ? Sum(lhs, rhs) : Sub(lhs, rhs);
			write(fd2[1], &res, sizeof(int));
		}
		close(fd1[0]);
		close(fd2[1]);
	}
	return 0;
}
