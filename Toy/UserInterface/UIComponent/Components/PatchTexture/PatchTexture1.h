#pragma once
#include "PatchTexture.h"
#include "PatchTextureCoord.h"

struct IRenderer;
struct TextureSourceInfo;
namespace DX { class StepTimer; }

class PatchTexture1 : public PatchTexture
{
public:
	~PatchTexture1();
	PatchTexture1();

	static ComponentID GetTypeStatic() { return ComponentID::PatchTexture1; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;
	//PatchTexture
	virtual void ChangeBindKey(const string& key, const TextureSourceInfo& sourceInfo) noexcept override;
	virtual const string& GetBindKey() const noexcept override { return m_bindKey; }
	virtual void SetIndexedSource(size_t index, const vector<Rectangle>& source) noexcept override { m_coord.SetIndexedSource(index, source); }
	virtual bool FitToTextureSource() noexcept override { return m_coord.FitToTextureSource(); }

	optional<vector<Rectangle>> GetTextureAreaList();
	bool Setup(const UILayout& layout, const string& bindKey, size_t sourceIndex) noexcept;
	void SetSourceInfo(const TextureSourceInfo& sourceInfo, ITextureController* texController) noexcept;
	void ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept;

	inline const Rectangle& GetSource() const noexcept { return m_coord.GetSource(); }
	inline wstring GetFilename() const noexcept { return m_filename; }
	inline UINT64 GetGraphicMemoryOffset() const noexcept { return m_gfxOffset; }

protected:
	PatchTexture1(const PatchTexture1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override; //Binder가 로딩을 다 하고 여기서 값만 연결한다.
	virtual void ImplementRender(ITextureRender* render) const override { m_coord.Render(render); }

private:
	string m_bindKey;
	size_t m_sourceIndex{ 0 }; //1, 3, 9 Patch 일때 몇번째 인지 기록하는 인덱스. 1 Patch 일때에는 0값.

	ITextureController* m_texController;
	wstring m_filename;
	UINT64 m_gfxOffset{}; //툴에서 Imgui window 만들때 사용
	PatchTextureCoord m_coord;
};

unique_ptr<PatchTexture1> CreatePatchTexture1(const UILayout& layout, const string& bindKey, size_t sourceIndex = 0u);