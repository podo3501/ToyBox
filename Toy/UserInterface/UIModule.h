//#pragma once
//
//struct IRenderer;
//struct ITextureRender;
//struct MouseState;
//struct IMouseEventReceiver;
//class UILayout;
//class UIComponent;
//class Panel;
//class UINameGenerator;
//class TextureResourceBinder;
//class SerializerIO;
//namespace DX { class StepTimer; }
//class UIModule
//{
//protected:
//	UIModule(const UIModule& other);
//
//public:
//	~UIModule();
//	UIModule() noexcept;
//	bool operator==(const UIModule& other) const noexcept;
//	unique_ptr<UIModule> Clone() const;
//
//	bool SetupMainComponent(const UILayout& layout, const string& name,
//		IRenderer* renderer, const wstring& srcBinderFilename);
//	bool SetupMainComponent(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename);
//	void AddRenderer() noexcept;
//	bool BindTextureResources() noexcept;
//	bool Update(const DX::StepTimer& timer) noexcept;
//	void Render(ITextureRender* render) const;
//	void ProcessIO(SerializerIO& serializer);
//	bool Write(const wstring& filename = L"") noexcept;
//	bool EnableToolMode(bool enable);
//	//UIComponent* FindComponent(const string& name) const noexcept;
//	UIComponent* GetMainPanel() const noexcept;
//	const vector<unique_ptr<UIComponent>>& GetChildren() const noexcept;
//	inline IRenderer* GetRenderer() const noexcept { return m_renderer; }
//	inline TextureResourceBinder* GetTexResBinder() const noexcept { return m_resBinder.get(); }
//	inline UINameGenerator* GetNameGenerator() const noexcept { return m_generator.get(); }
//	inline const wstring& GetFilename() const noexcept { return m_filename; }
//
//private:
//	bool Read(const wstring& filename = L"") noexcept;
//	void ReloadDatas() noexcept;
//	bool UpdateMainPanel(const DX::StepTimer& timer) noexcept;
//	void UpdateMouseState() noexcept;
//	void UpdateHoverState(vector<IMouseEventReceiver*> receivers, const XMINT2& pos) noexcept;
//	void ProcessCaptureComponent(const MouseState& mouseState) noexcept;
//	void CaptureComponent(const MouseState& mouseState) noexcept;
//	void ProcessMouseWheel(int wheelValue) noexcept;
//	//UIComponent* FindComponentInRegion(const string& regionName, const string& name) const noexcept;
//
//	unique_ptr<UINameGenerator> m_generator;
//	vector<unique_ptr<UIComponent>> m_children;	//children이지만 panel만 들어가 있다.
//	unique_ptr<TextureResourceBinder> m_resBinder;
//	IRenderer* m_renderer{ nullptr };
//	wstring m_filename;
//	vector<IMouseEventReceiver*> m_hoveredReceivers; //이전 호버된 컴포넌트와 비교해서 OnNormal 호출함.
//	IMouseEventReceiver* m_capture{ nullptr };
//};
//
//unique_ptr<UIModule> CreateUIModule(const UILayout& layout, const string& mainUIName,
//	IRenderer* renderer, const wstring& srcBinderFilename = L"");
//unique_ptr<UIModule> CreateUIModule(const wstring& filename, IRenderer* renderer, const wstring& srcBinderFilename = L"");