#pragma once

#include "../Include/IComponent.h"

class UILayout;
class JsonOperation;
class TransformComponent;
enum class Origin;

class UIComponent : public IComponent
{
protected:
	UIComponent();	//이 클래스는 단독으로 만들 수 없다. 상속 받은 클래스만이 생성 가능

public:
	virtual ~UIComponent();
	UIComponent(const string& name, const Rectangle& rect);
	UIComponent(const UIComponent& other);
	UIComponent(UIComponent&& o) noexcept;
	UIComponent& operator=(const UIComponent& other);
	virtual bool operator==(const UIComponent& other) const noexcept;
	virtual string GetType() const;

	virtual unique_ptr<UIComponent> Clone();
	virtual void Render(IRender* render);
	virtual bool LoadResources(ILoadData* load);
	virtual bool SetDatas(IGetValue*);
	virtual bool Update(const XMINT2& position, InputManager* inputManager) noexcept;

	void AddComponent(unique_ptr<UIComponent>&& comp, const Vector2& pos);
	void SetSize(const XMUINT2& size);
	XMUINT2 GetSize() const noexcept;
	bool ChangeArea(const Rectangle& area) noexcept;
	void ChangeOrigin(const Origin& origin) noexcept;
	bool IsPicking(const XMINT2& pos)  const noexcept;

	UIComponent* GetComponent(const string& name) const noexcept;
	void GetComponents(const XMINT2& pos, vector<UIComponent*>& outList) noexcept;
	const Rectangle& GetArea() const noexcept;	
	void SetChildPosition(const string& name, const Vector2& pos) noexcept;

	const string& GetName() const noexcept;
	void SetName(const string& name) noexcept;

	const wstring& GetFilename() const noexcept;
	void SetFilename(const wstring& filename) noexcept;

	UILayout* GetLayout() const noexcept;
	void SetLayout(const UILayout& layout) noexcept;

	void ClearSelected() noexcept;
	void SetSelected(bool selected) noexcept;
	bool GetSelected() const noexcept;

	virtual void SerializeIO(JsonOperation& operation);

protected:
	XMINT2 GetPositionByLayout(const XMINT2& position) noexcept;
	
private:
	TransformComponent* FindTransformComponent(const string& name) const noexcept;

	string m_name{};
	wstring m_filename{};
	unique_ptr<UILayout> m_layout;
	bool m_selected{ false };
	vector<unique_ptr<TransformComponent>> m_components;
};