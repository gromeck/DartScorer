/*

	DartScorer

	(c) 2023 by Christian.Lorenz@gromeck.de

	This file is part of DartScorer

    DartScorer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DartScorer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DartScorer.  If not, see <https://www.gnu.org/licenses/>.

*/
#include "Voice.h"

Voice::Voice()
{
	LOG_DEBUG("-");

	/*
	**	construct the base path
	*/
	Poco::Path path(DATADIR);
	path.pushDirectory("voices");
	baseDir = path.toString();

	/*
	**	use the default voice as the standard
	*/
	setDefaultVoice();

	LOG_DEBUG("baseDir=%s",baseDir);
	LOG_DEBUG("voicePath=%s",voicePath);

	playbackInit();
}

std::string Voice::checkVoicePath(std::string voice)
{
	LOG_DEBUG("voice=%s",voice);

	Poco::Path path = baseDir;

	path.pushDirectory(voice);
	path.setBaseName(GameOn);
	path.setExtension(VoiceExtension);

	Poco::File file(path);
	LOG_DEBUG("searching=%s",path.toString());
	if (file.exists()) {
		path = baseDir;
		path.pushDirectory(voice);
		LOG_DEBUG("path=%s",path.toString());
		return path.toString();
	}
	return "";
}

std::vector<std::string> Voice::getVoices(void)
{
	LOG_DEBUG("-");

	std::vector<std::string> filenames;

	try {
		Poco::File file(baseDir);
		file.list(filenames);

		voices.clear();
		for (int n = 0;n < (int) filenames.size();n++)
			if (!checkVoicePath(filenames[n]).empty())
				voices.push_back(filenames[n]);

		std::sort(voices.begin(), voices.end());
	} catch (Poco::Exception& exc) {
		std::cerr << exc.displayText() << std::endl;
	}

	return voices;
}

void Voice::noVoice(void)
{
	LOG_DEBUG("-");

	this->voice.clear();
}

bool Voice::setDefaultVoice(void)
{
	LOG_DEBUG("-");

	return setVoice(defaultVoice);
}

bool Voice::setVoice(std::string voice)
{
	LOG_DEBUG("voice=%s",voice);

	std::string path = checkVoicePath(voice);

	if (!path.empty()) {
		this->voice = voice;
		this->voicePath = path;
		LOG_DEBUG("voice=%s  voicePath=%s",voice,voicePath);
		return true;
	}
	noVoice();
	return false;
}

std::string Voice::getVoice(void)
{
	LOG_DEBUG("-");

	return this->voice;
}

