#pragma once

struct ILoadData;
struct IRender;
class UIComponent;
class Mouse::ButtonStateTracker;

struct IRenderScene
{
public:
	virtual ~IRenderScene() {};

	virtual unique_ptr<IRenderScene> Clone() = 0;
	virtual bool LoadScene(ILoadData* load) = 0;
	virtual bool Update(const Mouse::ButtonStateTracker* mouseTracker) = 0;
	virtual void RenderScene(IRender* render) = 0;
	virtual void AddComponent(const Vector2& position, unique_ptr<UIComponent> comp) = 0;
	virtual UIComponent* GetComponent(const string& name) = 0;
};
