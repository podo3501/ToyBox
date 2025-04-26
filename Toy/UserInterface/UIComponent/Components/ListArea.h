#pragma once
#include "../UIComponent.h"

class RenderTexture;
class TextureSwitcher;
class ScrollBar;
namespace DX { class StepTimer; }

//RenderTexture�� Prototype TextureSwitcher, ScrollBar�� �����ؼ� ��������� ������Ʈ
//�� ������Ʈ���� �ʿ��� ���� ���ͼ� ���⼭ �����Ѵ�. �� ������Ʈ�� ���������� �۵��Ѵ�.
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
	virtual bool ImplementChangeSize(const XMUINT2& size, bool isForce) noexcept;

private:
	void ReloadDatas() noexcept;
	bool ResizeContainerForScrollbar() noexcept;
	bool UpdateScrollBar() noexcept;
	XMUINT2 GetUsableContentSize() const noexcept;
	void UpdateContainersScroll(const DX::StepTimer& timer) noexcept;
	void ScrollContainers(const DX::StepTimer& timer) noexcept;
	void CheckMouseInteraction() noexcept;
	int32_t GetContainerHeight() const noexcept;
	void OnScrollChangedCB(float ratio);
	void MoveContainers(int32_t targetPos) noexcept;
	bool ChangeScrollBarSizeAndPos(const XMUINT2& size) noexcept;

	TextureSwitcher* m_prototypeContainer;
	UIComponent* m_bgImage;
	ScrollBar* m_scrollBar;
	RenderTexture* m_renderTex;
	uint32_t m_scrollPadding{ 2 };
	
	vector<UIComponent*> m_containers; //�̰� �������� �ʴ´�. ����ÿ� ä������ �������̴�.
};

unique_ptr<ListArea> CreateListArea(const UILayout& layout,
	unique_ptr<UIComponent> bgImage,
	unique_ptr<TextureSwitcher> switcher,
	unique_ptr<ScrollBar> scrollBar);

unique_ptr<ListArea> CreateListArea(
	unique_ptr<UIComponent> bgImage,
	unique_ptr<TextureSwitcher> switcher,
	unique_ptr<ScrollBar> scrollBar);