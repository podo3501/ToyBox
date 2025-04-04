#pragma once
#include "ImageGrid.h"

struct IRenderer;
struct TextureSourceInfo;
namespace DX { class StepTimer; }

class ImageGrid1 : public ImageGrid
{
public:
	~ImageGrid1();
	ImageGrid1();

	static ComponentID GetTypeStatic() { return ComponentID::ImageGrid1; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept override;

	optional<vector<Rectangle>> GetTextureAreaList();
	bool Setup(const UILayout& layout, const string& bindKey, size_t sourceIndex) noexcept;
	void SetSourceInfo(const TextureSourceInfo& sourceInfo, ITextureController* texController) noexcept;

	inline const string& GetBindKey() const noexcept { return m_bindKey; }
	void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex = 0u) noexcept;
	inline void SetSource(const Rectangle& source) noexcept { m_source = source; }
	inline const Rectangle& GetSource() const noexcept { return m_source; }
	inline wstring GetFilename() const noexcept { return m_filename; }
	inline UINT64 GetGraphicMemoryOffset() const noexcept { return m_gfxOffset; }

protected:
	ImageGrid1(const ImageGrid1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override; //Binder가 로딩을 다 하고 여기서 값만 연결한다.
	virtual void ImplementRender(ITextureRender* render) const override;

private:
	string m_bindKey;
	size_t m_sourceIndex{ 0 };

	ITextureController* m_texController;
	optional<size_t> m_index; //0값도 인덱스로 사용하기 때문에 optional
	wstring m_filename;//
	Rectangle m_source{};
	UINT64 m_gfxOffset{}; //툴에서 Imgui window 만들때 사용
};

unique_ptr<ImageGrid1> CreateImageGrid1(const UILayout& layout, const string& bindKey, size_t sourceIndex = 0u);