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
	virtual bool Update(const XMINT2& position, InputManager* inputManager) noexcept = 0;
	virtual void Render(IRender* render) = 0;
};
