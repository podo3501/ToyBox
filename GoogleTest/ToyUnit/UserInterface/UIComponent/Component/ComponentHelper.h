#pragma once
#include "Shared/SerializerIO/SerializerIO.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"

struct ITextureController;
class MockTextureResourceBinder;

template <typename T>
bool TestWriteAndRead(unique_ptr<T>& component, const wstring& filename,
	MockTextureResourceBinder* resBinder = nullptr, ITextureController* texController = nullptr)
{
	ReturnIfFalse(SerializerIO::WriteJsonToFile(component, filename));

	unique_ptr<T> read;
	ReturnIfFalse(SerializerIO::ReadJsonFromFile(filename, read));

	if (resBinder && texController)
		read->BindTextureSourceInfo(resBinder, texController);

	return CompareDerived(component, read);
}

template <typename T>
bool TestClone(unique_ptr<T>& component)
{
	auto clone = component->Clone();
	return CompareDerived(component, clone);
}

//���� EXPECT_CALL(render, Render(testing::_, testing::_, testing::_)).WillRepeatedly(RenderLogger(L"�׽�Ʈ"));
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
