#pragma once
#include "../Toy/UserInterface/UIComponent.h"

struct IRender;
struct ILoadData;

class ExtractTexture : public UIComponent
{
public:
    static ComponentID GetTypeStatic() { return ComponentID::Unknown; }
    virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
    virtual bool LoadResources(ILoadData* load);
    void SetImage(const wstring& filename) { m_filename = filename; }

protected:
    virtual unique_ptr<UIComponent> CreateClone() const override { return nullptr; }
    virtual void ImplementRender(IRender* render) const override;

private:
    size_t m_index{ 0 };
    wstring m_filename;
};