bool Voice::loadWave(std::string waveFile)
{
	LOG_DEBUG("waveFile=%s",waveFile);

	dropWave();	// clear the previous one

	FILE *wav;

	if ((wav = fopen(waveFile.c_str(),"rb")) == NULL) {
		LOG_ERROR("error opening file=%s",waveFile);
		dropWave();
		return false;
	}

	// read the header
	if (fread(&waveHeader,1,sizeof(waveHeader),wav) != sizeof(waveHeader)) {
		LOG_ERROR("error loading header from file=%s",waveFile);
		dropWave();
		return false;
	}

	LOG_DEBUG("sizeof(waveHeader)=%?d",sizeof(waveHeader));
	LOG_DEBUG("chunkId=%4.4s",std::string(waveHeader.chunkId));
	LOG_DEBUG("chunkSize=%?ld",(unsigned long) waveHeader.chunkSize);
	LOG_DEBUG("riffType=%4.4s",std::string(waveHeader.riffType));
	LOG_DEBUG("fmtChunkId=%4.4s",std::string(waveHeader.fmtChunkId));
	LOG_DEBUG("fmtChunkSize=%?ld",(unsigned long) waveHeader.fmtChunkSize);
	LOG_DEBUG("formatTag=%?d",waveHeader.formatTag);
	LOG_DEBUG("channels=%?d",waveHeader.channels);
	LOG_DEBUG("samplesPerSec=%?d",waveHeader.samplesPerSec);
	LOG_DEBUG("avgBytesPerSec=%?d",waveHeader.avgBytesPerSec);
	LOG_DEBUG("blockAlign=%?d",waveHeader.blockAlign);
	LOG_DEBUG("bitsPerSample=%?d",waveHeader.bitsPerSample);

	// check the header
	if (strncmp(waveHeader.chunkId,"RIFF",sizeof(waveHeader.chunkId))) {
		LOG_ERROR("%s: missing RIFF id",waveFile);
		dropWave();
		return false;
	}
	if (strncmp(waveHeader.riffType,"WAVE",sizeof(waveHeader.riffType))) {
		LOG_ERROR("%s: missing WAVE id",waveFile);
		dropWave();
		return false;
	}
	if (waveHeader.formatTag != 1) {
		LOG_ERROR("%s: not PCM encoded",waveFile);
		dropWave();
		return false;
	}
	if (waveHeader.channels < 1 || waveHeader.channels > 2) {
		LOG_ERROR("%s: channels=%d, expected is 1 or 2",waveFile,waveHeader.channels);
		dropWave();
		return false;
	}
	waveChannels = waveHeader.channels;
	waveSampleRate = waveHeader.samplesPerSec;
	switch (waveHeader.bitsPerSample) {
		case 8:
			waveSampleFormat = paInt8;
			break;
		case 16:
			waveSampleFormat = paInt16;
			break;
		case 32:
			waveSampleFormat = paInt32;
			break;
		default:
			LOG_ERROR("%s: bitsPerSample=%d is not supported",waveFile,waveHeader.bitsPerSample);
			dropWave();
			return false;
	}

	// read the following chunks until we reached the data chunk
	waveChunk.chunkSize = 0;
	do {
		fseek(wav,waveChunk.chunkSize,SEEK_CUR);
		if (fread(&waveChunk,1,sizeof(waveChunk),wav) != sizeof(waveChunk)) {
			LOG_ERROR("%s: error loading waveChunk",waveFile);
			dropWave();
			return false;
		}
		LOG_DEBUG("sizeof(waveChunk)=%?d",sizeof(waveChunk));
		LOG_DEBUG("chunkId=%4.4s",std::string(waveChunk.chunkId));
		LOG_DEBUG("chunkSize=%?ld",waveChunk.chunkSize);
	} while (strncmp(waveChunk.chunkId,"data",sizeof(waveChunk.chunkId)));
	
	if (waveChunk.chunkSize < 0 || waveChunk.chunkSize > MaxWaveDataSize) {
		LOG_ERROR("%s: found %lu bytes, which is too large -- we only support up to %lu bytes",waveFile,waveChunk.chunkSize,MaxWaveDataSize);
		dropWave();
		return false;
	}

	// allocate memory for the data
	waveDataSize = waveChunk.chunkSize;
	if (!(waveData = (unsigned char*) malloc(waveDataSize))) {
		LOG_ERROR("%s: couldn't allocate %lu bytes",waveFile,waveDataSize);
		dropWave();
		return false;
	}

	// read the data
	waveDataOffset = 0;
	unsigned long readWaveDataSize = 0;
	while (readWaveDataSize < waveDataSize) {
		int chunk = fread(&waveData[readWaveDataSize],1,waveDataSize - readWaveDataSize,wav);
		if (chunk > 0)
			readWaveDataSize += chunk;
		if (chunk <= 0 || feof(wav) || ferror(wav))
			break;
	}
	fclose(wav);

	if (readWaveDataSize < waveDataSize) {
		LOG_ERROR("%s: error reading file",waveFile);
		dropWave();
		return false;
	}

	LOG_DEBUG("%s: successfuly loaded %lu bytes of data",waveFile,waveDataSize);
	return true;
}

