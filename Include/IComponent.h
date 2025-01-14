#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;
class InputManager;

struct IComponent
{
public:
	virtual ~IComponent() {};

	virtual bool LoadResources(ILoadData* load) = 0;
	virtual bool SetDatas(IGetValue*) = 0;
	virtual bool RefreshPosition() noexcept = 0;
	virtual bool ProcessUpdate(const XMINT2& position, const InputManager& inputManager) noexcept = 0;
	virtual void ProcessRender(IRender* render) = 0;
};
