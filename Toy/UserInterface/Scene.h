#pragma once
#include "../Include/IRenderScene.h"

struct IRenderer;

class Scene : public IRenderScene
{
public:
	~Scene();
	Scene();
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

	//파일을 로드하는 부분. 지금은 파일이 없이 값을 바로 넣는다.
	bool LoadData(const wstring& filename);
	
	friend void to_json(nlohmann::ordered_json& j, const Scene& data);
	friend void from_json(const nlohmann::json& j, Scene& data);

private:
	unique_ptr<UIComponent> m_mainComponent;
};
