#pragma once
#include <string>
#include "Shared/System/StepTimer.h"

struct IRenderer;
class UIModule;
class Scene
{
public:
	virtual ~Scene() = default;
	Scene(const std::string& name, IRenderer* renderer) :
		m_name{ name },
		m_renderer{ renderer }
	{}

	virtual bool Enter() = 0;
	virtual bool Leave() = 0;
	virtual void Update(const DX::StepTimer& timer) = 0;

	inline const std::string& GetName() const noexcept { return m_name; }
	inline IRenderer* GetRenderer() const noexcept { return m_renderer; }

private:
	std::string m_name;
	IRenderer* m_renderer;
};
