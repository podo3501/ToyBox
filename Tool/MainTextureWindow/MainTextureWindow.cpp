#include "pch.h"
#include "MainTextureWindow.h"
#include "MainSourceExtractor.h"
#include "../Utility.h"
#include "../Dialog.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/TextureSourceBinder.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../MainWindow/EditUtility.h"
#include "../HelperClass.h"
#include "../Toy/UserInterface/JsonHelper.h"

class EditTextureWindow
{
public:
    ~EditTextureWindow() = default;
    EditTextureWindow() :
        m_textureWindow{ nullptr },
        m_sourceBinder{ nullptr },
        m_renameNotifier{ make_unique<RenameNotifier>() }
    {}

    void SetResourceAreas(optional<TextureSourceInfo> resourceAreas) noexcept
    { 
        if (m_sourceAreas == resourceAreas) return;

        //이 리소스 에어리어에 맞는 것을 찾아본다. 이름이 있으면 가져온다.
        m_sourceAreas = resourceAreas;
        m_sourceIdx = 0;
    }

    void SetTextureWindow(MainTextureWindow* texWindow) noexcept { m_textureWindow = texWindow; }

    void Update() noexcept
    {
        if (m_preTextureIdx != m_textureIdx)
        {
            const wstring& filename = StringToWString(m_textureFiles[m_textureIdx]);
            m_textureWindow->LoadTexture(filename);
            m_preTextureIdx = m_textureIdx;
        }
    }

