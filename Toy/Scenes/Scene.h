#pragma once
#include <string>
#include "Shared/System/StepTimer.h"

class UIModule;
class Scene
{
public:
	virtual ~Scene() = default;
	Scene(const std::string& name, UIModule* uiModule) :
		m_name{ name },
		m_uiModule{ uiModule }
	{}

	virtual bool Enter() = 0;
	virtual bool Leave() = 0;
	virtual void Update(const DX::StepTimer& timer) = 0;

	const std::string& GetName() const noexcept { return m_name; }
	UIModule* GetUIModule() const noexcept { return m_uiModule; }

private:
	std::string m_name;
	UIModule* m_uiModule;
};
