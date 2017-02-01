// UICreateClanName.h: interface for the UICreateClanName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__UICREATECLANNAME_H__)
#define __UICREATECLANNAME_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"
#include "N3UIBase.h"

#include "N3UIString.h"
#include "N3UIEdit.h"

//////////////////////////////////////////////////////////////////////

class CUICreateClanName : public CN3UIBase  
{
public:
	CN3UIString*	m_pText_Title;
	CN3UIEdit*		m_pEdit_ClanName;
	std::string		m_szClanName;

public:
	void SetVisible(bool bVisible);
	void Open(int msg = 0);
	bool MakeClan();
	void MsgSend_MakeClan();

	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	CUICreateClanName();
	virtual ~CUICreateClanName();	
};

#endif //#if !defined(__UICREATECLANNAME_H__)
