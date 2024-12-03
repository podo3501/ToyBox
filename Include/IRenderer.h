#pragma once

//�ε��Ҷ� ����ϴ� �������̽�
struct ILoadData
{
public:
    virtual ~ILoadData() {};

    virtual bool LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize) = 0;
    virtual bool LoadFont(const wstring& filename, size_t& outIndex) = 0;
};

//�����͸� ���ö� ����ϴ� �������̽�
struct IGetValue
{
public:
    virtual ~IGetValue() {};

    virtual Rectangle MeasureText(size_t index, const wstring& text, const Vector2& position) = 0;
    virtual float GetLineSpacing(size_t index) const noexcept = 0;
};

//������ �Ҷ� ����ϴ� �������̽�
struct IRender
{
public:
    virtual ~IRender() {};

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

    //Scene�� Component�� �ְ� �о���� �����͸� ������ LoadScene�� ȣ���Ѵ�. 
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