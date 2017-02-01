// UITransactionDlg.h: interface for the CUITransactionDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UITRANSACTIONDLG_H__42671245_FF4F_42FC_AF7B_DACEDA8734B7__INCLUDED_)
#define AFX_UITRANSACTIONDLG_H__42671245_FF4F_42FC_AF7B_DACEDA8734B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "N3UIArea.h"
#include "N3UIString.h"
#include "N3UIImage.h"
#include "N3UIButton.h"
#include "GameProcedure.h"

#include "GameDef.h"
#include "N3UIWndBase.h"

enum e_NpcTrade {	UI_BLACKSMITH = 1, UI_STORE, UI_INN	};

//////////////////////////////////////////////////////////////////////

class CUITransactionDlg   : public CN3UIWndBase  
{
	friend class CUIInventory;

public:
// ���� �����ؾ� �� ��ü ���� ������
	__IconItemSkill*		m_pMyTrade[MAX_ITEM_TRADE_PAGE][MAX_ITEM_TRADE];
	__IconItemSkill*		m_pMyTradeInv[MAX_ITEM_INVENTORY];
	CN3UIString*			m_pStrMyGold;

	int						m_iCurPage;
	int						m_iTradeID;
	int						m_iNpcID;
	CUIImageTooltipDlg*		m_pUITooltipDlg;

	CN3UIImage*				m_pUIInn;
	CN3UIImage*				m_pUIBlackSmith;
	CN3UIImage*				m_pUIStore;

	//this_ui_add_start
	CN3UIButton*			m_pBtnClose;
	CN3UIButton*			m_pBtnPageUp;
	CN3UIButton*			m_pBtnPageDown;
	//this_ui_add_end


protected:
	int					GetItemiOrder(__IconItemSkill* spItem, e_UIWND_DISTRICT eWndDist);
	RECT				GetSampleRect();

	e_UIWND_DISTRICT	GetWndDistrict(__IconItemSkill* spItem);

public:
	CUITransactionDlg();
	virtual				~CUITransactionDlg();
	void				Release();

	//this_ui_add_start
	bool				OnKeyPress(int iKey);
	bool				Load(HANDLE hFile);
	void				SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
	void				SetVisible(bool bVisible);
	//this_ui_add_end

	virtual DWORD		MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld);
	virtual bool		ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	void				Render();
	void				LeaveTransactionState();
	void				EnterTransactionState();

	void				InitIconWnd(e_UIWND eWnd);
	void				InitIconUpdate();

	__IconItemSkill*	GetHighlightIconItem(CN3UIIcon* pUIIcon);

	void				IconRestore();	

	bool				ReceiveIconDrop(__IconItemSkill* spItem, POINT ptCur);

	void				CancelIconDrop(__IconItemSkill* spItem);
	void				AcceptIconDrop(__IconItemSkill* spItem);

	void				SendToServerSellMsg(int itemID, byte pos, int iCount);
	void				SendToServerBuyMsg(int itemID, byte pos, int iCount);
	void				SendToServerMoveMsg(int itemID, byte startpos, byte destpos);

	// ���� ������ �������� �������� ����..
	void				ReceiveItemDropByTradeSuccess();

	CN3UIBase*			GetChildButtonByName(const std::string& szFN);

	// Item Count OK..
	void				ItemCountOK();
	void				ItemCountCancel();

	void				ItemMoveFromInvToThis();
	void				ItemMoveFromThisToInv();

	void				ReceiveResultTradeFromServer(byte bResult, byte bType, int	iMoney);
	void				ReceiveResultTradeMoveSuccess();
	void				ReceiveResultTradeMoveFail();

	// Ÿ��Ʋ ǥ��..
	void				ShowTitle(e_NpcTrade eNT);

	void				GoldUpdate();
};

#endif // !defined(AFX_UITRANSACTIONDLG_H__42671245_FF4F_42FC_AF7B_DACEDA8734B7__INCLUDED_)
