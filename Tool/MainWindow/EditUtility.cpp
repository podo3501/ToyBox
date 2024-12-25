#include "pch.h"
#include "EditUtility.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Dialog.h"

void EditRectangle(const char* label, Property<Rectangle>& rect)
{
    ImGui::Text("%s", label);
    ImGui::PushID(label);

    EditInteger("X", rect->x);
    EditInteger("Y", rect->y);
    EditInteger("Width", rect->width);
    EditInteger("Height", rect->height);

    ImGui::PopID();
}

void EditFilename(const string& label, Property<wstring>& filename)
{
    if (ImGui::Button("Change Filename"))
    {
        wstring selectedFilename{};
        if (!Tool::Dialog::ShowFileDialog(selectedFilename, FileDialogType::Open))
            return;

        if (selectedFilename.empty())
            return;

        const wstring& relativePath = GetRelativePath(selectedFilename);
        filename = relativePath;
    }

    char filenameBuffer[256] = ""; // 파일 이름을 저장할 버퍼
    WStringToChar(filename, filenameBuffer);
    ImGui::InputText(label.c_str(), filenameBuffer, sizeof(filenameBuffer));
}