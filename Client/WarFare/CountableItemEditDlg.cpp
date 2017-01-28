// CountableItemEditDlg.cpp: implementation of the CCountableItemEditDlg class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "resource.h"
#include "CountableItemEditDlg.h"

#include "GameProcedure.h"
#include "GameProcMain.h"
#include "UIInventory.h"
#include "UITransactionDlg.h"

#include "SubProcPerTrade.h"
#include "UIPerTradeDlg.h"
#include "UIWareHouseDlg.h"
#include "UIManager.h"

#include "N3UIEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCountableItemEditDlg::CCountableItemEditDlg()
{
	m_eCallerWnd = UIWND_UNKNOWN;
	m_eCallerWndDistrict = UIWND_DISTRICT_UNKNOWN;
	m_pArea = NULL;
	m_pImageOfIcon = NULL;
	m_bLocked = false;
	m_pEdit	= NULL;
	m_bWareGold = false;

	m_pBtnOk	 = NULL;
	m_pBtnCancel = NULL;
}

CCountableItemEditDlg::~CCountableItemEditDlg()
{

}

void CCountableItemEditDlg::Release()
{
	CN3UIBase::Release();

}

bool CCountableItemEditDlg::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if(NULL == pSender) return false;
	if(IsVisible() == false) return false;
	if(m_eCallerWnd == UIWND_UNKNOWN) return false;
	if(m_eCallerWndDistrict == UIWND_DISTRICT_UNKNOWN) return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)					
	{
		if(pSender == m_pBtnOk)
		{
			switch (m_eCallerWnd)
			{
				case UIWND_TRANSACTION:
					switch (m_eCallerWndDistrict)
					{
						case UIWND_DISTRICT_TRADE_NPC:
						case UIWND_DISTRICT_TRADE_MY:
							CGameProcedure::s_pProcMain->m_pUITransactionDlg->ItemCountOK();
							break;
					}
					break;

				case UIWND_PER_TRADE:
					switch (m_eCallerWndDistrict)
					{
						case UIWND_DISTRICT_PER_TRADE_MY:
							CGameProcedure::s_pProcMain->m_pSubProcPerTrade->ItemCountOK();
							break;
					}
					break;
				case UIWND_WARE_HOUSE:
					switch (m_eCallerWndDistrict)
					{
						case UIWND_DISTRICT_TRADE_NPC:
							if (!m_bWareGold)
								CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->ItemCountOK();
							else
								CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->GoldCountFromWareOK();
							break;
						case UIWND_DISTRICT_TRADE_MY:
							if (!m_bWareGold)
								CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->ItemCountOK();
							else
								CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->GoldCountToWareOK();
							break;
					}
					break;
			}
		}

		if(pSender == m_pBtnCancel)
		{
			switch (m_eCallerWnd)
			{
				case UIWND_TRANSACTION:
					switch (m_eCallerWndDistrict)
					{
						case UIWND_DISTRICT_TRADE_NPC:
						case UIWND_DISTRICT_TRADE_MY:
							CGameProcedure::s_pProcMain->m_pUITransactionDlg->ItemCountCancel();
							break;
					}
					break;

				case UIWND_PER_TRADE:
					switch (m_eCallerWndDistrict)
					{
						case UIWND_DISTRICT_PER_TRADE_MY:
							CGameProcedure::s_pProcMain->m_pSubProcPerTrade->ItemCountCancel();
							break;
					}
					break;
				case UIWND_WARE_HOUSE:
					switch (m_eCallerWndDistrict)
					{
						case UIWND_DISTRICT_TRADE_NPC:
							if (!m_bWareGold)
								CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->ItemCountCancel();
							else
								CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->GoldCountFromWareCancel();
							break;
						case UIWND_DISTRICT_TRADE_MY:
							if (!m_bWareGold)
								CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->ItemCountCancel();
							else
								CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->GoldCountToWareCancel();
							break;
					}
					break;
			}
		}
	}

	return true;
}

