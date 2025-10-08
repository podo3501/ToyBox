#include "pch.h"
#include "TestScene.h"

TestScene::TestScene(const string& name) :
	Scene(name)
{}

bool TestScene::Enter()
{
	return true;
}

bool TestScene::Leave()
{
	return true;
}