#pragma once
#include "Fixture.h"
//���־� ��Ʃ��� �׽�Ʈ Ž���⿡ �������� ǥ���ϱ� ���ؼ� Fixture�� ��ӹ޾Ƽ� ��.
//namespace ��ø���δ� ���������� ��Ÿ���� ����. a::b �̷������� ��Ÿ��. ��, �������� �׽�Ʈ �ϱ� �����

class BasicComponent : public Fixture {};
class ComplexComponent : public Fixture {};
class ContainerComponent : public Fixture {};
class Integration : public Fixture
{
protected:
	bool VerifyClone(unique_ptr<UIComponent> original);
};
class TextureResourceBinderTest : public Fixture {};
class TextureSwitcherComponent : public Fixture
{
protected:
	void FitToTextureSourceTest(const string& bindingKey);
	void CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey); //?!? ToyFixture Ŭ�� �Լ��� ��ĥ �� ������ ������.
};
class TracyBenchmark : public Fixture {};

class UICommandHistory;
class TexResCommandHistory;
class UIModuleConnection : public Fixture {};
class UndoRedo : public Fixture
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