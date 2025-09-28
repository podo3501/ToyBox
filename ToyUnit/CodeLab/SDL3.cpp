#include "pch.h"
#include "SDL3/SDL.h"

//그냥 테스트 할려고 만들기는 했지만, 이렇게 테스트 하는건 올바른 방법이 아니다.
//나중에 이 테스트는 지울 예정. 
namespace b_CodeLab
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