#include "pch.h"
#include "JsonOperation.h"
#include "UIComponent.h"
#include "../Config.h"
#include "UIType.h"
#include "TransformComponent.h"
#include "UIComponent.h"
#include "Dialog.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

JsonOperation::~JsonOperation() = default;
JsonOperation::JsonOperation()
{
    auto write = std::make_unique<ordered_json>();
    m_write = make_unique<JsonNavigator<ordered_json>>(move(write));
}

bool JsonOperation::IsWrite()
{
    //읽는게 아니면 쓰는 것이다.
    if (m_read == nullptr || m_read->IsEmpty())
        return true;

    return false;
}

ordered_json& JsonOperation::GetWrite()
{
    return m_write->getCurrent();
}

json& JsonOperation::GetRead()
{
    return m_read->getCurrent();
}

bool JsonOperation::Write(const wstring& filename)
{
    ofstream file(GetResourcePath() + filename);
    if (!file.is_open())
        return false;

    auto& json = m_write->getRoot();
    file << json.dump(4);
    file.close();

    return true;
}

bool JsonOperation::Read(const wstring& filename)
{
    ifstream file(GetResourcePath() + filename);
    if (!file.is_open())
        return false;

    auto readJson = make_unique<json>(json::parse(file));
    m_read = make_unique<JsonNavigator<json>>(move(readJson));

    return true;
}

void JsonOperation::Process(const string& key, Rectangle& data) noexcept
{
	if (IsWrite())
	{
        m_write->goToKey(key);
        auto& j = m_write->getCurrent();
		j["x"] = data.x;
		j["y"] = data.y;
		j["width"] = data.width;
		j["height"] = data.height;
        m_write->goBack();
	}
	else
	{
        m_read->goToKey(key);
        const auto& j = m_read->getCurrent();
		data.x = j["x"];
		data.y = j["y"];
		data.width = j["width"];
		data.height = j["height"];
        m_read->goBack();
	}
}

void JsonOperation::Process(const string& key, Origin& data) noexcept
{
    if (IsWrite())
    {
        auto& j = m_write->getCurrent();
        j[key] = EnumToString(data);
    }
    else
    {
        const auto& j = m_read->getCurrent();
        data = StringToEnum<Origin>(j[key]);
    }
}

double RoundToSixA(double value)
{
    return std::round(value * 1e6) / 1e6;
}

void JsonOperation::Process(const string & key, Vector2& data) noexcept
{
    if (IsWrite())
    {
        ordered_json json{};

        json["x"] = RoundToSixA(data.x);
        json["y"] = RoundToSixA(data.y);

        auto& j = m_write->getCurrent();
        j[key] = json;
    }
    else
    {
        const auto& j = m_read->getCurrent();
        const auto& keyJ = j[key];
        data.x = keyJ["x"];
        data.y = keyJ["y"];
    }
}

void JsonOperation::Process(const string& key, vector<unique_ptr<TransformComponent>>& data)
{
    if (IsWrite())
    {
        if (data.empty())
            return;

        m_write->goToKey(key, true);
        for (auto& comp : data)
        {
            JsonOperation jsOp{};
            comp->SerializeIO(&jsOp);
            m_write->getCurrent().push_back(jsOp.GetWrite());
        }
        m_write->goBack();
    }
    else
    {
        const auto& j = m_read->getCurrent();
        if (j.contains(key) == false)
            return;

        data.clear();
        m_read->goToKey(key);
        for (auto& compJson : m_read->getCurrent())
        {
            m_read->setCurrent(&compJson);
            auto comp = std::make_unique<TransformComponent>();
            comp->SerializeIO(this);
            data.emplace_back(move(comp));
        }
        m_read->goBack();
    }
}

void JsonOperation::UpdateJson(const unique_ptr<UIComponent>& component, const string& type) noexcept
{
    //nlohmann::ordered_json json{};
    JsonOperation jsOp{};
    if (type == "class UIComponent") component->SerializeIO(&jsOp);
    if (type == "class Dialog") static_cast<Dialog*>(component.get())->SerializeIO(&jsOp);

    //static_cast를 해서 json으로 할당하면 json내의 타입값이 바뀌면서 새로운 타입이 되어 이전 정보 (여기서는 position)값이 사라진다. 그래서 update를 해서 병합하는 것.
    m_write->getCurrent().update(jsOp.GetWrite());
}


template <typename T>
unique_ptr<T> CreateComponent(const json& j)
{
    auto comp = std::make_unique<T>();
    *comp = j.get<T>();  // JSON을 T 타입으로 변환하여 *comp에 할당
    return comp;
}

unique_ptr<UIComponent> JsonOperation::CreateComponentFromType(const string & typeName)
{
    unique_ptr<UIComponent> comp{ nullptr };
    if (typeName == "class UIComponent") comp = std::make_unique<UIComponent>();
    if (typeName == "class Dialog") comp = make_unique<Dialog>();
    if (comp == nullptr) return nullptr;

    comp->SerializeIO(this);
    return move(comp);
}

void JsonOperation::Process(const string& key, unique_ptr<UIComponent>& component)
{
    if (IsWrite())
    {
        const string& type = component->GetType();
        Process(key, const_cast<string&>(type));
        UpdateJson(component, type);
    }
    else
    {
        string curType{};
        Process(key, curType);
        component = move(CreateComponentFromType(curType));
    }
}