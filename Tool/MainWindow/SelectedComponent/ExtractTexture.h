#pragma once
#include "../Toy/UserInterface/UIComponent.h"

struct IRender;
struct ILoadData;

class ExtractTexture : public UIComponent
{
public:
    virtual unique_ptr<UIComponent> CreateClone() const override { return nullptr; }

    virtual bool LoadResources(ILoadData* load);
    virtual void Render(IRender* render) const override;

    void SetImage(const wstring& filename) { m_filename = filename; }

private:
    size_t m_index{ 0 };
    wstring m_filename;
};
