#pragma once
#include "../Toy/MainLoop.h"

class Window;
class UIComponent;
struct IRenderer;

class ToyTestFixture : public ::testing::Test
{
public:
	ToyTestFixture();
	~ToyTestFixture();

protected:
	void SetUp() override;
	void TearDown() override;

	unique_ptr<Window> m_window;
	unique_ptr<IRenderer> m_renderer;
	unique_ptr<UIComponent> m_panel;
};

//비주얼 스튜디오 테스트 탐색기에 계층구조 표시하기 위해서 Fixture를 상속받아서 함.
//namespace 중첩으로는 계층구조로 나타나지 않음. a::b 이런식으로 나타남. 즉, 묶음으로 테스트 하기 어려움
namespace ComponentTest
{
	class BasicFunctionalityTest : public ToyTestFixture
	{};

	class IntegrationTest : public ToyTestFixture
	{};

	class UndoRedoTest : public ToyTestFixture
	{
	};
}