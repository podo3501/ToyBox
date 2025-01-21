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

//���־� ��Ʃ��� �׽�Ʈ Ž���⿡ �������� ǥ���ϱ� ���ؼ� Fixture�� ��ӹ޾Ƽ� ��.
//namespace ��ø���δ� ���������� ��Ÿ���� ����. a::b �̷������� ��Ÿ��. ��, �������� �׽�Ʈ �ϱ� �����
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