#pragma once

class BaseTraverser;
class DerivedTraverser;
class NameTraverser;
struct Traversers
{
public:
	~Traversers();
	Traversers();

	inline BaseTraverser* GetBaseTraverser() const noexcept { return m_baseTraverser.get(); }
	inline DerivedTraverser* GetDerivedTraverser() const noexcept { return m_derivedTraverser.get(); }
	inline NameTraverser* GetNameTraverser() const noexcept { return m_nameTraverser.get(); }

private:
	unique_ptr<BaseTraverser> m_baseTraverser;
	unique_ptr<DerivedTraverser> m_derivedTraverser;
	unique_ptr<NameTraverser> m_nameTraverser;
};