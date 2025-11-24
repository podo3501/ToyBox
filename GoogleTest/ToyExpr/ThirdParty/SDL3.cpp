#include "pch.h"
#include "SDL3/SDL.h"

namespace ThirdParty
{
	TEST(SDL3, Initialize)
	{
        if (!SDL_Init(SDL_INIT_AUDIO))
            return;

        // WAV ·Îµå
        SDL_AudioSpec spec{};
        Uint8* wavData = nullptr;
        Uint32 wavLength = 0;
        if (!SDL_LoadWAV("../Resources/Sound/Wav/click5.wav", &spec, &wavData, &wavLength))
        {
            SDL_Quit();
            return;
        }

        SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,  &spec, nullptr, nullptr);

        if (!stream) 
        {
            SDL_free(wavData);
            SDL_Quit();
            return;
        }

        if (!SDL_PutAudioStreamData(stream, wavData, wavLength)) {
            SDL_DestroyAudioStream(stream);
            SDL_free(wavData);
            SDL_Quit();
            return;
        }

        if (!SDL_ResumeAudioStreamDevice(stream))
            return;

        int ms = (wavLength * 1000) / (spec.freq * spec.channels * (SDL_AUDIO_BITSIZE(spec.format) / 8));
        SDL_Delay(ms + 50);

        SDL_DestroyAudioStream(stream);
        SDL_free(wavData);
        SDL_Quit();
	}
}