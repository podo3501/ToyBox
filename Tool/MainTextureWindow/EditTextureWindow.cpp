#include "pch.h"
#include "EditTextureWindow.h"
#include "MainTextureWindow.h"
#include "../MainWindow/EditUtility.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Dialog.h"
#include "../Toy/Utility.h"
#include "../Utility.h"
#include "../HelperClass.h"

EditTextureWindow::~EditTextureWindow() = default;
EditTextureWindow::EditTextureWindow() :
    m_textureWindow{ nullptr },
    m_sourceBinder{ nullptr },
    m_selectImagePart{ ImagePart::One },
    m_renameNotifier{ make_unique<RenameNotifier>() }
{}

void EditTextureWindow::SetResourceAreas(optional<TextureSourceInfo> resourceAreas) noexcept
{
    if (m_sourceAreas == resourceAreas) return;

    //이 리소스 에어리어에 맞는 것을 찾아본다. 이름이 있으면 가져온다.
    m_sourceAreas = resourceAreas;
    m_sourceIdx = 0;
}

void EditTextureWindow::Update() noexcept
{
    if (m_preTextureIdx != m_textureIdx)
    {
        const wstring& filename = StringToWString(m_textureFiles[m_textureIdx]);
        m_textureWindow->LoadTexture(filename);
        m_preTextureIdx = m_textureIdx;
    }

    CheckSourcePartition();
    if(DeselectImageSource()) return;
    if (IsWindowFocus(m_textureWindow->GetWindow()))
        SelectImageSource();
}

