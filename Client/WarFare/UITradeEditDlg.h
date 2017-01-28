// UITradeEditDlg.h: interface for the CUITradeEditDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UITRADEEDITDLG_H__347A4D3E_DC71_4F03_8070_946095EB8120__INCLUDED_)
#define AFX_UITRADEEDITDLG_H__347A4D3E_DC71_4F03_8070_946095EB8120__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "N3UIArea.h"
#include "N3UIImage.h"
#include "GameProcedure.h"

#include "GameDef.h"


//////////////////////////////////////////////////////////////////////

class CGameProcMain;

class CUITradeEditDlg : public CN3UIBase  
{
public:
	CSubProcPerTrade*	m_pSubProcPerTrade;
	CN3UIArea*			m_pArea;
	CN3UIImage*			m_pImageOfIcon;

public:
	CUITradeEditDlg();
	virtual ~CUITradeEditDlg();

	int					GetQuantity();
	void				SetQuantity(int iQuantity); // "edit_trade" Edit Control ���� �������� ���ڿ��� �����Ѵ�..

	virtual void		Release();
	virtual bool		ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	void				Open(bool bCountGold);
	void				Close();
};

#endif // !defined(AFX_UITRADEEDITDLG_H__347A4D3E_DC71_4F03_8070_946095EB8120__INCLUDED_)
