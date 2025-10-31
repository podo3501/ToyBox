#include "pch.h"
#include "ListAreaT.h"
#include "UserInterface/UIComponent/Components/ComponentHelper.h"
#include "Toy/UserInterface/UIComponent/Components/PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "Toy/UserInterface/UIComponent/Components/TextureSwitcher.h"
#include "Shared/Utils/GeometryExt.h"

void ListAreaT::MakeTestPrototype()
{
	auto prototype = m_component->GetPrototypeContainer();
	auto texture = CreateComponent<PatchTextureStd1>("One32");
	UIEx(texture).Rename("Test");
	prototype->AttachComponentToCenter(move(texture), {});
}

void ListAreaT::MakeTestData(int itemCount)
{
	MakeTestPrototype();

	for (auto idx : views::iota(0, itemCount))
		m_component->PrepareContainer();
}

namespace UserInterfaceT::UIComponentT::ComponentT
{
	TEST_F(ListAreaT, ClearContainers)
	{
		MakeTestData(3);
		m_component->ClearContainers();

		EXPECT_EQ(m_component->GetContainerCount(), 0);
	}

	TEST_F(ListAreaT, Clone)
	{
		EXPECT_TRUE(TestClone(m_component));
	}

	TEST_F(ListAreaT, GetContainer)
	{
		MakeTestData(1);
		auto container = m_component->GetContainer(0);

		EXPECT_TRUE(UIEx(container).FindComponent<PatchTextureStd1*>("Test"));
	}

	TEST_F(ListAreaT, GetContainerCount)
	{
		MakeTestData(3);

		EXPECT_EQ(m_component->GetContainerCount(), 3);
	}

	TEST_F(ListAreaT, GetSize)
	{
		EXPECT_TRUE((m_component->GetSize() == XMUINT2{ 48, 48 })); //�ڽĵ��� ũ�⿡�� �ʱⰪ�� �����´�.
	}

	TEST_F(ListAreaT, PrepareContainer) //����Ʈ�� ������Ʈ �߰��ؼ� �� �ִ� ���
	{
		MakeTestPrototype(); // ���⿡ �⺻�� �Ǵ� ���� �����.

		auto container = m_component->PrepareContainer(); // �����̳� �ϳ��� �غ��ϸ� ������Ÿ�԰� ���� ���� �غ�ȴ�.
		EXPECT_TRUE(UIEx(container).FindComponent<PatchTextureStd1*>("Test"));
	}

	TEST_F(ListAreaT, RemoveContainer)
	{
		MakeTestData(2);
		m_component->RemoveContainer(0);

		EXPECT_EQ(m_component->GetContainerCount(), 1);
	}

	TEST_F(ListAreaT, ProcessRender)
	{
		m_component->ChangeSize({ 150, 50 });
		MakeTestData(3); //��ũ�� �ٰ� ��Ÿ����.

		size_t texIndex = 1; //���� ������ �ؽ��Ķ� �ε����� ������.
		//ȭ�鿡 �׷����� �κ�
		RECT destRenderTexture{ 0, 0, 150, 50 };
		RECT destTrackBottom{ 128, 43,148, 48 };
		RECT destButtonBottom{ 128,13,148,17 };

		//�ؽ��Ŀ��� �������� ����
		RECT srcRenderTexture{ 0, 0, 150, 50 };
		RECT srcTrackBottom{ 0, 45, 20, 50 };
		RECT srcButtonBottom{ 0, 16, 20, 20 };

		//�� 7���� Render ȣ�������� RenderTexture�� ScrollBar�� ������ Bottom 2���ؼ� 3���� �˻��Ѵ�.
		//������ �κ��� �ؽ��Ŀ� �������� �ؽ��ĸ� ����ϱ� ������ ���θ��� �ؽ��� ���常 ������.
		MockTextureRender render;
		EXPECT_CALL(render, Render).Times(::testing::AnyNumber());
		::testing::InSequence seq;
		EXPECT_CALL(render, Render(texIndex, destRenderTexture, ::testing::Pointee(srcRenderTexture))).Times(1);
		EXPECT_CALL(render, Render(0, destTrackBottom, ::testing::Pointee(srcTrackBottom))).Times(1);
		EXPECT_CALL(render, Render(0, destButtonBottom, ::testing::Pointee(srcButtonBottom))).Times(1);

		m_component->UpdatePositionsManually();
		m_component->ProcessRender(&render);
	}
	
	TEST_F(ListAreaT, WriteAndRead)
	{
		EXPECT_TRUE(TestWriteAndRead(m_component, GetTempDir() + L"ListAreaT_WR.json",
			GetResBinder(), GetTextureController()));
	}
}