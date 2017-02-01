// UINotice.h: interface for the CUINotice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UINOTICE_H__28178F32_B0C2_4742_B3C0_25C8F6034BD2__INCLUDED_)
#define AFX_UINOTICE_H__28178F32_B0C2_4742_B3C0_25C8F6034BD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include <list>
#include <string>

typedef std::list<std::string>::iterator it_String;

class CUINotice : public CN3UIBase  
{
public:
	class CN3UIString*		m_pText_Notice;
	class CN3UIScrollBar*	m_pScrollBar;
	class CN3UIButton*		m_pBtn_OK;

	std::list<std::string>	m_Texts;

public:
	void RemoveNotice();
	void SetVisible(bool bVisible);
	bool OnKeyPress(int iKey);
	void GenerateText();
	void Release();

	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	CUINotice();
	virtual ~CUINotice();

};

#endif // !defined(AFX_UINOTICE_H__28178F32_B0C2_4742_B3C0_25C8F6034BD2__INCLUDED_)
