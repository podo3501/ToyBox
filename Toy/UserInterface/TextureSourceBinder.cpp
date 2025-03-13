#include "pch.h"
#include "TextureSourceBinder.h"

TextureSourceBinder::~TextureSourceBinder() = default;
TextureSourceBinder::TextureSourceBinder()
{}

bool TextureSourceBinder::ChangeBindingKey(const string& bindingKey, const vector<Rectangle>& sourceAreas) noexcept
{
    if (auto it = m_bindingTable.find(bindingKey); it != m_bindingTable.end())
    {
        if (it->second != sourceAreas) return false;
        return true; 
    }

    erase_if(m_bindingTable, [&](const auto& pair) {
        return pair.second == sourceAreas;
        });

    m_bindingTable.emplace(bindingKey, sourceAreas);
    return true;
}

string TextureSourceBinder::GetBindingKey(const vector<Rectangle>& sourceAreas) const noexcept
{
    auto it = ranges::find_if(m_bindingTable, [&](const auto& pair) { return pair.second == sourceAreas; });
    return (it != m_bindingTable.end()) ? it->first : "";
}

