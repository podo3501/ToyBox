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

	//파일을 로드하는 부분. 지금은 파일이 없이 값을 바로 넣는다.
	bool LoadData(const wstring& filename);

private:
	IRenderer* m_renderer;
	vector<pair<Vector2, unique_ptr<IRenderItem>>> m_renderItems;
};
