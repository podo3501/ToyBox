#pragma once
#include "../Include/IRenderer.h"

class TestImgui : public IImguiItem
{
public:
    TestImgui(IRenderer* renderer);
    ~TestImgui();

    virtual void Render(ImGuiIO* io) override;
    virtual void Update() override;
};

