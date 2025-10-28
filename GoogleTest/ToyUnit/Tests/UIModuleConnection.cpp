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
	//RenderTexture + UIModuleAsComponent => ������ ���� ���
	//UIModule + RenderTexture + UIModuleAsComponent => Ŭ���̾�Ʈ���� ���� ���
	//��������� RenderTexture + UIModuleAsComponent�� ���� ������ �ؾ� �ؼ� ������ ���� ��İ� Ŭ���̾�Ʈ���� ���� ����� �ٸ��� �ʴ�.
	//�׷��� �ٸ��� �ʵ��� �ڵ��� �ؾ� �Ѵ�.
	//2���� �׽�Ʈ�� ���ؼ� ������Ʈ �� Render�� �� �Ǵ��� Ȯ���Ѵ�.
	//������ �������� AddRender�� ���� �ʾƾ� �Ѵ�. UIModule�� UIModuleAsComponent�� ������ Remove�� �ؼ� Render���� �����ϸ� �� �� ����.
	//ImplementRender�� ImplementUpdate	�� mock���� ó��.
	//��ġ���� ���� ��ġ���� ���� ��ġ���� ���ϴ� ������ ó��.
	TEST_F(UIModuleConnection, AttachUIModuleToRenderTexture)
	{
		//Imgui���� RenderTexture�� texture�� �ޱ� ������ ���� Render�� ������� �ʴ´�. �׷��� 0��.
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
		EXPECT_TRUE(module->EnableToolMode(toolMode)); //tool��� �϶����� active update(ImplementUpdate)�� ���� �ʾƵ� ��ġ ������Ʈ(�׳� update ��, ProcessUpdate)�� ���ƾ� �Ѵ�.

		auto [renderTex, renderTexPtr] = GetPtrs(CreateComponent<RenderTexture>(move(asComponent)));
		EXPECT_TRUE(renderTexPtr->BindTextureSourceInfo(nullptr, m_mockRenderer->GetTextureController()));

		UIComponent* tex0 = UIEx(switcherPtr).FindComponent("PatchTextureLite1_2");
		XMINT2 leftTop = tex0->GetLeftTop();

		switcherPtr->ChangeSize({ 100, 48 }); //ũ�⸦ �ٲ۴�. 
		renderTexPtr->EnableChildMouseEvents(!toolMode); //false�� �����ϸ� ���콺�� ������ �������� ������Ʈ�� ȣ���ϴ� �ڵ尡 ���� �ʴ´�.
		renderTexPtr->ChangeSize({ 50, 50 });
		renderTexPtr->ChangeOrigin(Origin::Center);

		m_mockInput->SetMouseState(10, 10); //�����ؽ��� �ȿ� ���콺 ��ġ��Ŵ
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