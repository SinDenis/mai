#include <iostream>
#include <utility>
#include <fstream>
#include "src/storage.h"

std::pair<uint16_t, uint16_t > parse(const std::string& filename) {
	std::ifstream in;
	std::string opt;
	uint16_t optVal;
	std::pair<uint16_t, uint16_t> ports;
	in.open(filename, std::ios_base::in);
	for (int i = 0; i < 2; ++i) {
		in >> opt >> optVal;
		if (opt == "server_port") {
			ports.first = optVal;
		} else if (opt == "push_reserve") {
			ports.second = optVal;
		}
	}
	return ports;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Usage: ./store1 config" << std::endl;
		return 0;
	}

	std::pair<uint16_t, uint16_t> ports;

	try {
		ports = parse(argv[1]);
	} catch (const std::exception& exc) {
		std::cout << "bad file" << std::endl;
		return 0;
	}

	MainStore mainStore(ports.first, ports.second);
	mainStore.run();
	return 0;
}
