#include "pch.h"
#include "../Toy/UserInterface/JsonHelper.h"
#include "../Toy/UserInterface/UIComponent.h"
#include "../Toy/UserInterface/UIType.h"
#include "ToyTestFixture.h"

namespace JsonSerializeTest
{
	class Data
	{
	public:
		Data() :
			position{}
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

			Data data1({ 0.1f, 0.1f });
			Data data2({ 0.2f, 0.2f });
			m_data.emplace_back(data1);
			m_data.emplace_back(data2);

			m_layout.Set({ 50, 100 }, Origin::Center);
		}

		bool operator==(const UserComponent& other) const noexcept
		{
			if (m_data != other.m_data) return false;
			if (m_layout != other.m_layout) return false;

			return true;
		}

		void SerializeIO(JsonOperation& operation)
		{
			operation.Process("Data", m_data);
			operation.Process("Layout", m_layout);
		}

	private:
		vector<Data> m_data;
		UILayout m_layout;
	};

	//컨테이너 안에 값이 클래스 또는 스트럭쳐 일 경우 Serialize 테스트
	TEST_F(ToyTestFixture, TestClassContainer)	
	{
		UserComponent wData(true);
		JsonFile::Write(wData, L"UI/Data/JsonSerializeTest.json");

		UserComponent rData(false);
		JsonFile::Read(L"UI/Data/JsonSerializeTest.json", rData);

		EXPECT_TRUE(wData == rData);
	}
}
