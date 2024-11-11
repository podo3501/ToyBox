#pragma once

struct ILoadData;
struct IGetValue;
struct IRender;
class UILayout;

class IRenderItem
{
public:
	IRenderItem();
	virtual ~IRenderItem();
	IRenderItem(const IRenderItem& other);

	virtual unique_ptr<IRenderItem> Clone() = 0;
	virtual bool ReadProperty(const nlohmann::json&) { return true; }	//Property는 Component의 구현부이기 때문에 상속받지 않기 때문에 각각의 클래스에서 재정의가 필요하다.
	virtual void Render(IRender* render);
	virtual bool SetResources(const wstring& filename);
	virtual bool LoadResources(ILoadData* load);
	virtual bool Update(const Vector2& position, const Mouse::ButtonStateTracker* mouseTracker) noexcept;

	void AddComponent(unique_ptr<IRenderItem>&& item, const Vector2& pos);
	bool ChangeArea(const Rectangle& area) noexcept;
	bool IsPicking(const Vector2& pos)  const noexcept;

	IRenderItem* GetRenderItem(const string& name) const noexcept;
	IRenderItem* GetSelected() const noexcept;
	const Rectangle& GetArea() const noexcept;
	const string& GetName() const noexcept;
	UILayout* GetLayout() const noexcept;

	void SetSelected(const string& name, bool selected) noexcept;
	void SetChildPosition(const string& name, const Vector2& pos) noexcept;
	void SetName(const string& name) noexcept;
	void SetLayout(const UILayout& layout) noexcept;
	
private:
	class Implementation;
	unique_ptr<Implementation> m_impl;
};