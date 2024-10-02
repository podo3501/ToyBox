#pragma once

struct ILoadData;
struct IRender;

struct IRenderItem
{
public:
	virtual ~IRenderItem() {};
	virtual void LoadResources(ILoadData* load) = 0;
	virtual void Render(IRender* render) = 0;
};
