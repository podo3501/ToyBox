#include "pch.h"
#include "SerializerIOT.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/Utils/StlTypeExt.hpp"
#include "Shared/Utils/StlExt.h"
#include "Toy/UserInterface/UIComponent/UIComponent.h"

class JPData
{
public:
	JPData() : position{} {}
	JPData(float x, float y) : position{ x, y } {}
	JPData(Vector2 pos) : position(pos) {}

	bool operator==(const JPData& other) const noexcept
	{
		if (position != other.position) return false;
		return true;
	}

	void ProcessIO(SerializerIO& serializer) { serializer.Process("Position", position); }

private:
	Vector2 position;
};

class UserComponent
{
public:
	virtual ~UserComponent() = default;
	UserComponent() = default;

	bool operator==(const UserComponent& other) const noexcept
	{
		if (m_layout != other.m_layout) return false;
		ReturnIfFalse(CompareAssoc(m_mapData, other.m_mapData));
		ReturnIfFalse(CompareUnorderedAssoc(m_svmapData, other.m_svmapData));
		if (m_data != other.m_data) return false;
		ReturnIfFalse(CompareSeq(m_uptrData, other.m_uptrData));

		return true;
	}

	virtual void ProcessIO(SerializerIO& serializer) = 0;

	UILayout m_layout;
	map<string, unique_ptr<JPData>> m_mapData;
	unordered_svmap<string, unique_ptr<JPData>> m_svmapData;
	vector<JPData> m_data;
	vector<unique_ptr<JPData>> m_uptrData;
};

class LayoutData : public UserComponent
{
public:
	virtual void ProcessIO(SerializerIO& serializer) override { serializer.Process("LayoutData", m_layout); }
};

class MapData : public UserComponent
{
public:
	virtual void ProcessIO(SerializerIO& serializer) override { serializer.Process("MapData", m_mapData); }
};

class SvmapData : public UserComponent
{
public:
	virtual void ProcessIO(SerializerIO& serializer) override { serializer.Process("SvmapData", m_svmapData); }
};

class VectorData : public UserComponent
{
public:
	virtual void ProcessIO(SerializerIO& serializer) override { serializer.Process("VectorData", m_data); }
};

class VectorUniqueData : public UserComponent
{
public:
	virtual void ProcessIO(SerializerIO& serializer) override { serializer.Process("VectorUniqueData", m_uptrData); }
};

template<typename T>
bool SerializerIOT::TestSerialization(T& inputData)
{
	const auto& serializeTestFilename = GetTempDir() + L"JsonSerializeTest.json";
	ReturnIfFalse(::SerializerIO::WriteJsonToFile(inputData, serializeTestFilename));

	T outputData;
	ReturnIfFalse(::SerializerIO::ReadJsonFromFile(serializeTestFilename, outputData));

	return inputData == outputData;
}

namespace D::SerializerIO
{
	TEST_F(SerializerIOT, Process_Layout)
	{
		LayoutData wData;
		wData.m_layout.Set({ 50, 100 }, Origin::Center);

		EXPECT_TRUE(TestSerialization(wData));
	}

	TEST_F(SerializerIOT, Process_MapData)
	{
		MapData wData;
		wData.m_mapData.emplace("1", make_unique<JPData>(0.4f, 0.4f));

		EXPECT_TRUE(TestSerialization(wData));
	}

	TEST_F(SerializerIOT, Process_SvmapMap)
	{
		SvmapData wData;
		wData.m_svmapData.emplace("2", make_unique<JPData>(0.5f, 0.5f));

		EXPECT_TRUE(TestSerialization(wData));
	}

	TEST_F(SerializerIOT, Process_VectorData)
	{
		VectorData wData;
		wData.m_data.emplace_back(JPData(0.1f, 0.1f));

		EXPECT_TRUE(TestSerialization(wData));
	}

	TEST_F(SerializerIOT, Process_VectorUniqueData)
	{
		VectorUniqueData wData;
		wData.m_uptrData.emplace_back(make_unique<JPData>(0.3f, 0.3f));

		EXPECT_TRUE(TestSerialization(wData));
	}
}