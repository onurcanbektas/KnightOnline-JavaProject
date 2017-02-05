// UICharacterSelect.cpp: implementation of the UICharacterSelect class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "Resource.h"

#include "GameProcCharacterSelect.h"
#include "UICharacterSelect.h"
#include "N3UIString.h"
#include "N3UITooltip.h"
#include "UIManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUICharacterSelect::CUICharacterSelect()
{
	m_eType = UI_TYPE_BASE;
	CUICharacterSelect::Release();

	m_pBtnLeft		= NULL;
	m_pBtnRight		= NULL;
	m_pBtnExit		= NULL;
	m_pBtnDelete	= NULL;
}

CUICharacterSelect::~CUICharacterSelect()
{
}

bool CUICharacterSelect::Load(HANDLE hFile)
{
	CN3UIBase::Load(hFile);

	m_pBtnLeft = this->GetChildByID("bt_left");
	__ASSERT(m_pBtnLeft, "NULL UI Component!!");

	m_pBtnRight = this->GetChildByID("bt_right");
	__ASSERT(m_pBtnRight, "NULL UI Component!!");

	m_pBtnExit = this->GetChildByID("bt_exit");
	__ASSERT(m_pBtnExit, "NULL UI Component!!");

	m_pBtnDelete = this->GetChildByID("bt_delete");
	__ASSERT(m_pBtnDelete, "NULL UI Component!!");

	POINT pt;
	RECT rc = this->GetRegion();
	float fRatio = (float)s_CameraData.vp.Width / (rc.right - rc.left);

	if(m_pBtnLeft)
	{
		pt = m_pBtnLeft->GetPos();
		pt.x *= fRatio;
		pt.y = s_CameraData.vp.Height - 10 - m_pBtnLeft->GetHeight();
		m_pBtnLeft->SetPos(pt.x, pt.y);
	}

	if(m_pBtnRight) 
	{
		pt = m_pBtnRight->GetPos();
		pt.x *= fRatio; 
		pt.y = s_CameraData.vp.Height - 10 - m_pBtnRight->GetHeight();
		m_pBtnRight->SetPos(pt.x, pt.y); 
	}

	if(m_pBtnExit) 
	{
		pt = m_pBtnExit->GetPos();
		pt.x *= fRatio; 
		pt.y = s_CameraData.vp.Height - 10 - m_pBtnExit->GetHeight();
		m_pBtnExit->SetPos(pt.x, pt.y); 
	}

	if(m_pBtnDelete)
	{
		pt = m_pBtnDelete->GetPos(); 
		pt.x *= fRatio;
		pt.y = 20; 
		m_pBtnDelete->SetPos(pt.x, pt.y); 
	}

	SetRect(&rc, 0, 0, s_CameraData.vp.Width, s_CameraData.vp.Height);
	this->SetRegion(rc);

	return true;
}

void CUICharacterSelect::Tick()
{
	CN3UIBase::Tick();
}

void CUICharacterSelect::Release()
{
	m_pBtnLeft = NULL;
	m_pBtnRight = NULL;
	m_pBtnExit = NULL;
	m_pBtnDelete = NULL;

	CN3UIBase::Release();
}

bool CUICharacterSelect::OnKeyPress(int iKey)
{
	if (CGameProcedure::s_pUIMgr->EnableOperation())
	{
		switch (iKey)
		{
		case SDL_SCANCODE_ESCAPE://DIK_ESCAPE:
			ReceiveMessage(m_pBtnExit, UIMSG_BUTTON_CLICK);
			return true;
		case SDL_SCANCODE_LEFT://DIK_LEFT:
			ReceiveMessage(m_pBtnLeft, UIMSG_BUTTON_CLICK);
			return true;
		case SDL_SCANCODE_RIGHT://DIK_RIGHT:
			ReceiveMessage(m_pBtnRight, UIMSG_BUTTON_CLICK);
			return true;
		case SDL_SCANCODE_KP_ENTER://DIK_NUMPADENTER:
		case SDL_SCANCODE_RETURN://DIK_RETURN:
			CGameProcedure::s_pProcCharacterSelect->CharacterSelectOrCreate();
			return true;
		}
	}

	return CN3UIBase::OnKeyPress(iKey);
}

