#include "pch.h"
#include "UINameGeneratorT.h"
#include "Shared/Utils/StlExt.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/SerializerIO/Storage/JsonStorageLocator.h"
#include "Toy/UserInterface/UIComponent/UIType.h"

namespace UserInterfaceT::UINameGeneratorT
{
	TEST_F(UINameGeneratorT, Clone)
	{
		m_nameGen->MakeRegionOf("region");
		m_nameGen->MakeNameOf("region", "name");

		auto clone = m_nameGen->Clone();
		EXPECT_TRUE(Compare(m_nameGen, clone));
	}

	TEST_F(UINameGeneratorT, MakeNameOf) 
	{
		//Component 이름이라면 이름과 상관없이 내부적으로 사용가능한 컴포넌트 이름으로 리턴한다.
		EXPECT_EQ(m_nameGen->MakeNameOf("region", "PatchTextureStd1_35"), make_pair("region", "PatchTextureStd1"));
		EXPECT_EQ(m_nameGen->MakeNameOf("region", "name"), make_pair("region", "name"));
		EXPECT_EQ(m_nameGen->MakeNameOf("region", "name"), make_pair("region", "name_1")); //같은 Name일때는 _1을 붙여 리턴한다.
	}

	TEST_F(UINameGeneratorT, MakeNameOf_EmptyRegion)
	{
		EXPECT_EQ(m_nameGen->MakeNameOf("", "name"), make_pair("", "name"));
		EXPECT_EQ(m_nameGen->MakeNameOf("", "name"), make_pair("", "name_1"));
	}

	TEST_F(UINameGeneratorT, MakeRegionOf)
	{
		EXPECT_EQ(m_nameGen->MakeRegionOf("region"), "region");
		EXPECT_EQ(m_nameGen->MakeRegionOf("region"), "region_1"); //같은 Region일때는 _1을 붙여 리턴한다.
	}

	TEST_F(UINameGeneratorT, MakeRegionOf_EmptyRegionName)
	{
		EXPECT_EQ(m_nameGen->MakeRegionOf(""), string(""));
		EXPECT_FALSE(m_nameGen->MakeRegionOf("")); //region이 ""인 경우는 "_1" 같은것이 생길 수 없으므로 ""를 만들었는데 또 만들라고 그러면 false이다.
	}

	TEST_F(UINameGeneratorT, RecycleName)
	{
		m_nameGen->MakeNameOf("region", "name");
		m_nameGen->MakeNameOf("region", "name"); //name_1로 생성됨.
		m_nameGen->MakeNameOf("region", "name"); //name_2로 생성됨.
		m_nameGen->RemoveName("region", "name_1"); //name_1을 지움. recycle에 1이 들어감.

		EXPECT_EQ(m_nameGen->MakeNameOf("region", "name"), make_pair("region", "name_1")); //recycle된 1값을 먼저 사용.
		EXPECT_EQ(m_nameGen->MakeNameOf("region", "name"), make_pair("region", "name_3")); //그다음 순번인 name_3을 만들어줌.
	}

	TEST_F(UINameGeneratorT, RemoveName)
	{
		m_nameGen->MakeRegionOf("region");
		m_nameGen->MakeNameOf("region", "name");

		EXPECT_TRUE(m_nameGen->RemoveName("region", "name"));
		EXPECT_TRUE(m_nameGen->IsUnusedName("region", "name"));
	}

	TEST_F(UINameGeneratorT, RemoveRegion)
	{
		//region을 삭제하면 밑에 딸려있는 name들도 모두 삭제된다. 이때 노드에 있는 이름과 동기화가 안되지만,
		//노드 탐색하면서 이름을 다시 만들어 주면서 동기화가 된다. 동기화의 책임은 node 클래스에 있다.
		//여기서 동기화를 시킨다면 generator 클래스가 node 클래스를 알아야 하는데, 그건 이치에 안 맞는 것 같다.
		m_nameGen->MakeRegionOf("region");
		m_nameGen->MakeNameOf("region", "name");

		EXPECT_TRUE(m_nameGen->RemoveRegion("region")); //region 밑에 딸려 있던 name도 지워진다.
		EXPECT_TRUE(m_nameGen->IsUnusedRegion("region"));
	}

	TEST_F(UINameGeneratorT, WriteAndRead)
	{
		auto storage = InitializeJsonStorage(StorageType::Memory);
		const wstring filename = GetTempDir() + L"UINameGeneratorT_WR.json";
		EXPECT_TRUE(SerializerIO::WriteJsonToFile(m_nameGen, filename));

		unique_ptr<UINameGenerator> read;
		EXPECT_TRUE(SerializerIO::ReadJsonFromFile(filename, read));

		EXPECT_TRUE(Compare(m_nameGen, read));
	}
}