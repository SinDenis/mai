//
// Created by Sin Denis on 2020-05-28.
//
#include "ArgParse.hpp"
#include "ParseConstants.hpp"

NArgParse::TParseException::TParseException(std::string msg) noexcept : Msg(std::move(msg))
{
}

const char* NArgParse::TParseException::what() const noexcept {
	return Msg.c_str();
}

static bool IsIndexingParseFlag(const char* arg) {
	return arg == NParseConst::INPUT_KEY || arg == NParseConst::OUTPUT_KEY;
}

static bool IsSearchParseFlag(const char *arg) {
	return IsIndexingParseFlag(arg) || arg == NParseConst::INDEX_KEY;
}

static void IndexingParse(std::unordered_map<std::string, std::string> &args, char **argv)  {
	for (int i = 2; i < 6; i += 2) {
		if (!IsIndexingParseFlag(argv[i]) || args.find(argv[i]) != args.end()) {
			throw NArgParse::TParseException("Wrong arguments");
		}
		args.emplace(argv[i], argv[i + 1]);
	}
}

static void SearchParse(std::unordered_map<std::string, std::string> &args, char **argv) {
	for (int i = 2; i < 8; i += 2) {
		if (!IsSearchParseFlag(argv[i]) || args.find(argv[i]) != args.end()) {
			throw NArgParse::TParseException("Wrong arguments");
		}
		args.emplace(argv[i], argv[i + 1]);
	}
}

std::unordered_map<std::string, std::string> NArgParse::Parse(int argc, char **argv) {

	std::unordered_map<std::string, std::string> args;

	if ((argc != 6 && argc != 8)
	|| (argv[1] != NParseConst::COMMAND_INDEX
	&& argv[1] != NParseConst::COMMAND_SEARCH)) {
		throw TParseException("Wrong arguments");
	}

	args.emplace(NParseConst::COMMAND, argv[1]);

	if (argv[1] == NParseConst::COMMAND_INDEX && argc == 6) {
		IndexingParse(args, argv);
	} else if (argv[1] == NParseConst::COMMAND_SEARCH && argc == 8) {
		SearchParse(args, argv);
	} else {
		throw NArgParse::TParseException("Wrong arguments");
	}

	return args;
}