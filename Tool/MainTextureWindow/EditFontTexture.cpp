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

//?!? Ű�� �ִµ� ��Ʈ������ Ű�� �������� �ʱ� ������ �浹�� ���ų� ���ʿ��� Ű�� �ö� ���� �� �ִ�.
//Ű�� �� �����ִ� ����� �ʿ�
void EditFontTexture::Render()
{
    vector<string> strFileList;
    ranges::transform(m_fontFiles, back_inserter(strFileList), WStringToString);
    vector<const char*> fontFiles; //const char*�� ���� �����ϴ°� �ƴ϶� �����͸� �����ϱ� ������ strFileList�� �������� ���ڰ� ������.
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