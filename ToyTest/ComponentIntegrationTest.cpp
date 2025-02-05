#include "pch.h"
#include "ToyTestFixture.h"
#include "IMockRenderer.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/Panel.h"
#include "../Toy/UserInterface/Component/SampleComponent.h"

namespace ComponentTest
{
	static bool AttachComponentHelper(UIComponent* panel, const string& componentName) noexcept
	{
		auto imgGrid1 = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		UIComponent* component = UIEx(panel).GetComponent(componentName);
		return UIEx(component).AttachComponent(move(imgGrid1), { 10, 10 }) ? false : true;
	}

	static bool DetachComponentHelper(UIComponent* panel, const string& componentName) noexcept 
	{
		UIComponent* component = UIEx(panel).GetComponent(componentName);
		auto [detach, parent] = UIEx(component).DetachComponent();
		return detach != nullptr;
	}

	TEST_F(IntegrationTest, AttachDetachTest)
	{
		unique_ptr<UIComponent> img9 = CreateSampleImageGrid9({ { 200, 100 }, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img9), { 80, 60 });

		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid9_0"), false);	//9���� �̹������� attach �Ұ�
		EXPECT_EQ(AttachComponentHelper(m_panel.get(), "ImageGrid1_0"), true);

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_0"), false);
		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid1_9"), true); //������ ImgGrid1�� attach �ߴ�.

		EXPECT_EQ(DetachComponentHelper(m_panel.get(), "ImageGrid9_0"), true); //������ ImgGrid1�� attach �ߴ�.

		auto img1 = CreateSampleImageGrid1({ { 200, 100 }, Origin::LeftTop });
		auto img2 = CreateSampleImageGrid1({ { 110, 60 }, Origin::LeftTop });
		auto img1Ptr = img1.get();
		UIEx(img1).AttachComponent(move(img2), { 100, 50 });	//������ attach �Ѵ�.
		UIEx(m_panel).AttachComponent(move(img1), { 100, 100 });
		m_panel->ProcessUpdate({}, true);

		EXPECT_EQ(UIEx(img1Ptr).GetTotalChildSize(), XMUINT2(210, 110));
		auto [detached, parent] = UIEx(img1Ptr).DetachComponent();
		EXPECT_EQ(UIEx(detached).GetTotalChildSize(), XMUINT2(210, 110));
	}

	template <typename T>
	bool VerifyClone(unique_ptr<T> original) 
	{
		if (!original) return false;
		auto clone = original->Clone();

		return CompareUniquePtr(original, clone);
	}

