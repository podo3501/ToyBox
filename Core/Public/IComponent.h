#pragma once

struct ITextureLoad;
struct ITextureController;
struct ITextureRender;

struct IComponent
{
public:
	virtual ~IComponent() {};

	virtual void ProcessRender(ITextureRender*) = 0;
};
