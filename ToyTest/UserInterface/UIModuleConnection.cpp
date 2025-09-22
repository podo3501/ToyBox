#include "pch.h"
#include "UIFixture.h"
#include "Utils/GeometryUtil.h"
#include "Helper.h"
#include "GameConfig.h"
#include "UserInterface/UIComponent/Components/UIModuleAsComponent.h"
#include "UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "UserInterface/UIComponent/Components/RenderTexture.h"
#include "UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "UserInterface/UIComponent/UIComponentEx.h"
#include "UserInterface/UIComponent/Components/SampleComponent.h"

using ::testing::_;
using ::testing::Exactly;

namespace a_UserInterface
{
	TEST_F(UIModuleConnection, AttachUIModuleToRenderTexture)
	{
		//Imgui에서 RenderTexture로 texture를 받기 때문에 따로 Render에 등록하지 않는다. 그래서 0값.
		EXPECT_CALL(*m_mockRenderer, AddRenderComponent(_))
			.Times(Exactly(0));

		UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
		wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
		auto [uiModule, uiModulePtr] = GetPtrs(CreateUIModule(layout, "Main", m_mockRenderer.get(), srcBinderFilename));
		auto [asComponent, asComponentPtr] = GetPtrs(CreateComponent<UIModuleAsComponent>(move(uiModule)));

		UIComponent* main = uiModulePtr->GetComponent();
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::ThreeH,
			GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(main).AttachComponent(move(switcher), { 100, 100 });
		
		bool toolMode{ false };
		UIModule* module = asComponent->GetUIModule();
		EXPECT_TRUE(module->EnableToolMode(toolMode)); //tool모드 일때에는 active update(ImplementUpdate)는 돌지 않아도 위치 업데이트(그냥 update 즉, ProcessUpdate)는 돌아야 한다.

		auto [renderTex, renderTexPtr] = GetPtrs(CreateComponent<RenderTexture>(move(asComponent)));
		EXPECT_TRUE(renderTexPtr->BindTextureSourceInfo(nullptr, m_mockRenderer->GetTextureController()));

		UIComponent* tex0 = UIEx(switcherPtr).FindComponent("PatchTextureLite1_2");
		XMINT2 pos = tex0->GetPosition();

		switcherPtr->ChangeSize({ 100, 48 }); //크기를 바꾼다. 
		renderTexPtr->EnableChildMouseEvents(!toolMode); //false로 셋팅하면 마우스가 영역에 들어왔을때 업데이트를 호출하는 코드가 돌지 않는다.
		renderTexPtr->ChangeSize({ 50, 50 });
		renderTexPtr->ChangeOrigin(Origin::Center);

		MockMouseInput(10, 10);	//렌더텍스쳐 안에 마우스 위치시킴
		renderTexPtr->ProcessUpdate(m_timer);

		UIComponent* tex1 = UIEx(switcherPtr).FindComponent("PatchTextureLite1_2");
		XMINT2 pos2 = tex1->GetPosition();

		EXPECT_FALSE(pos == pos2);
	}

	TEST_F(UIModuleConnection, UIModuleAsComponent)
	{
		UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
		wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
		unique_ptr<UIModule> uiModule = CreateUIModule(layout, "Main", m_mockRenderer.get(), srcBinderFilename);
		auto [asComponent, asComponentPtr] = GetPtrs(CreateComponent<UIModuleAsComponent>(move(uiModule)));

		UIModule* module = asComponent->GetUIModule();
		EXPECT_TRUE(module->EnableToolMode(true));

		UIComponent* main = module->GetComponent();
		auto [tex, img1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>("BackImage1"));
	}
}