DWORD CUICharacterSelect::MouseProc(DWORD dwFlags, const POINT &ptCur, const POINT &ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;

	if (!m_bVisible)
	{
		return dwRet;
	}

	if (UI_STATE_COMMON_MOVE == m_eState)
	{
		if (dwFlags&UI_MOUSE_LBCLICKED)
		{
			SetState(UI_STATE_COMMON_NONE);
		}

		else
		{
			MoveOffset(ptCur.x - ptOld.x, ptCur.y - ptOld.y);
		}

		dwRet |= UI_MOUSEPROC_DONESOMETHING;

		return dwRet;
	}

	if (false == IsIn(ptCur.x, ptCur.y))
	{
		if (false == IsIn(ptOld.x, ptOld.y))
		{
			return dwRet; 
		}
	}
	else
	{
		// tool tip
		if (s_pTooltipCtrl)
		{
			s_pTooltipCtrl->SetText(m_szToolTip);
		}
	}

	if (m_pChildUI && m_pChildUI->IsVisible())
	{
		return dwRet;
	}

	// child
	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		DWORD dwChildRet = pChild->MouseProc(dwFlags, ptCur, ptOld);
		
		if (UI_MOUSEPROC_DONESOMETHING & dwChildRet)
		{	
			dwRet |= (UI_MOUSEPROC_CHILDDONESOMETHING | UI_MOUSEPROC_DONESOMETHING);
			return dwRet;
		}
	}

	// UI
	if (UI_STATE_COMMON_MOVE != m_eState && PtInRect(&m_rcMovable, ptCur) && (dwFlags&UI_MOUSE_LBCLICK))
	{
		SetState(UI_STATE_COMMON_MOVE);
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	return dwRet;
}

bool CUICharacterSelect::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if (NULL == pSender)
	{
		return false;
	}

	if (!CGameProcedure::s_pUIMgr->EnableOperation())
	{
		return false;
	}

	if( dwMsg == UIMSG_BUTTON_CLICK )
	{
		if ( pSender->m_szID == "bt_left" )
		{
			// Rotate Left..
			CGameProcedure::s_pProcCharacterSelect->DoJobRotate(e_Side::left);
		}

		else if ( pSender->m_szID == "bt_right" )
		{
			// Rotate Right..
			CGameProcedure::s_pProcCharacterSelect->DoJobRotate(e_Side::right);
		}
		
		else if ( pSender->m_szID == "bt_exit" )
		{
			CGameProcedure::MessageBoxPost(StringConstants::EXIT_VALIDATION, "", MB_YESNO, BEHAVIOR_EXIT);
		}
		
		else if ( pSender->m_szID == "bt_delete" )
		{
			CGameProcedure::MessageBoxPost(StringConstants::DELETE_VALIDATION, "", MB_YESNO, BEHAVIOR_DELETE_CHR);
		}
	}
	
	return true;
}

void CUICharacterSelect::DisplayChrInfo(__CharacterSelectInfo* pCSInfo)
{
	CN3UIBase*	m_pUserInfoStr;
	std::string szTotal;

	m_pUserInfoStr = GetChildByID("text00");
	__ASSERT(m_pUserInfoStr, "NULL UI Component!!");

	if ( !pCSInfo->szID.empty() )
	{
		std::string szClass;
		CGameProcedure::GetTextByClass(pCSInfo->eClass, szClass);

		szTotal = StringParser::parse(StringConstants::CHR_SELECT_INFO, std::to_string(pCSInfo->iLevel), szClass.c_str(), pCSInfo->szID.c_str());
	}

	else
	{
		szTotal = StringConstants::CHR_SELECT_HINT;
	}

	if ( m_pUserInfoStr )
	{
		m_pUserInfoStr->SetVisible(true);
		((CN3UIString*)m_pUserInfoStr)->SetString(szTotal);
	}
}

void CUICharacterSelect::DontDisplayInfo()
{
	CN3UIBase*	m_pUserInfoStr;
	m_pUserInfoStr = GetChildByID("text00");
	__ASSERT(m_pUserInfoStr, "NULL UI Component!!");

	if (m_pUserInfoStr)
	{
		m_pUserInfoStr->SetVisible(false);
	}
}
