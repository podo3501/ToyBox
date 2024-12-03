#pragma once

//로딩할때 사용하는 인터페이스
struct ILoadData
{
public:
    virtual ~ILoadData() {};

    virtual bool LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize) = 0;
    virtual bool LoadFont(const wstring& filename, size_t& outIndex) = 0;
};

//데이터를 얻어올때 사용하는 인터페이스
struct IGetValue
{
public:
    virtual ~IGetValue() {};

    virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) = 0;
    virtual float GetLineSpacing(size_t index) const noexcept = 0;
};

//렌더링 할때 사용하는 인터페이스
struct IRender
{
public:
    virtual ~IRender() {};

    virtual void Render(size_t index, const RECT& dest, const RECT* source) = 0;
    virtual void DrawString(size_t index, const wstring& text, const Vector2& pos, const FXMVECTOR& color) const = 0;
};

//Imgui ui 만들때 사용
struct ImGuiIO;
struct IImguiComponent
{
public:
    virtual ~IImguiComponent() {};

    virtual void Render(ImGuiIO* io) = 0;
    //virtual void Update() = 0;
};

struct IComponent;

struct IRenderer
{
public:
    virtual ~IRenderer() {};

    virtual bool Initialize() = 0;

    virtual void AddComponent(IComponent* component, bool render) = 0;
    virtual void RemoveComponent(IComponent* component) = 0;

    virtual void AddImguiComponent(IImguiComponent* item) = 0;
    virtual void RemoveImguiComponent(IImguiComponent* comp) noexcept = 0;

    //Scene에 Component를 넣고 읽어들일 데이터를 셋팅후 LoadScene을 호출한다. 
    virtual bool LoadComponents() = 0;

    virtual IGetValue* GetValue() const noexcept = 0;
    virtual bool CreateRenderTexture(const XMUINT2& size, IComponent* component, ImTextureID& outTextureID) = 0;
    virtual bool ModifyRenderTexture(ImTextureID id, const XMUINT2& size) = 0;
    virtual void RemoveRenderTexture(ImTextureID textureID) = 0;
    virtual void Draw() = 0;

    virtual void OnActivated() = 0;
    virtual void OnDeactivated() = 0;
    virtual void OnSuspending() = 0;
    virtual void OnResuming() = 0;
    virtual void OnWindowMoved() = 0;
    virtual void OnDisplayChange() = 0;
    virtual void OnWindowSizeChanged(int width, int height) = 0;
};

std::unique_ptr<IRenderer> CreateRenderer(HWND hwnd, int width, int height, bool bUseImgui);