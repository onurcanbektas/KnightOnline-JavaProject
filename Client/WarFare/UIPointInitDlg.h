// UIPointInitDlg.h: interface for the UIPointInitDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIPOINTINITDLG_H__D784EB22_FE0A_4A62_83FF_4664854DE2EC__INCLUDED_)
#define AFX_UIPOINTINITDLG_H__D784EB22_FE0A_4A62_83FF_4664854DE2EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"

#include "N3UIBase.h"
#include "N3UIButton.h"
#include "N3UIString.h"

//////////////////////////////////////////////////////////////////////


class CUIPointInitDlg     : public CN3UIBase  
{
	CN3UIButton*		m_pBtn_Ok;
	CN3UIButton*		m_pBtn_Cancel;

	CN3UIString*		m_pText_NeedGold;
	bool				m_bAllpoint;

public:
	CUIPointInitDlg();
	virtual ~CUIPointInitDlg();

	void	InitDlg(bool bAllpoint, int iGold);
	bool	OnKeyPress(int iKey);
	void	Release();

	bool	Load(HANDLE hFile);
	bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	void	Close();

	void	PushOkButton();
};

#endif // !defined(AFX_UIPOINTINITDLG_H__D784EB22_FE0A_4A62_83FF_4664854DE2EC__INCLUDED_)
