#pragma once
#include "../Scene.h"

class TestScene : public Scene
{
public:
	TestScene(const string& name);
	
	virtual bool Enter() override;
	virtual bool Leave() override;

private:
};