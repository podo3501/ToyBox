#pragma once
#include "UIFixture.h"
#include "SystemFixture.h"
//���־� ��Ʃ��� �׽�Ʈ Ž���⿡ �������� ǥ���ϱ� ���ؼ� Fixture�� ��ӹ޾Ƽ� ��.
//namespace ��ø���δ� ���������� ��Ÿ���� ����. a::b �̷������� ��Ÿ��. ��, �������� �׽�Ʈ �ϱ� �����

class BasicComponent : public UIFixture {};
class ComplexComponent : public UIFixture {};
class ContainerComponent : public UIFixture {};
class Integration : public UIFixture
{
protected:
	bool VerifyClone(unique_ptr<UIComponent> original);
};
class TextureResourceBinderTest : public UIFixture {};
class TextureSwitcherComponent : public UIFixture
{
protected:
	void FitToTextureSourceTest(const string& bindingKey);
	void CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey); //?!? ToyFixture Ŭ�� �Լ��� ��ĥ �� ������ ������.
};
class TracyBenchmark : public UIFixture {};

class UICommandHistory;
class TexResCommandHistory;
class UIModuleConnection : public UIFixture {};
class UndoRedo : public UIFixture
{
protected:
	void CaptureSnapshot(vector<unique_ptr<UIComponent>>& history);
	void CaptureSnapshot(vector<unique_ptr<TextureResourceBinder>>& history);
	void VerifyUndoRedo(UICommandHistory& cmdHistory, const vector<unique_ptr<UIComponent>>& history);
	void VerifyUndoRedo(TexResCommandHistory& cmdHistory, const vector<unique_ptr<TextureResourceBinder>>& history);

	template <typename History, typename Func>
	void ExecuteAndCapture(History& history, Func&& func)
	{
		func();
		CaptureSnapshot(history);
	}
};

/////////////////////////////////////////////////////

class SceneManagerTest : public SystemFixture {};