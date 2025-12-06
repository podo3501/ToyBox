#pragma once
#include "ComponentT.h"
#include "Toy/UserInterface/UIComponent/Components/ListArea.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"

class ListAreaT : public ComponentT
{
protected:
	virtual void SetUp() override;
	virtual void RegisterBinderTextures(MockTextureResourceBinder* resBinder) override;
	void MakeTestPrototype();
	void MakeTestData(int itemCount);

	unique_ptr<ListArea> m_component;
	ScrollBar* m_scrollBar{ nullptr };
};

void ListAreaT::SetUp()
{
	ComponentT::SetUp();

	m_component = CreateSampleListArea({}, "BackImage", "ListBackground", "Track", "Button", &m_scrollBar);
	BindTextureSourceInfo(m_component.get(), GetResBinder());
}

void ListAreaT::RegisterBinderTextures(MockTextureResourceBinder* resBinder)
{
	resBinder->AddMockTextureKeys({
		{ "BackImage", { L"Texture512.png", TextureSlice::One, { { 100, 0, 32, 32 } } } },
		{ "ListBackground_Normal", { L"Texture512.png", TextureSlice::Nine, { //20, 8, 20 인 정사각형 9방향 좌표 (x 시작 200)
			{ 200, 0, 22, 22 }, { 222, 0, 4, 22 }, { 226, 0, 22, 22 },
			{ 200, 22, 22, 4 }, { 222, 22, 4, 4 }, { 226, 22, 22, 4 },
			{ 200, 26, 22, 22 }, { 222, 26, 4, 22 }, { 226, 26, 22, 22 } } } },
		{ "ListBackground_Hovered", { L"Texture512.png", TextureSlice::Nine, { //20, 8, 20 인 정사각형 9방향 좌표 (x 시작 300)
			{ 300, 0, 22, 22 }, { 322, 0, 4, 22 }, { 326, 0, 22, 22 },
			{ 300, 22, 22, 4 }, { 322, 22, 4, 4 }, { 326, 22, 22, 4 },
			{ 300, 26, 22, 22 }, { 322, 26, 4, 22 }, { 326, 26, 22, 22 } } } },
		{ "ListBackground_Pressed", { L"Texture512.png", TextureSlice::Nine, { //20, 8, 20 인 정사각형 9방향 좌표 (x 시작 400)
			{ 400, 0, 22, 22 }, { 422, 0, 4, 22 }, { 426, 0, 22, 22 },
			{ 400, 22, 22, 4 }, { 422, 22, 4, 4 }, { 426, 22, 22, 4 },
			{ 400, 26, 22, 22 }, { 422, 26, 4, 22 }, { 426, 26, 22, 22 } } } },
		{ "Track", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 20, 20, 5 }, { 0, 25, 20, 20 }, { 0, 45, 20, 5 } } } }, //폭 20, 높이 30
		{ "Button_Normal", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 0, 20, 4 }, { 0, 4, 20, 12 }, { 0, 16, 20, 4 } } } }, //폭 20, 높이 20
		{ "Button_Hovered", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 0, 20, 4 }, { 0, 4, 20, 12 }, { 0, 16, 20, 4 } } } },
		{ "Button_Pressed", { L"Texture512.png", TextureSlice::ThreeV, { { 0, 0, 20, 4 }, { 0, 4, 20, 12 }, { 0, 16, 20, 4 } } } },
		{ "One32", { L"Texture512.png", TextureSlice::One, { { 100, 100, 32, 32 } } } }
		});
}