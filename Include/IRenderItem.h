#pragma once

struct ILoadData;
struct IRenderer;

struct IRenderItem
{
public:
	virtual ~IRenderItem() {};
	virtual void LoadResources(ILoadData* load) = 0;
	virtual void Render(IRenderer* renderer) = 0;
};
