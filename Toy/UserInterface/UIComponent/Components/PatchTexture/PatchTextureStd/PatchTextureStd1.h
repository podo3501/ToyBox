#pragma once
#include "PatchTextureStd.h"
#include "../PatchTextureCoord.h"

struct IRenderer;
struct TextureSourceInfo;
namespace DX { class StepTimer; }

class PatchTextureStd1 : public PatchTextureStd
{
public:
	~PatchTextureStd1();
	PatchTextureStd1();

	static ComponentID GetTypeStatic() { return ComponentID::PatchTextureStd1; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;
	//PatchTexture
	virtual bool FitToTextureSource() noexcept override { return m_coord.FitToTextureSource(); }
	virtual UIComponent* GetCenterComponent() noexcept override { return this; }
	virtual bool ChangeBindKey(TextureResourceBinder* resBinder, const string& key) noexcept override;
	virtual const string& GetBindKey() const noexcept override { return m_bindKey; }

	optional<vector<Rectangle>> GetTextureAreaList();
	bool Setup(const UILayout& layout, const string& bindKey, size_t sourceIndex) noexcept;
	void SetSourceInfo(const TextureSourceInfo& sourceInfo, ITextureController* texController) noexcept;
	void ChangeBindKeyWithIndex(const string& key, const TextureSourceInfo& sourceInfo, size_t sourceIndex) noexcept;

	inline const Rectangle& GetSource() const noexcept { return m_coord.GetSource(); }
	inline wstring GetFilename() const noexcept { return m_filename; }
	inline UINT64 GetGraphicMemoryOffset() const noexcept { return m_gfxOffset; }

protected:
	PatchTextureStd1(const PatchTextureStd1& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override; //Binder가 로딩을 다 하고 여기서 값만 연결한다.
	virtual void ImplementRender(ITextureRender* render) const override { m_coord.Render(render); }
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept override { return UIComponent::ImplementChangeSize(size); }

private:
	string m_bindKey;
	size_t m_sourceIndex{ 0 }; //1, 3, 9 Patch 일때 몇번째 인지 기록하는 인덱스. 1 Patch 일때에는 0값.

	ITextureController* m_texController;
	wstring m_filename;
	UINT64 m_gfxOffset{}; //툴에서 Imgui window 만들때 사용
	PatchTextureCoord m_coord;
};

unique_ptr<PatchTextureStd1> CreatePatchTextureStd1(const UILayout& layout, const string& bindKey, size_t sourceIndex = 0u);
inline unique_ptr<PatchTextureStd1> CreatePatchTextureStd1(const string& bindKey, size_t sourceIndex = 0u) {
	return CreatePatchTextureStd1({}, bindKey, sourceIndex);
}