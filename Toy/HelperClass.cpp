#include "pch.h"
#include "HelperClass.h"
#include "UserInterface/Scene.h"
#include "../Include/IRenderer.h"
#include "Utility.h"

void MouseTracker::SetOffset(XMUINT2 offset) noexcept
{
    m_offset = offset;
}

XMUINT2 MouseTracker::GetOffsetPosition() const noexcept
{
    const auto& state = GetLastState();
    return { state.x - m_offset.x, state.y - m_offset.y };
}

bool LoadScene(const wstring& filename, Scene* scene, IRenderer* renderer)
{
    ReturnIfFalse(scene->LoadFile(filename));
    ReturnIfFalse(renderer->LoadScenes());
    ReturnIfFalse(scene->SetDatas(renderer->GetValue()));
    ReturnIfFalse(scene->Update(nullptr));	//position ����. ���ϸ� {0, 0}�� 1������ ���� ��� ��Ÿ����.

    return true;
}