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
	virtual bool Update(MouseTracker* mouseTracker) noexcept override;
	virtual void RenderScene(IRender* render) override;
	virtual void AddComponent(const Vector2& position, unique_ptr<UIComponent> comp) override;
	virtual UIComponent* GetComponent(const string& name) override;

	bool LoadFile(const wstring& filename);
	bool SaveFile(const wstring& filename);
	inline const wstring& GetFilename() const noexcept { return m_filename; }
	void SetSize(const XMUINT2& size);
	XMUINT2 GetSize() const noexcept;
	void SerializeIO(JsonOperation& operation);

private:
	wstring m_filename;
	unique_ptr<UIComponent> m_mainPanel;
};
