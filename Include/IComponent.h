#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;

struct IComponent
{
public:
	virtual ~IComponent() {};

	virtual bool LoadResources(ILoadData* load) = 0;
	virtual bool SetDatas(IGetValue*) = 0;
	virtual bool ProcessUpdate(const XMINT2& position, bool activeUpdate) noexcept = 0;
	virtual void ProcessRender(IRender* render) = 0;
};
