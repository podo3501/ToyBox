#pragma once
#include "../UIComponent.h"

struct IRenderer;
struct ImageSource;
class ImagePart;

class ImageGrid1 : public UIComponent
{
public:
	~ImageGrid1();
	ImageGrid1();

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid1; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool SetDatas(IGetValue* value) override;
	virtual void SerializeIO(JsonOperation& operation) override;

	optional<vector<Rectangle>> GetTextureAreaList();
	bool SetImage(const UILayout& layout, const ImageSource& source) noexcept;
	void SetFilenameToLoadInfo(const wstring& filename) noexcept;

	inline void SetSource(const Rectangle& source) noexcept { m_source = source; }
	inline const Rectangle& GetSource() const noexcept { return m_source; }
	inline void SetFilename(const wstring& filename) noexcept { m_filename = filename; }
	inline optional<wstring> GetFilename() const noexcept { return m_filename; }

protected:
	ImageGrid1(const ImageGrid1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementUpdatePosition(const XMINT2& position) noexcept override;
	virtual void ImplementRender(IRender* render) const override;

private:
	size_t m_index{ 0 };
	wstring m_filename;
	Rectangle m_source{};
	XMINT2 m_position{};
	IGetValue* m_resourceInfo;
};
