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

bool EditText(const string& label, Property<string>& text)
{
    char editBuffer[256] = "";
    StringToChar(text, editBuffer);
    if (ImGui::InputText(label.c_str(), editBuffer, IM_ARRAYSIZE(editBuffer)))
    {
        text = editBuffer;
        return true;
    }

    return false;
}

bool EditText(const string& label, const wstring& text)
{
    char editBuffer[256] = "";
    WStringToChar(text, editBuffer);
    if (ImGui::InputText(label.c_str(), editBuffer, IM_ARRAYSIZE(editBuffer)))
        return true;

    return false;
}

bool EditFilename(const string& label, Property<wstring>& filename)
{
    wstring editFilename{ filename };
    if (ImGui::Button("Change Filename"))
    {
        wstring selectedFilename{};
        if (!Tool::Dialog::ShowFileDialog(selectedFilename, FileDialogType::Open))
            return false;

        if (selectedFilename.empty())
            return false;

        const wstring& relativePath = GetRelativePath(selectedFilename);
        editFilename = relativePath;
    }

    EditText(label, editFilename);

    if (editFilename != filename)
    {
        filename = move(editFilename);
        return true;
    }

    return false;
}