#pragma once
#include "../Include/IRenderer.h"

class TestImgui : public IImguiComponent
{
public:
    TestImgui(IRenderer* renderer);
    ~TestImgui();
    void Update();

    virtual void Render(ImGuiIO* io) override;
    //virtual void Update() override;
};

