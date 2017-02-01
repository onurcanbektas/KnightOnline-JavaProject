// UINPCEvent.h: interface for the UINPCEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UINPCEVENT_H__27F6610C_5D61_4A22_97F2_93211B77BF9C__INCLUDED_)
#define AFX_UINPCEVENT_H__27F6610C_5D61_4A22_97F2_93211B77BF9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"

#include "N3UIBase.h"
#include "N3UIButton.h"
#include "N3UIString.h"

enum e_NpcEvent	{ NPC_EVENT_ITEM_TRADE = 0, NPC_EVENT_TRADE_REPAIR = 1, NPC_EVENT_KNIGHTS = 2 };

//////////////////////////////////////////////////////////////////////


class CUINPCEvent   : public CN3UIBase  
{
	int					m_iTradeID;
	int					m_iIDTarget;
	CN3UIButton*		m_pBtn_Repair;
	CN3UIString*		m_pText_Repair;
	CN3UIString*		m_pText_Title;

public:
	void SetVisible(bool bVisible);
	bool OnKeyPress(int iKey);
	void Release();

	CUINPCEvent();
	virtual ~CUINPCEvent();

	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	void Open(e_NpcEvent eNpcEvent, int iTradeId = -1, int iIDTarget = -1);
	void Close();
};

#endif // !defined(AFX_UINPCEVENT_H__27F6610C_5D61_4A22_97F2_93211B77BF9C__INCLUDED_)
