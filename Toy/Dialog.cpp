#include "pch.h"
#include "Dialog.h"
#include "../Include/IRenderer.h"
#include "UserInterfaceType.h"

#pragma warning(push)
#pragma warning(disable : 4100)	//�����ʴ� �μ��� ���� ��� ��� ����

Dialog::~Dialog() = default;
Dialog::Dialog(const wstring& resPath) : 
	m_resPath{ resPath } 
{}

bool Dialog::LoadResources(ILoadData* load)
{ 
	//load->LoadTexture(m_filename, )
	return true; 
}

void Dialog::Render(IRender* renderer)
{
}

void Dialog::SetImage(const wstring& filename, const Rectangle& area, const XMFLOAT2& pos, Origin origin)
{}

void Dialog::Update(const Vector2& resolution)
{}

#pragma warning(pop)