	TEST_F(IntegrationTest, Clone)
	{
		EXPECT_TRUE(VerifyClone(CreateSampleImageGrid1({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(CreateSampleImageGrid3({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(CreateSampleTextArea({ { 220, 190 }, Origin::LeftTop }, L"L< Hangle >�׽�Ʈ �Դϴ�!< / Hangle>")));
		EXPECT_TRUE(VerifyClone(CreateSampleButton1({ { 220, 190 }, Origin::LeftTop })));
		EXPECT_TRUE(VerifyClone(CreateSampleButton3({ { 220, 190 }, Origin::LeftTop })));
	}

	static size_t CheckComponentCount(UIComponent* panel, const XMINT2& position)
	{
		vector<UIComponent*> components = UIEx(panel).GetComponents(position);
		return components.size();
	}

	TEST_F(IntegrationTest, GetComponent)
	{
		unique_ptr<UIComponent> img2 = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		unique_ptr<UIComponent> img1 = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		auto img2Ptr = img2.get();
		auto img1Ptr = img1.get();
		UIEx(img1).AttachComponent(move(img2), { 100, 100 });
		UIEx(m_panel).AttachComponent(move(img1), { 100, 100 });

		img1Ptr->Rename("image1"); img1Ptr->SetBRegion(true);
		img2Ptr->Rename("image2");
		
		EXPECT_FALSE(UIEx(m_panel).GetComponent("image1")); //Img1�� �ٸ� Region�̶� ã�� �� ����.
		EXPECT_FALSE(UIEx(m_panel).GetComponent("image2"));
		EXPECT_FALSE(UIEx(img1Ptr).GetComponent("Main"));//Img1�� Region�̶� ���� ���� �� ã�´�.
		EXPECT_TRUE(UIEx(img1Ptr).GetComponent("image2"));
		EXPECT_FALSE(UIEx(img2Ptr).GetComponent("Main"));
		EXPECT_TRUE(UIEx(img2Ptr).GetComponent("image1"));
	}

	TEST_F(IntegrationTest, GetComponents)
	{
		unique_ptr<UIComponent> img9_0 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img9_0), { 80, 60 });
		m_panel->ProcessUpdate({}, true);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), {0, 0}) == 1);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 100, 100 }) == 4);

		unique_ptr<UIComponent> img9_1 = CreateSampleImageGrid9({ { 221, 191 }, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img9_1), { 88, 66 });
		m_panel->ProcessUpdate({}, true);
		EXPECT_TRUE(CheckComponentCount(m_panel.get(), { 180, 160 }) == 7);
	}

	TEST_F(IntegrationTest, GetPosition)
	{
		unique_ptr<UIComponent> img9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		unique_ptr<UIComponent> panel = make_unique<Panel>();
		panel->SetLayout({ { 400, 300 }, Origin::Center });
		UIEx(panel).AttachComponent(move(img9), { 40, 30 });
		UIEx(m_panel).AttachComponent(move(panel), { 400, 300 });
		m_panel->ProcessUpdate({}, true);

		UIComponent* component = UIEx(m_panel).GetComponent("ImageGrid1_4");
		XMINT2 pos = component->GetPosition();
		EXPECT_EQ(pos, XMINT2(270, 216));
		EXPECT_EQ(component->GetRectangle(), Rectangle(270, 216, 160, 128));
	}

	//�̸��� ������ ���� �ٸ� �����̸� ���� �̸��� �� �� �ְ� �Ѵ�. �׷��� close ���� �̸��� �ߺ��� �Ǿ
	//ī�� ������ �ٸ� �����̶�� close �̸��� �� �� �ִ�. ����Ʈ ������Ʈ���� �ٸ� �������� �����ϸ� �̸��� 
	//���Ƶ� �Ǵϱ� �ڵ��Ҷ� �̸�_1 �̷����� ã�� �ʾƵ� �ȴ�.
	TEST_F(IntegrationTest, Region)
	{
		unique_ptr<UIComponent> img1 = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		unique_ptr<UIComponent> img2 = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });

		auto img1Ptr = img1.get();
		auto img2Ptr = img2.get();

		UIEx(m_panel).AttachComponent(move(img1), { 100, 100 });
		UIEx(m_panel).AttachComponent(move(img2), { 100, 100 });

		img1Ptr->SetBRegion(true);
		img2Ptr->SetBRegion(true);

		//Region�� true ������ �ڽ��� ���� Region�� ���Ѵ�.
		//�ڽ��� �ٸ� Region�� ���ϸ� Region::Region �̷� �͵��� �����ؾ� �Ѵ�. ��ſ� �ܰ������θ� ������ �����ϰ�
		//���� �����ؾ� �Ѵٸ� Region�� �����ϰ� ã�� ����� ��� �Ѵ�. �׷����� �� ������Ʈ�� �̸��� ����ũ �ؾ� �Ѵ�.
		EXPECT_EQ(img1Ptr->GetName(), "ImageGrid1_0");
		EXPECT_EQ(img2Ptr->GetName(), "ImageGrid1_1");

		//unique_ptr<UIComponent> img3 = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		//img3->Rename("UnChanging Name");
		//auto img4 = img3->Clone();

		//auto img3Ptr = img3.get();
		//auto img4Ptr = img4.get();

		//UIEx(img1Ptr).AttachComponent(move(img3), { 100, 100 });
		//UIEx(img2Ptr).AttachComponent(move(img4), { 100, 100 });

		//EXPECT_EQ(img3Ptr->GetName(), "UnChanging Name");
		//EXPECT_EQ(img4Ptr->GetName(), "UnChanging Name");

		//unique_ptr<UIComponent> imgDummy = CreateSampleImageGrid1({ { 64, 64 }, Origin::LeftTop });
		//auto imgDummyPtr = imgDummy.get();
		//UIEx(m_panel).AttachComponent(move(imgDummy), { 100, 100 });
		//EXPECT_TRUE(imgDummyPtr->Rename("UnChanging Name"));

		//auto img5 = img1Ptr->Clone();
		//auto img5Ptr = img5.get();
		//UIEx(m_panel).AttachComponent(move(img5), { 100, 100 });
		//EXPECT_TRUE(UIEx(img5Ptr).GetComponent("UnChanging Name"));
	}

	TEST_F(IntegrationTest, Rename)
	{
		unique_ptr<UIComponent> img9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		UIEx(m_panel).AttachComponent(move(img9), { 80, 60 });

		UIComponent* component = UIEx(m_panel).GetComponent("ImageGrid1_0");
		EXPECT_FALSE(component->Rename("ImageGrid9_0")); //���� �̸��� ������ rename�� ���� �ʴ´�.

		unique_ptr<UIComponent> newImg9 = CreateSampleImageGrid9({ { 220, 190 }, Origin::LeftTop });
		auto failed = UIEx(m_panel).AttachComponent(move(newImg9), { 80, 60 }); //���� ������Ʈ�� attach�ϸ� ���������� �̸��� ������ �ش�.
		EXPECT_TRUE(failed == nullptr);
	}
}