// UINPCEvent.cpp: implementation of the UINPCEvent class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "resource.h"
#include "UINPCEvent.h"

#include "GameProcedure.h"
#include "GameProcMain.h"

#include "UIInventory.h"
#include "UITransactionDlg.h"
#include "UIItemExchange.h"
#include "UINpcTalk.h"

#include "SubProcPerTrade.h"
#include "UISkillTreeDlg.h"
#include "UIManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUINPCEvent::CUINPCEvent()
{

}

CUINPCEvent::~CUINPCEvent()
{

}

void CUINPCEvent::Release()
{
	CN3UIBase::Release();
}

bool CUINPCEvent::Load(HANDLE hFile)
{
	if(CN3UIBase::Load(hFile)==false) return false;

	m_pBtn_Repair	= (CN3UIButton*)GetChildByID("Btn_Repair");		__ASSERT(m_pBtn_Repair, "NULL UI Component!!");
	m_pText_Repair	= (CN3UIString*)GetChildByID("Text_Repair");	__ASSERT(m_pText_Repair, "NULL UI Component!!");
	m_pText_Title	= (CN3UIString*)GetChildByID("Text_Title");		__ASSERT(m_pText_Title, "NULL UI Component!!");

	return true;
}

bool CUINPCEvent::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)					
	{
		if(pSender->m_szID == "Btn_Sale")
		{
			if (CGameProcedure::s_pProcMain->m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE) 	/* ���ΰ� ��ŷ� ���̸�.. */
				return true;

			Close();
			if (CGameProcedure::s_pProcMain->m_pUIInventory->IsVisible())
				return true;
			if (CGameProcedure::s_pProcMain->m_pUITransactionDlg->IsVisible())
				return true;
			if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->IsVisible())
				CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->Close();
			CGameProcedure::s_pProcMain->DoCommercialTransaction(m_iTradeID);
		}

		if(pSender->m_szID == "Btn_Repair")
		{
			if (CGameProcedure::s_pProcMain->m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE) 	/* ���ΰ� ��ŷ� ���̸�.. */
				return true;

			Close();
			if (CGameProcedure::s_pProcMain->m_pUITransactionDlg->IsVisible())
				return true;
			if (CGameProcedure::s_pProcMain->m_pUIInventory->IsVisible())
				return true;
			if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->IsVisible())
				CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->Close();
			CGameProcedure::s_pProcMain->m_pUIInventory->Open(INV_STATE_REPAIR);

		}

/*		if(pSender->m_szID == "Btn_Change")
		{
			Close();
			CGameProcedure::s_pProcMain->m_pUIItemREDlg->Open();
		}

		if(pSender->m_szID == "Btn_Talk")
		{
			Close();
			CGameProcedure::s_pProcMain->m_pUINpcTalk->Open(m_iIDTarget);
		}*/

		if(pSender->m_szID == "btn_close")
			Close();
	}

	return true;
}

void CUINPCEvent::Open(e_NpcEvent eNpcEvent, int iTradeId, int iIDTarget)
{
	SetVisible(true);
	m_iIDTarget = iIDTarget;
	m_iTradeID  = iTradeId;
	std::string szStr;

	switch(eNpcEvent)
	{
		case NPC_EVENT_ITEM_TRADE:
			szStr = "IDS_NPC_EVENT_TITLE_TRADE";//::_LoadStringFromResource(IDS_NPC_EVENT_TITLE_TRADE, szStr);
			m_pText_Title->SetString(szStr);
			if (m_pBtn_Repair)
			{
				if (m_pBtn_Repair->IsVisible())
				{
					m_pBtn_Repair->SetVisible(false);
					m_pText_Repair->SetVisible(false);
				}
			}
			break;

		case NPC_EVENT_TRADE_REPAIR:
			szStr = "IDS_NPCEVENT_TITLE_REPAIR";//::_LoadStringFromResource(IDS_NPCEVENT_TITLE_REPAIR, szStr);
			m_pText_Title->SetString(szStr);
			if (m_pBtn_Repair)
			{
				if (!m_pBtn_Repair->IsVisible())
				{
					m_pBtn_Repair->SetVisible(true);
					m_pText_Repair->SetVisible(true);
				}
			}
			break;
	}
}

void CUINPCEvent::Close()
{
	SetVisible(false);
}

bool CUINPCEvent::OnKeyPress(int iKey)
{
	if(/*DIK_ESCAPE*/SDL_SCANCODE_ESCAPE == iKey)
	{
		this->Close();
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUINPCEvent::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}
