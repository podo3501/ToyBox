#include "pch.h"
#include "SDL3/SDL.h"

namespace ThirdParty
{
	TEST(SDL3, Initialize)
	{
        EXPECT_TRUE(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));

        //Set audio spec
        SDL_AudioSpec audioSpec;
        SDL_zero(audioSpec);
        audioSpec.format = SDL_AUDIO_F32;
        audioSpec.channels = 2;
        audioSpec.freq = 44100;

        //Open audio device
        SDL_AudioDeviceID gAudioDeviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec);
        EXPECT_TRUE(gAudioDeviceId != 0);

	}
}