#include "pch.h"
#include "EditFontTexture.h"
#include "../Toy/UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "../Toy/UserInterface/Command/TexResCommandList/TexResCommandList.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../HelperClass.h"

EditFontTexture::~EditFontTexture() = default;
EditFontTexture::EditFontTexture() :
    m_resBinder{ nullptr },
    m_cmdList{ nullptr },
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
    const wstring& bindingKey = m_resBinder->GetFontKey(fontFilename);
    m_renameNotifier->EditName("Font Bind Key", WStringToString(bindingKey), [this, &bindingKey, &fontFilename](const string& newKey) {
        wstring wstrNewKey = StringToWString(newKey);
        if (bindingKey.empty()) return m_cmdList->AddFontKey(m_resBinder, wstrNewKey, TextureFontInfo{ fontFilename });
        if (wstrNewKey.empty()) return m_resBinder->RemoveKeyByFilename(fontFilename);
        return m_resBinder->RenameFontKey(bindingKey, wstrNewKey);
        });
}

bool EditFontTexture::SetBinderAndCmdList(TextureResourceBinder* resBinder, TexResCommandList* cmdList) noexcept
{
	m_resBinder = resBinder;
    m_cmdList = cmdList;
    return true;
}