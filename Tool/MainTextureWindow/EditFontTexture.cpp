#include "pch.h"
#include "EditFontTexture.h"
#include "../Toy/UserInterface/TextureSourceBinder/TextureSourceBinder.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../HelperClass.h"

EditFontTexture::~EditFontTexture() = default;
EditFontTexture::EditFontTexture() :
    m_sourceBinder{ nullptr },
    m_renameNotifier{ make_unique<RenameNotifier>() }
{}

static vector<wstring> GetSpriteFontFiles(const wstring& folderPath)
{
    vector<wstring> spriteFontFiles;

    for (const auto& entry : filesystem::directory_iterator(folderPath))
        if (entry.is_regular_file() && entry.path().extension() == L".spritefont")
            spriteFontFiles.emplace_back(entry.path().filename().wstring());

    return spriteFontFiles;
}

void EditFontTexture::Update() noexcept
{
    if(m_fontFiles.empty())
        m_fontFiles = GetSpriteFontFiles(GetResourcePath() + GetResourceFontPath());
}

wstring EditFontTexture::GetSelectFontFile() const noexcept
{
    if (!IsVaildFontIndex()) return L"";
    return GetResourceFontPath() + m_fontFiles[m_fontIndex];
}

//?!? 키가 있는데 폰트에서는 키를 보여주지 않기 때문에 충돌이 나거나 불필요한 키가 올라가 있을 수 있다.
//키를 다 지워주는 기능이 필요
void EditFontTexture::Render()
{
    vector<string> strFileList;
    ranges::transform(m_fontFiles, back_inserter(strFileList), WStringToString);
    vector<const char*> fontFiles; //const char*은 값을 저장하는게 아니라 포인터를 저장하기 때문에 strFileList가 없어지면 글자가 깨진다.
    for (auto& str : strFileList)
        fontFiles.emplace_back(str.c_str());
    ImGui::ListBox("Font List", &m_fontIndex, fontFiles.data(), static_cast<int>(fontFiles.size()), 4);

    const wstring& fontFilename = GetSelectFontFile();
    const wstring& bindingKey = m_sourceBinder->GetBindingKey(fontFilename);
    m_renameNotifier->EditName("Font Bind Key", WStringToString(bindingKey), [this, &bindingKey, &fontFilename](const string& newKey) {
        wstring wstrNewKey = StringToWString(newKey);
        if (bindingKey.empty()) return m_sourceBinder->InsertBindingKey(wstrNewKey, TextureFontInfo{ fontFilename });
        if (wstrNewKey.empty()) return m_sourceBinder->RemoveBindingKey(fontFilename);
        return m_sourceBinder->ModifyBindingKey(bindingKey, wstrNewKey);
        });
}

bool EditFontTexture::SetSourceBinder(TextureSourceBinder* sourceBinder) noexcept
{
	m_sourceBinder = sourceBinder;
    return true;
}