void CCountableItemEditDlg::Open(e_UIWND eUW, e_UIWND_DISTRICT eUD, bool bCountGold, bool bWareGold)
{
	std::string szMsg;
	if (bCountGold)
		szMsg = "IDS_EDIT_BOX_GOLD";//::_LoadStringFromResource(IDS_EDIT_BOX_GOLD, szMsg);
	else
		szMsg = "IDS_EDIT_BOX_COUNT";//::_LoadStringFromResource(IDS_EDIT_BOX_COUNT, szMsg);

	CN3UIString* pString = NULL;
	pString = (CN3UIString*)this->GetChildByID("String_PersonTradeEdit_Msg");
	__ASSERT(pString, "NULL UI Component!!");
	if (pString)
		pString->SetString(szMsg);

	RECT rc, rcThis;
	int iCX, iCY;

	m_bLocked = true;
	this->SetQuantity(-1);

	SetVisible(true);
	CN3UIEdit* pEdit = (CN3UIEdit*)this->GetChildByID("edit_trade");
	__ASSERT(pEdit, "NULL UI Component!!");
	if(pEdit) pEdit->SetFocus();


	m_eCallerWnd = eUW;
	m_eCallerWndDistrict = eUD;
	m_bWareGold = bWareGold;

	switch (eUW)
	{
		case UIWND_TRANSACTION:
			rc = CGameProcedure::s_pProcMain->m_pUITransactionDlg->GetRegion();
			break;

		case UIWND_PER_TRADE:
			rc = CGameProcedure::s_pProcMain->m_pSubProcPerTrade->m_pUIPerTradeDlg->GetRegion();
			break;

		case UIWND_WARE_HOUSE:
			rc = CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->GetRegion();
			break;
	}

	if ( (eUW == UIWND_TRANSACTION) || (eUW == UIWND_PER_TRADE) || (eUW == UIWND_WARE_HOUSE) )
	{
		iCX = (rc.right+rc.left)/2;
		iCY = (rc.bottom+rc.top)/2;
		rcThis = GetRegion();
		SetPos(iCX-(rcThis.right-rcThis.left)/2, iCY-(rcThis.bottom-rcThis.top)/2);
	}
}

void CCountableItemEditDlg::Close()
{
	m_bLocked = false;
//	SetVisible(false);	//�̰� ���� ���� SetVisibleWithNoSound �Լ� ���ּ���...
	SetVisibleWithNoSound(false);

	CN3UIEdit* pEdit = GetFocusedEdit();
	if (pEdit) pEdit->KillFocus();
}

int	CCountableItemEditDlg::GetQuantity() // "edit_trade" Edit Control ���� �������� ����´�..
{
	CN3UIEdit* pEdit = (CN3UIEdit*)this->GetChildByID("edit_trade");
	__ASSERT(pEdit, "NULL UI Component!!");

	return atoi(pEdit->GetString().c_str());
}

void CCountableItemEditDlg::SetQuantity(int iQuantity) // "edit_trade" Edit Control ���� �������� ���ڿ��� �����Ѵ�..
{
	CN3UIEdit* pEdit = (CN3UIEdit*)this->GetChildByID("edit_trade");
	__ASSERT(pEdit, "NULL UI Component!!");

	char szBuff[64] = "";
	if (iQuantity != -1)
		sprintf(szBuff, "%d", iQuantity);

	pEdit->SetString(szBuff);
}

//this_ui_add_start
void CCountableItemEditDlg::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}

void CCountableItemEditDlg::SetVisibleWithNoSound(bool bVisible, bool bWork, bool bReFocus)
{
	if(bWork)
	{//���⼭�� ReceiveMessage���� �����̰� ���̴����� üũ�� �ؼ� �ϴ� ���ʿ� �д�.
		ReceiveMessage(m_pBtnCancel, UIMSG_BUTTON_CLICK);
	}
	
	CN3UIBase::SetVisibleWithNoSound(bVisible, bWork, bReFocus);
}

bool CCountableItemEditDlg::Load(HANDLE hFile)
{
	if(false == CN3UIBase::Load(hFile)) return false;

	m_pBtnOk	 = (CN3UIButton*)(this->GetChildByID("btn_ok"));		__ASSERT(m_pBtnOk, "NULL UI Component!!!");
	m_pBtnCancel = (CN3UIButton*)(this->GetChildByID("btn_cancel"));	__ASSERT(m_pBtnCancel, "NULL UI Component!!!");

	return true;
}

bool CCountableItemEditDlg::OnKeyPress(int iKey)
{
	switch(iKey)
	{
	case SDL_SCANCODE_RETURN://DIK_RETURN:
		ReceiveMessage(m_pBtnOk, UIMSG_BUTTON_CLICK);
		return true;
	case SDL_SCANCODE_ESCAPE://DIK_ESCAPE:
		ReceiveMessage(m_pBtnCancel, UIMSG_BUTTON_CLICK);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}
