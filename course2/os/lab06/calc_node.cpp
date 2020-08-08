#include <zmq.hpp>
#include <vector>
#include <unistd.h>
#include <iostream>
#include "src/node.h"
#include <cstdlib>
#include <signal.h>

void childHandler(int sig) {
	pid_t pid;
	pid = wait(nullptr);
}

int main(int argc, char** argv) {
	signal(SIGCHLD, childHandler);
	if (argc != 4)
		return 0;
	CalcNode calcNode(atoi(argv[0]), atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
	calcNode.Run();
	return 0;
}