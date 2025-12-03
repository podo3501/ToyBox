#include "pch.h"
#include "SDL3_Audio.h"
#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"

namespace ThirdParty
{
    TEST_F(SDL3_Audio, DISABLED_Playing_OneOgg)
    {
        if (!SDL_Init(SDL_INIT_AUDIO))
            return;

        if (!MIX_Init())
            return;

        MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!mixer)
            return;

        MIX_Audio* audio = MIX_LoadAudio(mixer, "../Resources/Sound/Ogg/mouseclick1.ogg", false);
        if (!audio)
            return;

        MIX_Track* track = MIX_CreateTrack(mixer);
        MIX_SetTrackAudio(track, audio);

        SDL_PropertiesID options;
        options = SDL_CreateProperties();
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
        MIX_PlayTrack(track, options);
        SDL_Delay(1000);
        
        if (track) MIX_DestroyTrack(track);
        if (audio) MIX_DestroyAudio(audio);
        if (mixer) MIX_DestroyMixer(mixer);
        MIX_Quit();
        SDL_Quit();
    }

    TEST_F(SDL3_Audio, DISABLED_Playing_Ogg)
    {
        if (!SDL_Init(SDL_INIT_AUDIO))
            return;

        if (!MIX_Init())
            return;

        MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!mixer) 
            return;
        
        MIX_Audio* audio = MIX_LoadAudio(mixer, "../Resources/Sound/Ogg/mouseclick1.ogg", false);
        if (!audio)
            return;

        MIX_Audio* audio2 = MIX_LoadAudio(mixer, "../Resources/Sound/Ogg/hope_it_will.ogg", false);
        if (!audio2)
            return;

        MIX_Track* track = MIX_CreateTrack(mixer);
        MIX_SetTrackAudio(track, audio);

        MIX_Track* track2 = MIX_CreateTrack(mixer);
        MIX_SetTrackAudio(track2, audio2);
        MIX_SetTrackGain(track2, 0.7f);

        SDL_PropertiesID options;
        options = SDL_CreateProperties();
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
        MIX_PlayTrack(track, options);
        MIX_PlayTrack(track2, options);
        SDL_Delay(10000);

        if (track2) MIX_DestroyTrack(track2);
        if (track) MIX_DestroyTrack(track);
        if (mixer) MIX_DestroyMixer(mixer);
        MIX_Quit();
        SDL_Quit();
    }

    TEST_F(SDL3_Audio, DISABLED_Playing_Wav)
    {
        //if (!SDL_Init(SDL_INIT_AUDIO))
        //    return;

        if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
            return;

        bool isInit = SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO;

        // WAV 로드
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
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        SDL_Quit();
	}

    TEST_F(SDL3_Audio, DISABLED_Playing_OggWav)
    {
        if (!SDL_Init(SDL_INIT_AUDIO))
            return;

        if (!MIX_Init())
            return;

        if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
            return;

        // WAV 로드
        SDL_AudioSpec spec{};
        Uint8* wavData = nullptr;
        Uint32 wavLength = 0;
        if (!SDL_LoadWAV("../Resources/Sound/Wav/click5.wav", &spec, &wavData, &wavLength))
        {
            SDL_Quit();
            return;
        }

        SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);

        if (!stream)
        {
            SDL_free(wavData);
            SDL_Quit();
            return;
        }

        MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!mixer)
            return;

        MIX_Audio* audio = MIX_LoadAudio(mixer, "../Resources/Sound/Ogg/hope_it_will.ogg", false);
        if (!audio)
            return;

        MIX_Track* track = MIX_CreateTrack(mixer);
        MIX_SetTrackAudio(track, audio);

        SDL_PropertiesID options;
        options = SDL_CreateProperties();
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
        MIX_PlayTrack(track, options);

        Uint64 start = SDL_GetTicks();  // 시작 시간 기록

        while (true)
        {
            Uint64 now = SDL_GetTicks();

            // 10초(10000ms) 지나면 루프 탈출
            if (now - start >= 10000)
                break;

            if (!SDL_PutAudioStreamData(stream, wavData, wavLength)) 
            {
                SDL_DestroyAudioStream(stream);
                SDL_free(wavData);
                SDL_Quit();
                return;
            }

            SDL_ResumeAudioStreamDevice(stream);
            SDL_Delay(1000); // 1초 대기
        }

        SDL_DestroyAudioStream(stream);
        SDL_free(wavData);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);

        if (track) MIX_DestroyTrack(track);
        if (mixer) MIX_DestroyMixer(mixer);
        MIX_Quit();
        SDL_Quit();
    }
}