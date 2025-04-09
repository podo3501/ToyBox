#pragma once
#include "../UIComponent.h"

class RenderTexture;
class TextureSwitcher;
class ScrollBar;
class ScrollSlider;
namespace DX { class StepTimer; }

//RenderTexture와 Prototype TextureSwitcher, SlideBar를 조합해서 만들어지는 컴포넌트
//각 컴포넌트에서 필요한 것을 들고와서 여기서 조합한다. 각 컴포넌트는 독립적으로 작동한다.
class ListArea : public UIComponent
{
public:
	~ListArea();
	ListArea() noexcept;

	static ComponentID GetTypeStatic() { return ComponentID::ListArea; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool Setup(const UILayout& layout, unique_ptr<UIComponent> bgImage, 
		unique_ptr<TextureSwitcher> switcher, unique_ptr<ScrollBar> scrollBar) noexcept;
	TextureSwitcher* GetPrototypeContainer() noexcept { return m_prototypeContainer; }
	UIComponent* PrepareContainer();
	inline UIComponent* GetContainer(unsigned int idx) const noexcept;
	bool RemoveContainer(unsigned int idx) noexcept;
	void ClearContainers() noexcept;

protected:
	ListArea(const ListArea& o) noexcept;
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	void ReloadDatas() noexcept;
	void ResizeContainerForScrollbar() noexcept;
	void UpdateScrollBar() noexcept;
	XMUINT2 GetUsableContentSize() const noexcept;
	void UpdateContainersScroll(const DX::StepTimer& timer) noexcept;
	void ScrollContainers(const DX::StepTimer& timer) noexcept;
	void CheckMouseInteraction() noexcept;
	int32_t GetContainerHeight() const noexcept;
	void OnScrollChangedCB(float ratio);
	void MoveContainers(int32_t targetPos) noexcept;

	TextureSwitcher* m_prototypeContainer;
	UIComponent* m_bgImage;
	ScrollBar* m_scrollBar;
	ScrollSlider* m_scrollSlider;
	RenderTexture* m_renderTex;
	
	vector<UIComponent*> m_containers; //이건 저장하지 않는다. 실행시에 채워지는 데이터이다.
};

unique_ptr<ListArea> CreateListArea(const UILayout& layout,
	unique_ptr<UIComponent> bgImage,
	unique_ptr<TextureSwitcher> switcher,
	unique_ptr<ScrollBar> scrollBar);