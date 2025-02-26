#pragma once
#include "../UIComponent.h"

struct IRenderer;
struct ImageSource;
class ImagePart;
namespace DX { class StepTimer; }

class ImageGrid1 : public UIComponent
{
public:
	~ImageGrid1();
	ImageGrid1();

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid1; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	optional<vector<Rectangle>> GetTextureAreaList();
	bool SetImage(const UILayout& layout, const ImageSource& source) noexcept;
	void SetFilenameToLoadInfo(const wstring& filename) noexcept;

	inline void SetSource(const Rectangle& source) noexcept { m_source = source; }
	inline const Rectangle& GetSource() const noexcept { return m_source; }
	inline void SetFilename(const wstring& filename) noexcept { m_filename = filename; }
	inline optional<wstring> GetFilename() const noexcept { return m_filename; }
	inline UINT64 GetGraphicMemoryOffset() const noexcept { return m_gfxOffset; }

protected:
	ImageGrid1(const ImageGrid1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementLoadResource(ITextureLoad* load) override;
	virtual bool ImplementPostLoaded(ITextureController* texController) override;
	virtual bool ImplementUpdatePosition(const DX::StepTimer& timer, const XMINT2& position) noexcept override;
	virtual void ImplementRender(ITextureRender* render) const override;

private:
	optional<size_t> m_index; //0값도 인덱스로 사용하기 때문에 optional
	wstring m_filename;
	Rectangle m_source{};
	XMINT2 m_position{};
	UINT64 m_gfxOffset{};
	ITextureController* m_texController;
};
