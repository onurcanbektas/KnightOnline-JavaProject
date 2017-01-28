// GameCursor.h: interface for the CGameCursor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMECURSOR_H__3BC4F8E1_6DEC_4848_A6EB_0352007AC780__INCLUDED_)
#define AFX_GAMECURSOR_H__3BC4F8E1_6DEC_4848_A6EB_0352007AC780__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"
#include "N3UIBase.h"

class CGameCursor   : public CN3UIBase  
{
private:
	class		CN3UIImage* m_pImageCursor[CURSOR_COUNT];
	bool		m_bCursorLocked;
	e_Cursor	m_ePrevGameCursor;
	e_Cursor	m_eCurGameCursor;
	HCURSOR		m_hCursor;

public:
	void Tick();
	void Render();
	void RestoreGameCursor();
	void SetGameCursor(e_Cursor eCursor, bool bLocked = false);
	bool Load(HANDLE hFile);
	CGameCursor();
	virtual ~CGameCursor();

};

#endif // !defined(AFX_GAMECURSOR_H__3BC4F8E1_6DEC_4848_A6EB_0352007AC780__INCLUDED_)
