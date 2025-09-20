#include "pch.h"
#include "GameConfig.h"
#include "UserInterface/UIComponent/UIUtility.h"
#include "UserInterface/UIComponent/UIComponent.h"
#include "UserInterface/JsonOperation/JsonOperation.h"
#include "Utils/StlTypeExt.hpp"
#include "Utils/StlUtil.h"

namespace b_CodeLab
{
	class JPData
	{
	public:
		JPData() :
			position{}
		{}

		JPData(float x, float y) :
			position{ x, y }
		{}

		JPData(Vector2 pos) :
			position(pos)
		{}

		bool operator==(const JPData& other) const noexcept
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

			m_data.emplace_back(JPData(0.1f, 0.1f));
			m_uptrData.emplace_back(make_unique<JPData>(0.3f, 0.3f));
			m_mapData.emplace("1", make_unique<JPData>(0.4f, 0.4f));
			m_svmapData.emplace("2", make_unique<JPData>(0.5f, 0.5f));

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
		vector<JPData> m_data;
		vector<unique_ptr<JPData>> m_uptrData;
		map<string, unique_ptr<JPData>> m_mapData;
		unordered_svmap<string, unique_ptr<JPData>> m_svmapData;
		UILayout m_layout;
	};

	//컨테이너 안에 값이 클래스 또는 스트럭쳐 일 경우 Serialize 테스트
	TEST(ClientUtility, JsonParser)
	{
		InitializeConfig(L"Resources/", { 800.f, 600.f });

		UserComponent wData(true);
		const auto& serializeTestFilename = L"Test/Data/JsonSerializeTest.json";
		EXPECT_TRUE(JsonOperation::WriteJsonToFile(wData, serializeTestFilename));

		UserComponent rData(false);
		EXPECT_TRUE(JsonOperation::ReadJsonFromFile(serializeTestFilename, rData));

		EXPECT_TRUE(wData == rData);
	}

	TEST(ClientUtility, TextAreaParser)
	{
		TextProperty textProperty;
		auto result = Parser(
			L"<Hangle><Red>테스트, </Red>!@#$%</Hangle><English>Test. ^<Blue>&*</Blue>()</English>",
			textProperty);
		EXPECT_TRUE(result);
	}
}
