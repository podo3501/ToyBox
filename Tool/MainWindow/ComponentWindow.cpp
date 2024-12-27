#include "pch.h"
#include "ComponentWindow.h"
#include "../Include/IRenderer.h"
#include "../Toy/UserInterface/ImageGrid1.h"
#include "../Toy/UserInterface/ImageGrid9.h"
#include "../Toy/Config.h"
#include "EditUtility.h"
#include "Utility.h"
#include "../Toy/Utility.h"

ComponentWindow::~ComponentWindow()
{

}
ComponentWindow::ComponentWindow() :
	m_component{ nullptr }
{}

void ComponentWindow::SetComponent(UIComponent* component)
{
	m_component = component;
	m_visible = (m_component != nullptr);
}

void ComponentWindow::Update()
{
    UpdateComponent();
}

void ComponentWindow::Render()
{
    if (!m_visible) return;

    ImGui::Begin("Component Window - ", &m_visible, ImGuiWindowFlags_NoFocusOnAppearing);

    // Component 이름 수정
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s", m_component->GetName().c_str());

    if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer)))
        m_component->SetName(buffer); // 이름 업데이트

    bool modify{ false };
    const auto& layout = m_component->GetLayout();
    modify |= EditSize(layout.GetSize());
    
    ImGui::Separator();
    ImGui::Spacing();

    RenderComponent(m_component, modify);
    if (modify)
        m_component->RefreshPosition();

    ImGui::End();
}

bool ComponentWindow::EditSize(const XMUINT2& size)
{
    XMUINT2 newSize = size;

    bool changed{ false };

    changed |= EditInteger("Width", newSize.x);
    changed |= EditInteger("Height", newSize.y);

    if (changed)
        m_component->ChangeSize(newSize);

    return changed;
}

////////////////////////////////////////////////

class ExtractTexture : public UIComponent
{
public:
    virtual unique_ptr<UIComponent> CreateClone() const override
    {
        return nullptr;
    }

    virtual bool LoadResources(ILoadData* load)
    {
        XMUINT2 size{};
        auto result = load->LoadTexture(GetResourceFullFilename(m_filename), nullptr, m_index, &size);
        SetSize(size);
        return result;
    }

    void SetImage(const wstring& filename)
    {
        m_filename = filename;
    }

    virtual void Render(IRender* render) const override
    {
        const auto& size = GetSize();
        RECT source(0, 0, size.x, size.y);
        RECT destination(source);

        render->Render(m_index, destination, &source);
    }

private:
    size_t m_index{ 0 };
    wstring m_filename;
};

class TextureWindow
{
public:
    ~TextureWindow()
    {
        m_renderer->RemoveRenderTexture(m_textureID);
        m_renderer->RemoveRenderComponent(m_extractTexture.get());
    }

    TextureWindow(IRenderer* renderer) :
        m_renderer{ renderer },
        m_extractTexture{ make_unique<ExtractTexture>() },
        m_size{}
    {}

    bool Create(const wstring& filename)
    {
        m_extractTexture->SetImage(filename);
        ReturnIfFalse(m_renderer->LoadComponent(m_extractTexture.get()));

        XMUINT2 size = m_extractTexture->GetSize();
        ReturnIfFalse(m_renderer->CreateRenderTexture(size, m_extractTexture.get(), m_textureID));

        m_size = XMUINT2ToImVec2(size);
        m_name = WStringToString(filename);
        m_isOpen = true;

        return true;
    }

    void Render()
    {
        if (!m_isOpen)
            return;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize({ m_size.x, m_size.y + GetFrameHeight() });
        //ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_None);
        ImGui::Begin(m_name.c_str(), &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::PopStyleVar();   //윈도우 스타일을 지정한다.

        ImGui::Image(m_textureID, m_size);

        ImGui::End();
    }

    bool IsOpen() const noexcept { return m_isOpen; }

private:
    IRenderer* m_renderer;
    unique_ptr<ExtractTexture> m_extractTexture;
    ImTextureID m_textureID{};
    ImVec2 m_size;
    string m_name;
    bool m_isOpen{ false };
};

ComponentImageGrid1::~ComponentImageGrid1() = default;
ComponentImageGrid1::ComponentImageGrid1() = default;

void ComponentImageGrid1::UpdateComponent()
{
    if (m_textureWindow != nullptr && !m_textureWindow->IsOpen())
        m_textureWindow = nullptr;
}

void ComponentImageGrid1::RenderComponent(UIComponent* component, bool& modify)
{
    if (m_textureWindow)
        m_textureWindow->Render();

    ImageGrid1* imgGrid1 = ComponentCast<ImageGrid1*>(component);
    EditFilename("Filename", imgGrid1->Filename);
    modify |= EditRectangle("Source", imgGrid1->Source);
    
    ImGui::Spacing();

    if (ImGui::Button("Extract Textrue Area"))
    {
        if (!m_textureWindow)
        {
            m_textureWindow = make_unique<TextureWindow>(GetRenderer());
            m_textureWindow->Create(imgGrid1->Filename);
        }
    }
}

////////////////////////////////////////////////

void ComponentImageGrid3::RenderComponent(UIComponent* component, bool& modify)
{
}

void ComponentImageGrid9::RenderComponent(UIComponent* component, bool& modify)
{
    ImageGrid9* imgGrid9 = ComponentCast<ImageGrid9*>(component);
}