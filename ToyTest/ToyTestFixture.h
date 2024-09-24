#pragma once

class ToyTest : public ::testing::Test
{
public:
	ToyTest() {};

protected:
	void SetUp() override;
	void TearDown() override;
};