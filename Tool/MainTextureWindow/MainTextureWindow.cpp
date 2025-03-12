#include "pch.h"
#include "MainTextureWindow.h"
#include "MainSourceExtractor.h"
#include "../Utility.h"
#include "../Include/IRenderer.h"
#include "../Toy/Utility.h"
#include "../Toy/UserInterface/Component/RenderTexture.h"
#include "../Toy/UserInterface/Component/ImageGrid1.h"
#include "../Toy/InputManager.h"
#include "../Toy/UserInterface/UIUtility.h"
#include "../MainWindow/EditUtility.h"

class EditTextureWindow
{
public:
    ~EditTextureWindow() = default;
    EditTextureWindow() :
        m_texWindow{ nullptr }
    {}

    void SetResourceAreas(optional<vector<Rectangle>> resourceAreas) noexcept 
    { 
        if (m_sourceAreas == resourceAreas) return;

        //이 리소스 에어리어에 맞는 것을 찾아본다. 이름이 있으면 가져온다.
        m_sourceAreas = resourceAreas;
        m_selectedIndex = 0;
    }
    void SetTextureWindow(ImGuiWindow* texWindow) noexcept { m_texWindow = texWindow; }

    void Render()
    {
        if (!m_sourceAreas) return;

        bool visible{ true };
        string wndName = string("Texture Source Window");
        ImGui::Begin(wndName.c_str(), &visible, ImGuiWindowFlags_NoFocusOnAppearing);
        if (!visible) SetResourceAreas(nullopt);

        if (m_sourceAreas)
            DrawRectangle(*m_sourceAreas, m_texWindow);

        RenderTextureInfo();

        ImGui::End();
    }

private:
    void RenderTextureInfo()
    {
        if (!m_sourceAreas) return; 
        if (m_sourceAreas->empty()) return;

        //이름을 출력하고 이름이 바뀌면 이름과 위치를 저장시킨다.
        constexpr string_view label{ "Resource Areas" };
        ImGui::Text("%s", label.data());
        ImGui::SliderInt("Area Index", &m_selectedIndex, 0, static_cast<int>(m_sourceAreas->size() - 1));
        Rectangle& currArea = m_sourceAreas->at(m_selectedIndex);
        EditRectangleNoLabel(label.data(), currArea);
    }

    ImGuiWindow* m_texWindow;
    int m_selectedIndex{ 0 };
    optional<vector<Rectangle>> m_sourceAreas;
};

MainTextureWindow::~MainTextureWindow() = default;
MainTextureWindow::MainTextureWindow(IRenderer* renderer) :
    m_renderer{ renderer },
    m_sourceTexture{ make_unique<ImageGrid1>() }
{}

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

    m_editTextureWindow = make_unique<EditTextureWindow>();

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
}

void MainTextureWindow::Render()
{
    if (!m_isOpen) return;

    ImVec2 size = XMUINT2ToImVec2(m_sourceTexture->GetSize());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize({ size.x, size.y + GetFrameHeight() });
    //ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_None);
    ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

    if (ImGui::IsWindowAppearing())
    {
        m_window = ImGui::FindWindowByName(m_name.c_str());
        m_editTextureWindow->SetTextureWindow(m_window);
    }
    ImGui::Image(m_sourceTexture->GetGraphicMemoryOffset(), size);

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