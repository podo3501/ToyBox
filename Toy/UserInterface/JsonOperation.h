#pragma once

enum class Origin;

class JsonOperation 
{
public:
    JsonOperation();
    bool WriteFile(const wstring& filename) const noexcept;
    bool ReadFile(const wstring& filename) noexcept;

    void Push(const string& type) noexcept;
    void Pop() noexcept;

    template <typename T>
    void Process(const string& key, T& data) noexcept
    {
        if (m_json.empty())
        {
            auto& json = GetWriteJson();
            json[key] = data;
        }
        else
            data = m_json[key].get<T>();
    }

    void Process(const string& key, wstring& data) noexcept;
    void Process(const string& key, map<wstring, wstring>& data) noexcept;
    void Process(const string& key, Rectangle& data) noexcept;
    void Process(const string& key, Origin& data) noexcept;

private:
    nlohmann::ordered_json& GetWriteJson();

    nlohmann::json m_json{};
    nlohmann::ordered_json m_ordered_json{};

    stack<nlohmann::ordered_json*> m_write;
};