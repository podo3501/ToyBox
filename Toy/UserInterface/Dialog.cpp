#include "pch.h"
#include "Dialog.h"
#include "../../Include/IRenderer.h"
#include "UIType.h"
#include "UILayout.h"
#include "UIType.h"

#pragma warning(push)
#pragma warning(disable : 4100)	//�����ʴ� �μ��� ���� ��� ��� ����

Dialog::~Dialog() = default;
Dialog::Dialog(const wstring& resPath) : 
	m_resPath{ resPath },
	m_layout{ make_unique<UILayout>() }
{}

bool Dialog::LoadResources(ILoadData* load)
{ 
	//load->LoadTexture(m_filename, )
	return true; 
}

void Dialog::Render(IRender* renderer)
{
}

void Dialog::SetImage(const ImageSource& sources, const Rectangle& area, const XMFLOAT2& pos, Origin origin)
{
	m_layout->Set(area, pos, origin);

	//m_parts->Set(sources);
}

void Dialog::Update(const Vector2& resolution)
{}

#pragma warning(pop)