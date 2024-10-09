#pragma once

class Window;
struct IRenderer;

class ToyTest : public ::testing::Test
{
public:
	ToyTest();
	~ToyTest();

protected:
	void SetUp() override;
	void TearDown() override;

	std::unique_ptr<Window> m_window;
	std::unique_ptr<IRenderer> m_renderer;
};