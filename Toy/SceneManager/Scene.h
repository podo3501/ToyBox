#pragma once
#include <string>

class Scene
{
public:
	virtual ~Scene() = default;
	Scene(const std::string& name) :
		m_name{ name }
	{}

	virtual bool Enter() = 0;
	virtual bool Leave() = 0;

	const std::string& GetName() const noexcept { return m_name; }

private:
	std::string m_name;
};
