#pragma once

class UINameGenerator;
class AttachmentTraverser;
class NameTraverser;
class UIComponentManager
{
public:
	UIComponentManager();

	UINameGenerator* GetNameGenerator() { return m_nameGenerator.get(); }
	NameTraverser* GetNameTraverser() { return m_nameTraverser.get(); }
	AttachmentTraverser* GetAttachmentTraverser() { return m_attachmentTraverser.get(); }

private:
	unique_ptr<UINameGenerator> m_nameGenerator;
	unique_ptr<AttachmentTraverser> m_attachmentTraverser;
	unique_ptr<NameTraverser> m_nameTraverser;
};