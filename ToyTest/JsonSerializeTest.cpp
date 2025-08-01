#include "pch.h"
#include "UserInterface/UIComponent/UIComponent.h"
#include "UserInterface/JsonOperation/JsonOperation.h"
#include "ToyTestFixture.h"
#include "Utils/StlTypeExt.hpp"
#include "Utils/StlUtil.h"

namespace JsonSerializeTest
{
	class Data
	{
	public:
		Data() :
			position{}
		{}

		Data(float x, float y) :
			position{ x, y }
		{}

		Data(Vector2 pos) :
			position(pos)
		{}

		bool operator==(const Data& other) const noexcept
		{
			if (position != other.position) return false;

			return true;
		}

		void SerializeIO(JsonOperation& operation)
		{
			operation.Process("Position", position);
		}

	private:
		Vector2 position;
	};

	class UserComponent
	{
	public:
		UserComponent(bool insertData)
		{
			if (!insertData) return;

			m_data.emplace_back(Data(0.1f, 0.1f));
			m_uptrData.emplace_back(make_unique<Data>(0.3f, 0.3f));
			m_mapData.emplace("1", make_unique<Data>(0.4f, 0.4f));
			m_svmapData.emplace("2", make_unique<Data>(0.5f, 0.5f));

			m_layout.Set({ 50, 100 }, Origin::Center);
		}

		bool operator==(const UserComponent& other) const noexcept
		{
			if (m_data != other.m_data) return false;
			if (m_layout != other.m_layout) return false;
			ReturnIfFalse(CompareSeq(m_uptrData, other.m_uptrData));
			ReturnIfFalse(CompareAssoc(m_mapData, other.m_mapData));
			ReturnIfFalse(CompareUnorderedAssoc(m_svmapData, other.m_svmapData));
			
			return true;
		}

		void SerializeIO(JsonOperation& operation)
		{
			operation.Process("Data", m_data);
			operation.Process("UPtrData", m_uptrData);
			operation.Process("MapData", m_mapData);
			operation.Process("SVMapData", m_svmapData);
			operation.Process("Layout", m_layout);
		}

	private:
		vector<Data> m_data;
		vector<unique_ptr<Data>> m_uptrData;
		map<string, unique_ptr<Data>> m_mapData;
		unordered_svmap<string, unique_ptr<Data>> m_svmapData;
		UILayout m_layout;
	};

	//컨테이너 안에 값이 클래스 또는 스트럭쳐 일 경우 Serialize 테스트
	TEST_F(ToyTestFixture, TestClassContainer)	
	{
		UserComponent wData(true);
		const auto& serializeTestFilename = L"Test/Data/JsonSerializeTest.json";
		EXPECT_TRUE(JsonOperation::WriteJsonToFile(wData, serializeTestFilename));

		UserComponent rData(false);
		EXPECT_TRUE(JsonOperation::ReadJsonFromFile(serializeTestFilename, rData));

		EXPECT_TRUE(wData == rData);
	}
}
