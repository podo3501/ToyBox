#include "pch.h"
#include "../Include/IRenderer.h"
#include "EditTextureWindow.h"
#include "MainTextureWindow.h"
#include "../MainWindow/EditUtility.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Dialog.h"
#include "../Toy/Utility.h"
#include "../Utility.h"
#include "../HelperClass.h"

enum class PendingAction : int
{
    LoadTextureFile,
    DeleteTextureFile,
    SelectTextureFile
};

EditTextureWindow::~EditTextureWindow() = default;
EditTextureWindow::EditTextureWindow(IRenderer* renderer) :
    m_textureWindow{ nullptr },
    m_renderer{ renderer },
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

//void EditTextureWindow::Update() noexcept
//{
//    //if (m_preTextureIdx != m_textureIdx)
//    //{
//    //    const wstring& filename = StringToWString(m_textureFiles[m_textureIdx]);
//    //    m_textureWindow->LoadTexture(filename);
//    //    m_preTextureIdx = m_textureIdx;
//    //}
//
//    ExecuteAction();
//    CheckSourcePartition();
//    if(DeselectImageSource()) return;
//    if (IsWindowFocus(m_textureWindow->GetWindow()))
//        SelectImageSource();
//}
//
//void EditTextureWindow::Render()
//{
//    string wndName = string("Texture Source Window");
//    ImGui::Begin(wndName.c_str(), nullptr, ImGuiWindowFlags_NoFocusOnAppearing);
//
//    RenderTextureList();
//    DrawImagePart();
//    RenderTextureInfo();
//    RenderLabeledAreas();
//    RenderHighlightArea();
//
//    ImGui::End();
//}

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
    None = 0,
    X = 1 << 0,
    Y = 1 << 1,
    XY = X | Y
};

static DivideType ImagePartToDivideType(ImagePart imgPart) noexcept
{
    switch (imgPart)
    {
    case ImagePart::One: return DivideType::None;
    case ImagePart::ThreeH: return DivideType::X;
    case ImagePart::ThreeV: return DivideType::Y;
    case ImagePart::Nine: return DivideType::XY;
    default: return DivideType::None;
    }
}

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
    if (divideType == DivideType::None) return { area };

    vector<int> widths, heights;
    DivideLengthByThree(area, divideType, widths, heights);
    return GetSourcesFromArea(area, widths, heights);
}

