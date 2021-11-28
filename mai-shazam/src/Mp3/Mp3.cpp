#include "Mp3.hpp"

static std::vector<float> ProcessFile(mpg123_handle *mh, const char* file) {
	// Open file
	auto errorCode = mpg123_open(mh, file);
	assert(errorCode == MPG123_OK);
	// Prevent warning (BEGIN)
	long rate;
	int channels, encoding;
	errorCode = mpg123_getformat(mh, &rate, &channels, &encoding);
	assert(errorCode == MPG123_OK);
	assert(rate == 44100);
	// Prevent warning (END)
	const int part_size = 1024;
	unsigned char part[part_size];
	size_t bytesRead;
	// Vector for decoded signal
	std::vector<float> samples;
	size_t bytesProcessed = 0;
	do {
    	int err = mpg123_read(mh, part, part_size, &bytesRead);
    	samples.resize((bytesProcessed + bytesRead) / 4 + 1);
    	memcpy((unsigned char*)samples.data() + bytesProcessed, part, bytesRead);
    	bytesProcessed += bytesRead;
    	if(err == MPG123_DONE)
      		break;
   		assert(err == MPG123_OK);
  	} while (bytesRead > 0);
  	samples.resize(bytesProcessed / 4);
  	// Close file
  	errorCode = mpg123_close(mh);
  	assert(errorCode == MPG123_OK);
  	return samples;
}

std::vector<float> NMP3Parser::GetSamples(const char* fileName) {
	// Library initialization (BEGIN)
	auto errorCode = mpg123_init();
	long flags = MPG123_MONO_MIX | MPG123_QUIET | MPG123_FORCE_FLOAT;
	std::vector<float> samples;
	assert(errorCode == MPG123_OK);
 	mpg123_handle *mh = mpg123_new(NULL, NULL);
 	assert(mh != NULL);
	errorCode = mpg123_param(mh, MPG123_FLAGS, flags, 0.);
	assert(errorCode == MPG123_OK);
	// Library initialization (END)
	samples = ProcessFile(mh, fileName);
	// Library deinitialization (BEGIN)
	mpg123_delete(mh);
	mpg123_exit();
	return samples;
}
