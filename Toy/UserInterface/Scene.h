#pragma once
#include "../Include/IRenderItem.h"

struct IRenderer;

class Scene : public IRenderScene
{
public:
	Scene(IRenderer* renderer);
	~Scene();

	virtual bool LoadScene(ILoadData* load) override;
	virtual void RenderScene(IRender* render) override;

	//������ �ε��ϴ� �κ�. ������ ������ ���� ���� �ٷ� �ִ´�.
	bool LoadData(const wstring& filename);

private:
	IRenderer* m_renderer;
	vector<pair<Vector2, unique_ptr<IRenderItem>>> m_renderItems;
};
