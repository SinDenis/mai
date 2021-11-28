#include "Fft.hpp"

TFFT::TFFT(std::vector<float>&& samples) {
	Samples = std::move(samples);
	InitKhanWindow();
}

int TFFT::GetLastStart() {
	return std::max(0, (int)Samples.size() - WINDOW_SIZE);
}

double TFFT::GetMaxElem(TComplex<double>* data) {
	double maxElem = -1.0;
	for (int i = 0; i < WINDOW_SIZE; ++i) {
		if (data[i].Abs() > maxElem) {
			maxElem = data[i].Abs();
		}
	}

	return maxElem;
}

std::vector<double> TFFT::GetMaxAmplitudes() {
	std::vector<double> result;

	size_t end = GetLastStart() + 1;
	size_t step = Step * 2;
	for (size_t i = 0; i < end; i += step) {
		PrepareData(i);
		PutKhanWindow();
		IterativeFFT();
		ExtractTwoSeq();
		result.push_back(GetMaxElem(FirstSeq));
		if (i + Step < end) {
			result.push_back(GetMaxElem(SecondSeq));
		}
	}
	return result;
}

size_t TFFT::BitRev(int num) {
	size_t n = 0;
	for (int i = 0; i < LOG_WIN_SIZE; ++i) {
		if (num & (1 << i)) {
			n |= 1 << (LOG_WIN_SIZE - 1 - i);
		}
	}

	return n;
}

void TFFT::BitReverseCopy() {
	for (size_t i = 0; i < WINDOW_SIZE; ++i) {
		if (i < BitRev(i)) {
			std::swap(Data[i], Data[BitRev(i)]);
		}
	}

}

void TFFT::IterativeFFT() {
	BitReverseCopy();
	for (uint32_t s = 2; s <= WINDOW_SIZE; s <<= 1) {
		double ang = -2 * M_PI / s;
		TComplex<double> wm(std::cos(ang), std::sin(ang));
		for (size_t i = 0; i < WINDOW_SIZE; i += s) {
			TComplex<double> w(1.0);
			for (size_t j = 0; j < s / 2; ++j) {
				TComplex<double> u = Data[i + j];
				TComplex<double> t = Data[i + j + s / 2] * w;
				Data[i + j] = u + t;
				Data[i + j + s / 2] = u - t;
				w *= wm;
			}
		}
	}
}

void TFFT::InitKhanWindow() {
	for (int i = 0; i < WINDOW_SIZE; ++i)
		Window[i] = 0.5 * (1.0 - std::cos(2 * i * M_PI / (WINDOW_SIZE - 1)));
}

void TFFT::PrepareData(int start) {
	size_t end = start + WINDOW_SIZE;
	for (size_t i = start, j = 0; i < end; ++i, ++j) {
		Data[j].Real() = i < Samples.size() ? Samples[i] : 0;
	}
	end += Step;
	for (size_t i = start + Step, j = 0; i < end; ++i, ++j) {
		Data[j].Image() = i < Samples.size() ? Samples[i] : 0;
	}

}

void TFFT::PutKhanWindow() {
	for (int i = 0; i < WINDOW_SIZE; ++i) {
		Data[i].Real() *= Window[i];
		Data[i].Image() *= Window[i];
	}
}

void TFFT::ExtractTwoSeq() {
	int halfSizeWin = WINDOW_SIZE / 2;
	FirstSeq[0].Real() = Data[0].Real();
	SecondSeq[0].Real() = Data[0].Image();
	FirstSeq[halfSizeWin].Real() = Data[halfSizeWin].Real();
	SecondSeq[halfSizeWin].Real() = Data[halfSizeWin].Image();
	FirstSeq[halfSizeWin].Image() = FirstSeq[0].Image() = SecondSeq[0].Image() = SecondSeq[halfSizeWin].Image() = 0;
	for (int i = 1; i < halfSizeWin; ++i) {
		FirstSeq[i].Real() = (Data[i].Real() + Data[WINDOW_SIZE - i].Real()) / 2;
		FirstSeq[i].Image() = (Data[i].Image() - Data[WINDOW_SIZE - i].Image()) / 2;
		FirstSeq[WINDOW_SIZE - i].Real() = FirstSeq[i].Real();
		FirstSeq[WINDOW_SIZE - i].Image() = -FirstSeq[i].Image();
		SecondSeq[i].Real() = (Data[i].Image() + Data[WINDOW_SIZE - i].Image()) / 2;
		SecondSeq[i].Image() = (Data[i].Real() - Data[WINDOW_SIZE - i].Real()) / -2;
		SecondSeq[WINDOW_SIZE - i].Real() = SecondSeq[i].Real();
		SecondSeq[WINDOW_SIZE - i].Image() = -SecondSeq[i].Image();
	}
}

int TFFT::GetFreqRangeIdx(int id) {
	if (id > 40 && id < 80) {
		return 0;
	}
	if (id < 120) {
		return 1;
	}
	if (id < 180) {
		return 2;
	}
	return 3;
}

uint64_t TFFT::GetHash(int* freqs) {
	return (freqs[3] - (freqs[3] % FUZ_FACTOR)) * 10000000
		+ (freqs[2] - (freqs[2] % FUZ_FACTOR)) * 100000
		+ (freqs[1] - (freqs[1] % FUZ_FACTOR)) * 100
		+ freqs[0] - (freqs[0] % FUZ_FACTOR);
}

uint64_t TFFT::GetSeqHash(TComplex<double>* seq) {
	int maxFreqs[] = {-1, -1, -1, -1};
	double maxAmpls[] = {-1, -1, -1, -1};
	double ampl;
	int idx;
	for (int i = START_FREQ; i < END_FREQ; ++i) {
		idx = GetFreqRangeIdx(i);
		ampl = seq[i].Abs();
		if (ampl > maxAmpls[idx]) {
			maxAmpls[idx] = ampl;
			maxFreqs[idx] = i;
		}
	}
	return GetHash(maxFreqs);
}

uint64_t TFFT::GetFirstSeqHash() {
	return GetSeqHash(FirstSeq);
}

uint64_t TFFT::GetSecondSeqHash() {
	return GetSeqHash(SecondSeq);
}

std::vector<uint64_t> TFFT::GetHashes() {
	std::vector<uint64_t> result;

	size_t end = GetLastStart() + 1;
	size_t step = Step * 2;

	for (size_t i = 0; i < end; i += step) {
		PrepareData(i);
		PutKhanWindow();
		IterativeFFT();
		ExtractTwoSeq();
		result.push_back(GetFirstSeqHash());
		if (i + Step < end) {
			result.push_back(GetSecondSeqHash());
		}
	}
	return result;
}