// UITradeExplanation.h: interface for the CUITradeExplanation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UITRADEEXPLANATION_H__DFFA77BD_3013_4389_84CB_EB1DFAD7F3FA__INCLUDED_)
#define AFX_UITRADEEXPLANATION_H__DFFA77BD_3013_4389_84CB_EB1DFAD7F3FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "N3UIBase.h"

class CUITradeExplanation : public CN3UIBase  
{
protected:
	class CN3UIButton*		m_pBtn_PageUp;
	class CN3UIButton*		m_pBtn_PageDown;
	class CN3UIButton*		m_pBtn_Close;
	class CN3UIString*		m_pText_Explanation;

	std::string m_szExplanation;
	int			m_iCurSel;
public:
	bool OnKeyPress(int iKey);
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	void SetExplanation(int iCurSel, std::string szExplanation);

	CUITradeExplanation();
	virtual ~CUITradeExplanation();

};

#endif // !defined(AFX_UITRADEEXPLANATION_H__DFFA77BD_3013_4389_84CB_EB1DFAD7F3FA__INCLUDED_)
