#pragma once

class UINameGenerator;
class BaseTraverser;
class NameTraverser;
class UIComponentManager
{
public:
	~UIComponentManager();
	UIComponentManager();

	UINameGenerator* GetNameGenerator() { return m_nameGenerator.get(); }
	NameTraverser* GetNameTraverser() { return m_nameTraverser.get(); }
	BaseTraverser* GetBaseTraverser() { return m_baseTraverser.get(); }

private:
	unique_ptr<UINameGenerator> m_nameGenerator;
	unique_ptr<BaseTraverser> m_baseTraverser;
	unique_ptr<NameTraverser> m_nameTraverser;
};