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
#ifndef __VOICE_H__
#define __VOICE_H__

#include "common.h"
#include "Points.h"
#include <portaudio.h>

class Voice
{
	public:
		static const int NumberMin = 0;
		static const int NumberMax = 180;

		static constexpr const char *GameOn		= "game-on";
		static constexpr const char *NoScore	= "no-score";
		static constexpr const char *WonLeg		= "won-leg";
		static constexpr const char *WonSet		= "won-set";
		static constexpr const char *WonGame	= "won-game";
		static constexpr const char *Busted		= "busted";
		static constexpr const char *Double		= "double";
		static constexpr const char *Tripple	= "tripple";
		static constexpr const char *Out     	= "out";
		static constexpr const char *Bull   	= "bull";
		static constexpr const char *Bullseye  	= "bullseye";

	private:
		std::string baseDir;
		std::string voicePath;
		std::string voice;
		std::vector<std::string> voices;

		static constexpr const char *defaultVoice = "espeak.en";
		static constexpr const char *VoiceDir = "voices";
		static constexpr const char *VoiceExtension	= "wav";

		bool checkVoice(std::string voice,Poco::Path& voicePath);

		static const int MaxWaveDataSize = 250 * 1024;	// 250 kB

		struct {
			char chunkId[4];			// "RIFF")
			uint32_t chunkSize;			//	file length - 8
			char riffType[4];			// "WAVE"

			char fmtChunkId[4];			// "fmt "
			uint32_t fmtChunkSize;		//	size of this chunk
			uint16_t formatTag;			// 1 = PCM
			uint16_t channels;			// 1 or 2
			uint32_t samplesPerSec;		// 22050 or 44100
			uint32_t avgBytesPerSec;
			uint16_t blockAlign;		// frame size
			uint16_t bitsPerSample;		// same for all channels
		} waveHeader;
		struct {
			char chunkId[4];			// "data", "list", "info"
			uint32_t chunkSize;			//	size of this chunk
		} waveChunk;

		int waveChannels = 0;
		int waveSampleFormat = 0;
		int waveSampleRate = 0;

		std::vector<std::string> waveFileQueue;

		unsigned char *waveData = NULL;
		unsigned long waveDataSize = 0;
		unsigned long waveDataOffset = 0;

		bool loadWave(std::string wavfile);
		bool dropWave(void);

		bool playbackInitialized = false;
		bool playbackInit(void);
		void playbackExit(void);
		bool playbackOpen(int channels,int SampleFormat,int SampleRate);
		static int playbackPlayCallback(const void *in,void *out,unsigned long frames,const PaStreamCallbackTimeInfo* timeInfo,PaStreamCallbackFlags statusFlags,void *data);
		static void playbackStopCallback(void *data);
		void playbackClose(void);
		bool playbackStart(void);
		bool playbackStop(void);

		PaStream *playbackStream = NULL;
		bool playbackPlaying = false;
		int playbackChannels = 0;
		int playbackSampleFormat = 0;
		int playbackSampleRate = 0;

		bool playbackFile(std::string wavfile);
		bool playbackQueued(void);
		bool playback(std::string wavfile);

	private:
		std::string checkVoicePath(std::string voice);

	public:
		Voice();
		~Voice();

		std::vector<std::string> getVoices(void);

		void noVoice(void);
		bool setDefaultVoice(void);
		bool setVoice(std::string voice);
		std::string getVoice(void);

		void shutup(void);

		bool say(std::string word,bool shutup = false);
		bool say(int number,bool shutup = false);
		bool say(Points& points,bool shutup = false);

		bool playback(void);
		void wait(void);
};

extern Voice *_voice;

#endif

/**/
