//
// pch.h
// Header for standard system include files.
//

#pragma once

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <winsdkver.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#include <sdkddkver.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef __MINGW32__
#include <unknwn.h>
#endif

#include <wrl/client.h>

#ifdef USING_DIRECTX_HEADERS
#include <directx/dxgiformat.h>
#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include <dxguids/dxguids.h>
#else
#include <d3d12.h>

#include "../Include/d3dx12.h"
#endif

#include <dxgi1_6.h>

#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <cmath>
#include <comdef.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <exception>
#include <fstream>
#include <functional>
#include <iterator>
#include <numbers>
#include <map>
#include <memory>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <windowsx.h>

//DirectXTK12 include
#include "../DirectXTK12/Inc/Audio.h"
#include "../DirectXTK12/Inc/BufferHelpers.h"
#include "../DirectXTK12/Inc/CommonStates.h"
#include "../DirectXTK12/Inc/DirectXHelpers.h"
#include "../DirectXTK12/Inc/DDSTextureLoader.h"
#include "../DirectXTK12/Inc/DescriptorHeap.h"
#include "../DirectXTK12/Inc/Effects.h"
#include "../DirectXTK12/Inc/GamePad.h"
#include "../DirectXTK12/Inc/GeometricPrimitive.h"
#include "../DirectXTK12/Inc/GraphicsMemory.h"
#include "../DirectXTK12/Inc/Keyboard.h"
#include "../DirectXTK12/Inc/Model.h"
#include "../DirectXTK12/Inc/Mouse.h"
#include "../DirectXTK12/Inc/PrimitiveBatch.h"
#include "../DirectXTK12/Inc/ResourceUploadBatch.h"
#include "../DirectXTK12/Inc/RenderTargetState.h"
#include "../DirectXTK12/Inc/SimpleMath.h"
#include "../DirectXTK12/Inc/SpriteBatch.h"
#include "../DirectXTK12/Inc/SpriteFont.h" 
#include "../DirectXTK12/Inc/VertexTypes.h"
#include "../DirectXTK12/Inc/WICTextureLoader.h"

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

// If using the DirectX Tool Kit for DX12, uncomment this line:
//#include "GraphicsMemory.h"

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) noexcept : result(hr) {}

        const char* what() const noexcept override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }
}

#ifdef __MINGW32__
namespace Microsoft
{
    namespace WRL
    {
        namespace Wrappers
        {
            class Event
            {
            public:
                Event() noexcept : m_handle{} {}
                explicit Event(HANDLE h) noexcept : m_handle{ h } {}
                ~Event() { if (m_handle) { ::CloseHandle(m_handle); m_handle = nullptr; } }

                void Attach(HANDLE h) noexcept
                {
                    if (h != m_handle)
                    {
                        if (m_handle) ::CloseHandle(m_handle);
                        m_handle = h;
                    }
                }

                bool IsValid() const { return m_handle != nullptr; }
                HANDLE Get() const { return m_handle; }

            private:
                HANDLE m_handle;
            };
        }
    }
}
#else
#include <wrl/event.h>
#endif

#ifdef __MINGW32__
constexpr UINT PIX_COLOR_DEFAULT = 0;

inline void PIXBeginEvent(UINT64, PCWSTR) {}

template<typename T>
inline void PIXBeginEvent(T*, UINT64, PCWSTR) {}

inline void PIXEndEvent() {}

template<typename T>
inline void PIXEndEvent(T*) {}
#else
// To use graphics and CPU markup events with the latest version of PIX, change this to include <pix3.h>
// then add the NuGet package WinPixEventRuntime to the project.
#include <pix.h>
#endif

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
