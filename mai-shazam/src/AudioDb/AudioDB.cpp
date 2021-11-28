//
// Created by Sin Denis on 2020-05-28.
//

#include "AudioDB.hpp"
#include "../Fft/Fft.hpp"
#include "../Mp3/Mp3.hpp"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>

TAudioDB::TDataPoint::TDataPoint(uint32_t time, uint32_t id):
	Time(time), Id(id)
{}

TAudioDB::TCoincidence::TCoincidence(double percent, uint32_t id, int32_t offset):
	Percent(percent), Id(id), Offset(offset)
{}

static std::vector<uint64_t> GetHashes(const std::string& recordName) {
	TFFT fftTransformer(NMP3Parser::GetSamples(recordName.c_str()));
	return fftTransformer.GetHashes();
}

std::unordered_map<uint32_t, std::unordered_map<int32_t, uint32_t>> TAudioDB::GetOffsetsSong(const std::vector<uint64_t>& hashes) {
	std::unordered_map<uint32_t, std::unordered_map<int32_t, uint32_t>> results;
	auto hash = hashes.begin();
	for (size_t time = 0; time < hashes.size(); ++time, ++hash) {
		if (Hashes.find(*hash) == Hashes.end()) {
			continue ;
		}

		for (auto& dp : Hashes[*hash]) {
			int32_t offset = dp.Time - time;
			if (results[dp.Id].find(offset) == results[dp.Id].end()) {
				results[dp.Id][offset] = 0;
			}
			++results[dp.Id][offset];
		}
	}
	return results;
}

std::string TAudioDB::Search(const std::string& recordName) {
	std::vector<uint64_t> hashes = GetHashes(recordName);
	std::unordered_map<uint32_t, std::unordered_map<int32_t, uint32_t>> offsetsSongs = GetOffsetsSong(hashes);
	std::vector<TCoincidence> coincidences;

	auto maxFunc = [](const std::pair<int32_t, uint32_t>& lhs, const std::pair<int32_t, uint32_t>& rhs) {
		return lhs.second < rhs.second;
	};

	for (auto& songs : offsetsSongs) {
		auto maxIt = std::max_element(songs.second.begin(), songs.second.end(), maxFunc);
		double coincidencePercent = (double)maxIt->second / hashes.size() * 100;
		if (coincidencePercent > 0.5) {
			coincidences.emplace_back(coincidencePercent, songs.first, maxIt->first);
		}
	}

	auto comp = [](const TCoincidence& lhs, const TCoincidence& rhs) { return lhs.Percent < rhs.Percent; };
	std::sort(coincidences.rbegin(), coincidences.rend(), comp);

	if (coincidences.empty()) {
		return "! NOT FOUND";
	}

	if (coincidences.size() == 1 ||
	(1.0 - coincidences[1].Percent / coincidences[0].Percent < 0.01
	&& std::abs(coincidences[0].Offset - coincidences[1].Offset) <= 1)) {
		return Records[coincidences.front().Id];
	}

	return 1.0 - coincidences[1].Percent / coincidences[0].Percent < 0.01 ? "! NOT FOUND" : Records[coincidences.front().Id];
}

void TAudioDB::Add(const std::string &recordName, const std::vector<uint64_t> &hashes) {
	size_t id = Records.size();
	Records.push_back(recordName);
	for (size_t i = 0; i < hashes.size(); ++i) {
		Hashes[hashes[i]].emplace_back(i, id);
	}
}

void TAudioDB::LoadRecords(std::ifstream& in) {
	int recordsNum;
	std::string recordName;

	in >> recordsNum;
	std::getline(in, recordName);
	for (int i = 0; i < recordsNum; ++i) {
		std::getline(in, recordName);
		Records.push_back(recordName);
	}
}

void TAudioDB::LoadHashes(std::ifstream& in) {
	int hashesNum;
	int dataPointsNum;
	int timePoint;
	int recordId;
	uint64_t hash;

	in >> hashesNum;
	for (int i = 0; i < hashesNum; ++i) {
		in >> hash >> dataPointsNum;
		for (int j = 0; j < dataPointsNum; ++j) {
			in >> timePoint >> recordId;
			Hashes[hash].emplace_back(timePoint, recordId);
		}
	}
}

void TAudioDB::SaveRecords(std::ofstream& out) {
	out << Records.size() << "\n";
	for (auto& record: Records) {
		out << record << "\n";
	}
}

void TAudioDB::SaveHashes(std::ofstream& out) {
	out << Hashes.size() << "\n";
	for (auto& p : Hashes) {
		out << p.first << "\t" << p.second.size() << "\t";
		for (size_t i = 0; i < p.second.size(); ++i) {
			out << p.second[i].Time << "\t" << p.second[i].Id;
			out << (i + 1 == p.second.size() ? "\n" : "\t");
		}
	}
}

void TAudioDB::Load(const std::string& filename) {
	std::ifstream in(filename);
	LoadRecords(in);
	LoadHashes(in);
}

void TAudioDB::Save(const std::string& filename) {
	std::ofstream out(filename);
	SaveRecords(out);
	SaveHashes(out);
}

