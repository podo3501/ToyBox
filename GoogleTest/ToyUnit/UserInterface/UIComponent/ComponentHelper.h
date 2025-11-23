#pragma once
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/SerializerIO/Storage/JsonStorageLocator.h"
#include "Shared/System/StepTimer.h"
#include "Toy/UserInterface/UIComponent/Traverser/UITraverser.h"

using namespace UITraverser;
struct ITextureController;
class MockTextureResourceBinder;

template <typename T>
bool TestWriteAndRead(unique_ptr<T>& component, const wstring& filename,
	MockTextureResourceBinder* resBinder = nullptr)
{
	auto storage = InitializeJsonStorage(StorageType::Memory);
	ReturnIfFalse(SerializerIO::WriteJsonToFile(component, filename));

	unique_ptr<T> read;
	ReturnIfFalse(SerializerIO::ReadJsonFromFile(filename, read));
	PropagateRoot(read.get());

	if (resBinder)
		BindTextureSourceInfo(read.get(), resBinder);

	return CompareDerived(component, read);
}

template <typename T>
bool TestClone(unique_ptr<T>& component)
{
	auto clone = Clone(component.get());
	return CompareDerived(component, clone);
}

//사용법 EXPECT_CALL(render, Render(testing::_, testing::_, testing::_)).WillRepeatedly(RenderLogger(L"테스트"));
auto RenderLogger(const wstring& label = L"")
{
	return [label](size_t index, const RECT& dest, const RECT* source) {
		wstring msg;
		msg += L"[" + label + L"]\n";
		msg += L" texture: " + to_wstring(index) + L"\n";
		msg += L" dest: (" + to_wstring(dest.left) + L"," +
			to_wstring(dest.top) + L"," +
			to_wstring(dest.right) + L"," +
			to_wstring(dest.bottom) + L")\n";

		if (source)
		{
			msg += L" source: (" + to_wstring(source->left) + L"," +
				to_wstring(source->top) + L"," +
				to_wstring(source->right) + L"," +
				to_wstring(source->bottom) + L")\n";
		}

		msg += L"\n";
		OutputDebugStringW(msg.c_str());
		};
}

template <typename Func>
auto ExecuteAndUpdate(UIComponent* component, Func&& func) noexcept
{
	if constexpr (is_void_v<decltype(func())>) {
		func();
		UpdatePositionsManually(component, true);
	}
	else
	{
		auto result = func();
		UpdatePositionsManually(component, true);
		return result;
	}
}

//틱값을 3ms 를 넣어서 약간 가게한 timer.
DX::StepTimer GetTickTimer() noexcept; 

void SimulateDrag(IMouseEventReceiver* receiver, const XMINT2& start, const XMINT2& end, bool inside = true) noexcept;
