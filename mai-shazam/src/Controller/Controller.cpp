//
// Created by Sin Denis on 2020-05-28.
//

#include <fstream>
#include <unordered_map>
#include <string>

#include "Controller.hpp"
#include "../ArgParsing/ParseConstants.hpp"
#include "../AudioDb/AudioDB.hpp"
#include "../Fft/Fft.hpp"
#include "../Mp3/Mp3.hpp"

static void IndexRecords(const std::string& input, const std::string& output) {
	std::ifstream in(input);
	std::string recordName;
	TAudioDB audioDb;

	while (std::getline(in, recordName)) {
		if (recordName.empty()) {
			continue;
		}
		TFFT fftTransformer(NMP3Parser::GetSamples(recordName.c_str()));
		audioDb.Add(recordName, fftTransformer.GetHashes());
	}

	audioDb.Save(output);
}

static void Search(const std::string& index, const std::string& input, const std::string& output) {
	std::ifstream in(input);
	std::ofstream out(output);
	std::string recordName;
	TAudioDB audioDb;

	audioDb.Load(index);

	while (std::getline(in, recordName)) {
		if (recordName.empty()) {
			continue;
		}
		out << audioDb.Search(recordName) << "\n";
	}
}

void NController::Execute(std::unordered_map<std::string, std::string>& args) {
	if (args[NParseConst::COMMAND] == NParseConst::COMMAND_INDEX) {
		IndexRecords(args[NParseConst::INPUT_KEY], args[NParseConst::OUTPUT_KEY]);
		return ;
	}
	Search(args[NParseConst::INDEX_KEY], args[NParseConst::INPUT_KEY], args[NParseConst::OUTPUT_KEY]);
}
