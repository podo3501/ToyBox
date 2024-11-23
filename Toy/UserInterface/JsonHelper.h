#pragma once

class Scene;

bool ReadScene(const wstring& filename, Scene& outScene);
bool WriteScene(Scene& scene, const wstring& filename);