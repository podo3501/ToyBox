#pragma once
#include "../Include/IRenderScene.h"

struct IRenderer;

class Scene : public IRenderScene
{
public:
	~Scene();
	Scene() = delete;
	Scene(IRenderer* renderer);
	Scene(const Scene& other);

	virtual unique_ptr<IRenderScene> Clone() override;
	virtual bool LoadScene(ILoadData* load) override;
	virtual bool Update(const Mouse::ButtonStateTracker* mouseTracker) override;
	virtual void RenderScene(IRender* render) override;
	virtual void AddComponent(const Vector2& position, unique_ptr<UIComponent> comp) override;
	virtual UIComponent* GetComponent(const string& name) override;

	//������ �ε��ϴ� �κ�. ������ ������ ���� ���� �ٷ� �ִ´�.
	bool LoadData(const wstring& filename);

private:
	IRenderer* m_renderer;
	vector<pair<Vector2, unique_ptr<UIComponent>>> m_components;
};
