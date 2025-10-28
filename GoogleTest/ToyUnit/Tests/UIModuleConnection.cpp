#include "pch.h"
#include "Fixtures/FixtureSuite.h"
#include "Mocks/MockInputManager.h"
#include "Shared/Utils/StlExt.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Helper.h"
#include "Toy/UserInterface/UIComponent/UIUtility.h"
#include "Toy/UserInterface/UIComponent/Components/UIModuleAsComponent.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponent/Components/RenderTexture.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Toy/UserInterface/UIComponent/UIComponentEx.h"
#include "Toy/UserInterface/UIComponent/Components/SampleComponent.h"

using ::testing::_;
using ::testing::Exactly;

namespace UserInterface
{
	//RenderTexture + UIModuleAsComponent => 툴에서 쓰는 방식
	//UIModule + RenderTexture + UIModuleAsComponent => 클라이언트에서 쓰는 방식
	//결론적으로 RenderTexture + UIModuleAsComponent는 같은 동작을 해야 해서 툴에서 쓰는 방식과 클라이언트에서 쓰는 방식은 다르지 않다.
	//그래서 다르지 않도록 코딩을 해야 한다.
	//2가지 테스트를 통해서 업데이트 및 Render가 잘 되는지 확인한다.
	//툴에서 쓸때에는 AddRender를 하지 않아야 한다. UIModule이 UIModuleAsComponent로 붙을때 Remove를 해서 Render에서 제거하면 될 것 같다.
	//ImplementRender와 ImplementUpdate	는 mock으로 처리.
	//위치값은 이전 위치값과 이후 위치값을 비교하는 것으로 처리.
	TEST_F(UIModuleConnection, AttachUIModuleToRenderTexture)
	{
		//Imgui에서 RenderTexture로 texture를 받기 때문에 따로 Render에 등록하지 않는다. 그래서 0값.
		EXPECT_CALL(*m_mockRenderer, AddRenderComponent(_))
			.Times(Exactly(0));

		UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
		wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
		auto [uiModule, uiModulePtr] = GetPtrs(CreateUIModule(layout, "Main", m_mockRenderer.get(), srcBinderFilename));
		auto [asComponent, asComponentPtr] = GetPtrs(CreateComponent<UIModuleAsComponent>(move(uiModule)));

		UIComponent* main = uiModulePtr->GetMainPanel();
		auto [switcher, switcherPtr] = GetPtrs(CreateComponent<TextureSwitcher>(TextureSlice::ThreeH,
			GetStateKeyMap("ScrollButton3_H"), BehaviorMode::Normal));
		UIEx(main).AttachComponent(move(switcher), { 100, 100 });
		
		bool toolMode{ true };
		UIModule* module = asComponent->GetUIModule();
		EXPECT_TRUE(module->EnableToolMode(toolMode)); //tool모드 일때에는 active update(ImplementUpdate)는 돌지 않아도 위치 업데이트(그냥 update 즉, ProcessUpdate)는 돌아야 한다.

		auto [renderTex, renderTexPtr] = GetPtrs(CreateComponent<RenderTexture>(move(asComponent)));
		EXPECT_TRUE(renderTexPtr->BindTextureSourceInfo(nullptr, m_mockRenderer->GetTextureController()));

		UIComponent* tex0 = UIEx(switcherPtr).FindComponent("PatchTextureLite1_2");
		XMINT2 leftTop = tex0->GetLeftTop();

		switcherPtr->ChangeSize({ 100, 48 }); //크기를 바꾼다. 
		renderTexPtr->EnableChildMouseEvents(!toolMode); //false로 셋팅하면 마우스가 영역에 들어왔을때 업데이트를 호출하는 코드가 돌지 않는다.
		renderTexPtr->ChangeSize({ 50, 50 });
		renderTexPtr->ChangeOrigin(Origin::Center);

		m_mockInput->SetMouseState(10, 10); //렌더텍스쳐 안에 마우스 위치시킴
		renderTexPtr->ProcessUpdate(m_timer);

		UIComponent* tex1 = UIEx(switcherPtr).FindComponent("PatchTextureLite1_2");
		XMINT2 leftTop2 = tex1->GetLeftTop();

		EXPECT_FALSE(leftTop == leftTop2);
	}

	TEST_F(UIModuleConnection, UIModuleAsComponent)
	{
		UILayout layout{ GetSizeFromRectangle(GetRectResolution()), Origin::LeftTop };
		wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
		unique_ptr<UIModule> uiModule = CreateUIModule(layout, "Main", m_mockRenderer.get(), srcBinderFilename);
		auto [asComponent, asComponentPtr] = GetPtrs(CreateComponent<UIModuleAsComponent>(move(uiModule)));

		UIModule* module = asComponent->GetUIModule();
		EXPECT_TRUE(module->EnableToolMode(true));

		UIComponent* main = module->GetMainPanel();
		auto [tex, img1Ptr] = GetPtrs(CreateComponent<PatchTextureStd1>("BackImage1"));
	}
}