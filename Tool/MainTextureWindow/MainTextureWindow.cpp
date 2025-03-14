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
        m_renameNotifier{ make_unique<RenameNotifier>() },
        m_sourceBinder{ make_unique<TextureSourceBinder>() }
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

    bool Save(const wstring& filename) noexcept
    {
        ReturnIfFalse(JsonFile::Write(*m_sourceBinder, filename));
        m_sourceBinderFilename = filename;
        return true;
    }

    static vector<string> WStringVecToStringVec(const vector<wstring>& v) 
    {
        vector<string> res(v.size());
        ranges::transform(v, res.begin(), WStringToString);
        return res;
    }

    bool Load(const wstring& filename) noexcept
    {
        m_sourceBinder = CreateSourceBinder(filename);
        ReturnIfFalse(m_sourceBinder);

        m_textureFiles = WStringVecToStringVec(m_sourceBinder->GetTextureFiles());
        m_sourceBinderFilename = filename;
        return true;
    }

    wstring GetSaveFilename() const noexcept
    {
        return m_sourceBinderFilename;
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
        
        DrawRectangle(m_sourceAreas->sources, m_textureWindow->GetWindow());

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

    MainTextureWindow* m_textureWindow;
    int m_preTextureIdx{ -1 };
    int m_textureIdx{ -1 };
    vector<string> m_textureFiles;
    int m_sourceIdx{ 0 };
    unique_ptr<RenameNotifier> m_renameNotifier;
    optional<TextureSourceInfo> m_sourceAreas;
    unique_ptr<TextureSourceBinder> m_sourceBinder; //?!? ToolSystem쪽으로 옮길꺼임
    wstring m_sourceBinderFilename{};
};

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
    ReturnIfFalse(m_editTextureWindow->Load(filename));
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
    return m_editTextureWindow->Save(filename);
}

wstring MainTextureWindow::GetSaveFilename() const noexcept
{
    return m_editTextureWindow->GetSaveFilename();
}

////////////////////////////////////////////////////////

Rectangle MainTextureWindow::FindRectangleFromMousePosition() const noexcept
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

static void DivideLengthByThree(const Rectangle& area, vector<int>* outWidth, vector<int>* outHeight)
{
    if (area.IsEmpty()) return;

    auto divideByThree = [](int totalSize) -> std::vector<int> {
        int oneThird = totalSize / 3;
        return { oneThird, totalSize - oneThird * 2, oneThird };
        };

    if (outWidth)
        (*outWidth) = divideByThree(area.width);
    if (outHeight)
        (*outHeight) = divideByThree(area.height);
}

static vector<Rectangle> GenerateSourceAreas(const Rectangle& area, bool is9Grid)
{
    vector<int> widths{};
    vector<int> heights = is9Grid ? vector<int>{} : vector<int>{ area.height };
    DivideLengthByThree(area, &widths, is9Grid ? &heights : nullptr);

    return GetSourcesFromArea(area, widths, heights);
}

void MainTextureWindow::CheckSourcePartition()
{
    using enum SourcePartition;
    m_hoveredAreas.clear();
    vector<Rectangle> hoveredAreas{};
    switch (m_sourcePartition)
    {
    case OnePart: m_hoveredAreas.emplace_back(FindRectangleFromMousePosition()); break;
    case ThreePartHorizontal: m_hoveredAreas = GenerateSourceAreas(FindRectangleFromMousePosition(), false); break;
    case NinePart: m_hoveredAreas = GenerateSourceAreas(FindRectangleFromMousePosition(), true); break;
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
    DrawRectangle(m_hoveredAreas, m_window);
}