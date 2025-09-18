#include "../Include/IRenderer.h"

struct ITextureLoadStub : public ITextureLoad
{
    virtual ~ITextureLoadStub() {};

    virtual bool LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize, UINT64* outGfxMemOffset) override { return true; }
    virtual bool LoadFont(const wstring& filename, size_t& outIndex) override { return true; }
    virtual void ReleaseTexture(size_t idx) noexcept override {}
    virtual void AddRef(size_t index) noexcept override {}
};

struct IRendererStub : public IRenderer
{
public:
    virtual ~IRendererStub() {};

    virtual bool Initialize() override { return true; }
    virtual void AddRenderComponent(IComponent* component) override {}
    virtual void RemoveRenderComponent(IComponent* component) override {}

    virtual void AddImguiComponent(IImguiComponent* item) override {}
    virtual void RemoveImguiComponent(IImguiComponent* comp) noexcept override {}

    virtual bool LoadTextureBinder(ITextureBinder* textureBinder) override { return true; }

    virtual void Draw() override {}
    virtual ITextureController* GetTextureController() const noexcept override { return nullptr; }

    virtual void OnActivated() override {}
    virtual void OnDeactivated() override {}
    virtual void OnSuspending() override {}
    virtual void OnResuming() override {}
    virtual void OnWindowMoved() override {}
    virtual void OnDisplayChange() override {}
    virtual void OnWindowSizeChanged(int width, int height) override {}
};