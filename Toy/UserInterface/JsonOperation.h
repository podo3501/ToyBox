#pragma once

//template �Լ��� �ϸ� read, write �Լ��� ������ ������ �Ǿ ����ϴµ� �ټ� �����ϴ�.
//�׷��� ��ӱ����� ���� Read Ŭ������ Write Ŭ������ �����ϸ� �˾Ƽ� �����ϴ� ���
//�Լ��� virtual �� �Ǳ� ������ template�� �������� type ���� ���� ����� �־�� �ϴ� ������ �ִ�.
//�� ������ �����ϱ� ���ؼ� switch�� ����� virtual�� ����� �� ������ ���� Ŭ���̾�Ʈ ��ſ�����
//uint8_t �̷� �������� ���� �� ��ȯ �ϱ� ������ ����������, Ŭ�󿡼��� ���� ����� 

class JsonOperation 
{
public:
    bool WriteFile(const wstring& filename) const noexcept;
    bool ReadFile(const wstring& filename) noexcept;

    template <typename T>
    void Process(const std::string& key, T& data) noexcept
    {
        if (m_json.contains(key)) 
        { 
            data = m_json[key].get<T>();
        }
        else 
        { 
            m_ordered_json[key] = data;
        }
    }

    void Process(const std::string& key, std::wstring& data) noexcept;

private:
    nlohmann::json m_json{};
    nlohmann::ordered_json m_ordered_json{};
};