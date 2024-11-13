#pragma once

//template 함수로 하면 read, write 함수가 쌍으로 나오게 되어서 사용하는데 다소 불편하다.
//그래서 상속구조로 가면 Read 클래스와 Write 클래스를 생성하면 알아서 저장하는 방식
//함수가 virtual 이 되기 때문에 template를 쓸수없어 type 마다 전부 만들어 주어야 하는 단점이 있다.
//그 단점을 제거하기 위해서 switch를 만들어 virtual을 대신할 수 있으나 서버 클라이언트 통신에서는
//uint8_t 이런 형식으로 전부 형 변환 하기 때문에 가능하지만, 클라에서는 딱히 방법이 

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