bool Voice::dropWave(void)
{
	LOG_DEBUG("-");

	if (waveData)
		free(waveData);
	waveData = NULL;
	waveDataSize = 0;
	waveDataOffset = 0;
	waveChannels = 0;
	waveSampleRate = 0;
	waveSampleFormat = 0;
	memset(&waveHeader,0,sizeof(waveHeader));

	return true;
}

bool Voice::playbackInit(void)
{
	LOG_DEBUG("-");

	PaError err;

	if ((err = Pa_Initialize()) == paNoError) {
		playbackInitialized = true;
	}
	else {
		LOG_ERROR("Failed to initialize PortAudio");
		playbackInitialized = false;
	}
	return playbackInitialized;
}

bool Voice::playbackOpen(int channels,int sampleFormat,int sampleRate)
{
	LOG_DEBUG("channels=%d  sampleFormat=%d  sampleRate=%d",channels,sampleFormat,sampleRate);

	if (playbackStream) {
		// can we reuse this stream
		if (channels == playbackChannels && sampleFormat == playbackSampleFormat && sampleRate == playbackSampleRate)
			return true;
		playbackClose();
	}

	PaError err;

	if ((err = Pa_OpenDefaultStream(&playbackStream,
			0,	// no input
			playbackChannels = channels,
			playbackSampleFormat = sampleFormat,
			playbackSampleRate = sampleRate,
			256,//paFramesPerBufferUnspecified,
			playbackPlayCallback,
			(void *) this)) == paNoError) {

		Pa_SetStreamFinishedCallback(playbackStream,playbackStopCallback);
		return true;
	}
	else {
		LOG_ERROR("Failed to open audio stream");
		playbackClose();
	}
	return false;
}

bool Voice::playbackStart(void)
{
	LOG_DEBUG("-");

	if (playbackStream) {
		PaError err;

		if ((err = Pa_StartStream(playbackStream)) == paNoError) {
			playbackPlaying = true;
			return true;
		}

		LOG_ERROR("Failed to start audio stream: %d/%s",err,std::string(Pa_GetErrorText(err)));
	}
	return false;
}

int Voice::playbackPlayCallback(const void *in,void *out,unsigned long framesPerBuffer,const PaStreamCallbackTimeInfo *timeInfo,PaStreamCallbackFlags statusFlags,void *data)
{
	LOG_DEBUG("in=%s  out=%s  framesPerBuffer=%lu  timeInfo=%s  statusFlags=%lx  data=%s",PTR2STR(in),PTR2STR(out),framesPerBuffer,PTR2STR(timeInfo),statusFlags,PTR2STR(data));

	Voice *voice = (Voice *) data;

	unsigned long frameSize = framesPerBuffer * voice->playbackChannels * voice->waveHeader.blockAlign;
	unsigned long chunkSize = frameSize;

	if (chunkSize > voice->waveDataSize - voice->waveDataOffset)
		chunkSize = voice->waveDataSize - voice->waveDataOffset;

	LOG_DEBUG("waveDataSize=%lu  waveDataOffset=%lu  chunkSize=%lu",voice->waveDataSize,voice->waveDataOffset,chunkSize);

	if (chunkSize > 0) {
		memcpy(out,&voice->waveData[voice->waveDataOffset],chunkSize);
		voice->waveDataOffset += chunkSize;
		memset(&((unsigned char *) out)[chunkSize],0,frameSize - chunkSize);
		return paContinue;
	}
	return paComplete;
}

void Voice::playbackStopCallback(void *data)
{
	LOG_DEBUG("data=%s",PTR2STR(data));

	Voice *voice = (Voice *) data;

	voice->playbackPlaying = false;
}

bool Voice::playbackStop(void)
{
	LOG_DEBUG("-");

	playbackPlaying = false;
	if (playbackStream) {
		PaError err;

		if ((err = Pa_StopStream(playbackStream)) == paNoError)
			return true;

		// this error will be surpressed, as it seems not to be a readl issue
		//LOG_ERROR("Failed to stop audio stream");
	}
	return false;
}

