#include "pch.h"
#include "SDL3/SDL.h"

namespace ThirdParty
{
	TEST(SDL3, Initialize)
	{
        // 오디오 시스템 초기화
        if (!SDL_Init(SDL_INIT_AUDIO))
            return;

        // WAV 로드
        SDL_AudioSpec spec{};
        Uint8* wavData = nullptr;
        Uint32 wavLength = 0;

        if (!SDL_LoadWAV("../Resources/Sound/Wav/click5.wav", &spec, &wavData, &wavLength)) {
            std::cerr << "SDL_LoadWAV failed: " << SDL_GetError() << "\n";
            SDL_Quit();
            return;
        }

        // 오디오 디바이스 + 스트림 생성 (callback은 null로 두고 나중에 Put으로 데이터 넣기)
        SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,  // 기본 재생 장치
            &spec,
            nullptr,  // 콜백 없음
            nullptr   // userdata 없음
        );

        if (!stream) {
            std::cerr << "SDL_OpenAudioDeviceStream failed: " << SDL_GetError() << "\n";
            SDL_free(wavData);
            SDL_Quit();
            return;
        }

        // 스트림에 WAV 데이터 넣기
        if (!SDL_PutAudioStreamData(stream, wavData, wavLength)) {
            std::cerr << "SDL_PutAudioStreamData failed: " << SDL_GetError() << "\n";
            SDL_DestroyAudioStream(stream);
            SDL_free(wavData);
            SDL_Quit();
            return;
        }

        // 재생 시작 (스트림 언페이즈)
        if (!SDL_ResumeAudioStreamDevice(stream)) {
            std::cerr << "SDL_ResumeAudioStreamDevice failed: " << SDL_GetError() << "\n";
        }

        // 재생이 끝날 때까지 대기
        int ms = (wavLength * 1000) / (spec.freq * spec.channels * (SDL_AUDIO_BITSIZE(spec.format) / 8));
        SDL_Delay(ms + 50);

        SDL_DestroyAudioStream(stream);
        SDL_free(wavData);
        SDL_Quit();
	}
}