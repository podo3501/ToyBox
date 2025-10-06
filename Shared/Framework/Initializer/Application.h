#pragma once
#include "../AppLoop.h"
#include "Core/Public/IRenderer.h"
#include "Shared/Window/WindowProcedure.h"
#include "Shared/Window/Window.h"

template<typename LoopType>
unique_ptr<AppLoop> CreateAppLoop(HINSTANCE hInstance, int nShowCmd,
	const RECT& windowRect, const wstring& resourcePath, bool bImgui)
{
	if (!XMVerifyCPUSupport())
		return nullptr;

	// ������ ����
	auto window = make_unique<Window>();
	HWND hwnd{};
	if (!window->Create(hInstance, nShowCmd, windowRect, hwnd))
		return nullptr;

	// ImGui ��� ����
	if (bImgui)
	{
		window->AddWndProcListener([](HWND wnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT {
			return ImguiWndProc(wnd, msg, wp, lp);
			});
	}

	// ������ ����
	const auto& outputSize = window->GetOutputSize();
	auto renderer = CreateRenderer(hwnd,
		static_cast<int>(outputSize.x),
		static_cast<int>(outputSize.y),
		bImgui);
	if (!renderer)
		return nullptr;

	// ���� ����
	auto appLoop = make_unique<LoopType>(move(window), move(renderer));
	if (!appLoop->Initialize(resourcePath, outputSize))
		return nullptr;

	return move(appLoop);
}
