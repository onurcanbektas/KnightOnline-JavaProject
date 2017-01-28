// UICharacterSelect.h: interface for the UICharacterSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UICharacterSelect_H__665CADA6_E25B_47D6_B962_6DA49673048F__INCLUDED_)
#define AFX_UICharacterSelect_H__665CADA6_E25B_47D6_B962_6DA49673048F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "GameProcCharacterSelect.h"

class CGameProcedure;

class CUICharacterSelect : public CN3UIBase
{
protected:
	CN3UIBase* m_pBtnLeft;
	CN3UIBase* m_pBtnRight;
	CN3UIBase* m_pBtnExit;
	CN3UIBase* m_pBtnDelete;

public:
	DWORD MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	CUICharacterSelect();
	virtual ~CUICharacterSelect();

	virtual void	Release();
	virtual bool	Load(HANDLE hFile);
	virtual void	Tick();
	virtual bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	void	DisplayChrInfo(__CharacterSelectInfo* pCSInfo);
	void	DontDisplayInfo();
};

#endif // !defined(AFX_UICharacterSelect_H__665CADA6_E25B_47D6_B962_6DA49673048F__INCLUDED_)
