#pragma once
#include "../Include/IRenderScene.h"

struct IRenderer;
class JsonOperation;

class Scene : public IRenderScene
{
public:
	~Scene();
	Scene(const Rectangle& rect);
	Scene(const Scene& other);
	Scene& operator=(const Scene& other);
	bool operator==(const Scene& o) const noexcept;

	virtual unique_ptr<IRenderScene> Clone() override;
	virtual bool LoadScene(ILoadData* load) override;
	virtual bool SetDatas(IGetValue* value) override;
	virtual bool Update(const Mouse::ButtonStateTracker* mouseTracker) override;
	virtual void RenderScene(IRender* render) override;
	virtual void AddComponent(const Vector2& position, unique_ptr<UIComponent> comp) override;
	virtual UIComponent* GetComponent(const string& name) override;

	//������ �ε��ϴ� �κ�. ������ ������ ���� ���� �ٷ� �ִ´�.
	bool LoadData(const wstring& filename);
	
	void SerializeIO(JsonOperation* operation);

private:
	unique_ptr<UIComponent> m_mainPanel;
};
