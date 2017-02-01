// N3UIIcon.cpp: implementation of the CN3UIIcon class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "..\WarFare\N3UIIcon.h"
#include "..\WarFare\N3UIWndBase.h"
#include "..\WarFare\GameProcedure.h"
#include "..\WarFare\UIImageTooltipDlg.h"

#ifdef _N3GAME
#include "..\WarFare\N3UIWndBase.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3UIIcon::CN3UIIcon()
{
	CN3UIImage::CN3UIImage();
	m_eType = UI_TYPE_ICON;
}

CN3UIIcon::~CN3UIIcon()
{
}

DWORD CN3UIIcon::MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;

	if (!m_bVisible) return dwRet;
	if ( (m_pParent->GetState() == UI_STATE_COMMON_NONE) || (m_pParent->GetState() == UI_STATE_ICON_MOVING) ) 
		SetStyle(GetStyle() & (~UISTYLE_ICON_HIGHLIGHT));	

#ifdef _N3GAME
	if ( CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer ) return dwRet;
#endif

	RECT rect = GetRegion();
	if ( ::PtInRect(&rect, ptCur) && (m_pParent->GetState() == UI_STATE_COMMON_NONE) )
		SetStyle(GetStyle() | UISTYLE_ICON_HIGHLIGHT);	

	rect = GetMoveRect();

	if(!::PtInRect(&rect, ptCur))		// ���� ���̸�
	{
		dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
		return dwRet;
	}

	if((dwFlags & UI_MOUSE_LBCLICK) && !(dwFlags & UI_MOUSE_RBDOWN))  // ���ʹ�ư ������ ����
	{
		m_pParent->SetState(UI_STATE_ICON_MOVING);
		m_pParent->ReceiveMessage(this, UIMSG_ICON_DOWN_FIRST); // �θ𿡰� ��ư Ŭ�� ����..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if((dwFlags & UI_MOUSE_LBCLICKED) && !(dwFlags & UI_MOUSE_RBDOWN)) // ���ʹ�ư�� ���� ����
	{
		if(m_pParent && m_pParent->GetState() == UI_STATE_ICON_MOVING) // ���� ���°� ��ư�� Down �����̸�
		{
			m_pParent->SetState(UI_STATE_COMMON_NONE);
			m_pParent->ReceiveMessage(this, UIMSG_ICON_UP); // �θ𿡰� ��ư Ŭ�� ����..
			dwRet |= UI_MOUSEPROC_DONESOMETHING;
			return dwRet;
		}
	}

	if ((dwFlags & UI_MOUSE_RBCLICK)  && !(dwFlags & UI_MOUSE_LBDOWN))	// ������
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_RDOWN_FIRST); // �θ𿡰� ��ư Ŭ�� ����..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if((dwFlags & UI_MOUSE_RBCLICKED)  && !(dwFlags & UI_MOUSE_LBDOWN))// ������ ��ư�� ���� ����
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_RUP); // �θ𿡰� ��ư Ŭ�� ����..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if (dwFlags & UI_MOUSE_LBDOWN)	// ����
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_DOWN); // �θ𿡰� ��ư Ŭ�� ����..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}
/*
	else
	{
		if(m_pParent && m_pParent->GetState() == UI_STATE_ICON_MOVING) // ���� ���°� ��ư�� Down �����̸�
		{
			m_pParent->ReceiveMessage(this, UIMSG_ICON_UP); // �θ𿡰� ��ư Ŭ�� ����..
			dwRet |= UI_MOUSEPROC_DONESOMETHING;
			return dwRet;
		}
	}
*/

	if (dwFlags & UI_MOUSE_LBDBLCLK)	// ���� ���� Ŭ��
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_DBLCLK); // �θ𿡰� ��ư Ŭ�� ����..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if (dwFlags & UI_MOUSE_RBDBLCLK)	// ������ ���� Ŭ��
	{
		m_pParent->ReceiveMessage(this, UIMSG_ICON_RDBLCLK); // �θ𿡰� ��ư Ŭ�� ����..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

void CN3UIIcon::Render()
{
	if (!IsVisible()) return;

	CN3UIWndBase::m_pSelectionImage->SetVisible(false);

/*	if ( m_dwStyle & UISTYLE_ICON_HIGHLIGHT )
	{		
		CN3UIWndBase::m_pSelectionImage->SetVisible(true);
		CN3UIWndBase::m_pSelectionImage->SetRegion(GetRegion());
		CN3UIWndBase::m_pSelectionImage->Render();
		CN3UIWndBase::m_pSelectionImage->SetVisible(false);
	}*/

	CN3UIImage::Render();

	if (m_dwStyle & UISTYLE_DISABLE_SKILL)
	{
		CN3UIWndBase::m_pSelectionImage->SetVisible(true);
		CN3UIWndBase::m_pSelectionImage->SetRegion(GetRegion());
		m_dc = CN3UIWndBase::m_pSelectionImage->GetColor();
		CN3UIWndBase::m_pSelectionImage->SetColor(D3DCOLOR_RGBA(40,40, 40, 160));
		CN3UIWndBase::m_pSelectionImage->RenderIconWrapper();
		CN3UIWndBase::m_pSelectionImage->SetColor(m_dc);
		CN3UIWndBase::m_pSelectionImage->SetVisible(false);
	}

	if( m_dwStyle & UISTYLE_DURABILITY_EXHAUST )
	{
		CN3UIWndBase::m_pSelectionImage->SetVisible(true);
		CN3UIWndBase::m_pSelectionImage->SetRegion(GetRegion());
		m_dc = CN3UIWndBase::m_pSelectionImage->GetColor();
		CN3UIWndBase::m_pSelectionImage->SetColor(D3DCOLOR_RGBA(200, 20, 20, 100));
		CN3UIWndBase::m_pSelectionImage->RenderIconWrapper();
		CN3UIWndBase::m_pSelectionImage->SetColor(m_dc);
		CN3UIWndBase::m_pSelectionImage->SetVisible(false);
	}
}





