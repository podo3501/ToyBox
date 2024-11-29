#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;
class MouseTracker;
class UIComponent;

struct IRenderScene
{
public:
	virtual ~IRenderScene() {};

	virtual unique_ptr<IRenderScene> Clone() = 0;
	virtual bool LoadScene(ILoadData* load) = 0;
	virtual bool SetDatas(IGetValue* value) = 0;
	virtual bool Update(MouseTracker* mouseTracker) noexcept = 0;
	virtual void RenderScene(IRender* render) = 0;
	virtual void AddComponent(const Vector2& position, unique_ptr<UIComponent> comp) = 0;
	virtual UIComponent* GetComponent(const string& name) = 0;
};
