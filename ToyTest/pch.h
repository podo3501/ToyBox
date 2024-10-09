#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

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
#include <wrl/event.h>

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

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

#define Bookmark EXPECT_FALSE("BOOKMARK")