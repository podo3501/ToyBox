#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;

struct IRenderItem
{
public:
	virtual ~IRenderItem() {};
	virtual bool LoadResources(ILoadData* load) = 0;
	virtual void Render(IRender* render) = 0;
};
