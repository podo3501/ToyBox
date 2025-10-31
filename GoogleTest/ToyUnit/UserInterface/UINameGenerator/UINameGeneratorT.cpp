#include "pch.h"
#include "UINameGeneratorT.h"
#include "Toy/UserInterface/UIComponent/UIType.h"
#include "Shared/Utils/StlExt.h"
#include "Shared/SerializerIO/SerializerIO.h"

namespace UserInterfaceT::UINameGeneratorT
{
	TEST_F(UINameGeneratorT, Clone)
	{
		m_nameGen->MakeRegionOf("region");
		m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1);

		unique_ptr<UINameGenerator> cloneGenerator = m_nameGen->Clone();
		EXPECT_TRUE(Compare(m_nameGen, cloneGenerator));
	}

	//?!? ���ڿ� ComponentID�� ���� �ʰ� ���������� ��ü ComponentID�� ���鼭 ã���� �� �� ����. 
	// �׷��� �� ���ڰ� ������ �⺻ �̸��� ���� ���� �˼��� ����. 
	// ��. �ٽú��ϱ� �̸��� �������� ���ϱ� ������ �Ƹ��� �ɲ� ����.
	TEST_F(UINameGeneratorT, MakeNameOf) 
	{
		//Component �̸��̶�� �̸��� ������� ���������� ��밡���� ������Ʈ �̸����� �����Ѵ�.
		EXPECT_EQ(m_nameGen->MakeNameOf("PatchTextureStd1_35", "region", ComponentID::PatchTextureStd1), 
			make_pair("region", "PatchTextureStd1"));
		EXPECT_EQ(m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1),
			make_pair("region", "name"));
		EXPECT_EQ(m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1),
			make_pair("region", "name_1")); //���� Name�϶��� _1�� �ٿ� �����Ѵ�.
	}

	TEST_F(UINameGeneratorT, MakeRegionOf)
	{
		EXPECT_EQ(m_nameGen->MakeRegionOf("region"), "region");
		EXPECT_EQ(m_nameGen->MakeRegionOf("region"), "region_1"); //���� Region�϶��� _1�� �ٿ� �����Ѵ�.
	}

	TEST_F(UINameGeneratorT, RecycleName)
	{
		m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1);
		m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1); //name_1�� ������.
		m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1); //name_2�� ������.
		m_nameGen->RemoveName("region", "name_1"); //name_1�� ����. recycle�� 1�� ��.

		EXPECT_EQ(m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1),
			make_pair("region", "name_1")); //recycle�� 1���� ���� ���.
		EXPECT_EQ(m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1),
			make_pair("region", "name_3")); //�״��� ������ name_3�� �������.
	}

	TEST_F(UINameGeneratorT, RemoveName)
	{
		m_nameGen->MakeRegionOf("region");
		m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1);

		EXPECT_TRUE(m_nameGen->RemoveName("region", "name"));
		EXPECT_TRUE(m_nameGen->IsUnusedName("region", "name"));
	}

	TEST_F(UINameGeneratorT, RemoveRegion)
	{
		//region�� �����ϸ� �ؿ� �����ִ� name�鵵 ��� �����ȴ�. �̶� ��忡 �ִ� �̸��� ����ȭ�� �ȵ�����,
		//��� Ž���ϸ鼭 �̸��� �ٽ� ����� �ָ鼭 ����ȭ�� �ȴ�. ����ȭ�� å���� node Ŭ������ �ִ�.
		//���⼭ ����ȭ�� ��Ų�ٸ� generator Ŭ������ node Ŭ������ �˾ƾ� �ϴµ�, �װ� ��ġ�� �� �´� �� ����.
		m_nameGen->MakeRegionOf("region");
		m_nameGen->MakeNameOf("name", "region", ComponentID::PatchTextureStd1);

		EXPECT_TRUE(m_nameGen->RemoveRegion("region")); //region �ؿ� ���� �ִ� name�� ��������.
		EXPECT_TRUE(m_nameGen->IsUnusedRegion("region"));
	}

	TEST_F(UINameGeneratorT, WriteAndRead)
	{
		const wstring filename = GetTempDir() + L"UINameGeneratorT_WR.json";
		EXPECT_TRUE(SerializerIO::WriteJsonToFile(m_nameGen, filename));

		unique_ptr<UINameGenerator> read;
		EXPECT_TRUE(SerializerIO::ReadJsonFromFile(filename, read));

		EXPECT_TRUE(Compare(m_nameGen, read));
	}
}