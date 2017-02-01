// GameCursor.cpp: implementation of the CGameCursor class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "N3UIBase.h"
#include "N3UIImage.h"
#include "GameProcedure.h"
#include "LocalInput.h"
#include "GameCursor.h"
#include "UIManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameCursor::CGameCursor()
{
	m_bCursorLocked		= false;
	m_eCurGameCursor	= CURSOR_EL_NORMAL;
	m_ePrevGameCursor	= CURSOR_EL_NORMAL;
	m_hCursor			= NULL;

	for(int i = 0 ; i < CURSOR_COUNT; i++)
	{
		m_pImageCursor[i] = NULL;
	}
}

CGameCursor::~CGameCursor()
{
	if(m_hCursor) ::SetCursor(m_hCursor);
}

bool CGameCursor::Load(HANDLE hFile)
{
	if(CN3UIBase::Load(hFile)==false) return false;

	m_hCursor = ::GetCursor();
	::SetCursor(NULL);

	char szBuf[128];
	for(int i = 0 ; i < CURSOR_COUNT; i++)
	{
		sprintf(szBuf,"Image_Cursor%.2d",i);

		m_pImageCursor[i] = (CN3UIImage*)(this->GetChildByID(szBuf));	__ASSERT(m_pImageCursor[i], "NULL UI Component!!!");
	}
	return true;
}

void CGameCursor::SetGameCursor(e_Cursor eCursor, bool bLocked)
{
	if ((m_bCursorLocked) && (!bLocked) ) return;
	else if ( ((m_bCursorLocked) && bLocked) || ((!m_bCursorLocked) && !bLocked) )
	{
		m_eCurGameCursor = eCursor;
		return;
	}
	else if ((!m_bCursorLocked) && bLocked)
	{
		m_ePrevGameCursor = m_eCurGameCursor;
		m_bCursorLocked = true;
		m_eCurGameCursor = eCursor;
	}
}

void CGameCursor::RestoreGameCursor()
{
	if (m_bCursorLocked) 
		m_bCursorLocked = false;

	m_eCurGameCursor = m_ePrevGameCursor;
}

void CGameCursor::Render()
{
	if(m_eCurGameCursor >= CURSOR_COUNT) return;

	if(m_pImageCursor[m_eCurGameCursor])
	{
		CGameProcedure::s_pUIMgr->RenderStateSet();
		DWORD dwZ;
		CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZ);
		CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, FALSE);

		m_pImageCursor[m_eCurGameCursor]->Render();
		CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZ);
		CGameProcedure::s_pUIMgr->RenderStateRestore();

	}
}

void CGameCursor::Tick()
{
	HCURSOR hCursor = ::GetCursor();
	if(hCursor)	::SetCursor(NULL);

	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
	for(int i = 0 ; i < CURSOR_COUNT; i++)
	{
		if(m_pImageCursor[i]) m_pImageCursor[i]->SetPos(ptCur.x, ptCur.y);
	}
}
