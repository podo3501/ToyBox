#pragma once

struct IRenderItem;

//�ε��Ҷ� ����ϴ� �������̽�
struct ILoadData
{
public:
    virtual ~ILoadData() {};

    virtual bool LoadTexture(const wstring& filename, const Rectangle* rect, size_t& outIndex, XMUINT2* outSize) = 0;
    virtual bool LoadFont(const wstring& filename, size_t& outIndex) = 0;
};

//�����͸� ���ö� ����ϴ� �������̽�
struct IUpdate
{
public:
    virtual ~IUpdate() {};

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

struct IRenderer
{
public:
    virtual ~IRenderer() {};

    virtual bool Initialize() = 0;
    virtual void AddRenderItem(IRenderItem* item) = 0;
    virtual bool LoadResources() = 0;
    virtual IUpdate* GetUpdate() const noexcept = 0;
    virtual void Draw() = 0;

    virtual void OnActivated() = 0;
    virtual void OnDeactivated() = 0;
    virtual void OnSuspending() = 0;
    virtual void OnResuming() = 0;
    virtual void OnWindowMoved() = 0;
    virtual void OnDisplayChange() = 0;
    virtual void OnWindowSizeChanged(int width, int height) = 0;
};

std::unique_ptr<IRenderer> CreateRenderer(HWND hwnd, int width, int height);