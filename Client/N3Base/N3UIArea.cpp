// N3UIArea.cpp: implementation of the CN3UIArea class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3UIArea.h"
#include "N3UIEdit.h"

#ifndef _REPENT
#ifdef _N3GAME
#include "..\WarFare\n3uiwndbase.h"
#include "..\WarFare\uiinventory.h"
#endif 
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3UIArea::CN3UIArea()
{
	m_eType = UI_TYPE_AREA;
	m_eAreaType = UI_AREA_TYPE_NONE;
}

CN3UIArea::~CN3UIArea()
{
}

void CN3UIArea::Release()
{
	CN3UIBase::Release();
}

void CN3UIArea::SetRegion(const RECT& Rect)
{
	CN3UIBase::SetRegion(Rect);
	for(UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		(*itor)->SetRegion(Rect);
	}
}

bool CN3UIArea::Load(HANDLE hFile)
{
	if (false == CN3UIBase::Load(hFile)) return false;

#ifndef _REPENT
	// �߰������� ������ �̰��� �߰��ϱ�
	DWORD dwNum;
	int iAreaType;
	ReadFile(hFile, &iAreaType, sizeof(int), &dwNum, NULL);	// click ����
	m_eAreaType = (eUI_AREA_TYPE)iAreaType;
#endif
	return true;
}

DWORD CN3UIArea::MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;
	if (!m_bVisible) return dwRet;
#ifndef _REPENT
#ifdef _N3GAME
	if ( CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer ) return dwRet;
	// �θ� ������ �Ŵ����̷� Inventory Wnd���..	
	if ( (m_pParent->UIType() == UI_TYPE_ICON_MANAGER) && (((CN3UIWndBase* )m_pParent)->GetUIWnd() == UIWND_INVENTORY) )
	{
		// Ư�� �̺�Ʈ�� ���� �޽��� ����..
		if(IsIn(ptCur.x, ptCur.y) && (dwFlags & UI_MOUSE_LBCLICK) )	
		{
			m_pParent->ReceiveMessage(this, UIMSG_AREA_DOWN_FIRST); // �θ𿡰� ��ư Ŭ�� ����..
			dwRet |= UI_MOUSEPROC_DONESOMETHING;
			return dwRet;
		}
	}
#endif
#endif
	// Ư�� �̺�Ʈ�� ���� �޽��� ����..
	if(IsIn(ptCur.x, ptCur.y) && (dwFlags & UI_MOUSE_LBCLICK) )	
	{
		m_pParent->ReceiveMessage(this, UIMSG_BUTTON_CLICK); // �θ𿡰� ��ư Ŭ�� ����..
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
	}

	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

#ifndef _REPENT
#ifdef _N3GAME
bool CN3UIArea::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if ( CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer ) return false;
	// �θ� ������ �Ŵ����̷� Inventory Wnd���..	
	if ( (m_pParent->UIType() == UI_TYPE_ICON_MANAGER) && (((CN3UIWndBase* )m_pParent)->GetUIWnd() == UIWND_INVENTORY) )
	{
		if (dwMsg == UIMSG_BUTTON_CLICK)					
		{
			if(pSender->m_szID == "btn_Destroy_ok")
			{
				// �κ��丮�� �� ������..
				((CUIInventory* )m_pParent)->ItemDestroyOK();
			}

			if(pSender->m_szID == "btn_Destroy_cancel")
			{
				// �κ��丮�� �� ������..
				((CUIInventory* )m_pParent)->ItemDestroyCancel();
			}
		}
	}

	return true;
}
#endif
#endif


#ifdef _N3TOOL
bool CN3UIArea::Save(HANDLE hFile)
{
	if (false == CN3UIBase::Save(hFile)) return false;
#ifndef _REPENT
	DWORD dwNum;
	int iAreaType = (int)m_eAreaType;
	WriteFile(hFile, &iAreaType, sizeof(int), &dwNum, NULL);	// click ����
#endif
	return true;
}

void CN3UIArea::operator = (const CN3UIArea& other)
{
	CN3UIBase::operator = (other);
#ifndef _REPENT
	m_eAreaType = other.m_eAreaType;
#endif
}
#endif

