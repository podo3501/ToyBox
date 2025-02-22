#pragma once

struct ITextureLoad;
struct ITextureController;
struct ITextureRender;

struct IComponent
{
public:
	virtual ~IComponent() {};

	virtual bool LoadResources(ITextureLoad*) = 0;
	virtual bool PostLoaded(ITextureController*) = 0;
	virtual void ProcessRenderTexture(ITextureRender*) = 0;
	virtual void ProcessRender(ITextureRender*) = 0;
};
