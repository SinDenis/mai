#include <iostream>
#include <unordered_map>
#include <fstream>

#include "AudioDb/AudioDB.hpp"
#include "ArgParsing/ArgParse.hpp"
#include "Controller/Controller.hpp"


int main(int argc, char *argv[]) {

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	try {
		std::unordered_map<std::string, std::string> args = NArgParse::Parse(argc, argv);
		NController::Execute(args);
	} catch (const std::exception& e) {
		std::cout << "Exception: " << e.what() << "\n";
	}

	return 0;
}
