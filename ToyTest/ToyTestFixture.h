#pragma once
#include "../Toy/MainLoop.h"

class Window;
class Scene;
struct IRenderer;

class ToyTest : public ::testing::Test
{
public:
	ToyTest();
	~ToyTest();

protected:
	void SetUp() override;
	void TearDown() override;

	unique_ptr<Window> m_window;
	unique_ptr<IRenderer> m_renderer;
	unique_ptr<Scene> m_testScene;
};