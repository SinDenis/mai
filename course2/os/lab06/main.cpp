#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <cstdlib>
#include "src/message.h"
#include "src/node.h"

void childHandler(int sig) {
	pid_t pid;
	pid = wait(nullptr);
}

int main() {
	signal(SIGCHLD, childHandler);
	ManageNode manageNode;

	manageNode.Run();
	return 0;
}