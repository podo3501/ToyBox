#include "IRenderer.h"

struct ITextureLoadStub : public ITextureLoad
{
    virtual ~ITextureLoadStub() {};

    virtual bool LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize, UINT64* outGfxMemOffset) override { return true; }
    virtual bool LoadFont(const wstring& filename, size_t& outIndex) override { return true; }
    virtual void ReleaseTexture(size_t idx) noexcept override {}
    virtual void AddRef(size_t index) noexcept override {}
};

struct ITextureControllerStub : public ITextureController
{
    virtual ~ITextureControllerStub() {};

    //Text관련
    virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) override { return {}; }
    virtual float GetLineSpacing(size_t index) const noexcept override { return 0.f; }

    //Texture
    virtual bool CreateRenderTexture(IComponent* component, const XMUINT2& size, const XMINT2& position, size_t& outIndex, UINT64* outGfxMemOffset) override { return true; }
    virtual optional<vector<Rectangle>> GetTextureAreaList(size_t index, const UINT32& bgColor) override { return nullopt; }
    virtual void ModifyRenderTexturePosition(size_t index, const XMINT2& position) noexcept override {}
    virtual bool ModifyRenderTextureSize(size_t index, const XMUINT2& size) override { return true; }

    //?!? 밑에 두 함수는 RenderTexture에서 쓰긴 한데.. 없애고 싶긴하다.
    virtual void ReleaseTexture(size_t idx) noexcept override {}
    virtual void AddRef(size_t index) noexcept override {}
};

struct ITextureRenderStub : public ITextureRender
{
public:
    virtual ~ITextureRenderStub() {};

    virtual void Render(size_t index, const RECT& dest, const RECT* source) override {}
    virtual void DrawString(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color) const override {}
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