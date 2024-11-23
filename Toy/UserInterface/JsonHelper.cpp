#include "pch.h"
#include "JsonHelper.h"
#include "JsonOperation.h"
#include "Scene.h"
#include "../Config.h"
#include "../Utility.h"

bool ReadScene(const wstring& filename, Scene& outScene)
{
	unique_ptr<JsonOperation> jsonReadOp = make_unique<JsonOperation>();
	ReturnIfFalse(jsonReadOp->Read(filename));

	outScene.SerializeIO(jsonReadOp.get());

	return true;
}

bool WriteScene(Scene& scene, const wstring& filename)
{
	unique_ptr<JsonOperation> jsonWriteOp = make_unique<JsonOperation>();
	scene.SerializeIO(jsonWriteOp.get());
	return jsonWriteOp->Write(filename);
}


