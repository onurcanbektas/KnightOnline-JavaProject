// UITradeList.h: interface for the CUITradeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UITRADELIST_H__B4807FE6_499A_47DE_BEA0_642E52708F0D__INCLUDED_)
#define AFX_UITRADELIST_H__B4807FE6_499A_47DE_BEA0_642E52708F0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"

#include "N3UIBase.h"
#include "N3UIButton.h"

//////////////////////////////////////////////////////////////////////


class CUITradeList       : public CN3UIBase  
{
	CN3UIString*		m_pStr_List[9];
	std::string			m_sStr[40];
	CN3UIScrollBar*		m_pScrollbar;	// scrollbar ����������(���� m_Child�� ����)

public:
	CUITradeList();
	virtual ~CUITradeList();

	void Release();

	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	void Open(int iIDTarget = -1);
	void Close();

	void SetTopLine(int iTopLine);		// �� ������ �������ش�.
};

#endif // !defined(AFX_UITRADELIST_H__B4807FE6_499A_47DE_BEA0_642E52708F0D__INCLUDED_)
