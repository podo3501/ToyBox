#pragma once
#include "../Include/IRenderer.h"

class TestImgui : public IImguiItem
{
public:
    TestImgui();
    ~TestImgui();

    virtual void Render(ImGuiIO* io) override;
};

