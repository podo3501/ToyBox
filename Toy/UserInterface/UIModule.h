#pragma once

struct IRenderer;
struct ITextureRender;
struct IMouseEventReceiver;
struct MouseState;
class TextureResourceBinder;
class UINameGenerator;
class Panel;
class UILayout;
class SerializerIO;
namespace DX { class StepTimer; }
class UIModule
{
public:
	~UIModule();
	UIModule();
	bool operator==(const UIModule& other) const noexcept;

	bool SetupMainComponent(const UILayout& layout, const string& name,
		IRenderer* renderer, const wstring& srcBinderFilename);
	bool SetupMainComponent(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename);
	bool BindTextureResources() noexcept;
	bool Update(const DX::StepTimer& timer) noexcept;
	void Render(ITextureRender* render) const;
	void ProcessIO(SerializerIO& serializer);
	bool Write(const wstring& filename = L"") noexcept;
	bool EnableToolMode(bool enable) noexcept;

	inline UINameGenerator* GetNameGenerator() const noexcept { return m_nameGen.get(); }
	inline TextureResourceBinder* GetTexResBinder() const noexcept { return m_resBinder.get(); }
	inline const wstring& GetFilename() const noexcept { return m_filename; }
	Panel* GetMainPanel() const noexcept;

private:
	void UpdateMouseState() noexcept;
	void UpdateHoverState(vector<IMouseEventReceiver*> receivers, const XMINT2& pos) noexcept;
	void ProcessCaptureComponent(const MouseState& mouseState) noexcept;
	void CaptureComponent(const MouseState& mouseState) noexcept;
	void ProcessMouseWheel(int wheelValue) noexcept;
	void ReloadDatas() noexcept;
	bool Read(const wstring& filename = L"") noexcept;

	IRenderer* m_renderer{ nullptr };
	unique_ptr<TextureResourceBinder> m_resBinder;
	unique_ptr<UINameGenerator> m_nameGen;
	unique_ptr<Panel> m_mainPanel;
	wstring m_filename;
	vector<IMouseEventReceiver*> m_hoveredReceivers; //이전 호버된 컴포넌트와 비교해서 OnNormal 호출함.
	IMouseEventReceiver* m_capture{ nullptr }; //?!? 마우스 관련된 것은 클래스를 만들어서 빼야할듯.
};