void EditTextureWindow::CheckSourcePartition()
{
    const XMINT2& pos = InputManager::GetMouse().GetPosition();
    m_hoveredAreas = m_sourceBinder->GetArea(SelectedTextureFilename(), pos);
    if (!m_hoveredAreas.empty()) return;

    const Rectangle& currRectangle = FindAreaFromMousePos(pos);
    m_hoveredAreas = GenerateSourceAreas(currRectangle, ImagePartToDivideType(m_selectImagePart));
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

static int GetImagePartCount(ImagePart imgPart) noexcept
{
    switch (imgPart)
    {
    case ImagePart::One: return 1;
    case ImagePart::ThreeH: return 3;
    case ImagePart::ThreeV: return 3;
    case ImagePart::Nine: return 9;
    default: return 0;
    }
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

    if (bindingKey.empty()) return;

    //현재 선택된 조그마한 파란색의 크기를 셋팅한다.
    constexpr string_view label{ "Resource Areas" };
    ImGui::Text("%s", label.data());
    ImGui::SliderInt("Area Index", &m_sourceIdx, 0, GetImagePartCount(m_selectImagePart) - 1);
    Rectangle currArea = m_sourceBinder->GetArea(bindingKey, m_sourceIdx);
    if (EditRectangleNoLabel(label.data(), currArea))
    {
        m_sourceBinder->SetArea(bindingKey, m_sourceIdx, currArea);
        //m_sourceAreas = currArea;
    }
}

Rectangle EditTextureWindow::FindAreaFromMousePos(const XMINT2& pos) const noexcept
{
    auto it = ranges::find_if(m_areaList, [&pos](const Rectangle& rect) {
        return Contains(rect, pos);
        });

    if (it != m_areaList.end()) {
        return *it;
    }

    return {};
}

void EditTextureWindow::RenderLabeledAreas() const
{
    return;
    if (!IsVaildTextureIndex()) return;

    wstring filename = StringToWString(m_textureFiles[m_textureIdx]);
    vector<TextureSourceInfo> sourceInfos = m_sourceBinder->GetAreas(filename, m_selectImagePart);
    for (auto& info : sourceInfos)
        DrawRectangles(m_textureWindow->GetWindow(), info.sources, ToColor(Colors::Blue));
}

void EditTextureWindow::RenderHighlightArea() const
{
    DrawRectangles(m_textureWindow->GetWindow(), m_hoveredAreas, ToColor(Colors::White), ToColor(Colors::White, 0.3f));
}

wstring EditTextureWindow::SelectedTextureFilename() const noexcept
{
    //return IsVaildTextureIndex() ? StringToWString(m_textureFiless[m_textureIdx]) : L"";
    return L"";
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void EditTextureWindow::Update() noexcept
{
    ExecuteAction();
    //CheckSourcePartition();
    //if (DeselectImageSource()) return;
    //if (IsWindowFocus(m_textureWindow->GetWindow()))
       // SelectImageSource();
}

void EditTextureWindow::Render()
{
    string wndName = string("Texture Source Window");
    ImGui::Begin(wndName.c_str(), nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

    RenderTextureList();
    DrawImagePart();
    //RenderTextureInfo();
    //RenderLabeledAreas();
    //RenderHighlightArea();

    ImGui::End();
}

void EditTextureWindow::AddTextureFile(const wstring& filename) noexcept
{ 
    const string& strName = WStringToString(filename);
    auto it = ranges::find(m_textureFiles, strName);
    if (it != m_textureFiles.end()) return;

    m_textureFiles.push_back(strName);
}

void EditTextureWindow::AddTexture(unique_ptr<ImageGrid1> texture) noexcept
{
    m_textureFiless.emplace_back(move(texture));
    m_textureIdx = static_cast<int>(m_textureFiless.size() - 1);
}

void EditTextureWindow::RemoveTextureFile(const wstring& filename) noexcept
{
    const string& strName = WStringToString(filename);
    erase(m_textureFiles, strName);
}

bool EditTextureWindow::IsLoadedTexture(const wstring& filename) const noexcept
{
    auto it = ranges::find_if(m_textureFiless, [&filename](auto& tex) { return tex->GetFilename() == filename; });
    return (it != m_textureFiless.end()) ? true : false;
}

bool EditTextureWindow::LoadTextureFile()
{
    wstring filename;
    GetRelativePathFromDialog(filename);
    if (filename.empty()) return true;
    if (IsLoadedTexture(filename)) return true;

    unique_ptr<ImageGrid1> texture = make_unique<ImageGrid1>();
    texture->SetFilenameToLoadInfo(filename);
    ReturnIfFalse(m_renderer->LoadComponent(texture.get()));
    if (const auto& areaList = texture->GetTextureAreaList(); areaList)
        m_areaList = *areaList;

    m_textureWindow->SetTexture(texture.get());
    AddTexture(move(texture));

    return true;
}

bool EditTextureWindow::DeleteTextureFile() noexcept
{
    ReturnIfFalse(IsVaildTextureIndex());
    m_textureFiless.erase(m_textureFiless.begin() + m_textureIdx);

    if (m_textureFiless.empty())
    {
        m_textureWindow->SetTexture(nullptr);
        m_areaList = {};
        return true;
    }

    m_textureIdx = 0;
    SelectTextureFile();

    return true;
}

bool EditTextureWindow::SelectTextureFile() noexcept
{
    if (!IsVaildTextureIndex()) return true;
    const auto& curTexture = m_textureFiless[m_textureIdx].get();
    m_textureWindow->SetTexture(curTexture);
    if (const auto& areaList = curTexture->GetTextureAreaList(); areaList)
        m_areaList = *areaList;

    return true;
}

bool EditTextureWindow::ExecuteAction() noexcept
{
    if (!m_pendingAction.has_value()) return true;

    auto result{ true };
    switch (*m_pendingAction)
    {
    case PendingAction::LoadTextureFile: result = LoadTextureFile(); break;
    case PendingAction::DeleteTextureFile: result = DeleteTextureFile(); break;
    case PendingAction::SelectTextureFile: result = SelectTextureFile(); break;
    default: break;
    }

    m_pendingAction.reset(); // 상태 초기화

    return result;
}

static vector<string> GetTextureFiles(const vector<unique_ptr<ImageGrid1>>& texFiles) noexcept
{
    vector<string> strList;
    for (auto& ws : texFiles)
        strList.emplace_back(WStringToString(ws->GetFilename()));
    return strList;
}

void EditTextureWindow::RenderTextureList()
{
    ImVec2 btnSize{ 130, 22 };
    if (ImGui::Button("Add Texture File", btnSize)) m_pendingAction = PendingAction::LoadTextureFile; ImGui::SameLine();
    if (ImGui::Button("Delete Texture File", btnSize)) m_pendingAction = PendingAction::DeleteTextureFile;

    auto strFileList = GetTextureFiles(m_textureFiless);
    vector<const char*> texFiles; //const char*은 값을 저장하는게 아니라 포인터를 저장하기 때문에 strFileList가 없어지면 글자가 깨진다.
    for (auto& str : strFileList)
        texFiles.emplace_back(str.c_str());
    if (ImGui::ListBox("Texture List", &m_textureIdx, texFiles.data(), static_cast<int>(texFiles.size()), 4))
        m_pendingAction = PendingAction::SelectTextureFile;
}

void EditTextureWindow::DrawImagePart()
{
    if (!IsVaildTextureIndex()) return;

    static int selected{ 0 };
    if (ImGui::RadioButton("1 Part", &selected, 0)) m_selectImagePart = ImagePart::One; ImGui::SameLine();
    if (ImGui::RadioButton("3H Part", &selected, 1)) m_selectImagePart = ImagePart::ThreeH; ImGui::SameLine();
    if (ImGui::RadioButton("3V Part", &selected, 2)) m_selectImagePart = ImagePart::ThreeV; ImGui::SameLine();
    if (ImGui::RadioButton("9 Part", &selected, 3)) m_selectImagePart = ImagePart::Nine;
}