#include "pch.h"
#include "EditFontTexture.h"
#include "Shared/Utils/StringExt.h"
#include "Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "Toy/UserInterface/CommandHistory/TextureResource/TexResCommandHistory.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "../HelperClass.h"
#include "Window/Utils/EditUtility.h"

EditFontTexture::~EditFontTexture() = default;
EditFontTexture::EditFontTexture() :
    m_cmdHistory{ nullptr },
    m_listboxFont{ make_unique<EditListBox>("Font Bind Key", 4) },
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

static vector<string> GetFontFiles(const auto& fontFiles) noexcept
{
    vector<string> strFileList;
    ranges::transform(fontFiles, back_inserter(strFileList), WStringToString);
    return strFileList;
}

void EditFontTexture::Update() noexcept
{
    if (m_fontFiles.empty())
    {
        m_fontFiles = GetSpriteFontFiles(GetResourcePath() + GetResourceFontPath());
        m_listboxFont->SetItems(GetFontFiles(m_fontFiles));
    }
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
    m_listboxFont->Render([this](int index) { m_fontIndex = index; });

    auto binder = m_cmdHistory->GetReceiver();
    const wstring& fontFilename = GetSelectFontFile();
    const wstring& bindingKey = binder->GetFontKey(fontFilename);
    m_renameNotifier->EditName("Font Bind Key", WStringToString(bindingKey), [this, &bindingKey, &fontFilename](const string& newKey) {
        wstring wstrNewKey = StringToWString(newKey);
        if (bindingKey.empty()) return m_cmdHistory->AddFontKey(wstrNewKey, TextureFontInfo{ fontFilename });
        if (wstrNewKey.empty()) return m_cmdHistory->RemoveKeyByFilename(fontFilename);
        return m_cmdHistory->RenameFontKey(bindingKey, wstrNewKey);
        });
}