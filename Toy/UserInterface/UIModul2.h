#pragma once

struct IRenderer;
class TextureResourceBinder;
class UINameGenerator;
class Panel;
class UILayout;
class UIModul2
{
public:
	~UIModul2();
	UIModul2();

	bool SetupMainComponent(const UILayout& layout, const string& name,
		IRenderer* renderer, const wstring& srcBinderFilename);

	inline UINameGenerator* GetNameGenerator() const noexcept { return m_nameGen.get(); }
	inline TextureResourceBinder* GetTexResBinder() const noexcept { return m_resBinder.get(); }
	inline Panel* GetMainPanel() const noexcept { return m_mainPanel.get(); }

private:
	IRenderer* m_renderer{ nullptr };
	unique_ptr<TextureResourceBinder> m_resBinder;
	unique_ptr<UINameGenerator> m_nameGen;
	unique_ptr<Panel> m_mainPanel;
};
