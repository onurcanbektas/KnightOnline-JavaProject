// UITradeBBSSelector.h: interface for the CUITradeBBSSelector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UITRADEBBSSELECTOR_H__034D575A_E014_4C70_ABD5_EE647725A1DF__INCLUDED_)
#define AFX_UITRADEBBSSELECTOR_H__034D575A_E014_4C70_ABD5_EE647725A1DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "N3UIButton.h"

#define TRADE_BBS_SELL		1
#define TRADE_BBS_BUY		2
#define TRADE_BBS_CANCEL	3

class CUITradeBBSSelector : public CN3UIBase 
{
protected:
	CN3UIButton*	m_pBtn_BBSSell;
	CN3UIButton*	m_pBtn_BBSBuy;
	CN3UIButton*	m_pBtn_BBSCancel;

public:
	void SetVisible(bool bVisible);
	bool OnKeyPress(int iChar);
	void MsgSend_OpenTradeBuyBBS();
	void MsgSend_OpenTradeSellBBS();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	CUITradeBBSSelector();
	virtual ~CUITradeBBSSelector();

};

#endif // !defined(AFX_UITRADEBBSSELECTOR_H__034D575A_E014_4C70_ABD5_EE647725A1DF__INCLUDED_)