void EditTextureWindow::Render()
{
    string wndName = string("Texture Source Window");
    ImGui::Begin(wndName.c_str(), nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

    RenderTextureList();
    DrawImagePart();
    RenderTextureInfo();
    RenderHighlightArea();

    ImGui::End();
}

bool EditTextureWindow::DeselectImageSource() noexcept
{
    if (!IsInputAction(Keyboard::Escape, KeyState::Pressed)) return false;

    m_sourceAreas = nullopt;
    m_sourceIdx = 0;

    return true;
}

void EditTextureWindow::SelectImageSource() noexcept
{
    if (!IsInputAction(MouseButton::Left, KeyState::Pressed)) return;

    if (!m_hoveredAreas.size() || m_hoveredAreas[0] == Rectangle{}) return;
    m_sourceAreas = { SelectedTextureFilename(), m_selectImagePart, m_hoveredAreas };
    m_sourceIdx = 0;
}

enum DivideType : int
{
    X = 1 << 0,
    Y = 1 << 1,
    XY = X | Y
};

static void DivideLengthByThree(const Rectangle& area, DivideType divideType,
    vector<int>& outWidth, vector<int>& outHeight) noexcept
{
    if (area.IsEmpty()) return;
    outWidth = { area.width };
    outHeight = { area.height };

    auto divideByThree = [](int totalSize) -> std::vector<int> {
        int oneThird = totalSize / 3;
        return { oneThird, totalSize - oneThird * 2, oneThird };
        };

    if (divideType & DivideType::X) outWidth = divideByThree(area.width);
    if (divideType & DivideType::Y) outHeight = divideByThree(area.height);
}

static vector<Rectangle> GenerateSourceAreas(const Rectangle& area, DivideType divideType)
{
    vector<int> widths, heights;
    DivideLengthByThree(area, divideType, widths, heights);
    return GetSourcesFromArea(area, widths, heights);
}

void EditTextureWindow::CheckSourcePartition()
{
    m_hoveredAreas.clear();
    vector<Rectangle> hoveredAreas{};
    switch (m_selectImagePart)
    {
    case ImagePart::One: m_hoveredAreas.emplace_back(FindAreaFromMousePos()); break;
    case ImagePart::ThreeH: m_hoveredAreas = GenerateSourceAreas(FindAreaFromMousePos(), DivideType::X); break;
    case ImagePart::ThreeV: m_hoveredAreas = GenerateSourceAreas(FindAreaFromMousePos(), DivideType::Y); break;
    case ImagePart::Nine: m_hoveredAreas = GenerateSourceAreas(FindAreaFromMousePos(), DivideType::XY); break;
    }
}

void EditTextureWindow::DrawImagePart()
{
    wstring filename;
    if (m_textureFiles.empty()) return;

    static int selected{ 0 };
    if (ImGui::RadioButton("1 Part", &selected, 0)) m_selectImagePart = ImagePart::One; ImGui::SameLine();
    if (ImGui::RadioButton("3H Part", &selected, 1)) m_selectImagePart = ImagePart::ThreeH; ImGui::SameLine();
    if (ImGui::RadioButton("3V Part", &selected, 2)) m_selectImagePart = ImagePart::ThreeV; ImGui::SameLine();
    if (ImGui::RadioButton("9 Part", &selected, 3)) m_selectImagePart = ImagePart::Nine;

    if (m_textureIdx < 0 || m_textureIdx >= m_textureFiles.size()) return;

    filename = StringToWString(m_textureFiles[m_textureIdx]);
    vector<TextureSourceInfo> sourceInfos = m_sourceBinder->GetAreas(filename, m_selectImagePart);
    for (auto& info : sourceInfos)
        DrawRectangles(m_textureWindow->GetWindow(), info.sources, ToColor(Colors::Blue));
}

static vector<string> WStringVecToStringVec(const vector<wstring>& v)
{
    vector<string> res(v.size());
    ranges::transform(v, res.begin(), WStringToString);
    return res;
}

void EditTextureWindow::SetSourceBinder(TextureSourceBinder* sourceBinder) noexcept
{
    m_sourceBinder = sourceBinder;
    m_textureFiles = WStringVecToStringVec(m_sourceBinder->GetTextureFiles());
}

void EditTextureWindow::RenderTextureList()
{
    if (ImGui::Button("Add Texture File"))
    {
        wstring filename;
        GetRelativePathFromDialog(filename);
        if (!filename.empty()) m_textureFiles.emplace_back(WStringToString(filename));
    }

    vector<const char*> texFiles;
    for (const auto& curFile : m_textureFiles)
        texFiles.push_back(curFile.c_str());

    ImGui::ListBox("Texture List", &m_textureIdx, texFiles.data(), static_cast<int>(texFiles.size()), 4);
}

void EditTextureWindow::RenderTextureInfo()
{
    if (!m_sourceAreas) return;
    if (m_sourceAreas->IsEmpty()) return;

    DrawRectangles(m_textureWindow->GetWindow(), m_sourceAreas->sources,
        ToColor(Colors::White), ToColor(Colors::White, 0.3f));

    const string& bindingKey = m_sourceBinder->GetBindingKey(*m_sourceAreas);
    m_renameNotifier->EditName("Binding Key", bindingKey, [this](const string& newKey) {
        return m_sourceBinder->ChangeBindingKey(newKey, *m_sourceAreas); });

    constexpr string_view label{ "Resource Areas" };
    ImGui::Text("%s", label.data());
    ImGui::SliderInt("Area Index", &m_sourceIdx, 0, m_sourceAreas->GetSourceSize() - 1);
    Rectangle currArea = m_sourceAreas->GetSource(m_sourceIdx);
    if (EditRectangleNoLabel(label.data(), currArea))
        m_sourceAreas->SetSource(m_sourceIdx, currArea);
}

Rectangle EditTextureWindow::FindAreaFromMousePos() const noexcept
{
    const XMINT2& pos = InputManager::GetMouse().GetPosition();
    auto it = ranges::find_if(m_areaList, [&pos](const Rectangle& rect) {
        return Contains(rect, pos);
        });

    if (it != m_areaList.end()) {
        return *it;
    }

    return {};
}

wstring EditTextureWindow::SelectedTextureFilename() const noexcept 
{
    return (m_textureIdx > 0) ? StringToWString(m_textureFiles[m_textureIdx]) : L""; 
}

void EditTextureWindow::RenderHighlightArea() const
{
    DrawRectangles(m_textureWindow->GetWindow(), m_hoveredAreas, ToColor(Colors::White), ToColor(Colors::White, 0.3f));
}