    void Render()
    {
        string wndName = string("Texture Source Window");
        ImGui::Begin(wndName.c_str(), nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

        RenderTextureList();
        RenderTextureInfo();

        ImGui::End();
    }

    void SetSourceBinder(TextureSourceBinder* sourceBinder) noexcept
    {
        m_sourceBinder = sourceBinder;
        m_textureFiles = WStringVecToStringVec(m_sourceBinder->GetTextureFiles());
    }

    static vector<string> WStringVecToStringVec(const vector<wstring>& v) 
    {
        vector<string> res(v.size());
        ranges::transform(v, res.begin(), WStringToString);
        return res;
    }

private:
    void RenderTextureList()
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

    void RenderTextureInfo()
    {
        if (!m_sourceAreas) return; 
        if (m_sourceAreas->IsEmpty()) return;
        
        DrawImagePart();
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

    void DrawImagePart()
    {
        wstring filename;
        if (m_textureFiles.empty()) return;

        static int selected{ 0 };
        if (ImGui::RadioButton("1 Part", &selected, 0)) m_selectImagePart = ImagePart::One; ImGui::SameLine();
        if (ImGui::RadioButton("3 Part", &selected, 1)) m_selectImagePart = ImagePart::Three; ImGui::SameLine();
        if (ImGui::RadioButton("9 Part", &selected, 2)) m_selectImagePart = ImagePart::Nine;

        filename = StringToWString(m_textureFiles[m_textureIdx]);
        vector<TextureSourceInfo> sourceInfos = m_sourceBinder->GetAreas(filename, m_selectImagePart);
        for( auto& info : sourceInfos)
            DrawRectangles(m_textureWindow->GetWindow(), info.sources, ToColor(Colors::Blue));
    }

    MainTextureWindow* m_textureWindow;
    TextureSourceBinder* m_sourceBinder;
    int m_preTextureIdx{ -1 };
    int m_textureIdx{ -1 };
    vector<string> m_textureFiles;
    int m_sourceIdx{ 0 };
    ImagePart m_selectImagePart{ ImagePart::One };
    unique_ptr<RenameNotifier> m_renameNotifier;
    optional<TextureSourceInfo> m_sourceAreas;
};

//////////////////////////////////////////////////////////////////

MainTextureWindow::~MainTextureWindow()
{
    m_renderer->RemoveImguiComponent(this);
}

MainTextureWindow::MainTextureWindow(IRenderer* renderer) :
    InnerWindow{ "empty" },
    m_renderer{ renderer },
    m_sourceTexture{ make_unique<ImageGrid1>() },
    m_editTextureWindow{ make_unique<EditTextureWindow>() }
{
    m_renderer->AddImguiComponent(this);
}

bool MainTextureWindow::CreateNew()
{
    m_sourceBinder = CreateSourceBinder();
    ReturnIfFalse(m_sourceBinder);

    m_editTextureWindow->SetSourceBinder(m_sourceBinder.get());
    m_isOpen = true;
    return true;
}

bool MainTextureWindow::LoadTexture(const wstring& filename)
{
    m_sourceTexture->SetFilenameToLoadInfo(filename);
    ReturnIfFalse(m_renderer->LoadComponent(m_sourceTexture.get()));
    if(const auto& areaList = m_sourceTexture->GetTextureAreaList(); areaList)
        m_areaList = (*areaList);

    SetName(WStringToString(filename));
    return true;
}

bool MainTextureWindow::Create(const wstring& filename)
{
    m_sourceBinder = CreateSourceBinder(filename);
    ReturnIfFalse(m_sourceBinder);

    m_editTextureWindow->SetSourceBinder(m_sourceBinder.get());
    m_isOpen = true;
    return true;
}

void MainTextureWindow::SelectImageSource() noexcept
{
    if (!IsInputAction(MouseButton::Left, KeyState::Pressed)) return;

    if (!m_hoveredAreas.size() || m_hoveredAreas[0] == Rectangle{}) return;
    TextureSourceInfo texureSourceInfo{ m_sourceTexture->GetFilename(), m_hoveredAreas};
    m_editTextureWindow->SetResourceAreas(texureSourceInfo);
}

bool MainTextureWindow::DeselectImageSource() noexcept
{
    if (!IsInputAction(Keyboard::Escape, KeyState::Pressed)) return false;
    m_editTextureWindow->SetResourceAreas(nullopt);

    return true;
}

void MainTextureWindow::Update()
{
    if (!m_window) return;
    SetMouseStartOffset(m_window);

    CheckSourcePartition();
    if (DeselectImageSource()) return;
    if (IsWindowFocus(m_window))
        SelectImageSource();
    m_editTextureWindow->Update();
}

ImVec2 MainTextureWindow::GetWindowSize() const noexcept
{
    const auto& size = m_sourceTexture->GetSize();
    return size != XMUINT2{} ? XMUINT2ToImVec2(size) : ImVec2{ 512, 512 };
}

void MainTextureWindow::Render(ImGuiIO* io)
{
    if (!m_isOpen) return;

    const ImVec2& size = GetWindowSize();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize({ size.x, size.y + GetFrameHeight() });
    ImGui::Begin(GetName().c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

    if (ImGui::IsWindowAppearing())
    {
        m_window = GetImGuiWindow();
        m_editTextureWindow->SetTextureWindow(this);
    }

    if(auto offset = m_sourceTexture->GetGraphicMemoryOffset(); offset != 0)
        ImGui::Image(offset, size);

    IgnoreMouseClick(m_window);
    RenderPopupMenu();
    RenderHighlightArea();
    m_editTextureWindow->Render();

    ImGui::End();
}

bool MainTextureWindow::SaveScene(const wstring& filename)
{
    return m_sourceBinder->Save(filename);
}

wstring MainTextureWindow::GetSaveFilename() const noexcept
{
    return m_sourceBinder->GetJsonFilename();
}

////////////////////////////////////////////////////////

Rectangle MainTextureWindow::FindAreaFromMousePos() const noexcept
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

void MainTextureWindow::CheckSourcePartition()
{
    using enum SourcePartition;
    m_hoveredAreas.clear();
    vector<Rectangle> hoveredAreas{};
    switch (m_sourcePartition)
    {
    case OnePart: m_hoveredAreas.emplace_back(FindAreaFromMousePos()); break;
    case ThreePartHorizontal: m_hoveredAreas = GenerateSourceAreas(FindAreaFromMousePos(), DivideType::X); break;
    case ThreePartVertical: m_hoveredAreas = GenerateSourceAreas(FindAreaFromMousePos(), DivideType::Y); break;
    case NinePart: m_hoveredAreas = GenerateSourceAreas(FindAreaFromMousePos(), DivideType::XY); break;
    }
}

void MainTextureWindow::RenderPopupMenu()
{
    if (!ImGui::BeginPopupContextWindow("PopupMenu_TextureWindow")) return;

    using enum SourcePartition;
    if (ImGui::MenuItem("One-part")) m_sourcePartition = OnePart;
    if (ImGui::MenuItem("Three-part horizontal")) m_sourcePartition = ThreePartHorizontal;
    if (ImGui::MenuItem("Three-part vertical")) m_sourcePartition = ThreePartVertical;
    if (ImGui::MenuItem("Nine-part")) m_sourcePartition = NinePart;

    ImGui::EndPopup();
}

void MainTextureWindow::RenderHighlightArea() const
{
    DrawRectangles(m_window, m_hoveredAreas, ToColor(Colors::White), ToColor(Colors::White, 0.3f));
}