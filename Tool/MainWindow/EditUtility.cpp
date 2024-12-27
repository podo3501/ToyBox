#include "pch.h"
#include "EditUtility.h"
#include "../Toy/Config.h"
#include "../Toy/Utility.h"
#include "../Dialog.h"

bool EditRectangle(const char* label, Property<Rectangle>& rect)
{
    ImGui::Text("%s", label);
    ImGui::PushID(label);

    bool modify{ false };
    modify |= EditInteger("X", rect->x);
    modify |= EditInteger("Y", rect->y);
    modify |= EditInteger("Width", rect->width);
    modify |= EditInteger("Height", rect->height);

    ImGui::PopID();

    return modify;
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

    char filenameBuffer[256] = "";
    WStringToChar(filename, filenameBuffer);
    ImGui::InputText(label.c_str(), filenameBuffer, sizeof(filenameBuffer));
}