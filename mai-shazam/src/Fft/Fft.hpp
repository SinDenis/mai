#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include "../Complex/Complex.hpp"

class TFFT {
public:
	explicit TFFT(std::vector<float>&& sample);
	std::vector<double> GetMaxAmplitudes();
	std::vector<uint64_t> GetHashes();

private:

	static constexpr int  WINDOW_SIZE = 4096;
	static constexpr int  FUZ_FACTOR = 2;
	static constexpr int  LOG_WIN_SIZE = 12;
	static constexpr int  START_FREQ = 40;
	static constexpr int  END_FREQ = 300;
	static constexpr int  Step = 1024;

	std::vector<float> Samples;

	double Window[WINDOW_SIZE];
	TComplex<double> FirstSeq[WINDOW_SIZE];
	TComplex<double> SecondSeq[WINDOW_SIZE];
	TComplex<double> Data[WINDOW_SIZE];

	int GetLastStart();
	size_t BitRev(int num);
	void BitReverseCopy();
	void IterativeFFT();
	void InitKhanWindow();
	void PrepareData(int start);
	void PutKhanWindow();
	void ExtractTwoSeq();
	static double GetMaxElem(TComplex<double>* data);


	int GetFreqRangeIdx(int id);
	uint64_t GetSeqHash(TComplex<double>* seq);

	uint64_t GetFirstSeqHash();
	uint64_t GetSecondSeqHash();

	uint64_t GetHash(int* freqs);
};
