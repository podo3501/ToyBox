#pragma once

struct IComponent;
struct ITextureBinder;
//�ε��Ҷ� ����ϴ� �������̽�

struct ITextureLoad
{
public:
    virtual ~ITextureLoad() {};

    virtual bool LoadTexture(const wstring& filename, size_t& outIndex, XMUINT2* outSize, UINT64* outGfxMemOffset) = 0;
    virtual bool LoadFont(const wstring& filename, size_t& outIndex) = 0;
    virtual void ReleaseTexture(size_t idx) noexcept = 0;
    virtual void AddRef(size_t index) noexcept = 0; //Clone������ refcount�� ���߾� �ֱ����� ����ϴ� �Լ��ε�, TextureResourceBinder�� Clone�� �ʿ䰡 ���� ������ �� �Ⱦ��� ���ɼ��� �ִ�.
};

//�����͸� ���ö� ����ϴ� �������̽�
struct ITextureController
{
public:
    virtual ~ITextureController() {};

    //Text����
    virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) = 0;
    virtual float GetLineSpacing(size_t index) const noexcept = 0;

    //Texture
    virtual bool CreateRenderTexture(IComponent* component, const XMUINT2& size, const XMINT2& position, size_t& outIndex, UINT64* outGfxMemOffset) = 0;
    virtual optional<vector<Rectangle>> GetTextureAreaList(size_t index, const UINT32& bgColor) = 0;
    virtual void ModifyRenderTexturePosition(size_t index, const XMINT2& position) noexcept = 0;
    virtual bool ModifyRenderTextureSize(size_t index, const XMUINT2& size) = 0;

    //?!? �ؿ� �� �Լ��� RenderTexture���� ���� �ѵ�.. ���ְ� �ͱ��ϴ�.
    virtual void ReleaseTexture(size_t idx) noexcept = 0; 
    virtual void AddRef(size_t index) noexcept = 0;
};

struct ITextureRender
{
public:
    virtual ~ITextureRender() {};

    virtual void Render(size_t index, const RECT& dest, const RECT* source) = 0;
    virtual void DrawString(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color) const = 0;
};

//Imgui ui ���鶧 ���
struct ImGuiIO;
struct IImguiComponent
{
public:
    virtual ~IImguiComponent() {};

    virtual void Render(ImGuiIO* io) = 0;
    //virtual void Update() = 0;
};

struct IRenderer
{
public:
    virtual ~IRenderer() {};

    virtual bool Initialize() = 0;

    virtual void AddRenderComponent(IComponent* component) = 0;
    virtual void RemoveRenderComponent(IComponent* component) = 0;

    virtual void AddImguiComponent(IImguiComponent* item) = 0;
    virtual void RemoveImguiComponent(IImguiComponent* comp) noexcept = 0;

    virtual bool LoadTextureBinder(ITextureBinder* textureBinder) = 0;

    virtual void Draw() = 0;
    virtual ITextureController* GetTextureController() const noexcept = 0;
    
    virtual void OnActivated() = 0;
    virtual void OnDeactivated() = 0;
    virtual void OnSuspending() = 0;
    virtual void OnResuming() = 0;
    virtual void OnWindowMoved() = 0;
    virtual void OnDisplayChange() = 0;
    virtual void OnWindowSizeChanged(int width, int height) = 0;
};

std::unique_ptr<IRenderer> CreateRenderer(HWND hwnd, int width, int height, bool bUseImgui);