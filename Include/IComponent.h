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
	virtual bool ProcessUpdate(const XMINT2& position, bool activeUpdate) noexcept = 0;
	virtual void ProcessRender(ITextureRender*) = 0;
};
