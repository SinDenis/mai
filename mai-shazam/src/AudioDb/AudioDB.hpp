//
// Created by Sin Denis on 2020-05-27.
//
#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <unordered_set>
#include <string>
#include <functional>


class TAudioDB {
public:

	TAudioDB() = default;

	std::string Search(const std::string& recordName);

	void Add(const std::string& recordName, const std::vector<uint64_t>& hashes);

	void Load(const std::string& filename);

	void Save(const std::string& filename);

private:

	struct TDataPoint {
		uint32_t Time;
		uint32_t Id;

		TDataPoint() = default;
		TDataPoint(uint32_t time, uint32_t id);
	};

	struct TCoincidence {
		double Percent;
		uint32_t Id;
		int32_t Offset;

		TCoincidence() = default;
		TCoincidence(double percent, uint32_t id, int32_t offset);
	};

	std::vector<std::string> Records;
	std::unordered_map<uint64_t, std::vector<TDataPoint>> Hashes;

	void LoadRecords(std::ifstream& in);

	void LoadHashes(std::ifstream& in);

	void SaveRecords(std::ofstream& out);

	void SaveHashes(std::ofstream& out);

	std::unordered_map<uint32_t, std::unordered_map<int32_t, uint32_t>> GetOffsetsSong(const std::vector<uint64_t>& hashes);

};