void Voice::playbackClose(void)
{
	LOG_DEBUG("-");

	waveFileQueue.clear();
	playbackStop();

	Pa_StopStream(playbackStream);
	Pa_CloseStream(playbackStream);

	playbackStream = NULL;
	playbackChannels = 0;
	playbackSampleFormat = 0;
	playbackSampleRate = 0;
}

void Voice::playbackExit(void)
{
	LOG_DEBUG("-");

	playbackClose();
	Pa_Terminate();
}

bool Voice::playbackFile(std::string waveFile)
{
	if (!loadWave(waveFile))
		return false;

	if (!playbackInitialized)
		if (!playbackInit())
			return false;

	if (!playbackOpen(waveChannels,waveSampleFormat,waveSampleRate))
		return false;

	playbackStop();

	if (!playbackStart())
		return false;
	
	return true;
}

bool Voice::playbackQueued(void)
{
//	LOG_DEBUG("-");

#if 0
	for (int n = 0;n < (int) waveFileQueue.size();n++)
		LOG_DEBUG("queue[%d] %s",n,waveFileQueue[n].c_str());
#endif

	if (playbackStream && Pa_IsStreamActive(playbackStream))
		return true;

	if (waveFileQueue.size() > 0) {
		LOG_DEBUG("queue not empty");
		std::string waveFile = waveFileQueue.front();
		waveFileQueue.erase(waveFileQueue.begin());
		return playbackFile(waveFile);
	}
	return false;
}

bool Voice::playback(std::string waveFile)
{
	LOG_DEBUG("waveFile=%s",waveFile);

	waveFileQueue.push_back(waveFile);
	return playbackQueued();
}

bool Voice::playback(void)
{
//	LOG_DEBUG("-");

	if (!playbackStream || !Pa_IsStreamActive(playbackStream))
		return playbackQueued();
	return false;
}

void Voice::wait(void)
{
	LOG_DEBUG("-");

	while (!_shutdown && playbackInitialized && (((playbackStream && Pa_IsStreamActive(playbackStream)) || waveFileQueue.size() > 0))) {
		playback();
		usleep(10 * 1000);
	}
}

void Voice::shutup(void)
{
	LOG_DEBUG("-");

	waveFileQueue.clear();
	playbackStop();
}

bool Voice::say(std::string word,bool shutup)
{
	LOG_DEBUG("word=%s  shutup=%b",word,shutup);

	if (shutup)
		this->shutup();

	if (voice.empty())	// voice is off
		return false;

	if (voicePath.empty())
		return false;
	
	LOG_DEBUG("searching in %s",voicePath);

	Poco::Path path = voicePath;
	path.setBaseName(word);
	path.setExtension(VoiceExtension);

	LOG_DEBUG("searching in %s",path.toString());

	Poco::File file = Poco::File(path);
	if (!file.exists()) {
		LOG_ERROR("no wave file %s found for word %s",path.toString(),word);
		return false;
	}
	return playback(path.toString());
}

bool Voice::say(int number,bool shutup)
{
	LOG_DEBUG("number=%d  shutup=%b",number,shutup);

	if (shutup)
		this->shutup();

	if (number >= NumberMin && number <= NumberMax)
		return say(std::to_string(number));

	return false;
}

bool Voice::say(Points& points,bool shutup)
{
	LOG_DEBUG("points=%s  shutup=%b",points.toString(),shutup);

	if (shutup)
		this->shutup();

	int value = points.getValue();
	int factor = points.getFactor();

	if (value == Points::Bull && !_config->getConfigInt(ConfigKeys::OptionsShowBullAs25,1))
		return say((factor == Points::DoubleFactor) ? Bullseye : Bull);

	switch (factor) {
		case Points::TrippleFactor:
			if (!say(Tripple))
				return false;
			break;
		case Points::DoubleFactor:
			if (!say(Double))
				return false;
			break;
	}
	if (!value)
		return say(NoScore);
	return say(value);
}

Voice::~Voice()
{
	LOG_DEBUG("-");

	playbackExit();
	dropWave();
}/**/
