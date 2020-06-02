/*
 * Copyright 2007-2020 The OpenRA Developers (see AUTHORS)
 * This file is part of OpenRA, which is free software. It is made
 * available to you under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version. For more
 * information, see COPYING.
 */

#include	"function.h"
#include    "AUDIOMIX.H"

#include <AL/al.h>
#include <AL/alc.h>

ALCdevice* device = nullptr;
ALCcontext* context = nullptr;

struct AudioBuffer_t {
	ALuint sourceId;
	ALuint buffer;
	ALsizei size;
	ALsizei frequency;
	ALenum format;
};

enum AudioBufferType_t {
	AUDIO_BUFFER_HOUSE_NONE = 0,
	AUDIO_BUFFER_HOUSE_ALIED,
	AUDIO_BUFFER_HOUSE_SOVIET,
	AUDIO_BUFFER_NUMTYPES
};

#define MAX_PRECACHE_AUDIO				512

AudioBuffer_t precacheAudioTable[AUDIO_BUFFER_NUMTYPES][MAX_PRECACHE_AUDIO];
AudioBuffer_t musicPrecacheAudioTable[MAX_PRECACHE_AUDIO];

int currentMusicPlaying = -1;

struct RIFF_Header {
	char chunkID[4];
	long chunkSize;
	char format[4];
};

struct WAVE_Format {
	char subChunkID[4];
	long subChunkSize;
	short audioFormat;
	short numChannels;
	long sampleRate;
	long byteRate;
	short blockAlign;
	short bitsPerSample;
};

struct WAVE_Data {
	char subChunkID[4];
	long subChunk2Size;
};

bool loadWavFile(const char* filename, AudioBuffer_t &audioBuffer) {
	FILE* soundFile = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;

	ALuint* buffer = &audioBuffer.buffer;
	ALsizei* size = &audioBuffer.size;
	ALsizei* frequency = &audioBuffer.frequency;
	ALenum* format = &audioBuffer.format;

	try {
		soundFile = fopen(filename, "rb");
		if (!soundFile) {
			return false;
		}

		fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

		if ((riff_header.chunkID[0] != 'R' ||
			riff_header.chunkID[1] != 'I' ||
			riff_header.chunkID[2] != 'F' ||
			riff_header.chunkID[3] != 'F') &&
			(riff_header.format[0] != 'W' ||
				riff_header.format[1] != 'A' ||
				riff_header.format[2] != 'V' ||
				riff_header.format[3] != 'E'))
			throw ("Invalid RIFF or WAVE Header");

		fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);

		if (wave_format.subChunkID[0] != 'f' ||
			wave_format.subChunkID[1] != 'm' ||
			wave_format.subChunkID[2] != 't' ||
			wave_format.subChunkID[3] != ' ')
			throw ("Invalid Wave Format");

		if (wave_format.subChunkSize > 16)
			fseek(soundFile, sizeof(short), SEEK_CUR);

		fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);

		if (wave_data.subChunkID[0] != 'd' ||
			wave_data.subChunkID[1] != 'a' ||
			wave_data.subChunkID[2] != 't' ||
			wave_data.subChunkID[3] != 'a')
			throw ("Invalid data header");

		data = new unsigned char[wave_data.subChunk2Size];

		if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
			throw ("error loading WAVE data into struct!");

		*size = wave_data.subChunk2Size;
		*frequency = wave_format.sampleRate;

		if (wave_format.numChannels == 1) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_MONO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_MONO16;
		}
		else if (wave_format.numChannels == 2) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_STEREO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_STEREO16;
		}

		alGenBuffers(1, buffer);
		alBufferData(*buffer, *format, (void*)data,
			*size, *frequency);
		fclose(soundFile);

		alGenSources(1, &audioBuffer.sourceId);
		alSourcei(audioBuffer.sourceId, AL_BUFFER, *buffer);
		alSource3f(audioBuffer.sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f);
		return true;
	}
	catch (char* error) {
		if (soundFile != NULL)
			fclose(soundFile);
		return false;
	}
}


/*
=============
AudMix_IsEnabled
=============
*/
bool AudMix_IsEnabled(void) {
	return device != NULL && context != NULL;
}

/*
=============
AudMix_PlayMusic
=============
*/
void AudMix_PlayMusic(int musicId) {
	if (currentMusicPlaying != -1) {
		alSourcei(musicPrecacheAudioTable[currentMusicPlaying].sourceId, AL_LOOPING, 0);
		alSourceStop(musicPrecacheAudioTable[currentMusicPlaying].sourceId);
	}
	
	alSourcei(musicPrecacheAudioTable[musicId].sourceId, AL_LOOPING, 1);
	alSourcePlay(musicPrecacheAudioTable[musicId].sourceId);
	currentMusicPlaying = musicId;
}

/*
=============
AudMix_Init
=============
*/
void AudMix_Init(void) {
	// Open up the audio device.
	device = alcOpenDevice(NULL);
	if (!device) {
		return;
	}

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {		
		return;
	}

	// Precache all of the audio.
	for (int i = 0; i < VOC_COUNT; i++) {
		char expandedFilePath[512];

		sprintf(expandedFilePath, "sound/%s.wav", GetEffectFileName(i));

		if (!loadWavFile(expandedFilePath, precacheAudioTable[AUDIO_BUFFER_HOUSE_NONE][i]))
		{
			sprintf(expandedFilePath, "sound/alied/%s.wav", GetEffectFileName(i));
			if (!loadWavFile(expandedFilePath, precacheAudioTable[AUDIO_BUFFER_HOUSE_ALIED][i])) {
			//	assert(!"failed to load audio!");
			}

			sprintf(expandedFilePath, "sound/russian/%s.wav", GetEffectFileName(i));
			if (!loadWavFile(expandedFilePath, precacheAudioTable[AUDIO_BUFFER_HOUSE_SOVIET][i])) {
				//assert(!"failed to load audio!");
			}
		}
	}

	// Precache the music.
	for (int i = 0; i < THEME_COUNT; i++) {
		char expandedFilePath[512];

		sprintf(expandedFilePath, "sound/music/%s.wav", GetThemeMusicFileName(i));
		if (!loadWavFile(expandedFilePath, musicPrecacheAudioTable[i])) {
			//assert(!"failed to load music!");
		}
	}

	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);	
}


void On_Sound_Effect(int sound_index, int variation, COORDINATE coord, int house)
{
	// MBL 06.17.2019
	int voc = sound_index;
	if (voc == VOC_NONE)
	{
		return;
	}
	alSourcePlay(precacheAudioTable[AUDIO_BUFFER_HOUSE_NONE][sound_index].sourceId);
}


// void On_Speech(int speech_index) // MBL 02.06.2020
void On_Speech(int speech_index, HouseClass* house)
{
	if (house == NULL) {
		alSourcePlay(precacheAudioTable[AUDIO_BUFFER_HOUSE_NONE][speech_index].sourceId);
	}

	// DLLExportClass::On_Speech(PlayerPtr, speech_index); // MBL 02.06.2020
	//if (house == NULL) {
	//	DLLExportClass::On_Speech(PlayerPtr, speech_index);
	//}
	//else
	//{
	//	DLLExportClass::On_Speech(house, speech_index);
	//}
}
