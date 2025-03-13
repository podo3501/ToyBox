#include "pch.h"
#include "MainTextureWindow.h"
#include "MainSourceExtractor.h"
#include "../Utility.h"
#include "../Include/IRenderer.h"
#include "../Dialog.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/UserInterface/TextureSourceBinder.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../MainWindow/EditUtility.h"
#include "../HelperClass.h"

class EditTextureWindow
{
public:
    ~EditTextureWindow() = default;
    EditTextureWindow() :
        m_textureWindow{ nullptr },
        m_renameNotifier{ make_unique<RenameNotifier>() },
        m_sourceBinder{ make_unique<TextureSourceBinder>() }
    {}

    void SetResourceAreas(optional<vector<Rectangle>> resourceAreas) noexcept 
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
            m_textureWindow->Create(filename);
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
        if (m_sourceAreas->empty()) return;
        
        DrawRectangle(*m_sourceAreas, m_textureWindow->GetWindow());

        const string& bindingKey = m_sourceBinder->GetBindingKey(*m_sourceAreas);
        //이름을 출력하고 이름이 바뀌면 이름과 위치를 저장시킨다.
        m_renameNotifier->EditName("Binding Key", bindingKey, [this](const string& newKey) {
            return m_sourceBinder->ChangeBindingKey(newKey, *m_sourceAreas); });

        constexpr string_view label{ "Resource Areas" };
        ImGui::Text("%s", label.data());
        ImGui::SliderInt("Area Index", &m_sourceIdx, 0, static_cast<int>(m_sourceAreas->size() - 1));
        Rectangle& currArea = m_sourceAreas->at(m_sourceIdx);
        EditRectangleNoLabel(label.data(), currArea);
    }

    MainTextureWindow* m_textureWindow;
    int m_preTextureIdx{ -1 };
    int m_textureIdx{ -1 };
    vector<string> m_textureFiles;
    int m_sourceIdx{ 0 };
    unique_ptr<RenameNotifier> m_renameNotifier;
    optional<vector<Rectangle>> m_sourceAreas;
    unique_ptr<TextureSourceBinder> m_sourceBinder;
};

MainTextureWindow::~MainTextureWindow() = default;
MainTextureWindow::MainTextureWindow(IRenderer* renderer) :
    m_renderer{ renderer },
    m_sourceTexture{ make_unique<ImageGrid1>() },
    m_editTextureWindow{ make_unique<EditTextureWindow>() }
{}

bool MainTextureWindow::CreateNew()
{
    m_name = "empty";
    m_isOpen = true;

    return true;
}

bool MainTextureWindow::Create(const wstring& filename)
{
    m_sourceTexture->SetFilenameToLoadInfo(filename);
    ReturnIfFalse(m_renderer->LoadComponent(m_sourceTexture.get()));
    if(const auto& areaList = m_sourceTexture->GetTextureAreaList(); areaList)
        m_areaList = (*areaList);

    wstring jsonFile = ReplaceFileExtension(filename, L".json"); //파일에 매칭되는 json파일을 읽는다.

    //TextureSourceBinder 클래스 작성후 파일을 읽은후 editTexWindow에 넘겨준다.

    m_name = WStringToString(filename);
    m_isOpen = true;

    return true;
}

void MainTextureWindow::SelectImageSource() noexcept
{
    if (!IsInputAction(MouseButton::Left, KeyState::Pressed)) return;

    if (!m_hoveredAreas.size() || m_hoveredAreas[0] == Rectangle{}) return;
    m_editTextureWindow->SetResourceAreas(m_hoveredAreas);
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

void MainTextureWindow::Render()
{
    if (!m_isOpen) return;

    const ImVec2& size = GetWindowSize();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize({ size.x, size.y + GetFrameHeight() });
    ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

    if (ImGui::IsWindowAppearing())
    {
        m_window = ImGui::FindWindowByName(m_name.c_str());
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