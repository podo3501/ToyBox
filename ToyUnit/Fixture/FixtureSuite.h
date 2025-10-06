#pragma once
#include "Fixture.h"
//비주얼 스튜디오 테스트 탐색기에 계층구조 표시하기 위해서 Fixture를 상속받아서 함.
//namespace 중첩으로는 계층구조로 나타나지 않음. a::b 이런식으로 나타남. 즉, 묶음으로 테스트 하기 어려움

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
	void CloneTestForSwitcher(const vector<RECT>& expectDest, const string& bindKey); //?!? ToyFixture 클론 함수와 합칠 수 있을꺼 같은데.
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