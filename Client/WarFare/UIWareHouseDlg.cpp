// UIWareHouseDlg.cpp: implementation of the UIWareHouseDlg class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include "LogWriter.h"

#include "PacketDef.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "GameProcMain.h"
#include "PlayerMySelf.h"
#include "GameBase.h"

#include "UIWareHouseDlg.h"
#include "UIImageTooltipDlg.h"
#include "UIInventory.h"
#include "UIManager.h"
#include "PlayerMySelf.h"

#include "CountableItemEditDlg.h"
#include "SubProcPerTrade.h"

#include "UIHotKeyDlg.h"
#include "UISkillTreeDlg.h"

#include "N3UIString.h"
#include "N3UIEdit.h"
#include "N3SndObj.h"
//#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIWareHouseDlg::CUIWareHouseDlg()
{
	int i, j;
	m_iCurPage = 0;
	for( j = 0; j < MAX_ITEM_WARE_PAGE; j++ )
		for( i = 0; i < MAX_ITEM_TRADE; i++ )		
			m_pMyWare[j][i] = NULL;
	for( i = 0; i < MAX_ITEM_INVENTORY; i++ )	m_pMyWareInv[i] = NULL;

	m_pUITooltipDlg		= NULL;
	m_pStrMyGold		= NULL;
	m_pStrWareGold		= NULL;

	m_pBtnGold			= NULL;
	m_pBtnGoldWareHouse = NULL;
	m_pBtnClose			= NULL;
	m_pBtnPageUp		= NULL;
	m_pBtnPageDown		= NULL;

	m_bSendedItemGold = false;
	m_iGoldOffsetBackup = 0;

	this->SetVisible(false);
}

CUIWareHouseDlg::~CUIWareHouseDlg()
{
	Release();
}

void CUIWareHouseDlg::Release()
{
	CN3UIBase::Release();

	int i, j;
	for( j = 0; j < MAX_ITEM_WARE_PAGE; j++ )
		for( i = 0; i < MAX_ITEM_TRADE; i++ )
		{
			if ( m_pMyWare[j][i] != NULL )
			{
				delete m_pMyWare[j][i];
				m_pMyWare[j][i] = NULL;
			}
		}

	for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
	{
		if ( m_pMyWareInv[i] != NULL )
		{
			delete m_pMyWareInv[i];
			m_pMyWareInv[i] = NULL;
		}
	}
}

void CUIWareHouseDlg::Render()
{
	if (!m_bVisible) return;	// ������ ������ �ڽĵ��� render���� �ʴ´�.

	int i;

	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
	m_pUITooltipDlg->DisplayTooltipsDisable();

	bool bTooltipRender = false;
	__IconItemSkill* spItem = NULL;

	for(UIListReverseItor itor = m_Children.rbegin(); m_Children.rend() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		if ( (GetState() == UI_STATE_ICON_MOVING) && (pChild->UIType() == UI_TYPE_ICON) && (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) &&
			((CN3UIIcon *)pChild == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon) )	continue;
			pChild->Render();
		if ( (GetState() == UI_STATE_COMMON_NONE) && 
				(pChild->UIType() == UI_TYPE_ICON) && (pChild->GetStyle() & UISTYLE_ICON_HIGHLIGHT) )
		{
			bTooltipRender = true;
			spItem = GetHighlightIconItem( (CN3UIIcon* )pChild );
		}
	}

	// ���� ǥ�õǾ� �� ������ ���� ǥ��..
	for( i = 0; i < MAX_ITEM_TRADE; i++ )
	{
		if ( m_pMyWare[m_iCurPage][i] && ( (m_pMyWare[m_iCurPage][i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) || 
			(m_pMyWare[m_iCurPage][i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL) ) )
		{
			// string ���..
			CN3UIString* pStr = GetChildStringByiOrder(i+100);
			if(pStr) 
			{
				if ( (GetState() == UI_STATE_ICON_MOVING) && (m_pMyWare[m_iCurPage][i] == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) )
				{
					pStr->SetVisible(false);
				}
				else
				{
					if ( m_pMyWare[m_iCurPage][i]->pUIIcon->IsVisible() )
					{
						pStr->SetVisible(true);
						pStr->SetStringAsInt(m_pMyWare[m_iCurPage][i]->iCount);
						pStr->Render();
					}
					else
					{
						pStr->SetVisible(false);
					}
				}
			}
		}
		else
		{
			// string ���..
			CN3UIString* pStr = GetChildStringByiOrder(i+100);
			if(pStr) 
				pStr->SetVisible(false);
		}
	}

	for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
	{
		if ( m_pMyWareInv[i] && ( (m_pMyWareInv[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE)  || (m_pMyWareInv[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL)  ) )
		{
			// string ���..
			CN3UIString* pStr = GetChildStringByiOrder(i);
			if(pStr) 
			{
				if ( (GetState() == UI_STATE_ICON_MOVING) && (m_pMyWareInv[i] == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) )
				{
					pStr->SetVisible(false);
				}
				else
				{
					if ( m_pMyWareInv[i]->pUIIcon->IsVisible() )
					{
						pStr->SetVisible(true);
						pStr->SetStringAsInt(m_pMyWareInv[i]->iCount);
						pStr->Render();
					}
					else
					{
						pStr->SetVisible(false);
					}
				}
			}
		}
		else
		{
			// string ���..
			CN3UIString* pStr = GetChildStringByiOrder(i);
			if(pStr) 
				pStr->SetVisible(false);
		}
	}

	if ( (GetState() == UI_STATE_ICON_MOVING) && (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) ) 
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->Render();		

	if ( bTooltipRender && spItem )
		m_pUITooltipDlg->DisplayTooltipsEnable(ptCur.x, ptCur.y, spItem);
}

void CUIWareHouseDlg::InitIconWnd(e_UIWND eWnd)
{
	__TABLE_UI_RESRC* pTbl = CGameBase::s_pTbl_UI->Find(CGameBase::s_pPlayer->m_InfoBase.eNation);

	m_pUITooltipDlg = new CUIImageTooltipDlg();
	m_pUITooltipDlg->Init(this);
	m_pUITooltipDlg->LoadFromFile(pTbl->szItemInfo);
	m_pUITooltipDlg->InitPos();
	m_pUITooltipDlg->SetVisible(FALSE);	

	CN3UIWndBase::InitIconWnd(eWnd);

	m_pStrWareGold    = (CN3UIString* )GetChildByID("string_wareitem_name"); __ASSERT(m_pStrWareGold, "NULL UI Component!!");
	if(m_pStrWareGold) m_pStrWareGold->SetString("0");
	m_pStrMyGold    = (CN3UIString* )GetChildByID("string_item_name"); __ASSERT(m_pStrMyGold, "NULL UI Component!!");
	if(m_pStrMyGold) m_pStrMyGold->SetString("0");
}

void CUIWareHouseDlg::InitIconUpdate()
{
	CN3UIArea* pArea;
	float fUVAspect = (float)45.0f/(float)64.0f;
	int i, j;

	for( j = 0; j < MAX_ITEM_WARE_PAGE; j++ )
		for( i = 0; i < MAX_ITEM_TRADE; i++ )
		{
			if ( m_pMyWare[j][i] != NULL )
			{
				m_pMyWare[j][i]->pUIIcon = new CN3UIIcon;
				m_pMyWare[j][i]->pUIIcon->Init(this);
				m_pMyWare[j][i]->pUIIcon->SetTex(m_pMyWare[j][i]->szIconFN);
				m_pMyWare[j][i]->pUIIcon->SetUVRect(0,0,fUVAspect,fUVAspect);
				m_pMyWare[j][i]->pUIIcon->SetUIType(UI_TYPE_ICON);
				m_pMyWare[j][i]->pUIIcon->SetStyle(UISTYLE_ICON_ITEM|UISTYLE_ICON_CERTIFICATION_NEED);
				pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, i);
				if ( pArea )
				{
					m_pMyWare[j][i]->pUIIcon->SetRegion(pArea->GetRegion());
					m_pMyWare[j][i]->pUIIcon->SetMoveRect(pArea->GetRegion());
				}
			}
		}
}

int CUIWareHouseDlg::GetItemiOrder(__IconItemSkill* spItem, e_UIWND_DISTRICT eWndDist)
{
	int iReturn = -1;
	int i;

	switch ( eWndDist )
	{
		case UIWND_DISTRICT_TRADE_NPC:
			for( i = 0; i < MAX_ITEM_TRADE; i++ )
			{
				if ( (m_pMyWare[m_iCurPage][i] != NULL) && (m_pMyWare[m_iCurPage][i] == spItem) )
					return i;
			}
			break;

		case UIWND_DISTRICT_TRADE_MY:
			for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
			{
				if ( (m_pMyWareInv[i] != NULL) && (m_pMyWareInv[i] == spItem) )
					return i;
			}
			break;
	}

	return iReturn;
}

RECT CUIWareHouseDlg::GetSampleRect()
{
	RECT rect;
	CN3UIArea* pArea;
	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
	pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, 0);
	rect = pArea->GetRegion();
	float fWidth = rect.right - rect.left;
	float fHeight = rect.bottom - rect.top;
	fWidth *= 0.5f; fHeight *= 0.5f;
	rect.left = ptCur.x - (int)fWidth;  rect.right  = ptCur.x + (int)fWidth;
	rect.top  = ptCur.y - (int)fHeight; rect.bottom = ptCur.y + (int)fHeight;
	return rect;
}

e_UIWND_DISTRICT CUIWareHouseDlg::GetWndDistrict(__IconItemSkill* spItem)
{
	for( int i = 0; i < MAX_ITEM_TRADE; i++ )
	{
		if ( (m_pMyWare[m_iCurPage][i] != NULL) && (m_pMyWare[m_iCurPage][i] == spItem) )
			return UIWND_DISTRICT_TRADE_NPC;
	}

	for(int i = 0; i < MAX_ITEM_INVENTORY; i++ )
	{
		if ( (m_pMyWareInv[i] != NULL) && (m_pMyWareInv[i] == spItem) )
			return UIWND_DISTRICT_TRADE_MY;
	}
	return UIWND_DISTRICT_UNKNOWN;
}

DWORD CUIWareHouseDlg::MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;
	if (!m_bVisible) return dwRet;
	if (CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer) { dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);  return dwRet; }

	// �巡�� �Ǵ� ������ ����..
	if ( (GetState() == UI_STATE_ICON_MOVING) && 
			(CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd == UIWND_WARE_HOUSE) )
	{
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetRegion(GetSampleRect());
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetMoveRect(GetSampleRect());
	}

	return CN3UIWndBase::MouseProc(dwFlags, ptCur, ptOld);
}

bool CUIWareHouseDlg::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
// Temp Define
#define FAIL_CODE {		\
				SetState(UI_STATE_COMMON_NONE);	\
				return false;	\
			}

	if(NULL == pSender) return false;
	int i, j;

	if (dwMsg == UIMSG_BUTTON_CLICK)					
	{
		if(pSender == m_pBtnGold)
		{
			// �κ��丮�� �� ������..
			CN3UIWndBase::m_pCountableItemEdit->Open(UIWND_WARE_HOUSE, UIWND_DISTRICT_TRADE_MY, true, true);
			return true;
		}

		if(pSender == m_pBtnGoldWareHouse)
		{
			// �κ��丮�� �� ������..
			CN3UIWndBase::m_pCountableItemEdit->Open(UIWND_WARE_HOUSE, UIWND_DISTRICT_TRADE_NPC, true, true);
			return true;
		}

		if(pSender == m_pBtnClose)
			LeaveWareHouseState();

		CN3UIString* pStr;

		if(pSender == m_pBtnPageUp)
		{
			m_iCurPage--;
			if(m_iCurPage<0)
				m_iCurPage = 0;

			pStr = (CN3UIString* )GetChildByID("string_page");
			if (pStr)
			{
				char pszID[32];
				sprintf(pszID, "%d",m_iCurPage+1);
				pStr->SetString(pszID);
			}

			for( j = 0; j < MAX_ITEM_WARE_PAGE; j++ )
			{
				if (j == m_iCurPage)
				{
					for( i = 0; i < MAX_ITEM_TRADE; i++ )
					{
						if ( m_pMyWare[j][i] != NULL )
							m_pMyWare[j][i]->pUIIcon->SetVisible(true);
					}	
				}
				else
				{
					for( i = 0; i < MAX_ITEM_TRADE; i++ )
					{
						if ( m_pMyWare[j][i] != NULL )
							m_pMyWare[j][i]->pUIIcon->SetVisible(false);
					}	
				}
			}
		}

		if(pSender == m_pBtnPageDown)
		{
			m_iCurPage++;
			if (m_iCurPage >= MAX_ITEM_WARE_PAGE)
				m_iCurPage = MAX_ITEM_WARE_PAGE-1;

			pStr = (CN3UIString* )GetChildByID("string_page");
			if (pStr)
			{
				char pszID[32];
				sprintf(pszID, "%d",m_iCurPage+1);
				pStr->SetString(pszID);
			}

			for( j = 0; j < MAX_ITEM_WARE_PAGE; j++ )
			{
				if (j == m_iCurPage)
				{
					for( i = 0; i < MAX_ITEM_TRADE; i++ )
					{
						if ( m_pMyWare[j][i] != NULL )
							m_pMyWare[j][i]->pUIIcon->SetVisible(true);
					}	
				}
				else
				{
					for( i = 0; i < MAX_ITEM_TRADE; i++ )
					{
						if ( m_pMyWare[j][i] != NULL )
							m_pMyWare[j][i]->pUIIcon->SetVisible(false);
					}	
				}
			}
		}
	}

	__IconItemSkill* spItem = NULL;
	e_UIWND_DISTRICT eUIWnd;
	int iOrder;

	DWORD dwBitMask = 0x000f0000;

	switch (dwMsg & dwBitMask)
	{
		case UIMSG_ICON_DOWN_FIRST:
			CN3UIWndBase::AllHighLightIconFree();

			// Get Item..
			spItem = GetHighlightIconItem((CN3UIIcon* )pSender);

			// Save Select Info..
			CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd = UIWND_WARE_HOUSE;
			eUIWnd = GetWndDistrict(spItem);
			if ( eUIWnd == UIWND_DISTRICT_UNKNOWN )	FAIL_CODE
			CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict = eUIWnd;
			iOrder = GetItemiOrder(spItem, eUIWnd);
			if ( iOrder == -1 )	FAIL_CODE
			CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder = iOrder;
			CN3UIWndBase::m_sSelectedIconInfo.pItemSelect = spItem;
			// Do Ops..
			((CN3UIIcon* )pSender)->SetRegion(GetSampleRect());
			((CN3UIIcon* )pSender)->SetMoveRect(GetSampleRect());
			// Sound..
			if (spItem) PlayItemSound(spItem->pItemBasic);
			break;

		case UIMSG_ICON_UP:
			// ������ �Ŵ��� ��������� ���� �ٴϸ鼭 �˻�..
			if ( !CGameProcedure::s_pUIMgr->BroadcastIconDropMsg(CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) )
				// ������ ��ġ �������..
				IconRestore();
			// Sound..
			if (CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) PlayItemSound(CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic);
			break;

		case UIMSG_ICON_DOWN:
			if ( GetState()  == UI_STATE_ICON_MOVING )
			{
				CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetRegion(GetSampleRect());
				CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pUIIcon->SetMoveRect(GetSampleRect());
			}
			break;
	}

	return true;
}

void CUIWareHouseDlg::LeaveWareHouseState()
{
	if ( IsVisible() )
		SetVisible(false);

	if (GetState() == UI_STATE_ICON_MOVING)
		IconRestore();
	SetState(UI_STATE_COMMON_NONE);
	CN3UIWndBase::AllHighLightIconFree();

	// �� �������� inv ������ �������� �� �κ��丮 �������� inv�������� �ű��..	
	ItemMoveFromThisToInv();

	if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
	if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();
}

void CUIWareHouseDlg::EnterWareHouseStateStart(int iWareGold)
{
	int i, j;
	for( j = 0; j < MAX_ITEM_WARE_PAGE; j++ )
		for( i = 0; i < MAX_ITEM_TRADE; i++ )
		{
			if ( m_pMyWare[j][i] != NULL )
			{
				if ( m_pMyWare[j][i]->pUIIcon )
				{
					RemoveChild(m_pMyWare[j][i]->pUIIcon);
					m_pMyWare[j][i]->pUIIcon->Release();
					delete m_pMyWare[j][i]->pUIIcon;
					m_pMyWare[j][i]->pUIIcon = NULL;
				}
				delete m_pMyWare[j][i];	
				m_pMyWare[j][i] = NULL;
			}
		}

	for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
	{
		if ( m_pMyWareInv[i] != NULL )
		{
			if ( m_pMyWareInv[i]->pUIIcon )
			{
				RemoveChild(m_pMyWareInv[i]->pUIIcon);
				m_pMyWareInv[i]->pUIIcon->Release();
				delete m_pMyWareInv[i]->pUIIcon;
				m_pMyWareInv[i]->pUIIcon = NULL;
			}
			delete m_pMyWareInv[i];	
			m_pMyWareInv[i] = NULL;
		}
	}

	if(m_pStrWareGold)
		m_pStrWareGold->SetStringAsInt(iWareGold);
}

void CUIWareHouseDlg::EnterWareHouseStateEnd()
{
	InitIconUpdate();

	m_iCurPage = 0;
	CN3UIString* pStr = (CN3UIString* )GetChildByID("string_page");
	if (pStr)
	{
		char pszID[32];
		sprintf(pszID, "%d",m_iCurPage+1);
		pStr->SetString(pszID);
	}

	int i, j;
	for( j = 0; j < MAX_ITEM_WARE_PAGE; j++ )
	{
		if (j == m_iCurPage)
		{
			for( i = 0; i < MAX_ITEM_TRADE; i++ )
			{
				if ( m_pMyWare[j][i] != NULL )
					m_pMyWare[j][i]->pUIIcon->SetVisible(true);
			}	
		}
		else
		{
			for( i = 0; i < MAX_ITEM_TRADE; i++ )
			{
				if ( m_pMyWare[j][i] != NULL )
					m_pMyWare[j][i]->pUIIcon->SetVisible(false);
			}	
		}
	}

	ItemMoveFromInvToThis();

	if(m_pStrMyGold)
	{
		__InfoPlayerMySelf*	pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);
		m_pStrMyGold->SetStringAsInt(pInfoExt->iGold);
	}
}

__IconItemSkill* CUIWareHouseDlg::GetHighlightIconItem(CN3UIIcon* pUIIcon)
{
	int i;
	for( i = 0; i < MAX_ITEM_TRADE; i++ )
	{
		if ( (m_pMyWare[m_iCurPage][i] != NULL) && (m_pMyWare[m_iCurPage][i]->pUIIcon == pUIIcon) )
			return m_pMyWare[m_iCurPage][i];
	}

	for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
	{
		if ( (m_pMyWareInv[i] != NULL) && (m_pMyWareInv[i]->pUIIcon == pUIIcon) ) 
			return m_pMyWareInv[i];
	}

	return NULL;
}

void CUIWareHouseDlg::IconRestore()
{
	CN3UIArea* pArea;

	switch ( CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict )
	{
		case UIWND_DISTRICT_TRADE_NPC:
			if ( m_pMyWare[m_iCurPage][CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder] != NULL )
			{
				pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder);
				if ( pArea )
				{
					m_pMyWare[m_iCurPage][CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetRegion(pArea->GetRegion());
					m_pMyWare[m_iCurPage][CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetMoveRect(pArea->GetRegion());
				}
			}
			break;

		case UIWND_DISTRICT_TRADE_MY:
			if ( m_pMyWareInv[CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder] != NULL )
			{
				pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder);
				if ( pArea )
				{
					m_pMyWareInv[CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetRegion(pArea->GetRegion());
					m_pMyWareInv[CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder]->pUIIcon->SetMoveRect(pArea->GetRegion());
				}
			}
			break;
	}
}

bool CUIWareHouseDlg::ReceiveIconDrop(__IconItemSkill* spItem, POINT ptCur)
{
// Temp Define 
#define FAIL_RETURN {	\
		CN3UIWndBase::AllHighLightIconFree();	\
		SetState(UI_STATE_COMMON_NONE);	\
		return false;	\
	}

	CN3UIArea* pArea;
	e_UIWND_DISTRICT eUIWnd = UIWND_DISTRICT_UNKNOWN;
	if (!m_bVisible) return false;

	// ���� ������ �������� �ƴϸ�..
	if ( CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd != m_eUIWnd )
		FAIL_RETURN
	if ( (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict != UIWND_DISTRICT_TRADE_NPC) &&
			(CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict != UIWND_DISTRICT_TRADE_MY) )
		FAIL_RETURN

	// ���� ������ �������̸�.. npc�������� �˻��Ѵ�..
	int i, iDestiOrder = -1; bool bFound = false;
	for( i = 0; i < MAX_ITEM_TRADE; i++ )
	{
		pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, i);
		if ( (pArea) && (pArea->IsIn(ptCur.x, ptCur.y)) )
		{
			bFound = true;
			eUIWnd = UIWND_DISTRICT_TRADE_NPC;
			iDestiOrder = i;
			break;
		}
	}

	if (!bFound)
	{
		for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
		{
			pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, i);
			if ( (pArea) && (pArea->IsIn(ptCur.x, ptCur.y)) )
			{
				bFound = true;
				eUIWnd = UIWND_DISTRICT_TRADE_MY;
				iDestiOrder = i;
				break;
			}
		}
	}

	if (!bFound)	FAIL_RETURN

	// ���������� Recovery Info�� Ȱ���ϱ� �����Ѵ�..
	// ���� WaitFromServer�� On���� �ϰ�.. Select Info�� Recovery Info�� ����.. �̶� Dest�� �ӿ����..
	if ( spItem != CN3UIWndBase::m_sSelectedIconInfo.pItemSelect )
		CN3UIWndBase::m_sSelectedIconInfo.pItemSelect = spItem;

	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer					= true;
	CN3UIWndBase::m_sRecoveryJobInfo.pItemSource						= CN3UIWndBase::m_sSelectedIconInfo.pItemSelect;
	CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.UIWnd				= CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd;
	CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.UIWndDistrict		= CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict;
	CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder			= CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.iOrder;
	CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget						= NULL;

	CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.UIWnd				= UIWND_WARE_HOUSE;
	CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.UIWndDistrict		= eUIWnd;

	switch (CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict)
	{
		case UIWND_DISTRICT_TRADE_NPC:
			if (eUIWnd == UIWND_DISTRICT_TRADE_MY)		// ���� ���..
			{
				if( (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
					(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL) )
				{
					// Ȱ�̳� ����� �������� ���..
					// ���� �κ��丮�� �ش� �������� �ִ��� �˾ƺ���..
					bFound = false;

					for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
					{
						if( (m_pMyWareInv[i]) && (m_pMyWareInv[i]->pItemBasic->dwID == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic->dwID) &&
							(m_pMyWareInv[i]->pItemExt->dwID == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemExt->dwID) )
						{
							bFound = true;
							iDestiOrder = i;
							CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder	= iDestiOrder;
							break;
						}
					}

					// ��ã������.. 
					if ( !bFound )
					{
						if ( m_pMyWareInv[iDestiOrder] )	// �ش� ��ġ�� �������� ������..
						{
							// �κ��丮 �󽽷��� ã�� ����..
							for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
							{
								if ( !m_pMyWareInv[i] )
								{
									bFound = true;
									iDestiOrder = i;
									break;
								}
							}

							if ( !bFound )	// �� ������ ã�� ��������..
							{
								CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
								CN3UIWndBase::m_sRecoveryJobInfo.pItemSource		= NULL;
								CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget		= NULL;
								FAIL_RETURN
							}
						}
					}

					CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder	= iDestiOrder;

					CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer				= false;
					CN3UIWndBase::m_pCountableItemEdit->Open(UIWND_WARE_HOUSE, CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict, false);
					FAIL_RETURN
				}
				else
				{
					// �Ϲ� �������� ���..
					if ( m_pMyWareInv[iDestiOrder] )	// �ش� ��ġ�� �������� ������..
					{
						// �κ��丮 �󽽷��� ã�� ����..
						bFound = false;
						for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
						{
							if ( !m_pMyWareInv[i] )
							{
								bFound = true;
								iDestiOrder = i;
								break;
							}
						}

						if ( !bFound )	// �� ������ ã�� ��������..
						{
							CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
							CN3UIWndBase::m_sRecoveryJobInfo.pItemSource		= NULL;
							CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget		= NULL;
							FAIL_RETURN
						}

						CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder	= iDestiOrder;
					}
					else
						CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder	= iDestiOrder;

					// ���� üũ..
					__InfoPlayerMySelf*	pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);
					if ( (pInfoExt->iWeight + CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->siWeight) > pInfoExt->iWeightMax)
					{	 
						std::string szMsg = "IDS_ITEM_WEIGHT_OVERFLOW"; //::_LoadStringFromResource(IDS_ITEM_WEIGHT_OVERFLOW, szMsg);	
						CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);	
						CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
						CN3UIWndBase::m_sRecoveryJobInfo.pItemSource		= NULL;
						CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget		= NULL;
						FAIL_RETURN	
					}

					SendToServerFromWareMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID+
						CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
						m_iCurPage,
						CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder, 
						iDestiOrder, 
						CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->iCount);

					m_pMyWareInv[iDestiOrder] = m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
					m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = NULL;
					pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, iDestiOrder);
					if ( pArea )
					{
						m_pMyWareInv[iDestiOrder]->pUIIcon->SetRegion(pArea->GetRegion());
						m_pMyWareInv[iDestiOrder]->pUIIcon->SetMoveRect(pArea->GetRegion());
					}			
					FAIL_RETURN
				}
			}
			else
			{
				// �̵�.. 
				__IconItemSkill *spItemSource, *spItemTarget = NULL;
				spItemSource = CN3UIWndBase::m_sRecoveryJobInfo.pItemSource;

				pArea = NULL;
				pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, iDestiOrder);
				if ( pArea )
				{
					spItemSource->pUIIcon->SetRegion(pArea->GetRegion());
					spItemSource->pUIIcon->SetMoveRect(pArea->GetRegion());
				}

				CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder				= iDestiOrder;

				// â�� ������ �̵�..	(��� �Ϲ� ���������� ����Ѵ�..)
				if ( m_pMyWare[m_iCurPage][iDestiOrder] )	// �ش� ��ġ�� �������� ������..
				{
					CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
					CN3UIWndBase::m_sRecoveryJobInfo.pItemSource		= NULL;
					CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget		= NULL;
					FAIL_RETURN
				}
				else
					CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget					= NULL;

				m_pMyWare[m_iCurPage][iDestiOrder] = spItemSource;
				m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = spItemTarget;

				// �̵� �޽����� ������..
				SendToServerWareToWareMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID+
					CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
					m_iCurPage,
					CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder, iDestiOrder
					);

				FAIL_RETURN					
			}
			break;

		case UIWND_DISTRICT_TRADE_MY:
			if (eUIWnd == UIWND_DISTRICT_TRADE_NPC)		// �ִ� ���..
			{
				if( (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) || 
					(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL) )
				{
					// Ȱ�̳� ����� �������� ���..
					// ���� Ware�� �������� �ִ��� �˾ƺ���..
					bFound = false;

					// 10���� �������� �� ������..
					for( int iPage = 0; iPage < MAX_ITEM_WARE_PAGE; iPage++ )
					{
						if ( bFound )
							break;

						for( i = 0; i < MAX_ITEM_TRADE; i++ )
						{
							if( (m_pMyWare[iPage][i]) && (m_pMyWare[iPage][i]->pItemBasic->dwID == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemBasic->dwID) &&
								(m_pMyWare[iPage][i]->pItemExt->dwID == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect->pItemExt->dwID) )
							{
								bFound = true;
								iDestiOrder = i;
								CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder	= iDestiOrder;
								CN3UIWndBase::m_sRecoveryJobInfo.m_iPage = iPage;
								break;
							}
						}
					}

					// ��ã������.. 
					if ( !bFound )
					{
						if ( m_pMyWare[m_iCurPage][iDestiOrder] )	// �ش� ��ġ�� �������� ������..
						{
							// �󽽷��� ã�� ����..
							for( int iPage = 0; iPage < MAX_ITEM_WARE_PAGE; iPage++ )
							{
								if ( bFound )
									break;

								for( i = 0; i < MAX_ITEM_TRADE; i++ )
								{
									if ( bFound )
									{
										CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder	= iDestiOrder;
										CN3UIWndBase::m_sRecoveryJobInfo.m_iPage = iPage;
										break;
									}

									if ( !m_pMyWare[iPage][i] )
									{
										bFound = true;
										iDestiOrder = i;
									}
								}

								if ( !bFound )	// �� ������ ã�� ��������..
								{
									CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
									CN3UIWndBase::m_sRecoveryJobInfo.pItemSource		= NULL;
									CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget		= NULL;
									FAIL_RETURN
								}
							}
						}
						else
						{
							CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder	= iDestiOrder;
							CN3UIWndBase::m_sRecoveryJobInfo.m_iPage = m_iCurPage;
						}
					}

					CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer				= false;
					CN3UIWndBase::m_pCountableItemEdit->Open(UIWND_WARE_HOUSE, CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWndDistrict, false);
					FAIL_RETURN
				}
				else
				{
					// �Ϲ� �������� ���..
					if ( m_pMyWare[m_iCurPage][iDestiOrder] )	// �ش� ��ġ�� �������� ������..
					{
						// �κ��丮 �󽽷��� ã�� ����..
						bFound = false;

						// 10���� �������� �� ������..
						for( int iPage = 0; iPage < MAX_ITEM_WARE_PAGE; iPage++ )
						{
							if ( bFound )
								break;

							for( i = 0; i < MAX_ITEM_TRADE; i++ )
							{
								if ( bFound )
								{
									CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder	= iDestiOrder;
									CN3UIWndBase::m_sRecoveryJobInfo.m_iPage = iPage;
									break;
								}

								if ( !m_pMyWare[iPage][i] )
								{
									bFound = true;
									iDestiOrder = i;
								}
							}
						}

						if ( !bFound )	// �� ������ ã�� ��������..
						{
							CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
							CN3UIWndBase::m_sRecoveryJobInfo.pItemSource		= NULL;
							CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget		= NULL;
							FAIL_RETURN
						}
					}
					else
					{
						CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder	= iDestiOrder;
						CN3UIWndBase::m_sRecoveryJobInfo.m_iPage = m_iCurPage;
					}

					SendToServerToWareMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID+
						CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
						CN3UIWndBase::m_sRecoveryJobInfo.m_iPage,
						CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder, 
						iDestiOrder, 
						CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->iCount);

					m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][iDestiOrder] = m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
					m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = NULL;

					pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, iDestiOrder);
					if ( pArea )
					{
						m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][iDestiOrder]->pUIIcon->SetRegion(pArea->GetRegion());
						m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][iDestiOrder]->pUIIcon->SetMoveRect(pArea->GetRegion());
					}
					
					if (CN3UIWndBase::m_sRecoveryJobInfo.m_iPage != m_iCurPage)
						m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][iDestiOrder]->pUIIcon->SetVisibleWithNoSound(false);

					FAIL_RETURN
				}
			}
			else
			{
				// �̵�.. 
				__IconItemSkill *spItemSource, *spItemTarget = NULL;
				spItemSource = CN3UIWndBase::m_sRecoveryJobInfo.pItemSource;

				pArea = NULL;
				pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, iDestiOrder);
				if ( pArea )
				{
					spItemSource->pUIIcon->SetRegion(pArea->GetRegion());
					spItemSource->pUIIcon->SetMoveRect(pArea->GetRegion());
				}

				CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder				= iDestiOrder;

				// Inv ������ �̵�..	(��� �Ϲ� ���������� ����Ѵ�..)
				if ( m_pMyWareInv[iDestiOrder] )	// �ش� ��ġ�� �������� ������..
				{
					CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget					= m_pMyWareInv[iDestiOrder];
					CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetStart.UIWnd			= UIWND_WARE_HOUSE;
					CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetStart.UIWndDistrict = UIWND_DISTRICT_TRADE_MY;
					CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetStart.iOrder		= iDestiOrder;
					CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetEnd.UIWnd			= UIWND_WARE_HOUSE;
					CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetEnd.UIWndDistrict	= UIWND_DISTRICT_TRADE_MY;
					CN3UIWndBase::m_sRecoveryJobInfo.UIWndTargetEnd.iOrder			= 
						CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder;

					spItemTarget = CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget;

					pArea = NULL;
					pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
					if ( pArea )
					{
						spItemTarget->pUIIcon->SetRegion(pArea->GetRegion());
						spItemTarget->pUIIcon->SetMoveRect(pArea->GetRegion());
					}
				}
				else
					CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget					= NULL;

				m_pMyWareInv[iDestiOrder] = spItemSource;
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = spItemTarget;

				// �̵� �޽����� ������..
				SendToServerInvToInvMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID+
					CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
					m_iCurPage,
					CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder, 
					iDestiOrder
					);

				FAIL_RETURN
			}
			break;
	}


	CN3UIWndBase::AllHighLightIconFree();
	SetState(UI_STATE_COMMON_NONE);

	return false;
}

void CUIWareHouseDlg::CancelIconDrop(__IconItemSkill* spItem)
{
	CN3UIWndBase::AllHighLightIconFree();
	SetState(UI_STATE_COMMON_NONE);
}

void CUIWareHouseDlg::AcceptIconDrop(__IconItemSkill* spItem)
{
	CN3UIWndBase::AllHighLightIconFree();
	SetState(UI_STATE_COMMON_NONE);
}

void CUIWareHouseDlg::SendToServerToWareMsg(int iItemID, byte page, byte startpos, byte pos, int iCount)
{
	BYTE byBuff[32];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_WAREHOUSE);
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_WARE_GET_IN);
	CAPISocket::MP_AddDword(byBuff, iOffset, iItemID);	
	CAPISocket::MP_AddByte(byBuff, iOffset, page);
	CAPISocket::MP_AddByte(byBuff, iOffset, startpos);
	CAPISocket::MP_AddByte(byBuff, iOffset, pos);
	CAPISocket::MP_AddDword(byBuff, iOffset, iCount);

	CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUIWareHouseDlg::SendToServerFromWareMsg(int iItemID, byte page, byte startpos, byte pos, int iCount)
{
	BYTE byBuff[32];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_WAREHOUSE);
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_WARE_GET_OUT);
	CAPISocket::MP_AddDword(byBuff, iOffset, iItemID);	
	CAPISocket::MP_AddByte(byBuff, iOffset, page);
	CAPISocket::MP_AddByte(byBuff, iOffset, startpos);
	CAPISocket::MP_AddByte(byBuff, iOffset, pos);
	CAPISocket::MP_AddDword(byBuff, iOffset, iCount);

	CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUIWareHouseDlg::SendToServerWareToWareMsg(int iItemID, byte page, byte startpos, byte destpos)
{
	BYTE byBuff[32];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_WAREHOUSE);
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_WARE_WARE_MOVE);
	CAPISocket::MP_AddDword(byBuff, iOffset, iItemID);	
	CAPISocket::MP_AddByte(byBuff, iOffset, page);	
	CAPISocket::MP_AddByte(byBuff, iOffset, startpos);
	CAPISocket::MP_AddByte(byBuff, iOffset, destpos);

	CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUIWareHouseDlg::SendToServerInvToInvMsg(int iItemID, byte page, byte startpos, byte destpos)
{
	BYTE byBuff[32];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_WAREHOUSE);
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_WARE_INV_MOVE);
	CAPISocket::MP_AddDword(byBuff, iOffset, iItemID);	
	CAPISocket::MP_AddByte(byBuff, iOffset, page);	
	CAPISocket::MP_AddByte(byBuff, iOffset, startpos);
	CAPISocket::MP_AddByte(byBuff, iOffset, destpos);

	CGameProcedure::s_pSocket->Send(byBuff, iOffset);
}

void CUIWareHouseDlg::ReceiveResultToWareMsg(BYTE bResult)	// �ִ� ���..
{
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
	int iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();
	__IconItemSkill* spItem;
	CN3UIArea* pArea = NULL;

	if (bResult != 0x01)	// ����..
	{
		if (m_bSendedItemGold) {	ReceiveResultGoldToWareFail();	return;		}
		if( (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) ||
			(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL) )			// Ȱ�̳� ����� �������� ���..
		{
			// Ware Side..	//////////////////////////////////////////////////////

			if ( (m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount - iGold) > 0 )
			{	
				//  ���� ������Ʈ..
				m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount -= iGold;
			}
			else
			{
				// ������ ����.. ���� �κ��丮 �����츸.. 
				spItem = m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];

				// �κ��丮������ �����..
				m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;

				// iOrder�� �� �Ŵ����� �������� ����Ʈ���� �����Ѵ�..
				RemoveChild(spItem->pUIIcon);

				// ������ ���ҽ� ����...
				spItem->pUIIcon->Release();
				delete spItem->pUIIcon;
				spItem->pUIIcon = NULL;
				delete spItem;
				spItem = NULL;
			}

			// Inv Side..	//////////////////////////////////////////////////////

			if ( !m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->IsVisible() )
			{
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->SetVisible(true);
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->iCount = iGold;
			}
			else
			{
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->iCount += iGold;
			}
		}
		else	// �Ϲ� �������� ���..														
		{
			m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = 
				m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]; 

			pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
			if ( pArea )
			{
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->SetRegion(pArea->GetRegion());
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->SetMoveRect(pArea->GetRegion());
			}			

			m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;
		}
	}

	// ����.. 
	else
	{
		if (m_bSendedItemGold) 
		{
			m_bSendedItemGold = false;		// ���� ���..
			return;
		}
		if( ( (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) || 
			(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL) ) &&
			!m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->IsVisible() )			// Ȱ�̳� ����� �������� ���..
		{				
			spItem = m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];

			// �κ��丮������ �����..
			m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = NULL;

			// iOrder�� �� �Ŵ����� �������� ����Ʈ���� �����Ѵ�..
			RemoveChild(spItem->pUIIcon);

			// ������ ���ҽ� ����...
			spItem->pUIIcon->Release();
			delete spItem->pUIIcon;
			spItem->pUIIcon = NULL;
			delete spItem;
			spItem = NULL;
		}
	}
}

void CUIWareHouseDlg::ReceiveResultFromWareMsg(BYTE bResult)	// ���� ���..
{
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
	int iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();
	__IconItemSkill* spItem;
	CN3UIArea* pArea = NULL;

	if (bResult != 0x01)	// ����..
	{
		if (m_bSendedItemGold) {	ReceiveResultGoldFromWareFail();	return;		}
		if( (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) || 
			(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL) )
		{
			// Inv Side..	//////////////////////////////////////////////////////

			if ( (m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount - iGold) > 0 )
			{	
				//  ���� ������Ʈ..
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount -= iGold;
			}
			else
			{
				// ������ ����.. ���� �κ��丮 �����츸.. 
				spItem = m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];

				// �κ��丮������ �����..
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;

				// iOrder�� �� �Ŵ����� �������� ����Ʈ���� �����Ѵ�..
				RemoveChild(spItem->pUIIcon);

				// ������ ���ҽ� ����...
				spItem->pUIIcon->Release();
				delete spItem->pUIIcon;
				spItem->pUIIcon = NULL;
				delete spItem;
				spItem = NULL;
			}

			// Ware Side..	//////////////////////////////////////////////////////

			if ( !m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->IsVisible() )
			{
				m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->SetVisible(true);
				m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->iCount = iGold;
			}
			else
			{
				m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->iCount+= iGold;
			}
		}
		else	// �Ϲ� ������..
		{
			m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = 
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]; 

			pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
			if ( pArea )
			{
				m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->SetRegion(pArea->GetRegion());
				m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->SetMoveRect(pArea->GetRegion());
			}			

			m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;
		}

		std::string szMsg = "You cannot trade or pick up items because you have either exceeded the possible quantity or the weight."; //::_LoadStringFromResource(IDS_ITEM_TOOMANY_OR_HEAVY, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
	}

	// ����.. 
	else
	{
		if (m_bSendedItemGold) 
		{
			m_bSendedItemGold = false;		// ���� ���..
			return;
		}
		if( ( (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) || 
			(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL) ) &&
			!m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder]->pUIIcon->IsVisible() )			// Ȱ�̳� ����� �������� ���..
		{				
			spItem = m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];

			// �κ��丮������ �����..
			m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = NULL;

			// iOrder�� �� �Ŵ����� �������� ����Ʈ���� �����Ѵ�..
			RemoveChild(spItem->pUIIcon);

			// ������ ���ҽ� ����...
			spItem->pUIIcon->Release();
			delete spItem->pUIIcon;
			spItem->pUIIcon = NULL;
			delete spItem;
			spItem = NULL;
		}
	}
}

void CUIWareHouseDlg::ReceiveResultWareToWareMsg(BYTE bResult)
{
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
	CN3UIArea* pArea = NULL;

	if (bResult != 0x01)	// ����..
	{
		__IconItemSkill *spItemSource = NULL, *spItemTarget = NULL;
		spItemSource = CN3UIWndBase::m_sRecoveryJobInfo.pItemSource;
		spItemTarget = CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget;

		if (spItemSource)
		{
			pArea = NULL;
			pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
			if ( pArea )
			{
				spItemSource->pUIIcon->SetRegion(pArea->GetRegion());
				spItemSource->pUIIcon->SetMoveRect(pArea->GetRegion());
			}

			m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = spItemSource;
		}

		if (spItemTarget)
		{
			pArea = NULL;
			pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder);
			if ( pArea )
			{
				spItemTarget->pUIIcon->SetRegion(pArea->GetRegion());
				spItemTarget->pUIIcon->SetMoveRect(pArea->GetRegion());
			}

			m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = spItemTarget;
		}
		else
		{
			m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;
		}
	}

	CN3UIWndBase::AllHighLightIconFree();
	SetState(UI_STATE_COMMON_NONE);
}

void CUIWareHouseDlg::ReceiveResultInvToInvMsg(BYTE bResult)
{
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
	CN3UIArea* pArea = NULL;

	if (bResult != 0x01)	// ����..
	{
		__IconItemSkill *spItemSource = NULL, *spItemTarget = NULL;
		spItemSource = CN3UIWndBase::m_sRecoveryJobInfo.pItemSource;
		spItemTarget = CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget;

		if (spItemSource)
		{
			pArea = NULL;
			pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder);
			if ( pArea )
			{
				spItemSource->pUIIcon->SetRegion(pArea->GetRegion());
				spItemSource->pUIIcon->SetMoveRect(pArea->GetRegion());
			}

			m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = spItemSource;
		}

		if (spItemTarget)
		{
			pArea = NULL;
			pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder);
			if ( pArea )
			{
				spItemTarget->pUIIcon->SetRegion(pArea->GetRegion());
				spItemTarget->pUIIcon->SetMoveRect(pArea->GetRegion());
			}

			m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = spItemTarget;
		}
		else
		{
			m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = NULL;
		}
	}

	CN3UIWndBase::AllHighLightIconFree();
	SetState(UI_STATE_COMMON_NONE);
}

void CUIWareHouseDlg::ItemCountOK()
{
	int iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();
	__IconItemSkill* spItem;
	CN3UIArea* pArea = NULL;
	float fUVAspect		= (float)45.0f/(float)64.0f;
	int iWeight;
	__InfoPlayerMySelf*	pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

	switch (CN3UIWndBase::m_pCountableItemEdit->GetCallerWndDistrict())
	{
		case UIWND_DISTRICT_TRADE_NPC:		// ���� ���..
			spItem = m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
			if ( iGold > spItem->iCount ) return;

			switch (spItem->pItemBasic->byContable)
			{
				case UIITEM_TYPE_ONLYONE:
				case UIITEM_TYPE_SOMOONE:
					iWeight = spItem->pItemBasic->siWeight;

					// ���� üũ..
					if ( (pInfoExt->iWeight + iWeight) > pInfoExt->iWeightMax)
					{	 
						std::string szMsg = "IDS_ITEM_WEIGHT_OVERFLOW"; //::_LoadStringFromResource(IDS_ITEM_WEIGHT_OVERFLOW, szMsg);	
						CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);	
						return;
					}
					break;

				case UIITEM_TYPE_COUNTABLE:
					if ( iGold <= 0 ) return;
					// short �����̻��� ��� ����..
					if ( iGold > UIITEM_COUNT_MANY ) 
					{
						std::string szMsg = "IDS_MANY_COUNTABLE_ITEM_GET_MANY"; //::_LoadStringFromResource(IDS_MANY_COUNTABLE_ITEM_GET_MANY, szMsg);
						CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);				
						return;
					}

					if (m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder])				
					{
						spItem = m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];
						if (spItem->iCount + iGold > UIITEM_COUNT_MANY)
						{
							std::string szMsg = "IDS_MANY_COUNTABLE_ITEM_GET_MANY"; //::_LoadStringFromResource(IDS_MANY_COUNTABLE_ITEM_GET_MANY, szMsg);
							CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);				
							return;
						}
					}

					// ���� üũ..
					iWeight = iGold * spItem->pItemBasic->siWeight;
					if ( (pInfoExt->iWeight + iWeight) > pInfoExt->iWeightMax)
					{	 
						std::string szMsg = "IDS_ITEM_WEIGHT_OVERFLOW"; //::_LoadStringFromResource(IDS_ITEM_WEIGHT_OVERFLOW, szMsg);	
						CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);	
						return;
					}
					break;

				case UIITEM_TYPE_COUNTABLE_SMALL:
					if ( iGold <= 0 ) return;
					// short �����̻��� ��� ����..
					if ( iGold > UIITEM_COUNT_FEW ) 
					{
						std::string szMsg = "IDS_SMALL_COUNTABLE_ITEM_GET_MANY"; //::_LoadStringFromResource(IDS_SMALL_COUNTABLE_ITEM_GET_MANY, szMsg);
						CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);				
						return;
					}

					if (m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder])				
					{
						spItem = m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];
						if (spItem->iCount + iGold > UIITEM_COUNT_FEW)
						{
							std::string szMsg = "IDS_SMALL_COUNTABLE_ITEM_GET_MANY"; //::_LoadStringFromResource(IDS_SMALL_COUNTABLE_ITEM_GET_MANY, szMsg);
							CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);				
							return;
						}
					}

					// ���� üũ..
					iWeight = iGold * spItem->pItemBasic->siWeight;
					if ( (pInfoExt->iWeight + iWeight) > pInfoExt->iWeightMax)
					{	 
						std::string szMsg = "IDS_ITEM_WEIGHT_OVERFLOW"; //::_LoadStringFromResource(IDS_ITEM_WEIGHT_OVERFLOW, szMsg);	
						CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);	
						return;
					}
					break;
			}

			spItem = m_pMyWare[m_iCurPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
			CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer	= true;

			if ( m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] )	// �ش� ��ġ�� �������� ������..
			{
				//  ���� ������Ʈ..
				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount += iGold;
			}
			else	// ������ �������� �����...
			{
				__IconItemSkill* spItemNew;
				spItemNew				= new __IconItemSkill;
				spItemNew->pItemBasic	= spItem->pItemBasic;
				spItemNew->pItemExt		= spItem->pItemExt;
				spItemNew->szIconFN		= spItem->szIconFN;
				spItemNew->iCount		= iGold;
				spItemNew->iDurability	= spItem->iDurability;
				spItemNew->pUIIcon		= new CN3UIIcon;
				spItemNew->pUIIcon->Init(this);
				spItemNew->pUIIcon->SetTex(spItem->szIconFN);
				spItemNew->pUIIcon->SetUVRect(0,0, fUVAspect, fUVAspect);
				spItemNew->pUIIcon->SetUIType(UI_TYPE_ICON);
				spItemNew->pUIIcon->SetStyle(UISTYLE_ICON_ITEM|UISTYLE_ICON_CERTIFICATION_NEED);
				spItemNew->pUIIcon->SetVisible(true);
				pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder);
				if ( pArea )
				{
					spItemNew->pUIIcon->SetRegion(pArea->GetRegion());
					spItemNew->pUIIcon->SetMoveRect(pArea->GetRegion());
				}

				m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = spItemNew;
			}

			if ( (spItem->iCount - iGold) > 0 )
			{	
				//  ���� ������Ʈ..
				spItem->iCount -= iGold;
			}
			else
			{
				spItem->pUIIcon->SetVisible(false);
			}

			// ǥ�ô� ������ �������Ҷ�.. Inventory�� Render����..
			// �������� ����..
			SendToServerFromWareMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID+
				CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
				m_iCurPage,
				CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder,
				CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder, 
				iGold);

			// Sound..
			if (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource) PlayItemSound(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic);
			break;

		case UIWND_DISTRICT_TRADE_MY:		//  �ִ� ���..
			spItem = m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
			if ( iGold > spItem->iCount ) return;

			switch (spItem->pItemBasic->byContable)
			{
				case UIITEM_TYPE_COUNTABLE:
					if ( iGold <= 0 ) return;
					// short �����̻��� ��� ����..
					if ( iGold > UIITEM_COUNT_MANY ) 
					{
						std::string szMsg = "IDS_MANY_COUNTABLE_ITEM_GET_MANY"; //::_LoadStringFromResource(IDS_MANY_COUNTABLE_ITEM_GET_MANY, szMsg);
						CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);				
						return;
					}

					if (m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder])
					{
						spItem = m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];
						if (spItem->iCount + iGold > UIITEM_COUNT_MANY)
						{
							std::string szMsg = "IDS_MANY_COUNTABLE_ITEM_GET_MANY"; //::_LoadStringFromResource(IDS_MANY_COUNTABLE_ITEM_GET_MANY, szMsg);
							CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);				
							return;
						}
					}
					break;

				case UIITEM_TYPE_COUNTABLE_SMALL:
					if ( iGold <= 0 ) return;
					// short �����̻��� ��� ����..
					if ( iGold > UIITEM_COUNT_FEW ) 
					{
						std::string szMsg = "IDS_SMALL_COUNTABLE_ITEM_GET_MANY"; //::_LoadStringFromResource(IDS_SMALL_COUNTABLE_ITEM_GET_MANY, szMsg);
						CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);				
						return;
					}

					if (m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder])
					{
						spItem = m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder];
						if (spItem->iCount + iGold > UIITEM_COUNT_FEW)
						{
							std::string szMsg = "IDS_SMALL_COUNTABLE_ITEM_GET_MANY"; //::_LoadStringFromResource(IDS_SMALL_COUNTABLE_ITEM_GET_MANY, szMsg);
							CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);				
							return;
						}
					}
					break;
			}

			spItem = m_pMyWareInv[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
			CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer	= true;

			if ( m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] )	// �ش� ��ġ�� �������� ������..
			{
				//  ���� ������Ʈ..
				m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder]->iCount += iGold;
			}
			else	// ������ �������� �����..
			{
				__IconItemSkill* spItemNew;
				spItemNew				= new __IconItemSkill;
				spItemNew->pItemBasic	= spItem->pItemBasic;
				spItemNew->pItemExt		= spItem->pItemExt;
				spItemNew->szIconFN		= spItem->szIconFN;
				spItemNew->iCount		= iGold;
				spItemNew->iDurability	= spItem->iDurability;
				spItemNew->pUIIcon		= new CN3UIIcon;
				spItemNew->pUIIcon->Init(this);
				spItemNew->pUIIcon->SetTex(spItem->szIconFN);
				spItemNew->pUIIcon->SetUVRect(0,0, fUVAspect, fUVAspect);
				spItemNew->pUIIcon->SetUIType(UI_TYPE_ICON);
				spItemNew->pUIIcon->SetStyle(UISTYLE_ICON_ITEM|UISTYLE_ICON_CERTIFICATION_NEED);
				spItemNew->pUIIcon->SetVisible(true);
				pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_NPC, CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder);
				if ( pArea )
				{
					spItemNew->pUIIcon->SetRegion(pArea->GetRegion());
					spItemNew->pUIIcon->SetMoveRect(pArea->GetRegion());
				}

				if (CN3UIWndBase::m_sRecoveryJobInfo.m_iPage != m_iCurPage)
					spItemNew->pUIIcon->SetVisibleWithNoSound(false);

				m_pMyWare[CN3UIWndBase::m_sRecoveryJobInfo.m_iPage][CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder] = spItemNew;
			}

			if ( (spItem->iCount - iGold) > 0 )
			{	
				//  ���� ������Ʈ..
				spItem->iCount -= iGold;
			}
			else
			{
				spItem->pUIIcon->SetVisible(false);
			}

			// ǥ�ô� ������ �������Ҷ�.. Inventory�� Render����..
			// �������� ����..
			SendToServerToWareMsg(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic->dwID+
				CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemExt->dwID,
				CN3UIWndBase::m_sRecoveryJobInfo.m_iPage,
				CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder,
				CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder, iGold);

			break;
	}

	CN3UIWndBase::m_pCountableItemEdit->Close();
}

void CUIWareHouseDlg::ItemCountCancel()
{
	// Sound..
	if (CN3UIWndBase::m_sRecoveryJobInfo.pItemSource) PlayItemSound(CN3UIWndBase::m_sRecoveryJobInfo.pItemSource->pItemBasic);

	// ���..
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
	CN3UIWndBase::m_sRecoveryJobInfo.pItemSource		= NULL;
	CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget		= NULL;

	CN3UIWndBase::m_pCountableItemEdit->Close();
}

void CUIWareHouseDlg::ItemMoveFromInvToThis()
{
	CUIInventory* pInven = CGameProcedure::s_pProcMain->m_pUIInventory;
	if(!pInven) return;

	int i;
	for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
	{
		m_pMyWareInv[i] = NULL;
	}

	for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
	{
		if(pInven->m_pMyInvWnd[i])
		{
			__IconItemSkill* spItem = pInven->m_pMyInvWnd[i];
			spItem->pUIIcon->SetParent(this);

			pInven->m_pMyInvWnd[i] = NULL;
			CN3UIArea* pArea;

			pArea = GetChildAreaByiOrder(UI_AREA_TYPE_TRADE_MY, i);
			if ( pArea )
			{
				spItem->pUIIcon->SetRegion(pArea->GetRegion());
				spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
			}			

			m_pMyWareInv[i] = spItem;
		}
	}
}

void CUIWareHouseDlg::ItemMoveFromThisToInv()
{
	CUIInventory* pInven = CGameProcedure::s_pProcMain->m_pUIInventory;
	if(!pInven) return;

	int i;
	for( i = 0; i < MAX_ITEM_INVENTORY; i++ )
	{
		if(m_pMyWareInv[i])
		{
			__IconItemSkill* spItem = m_pMyWareInv[i];
			spItem->pUIIcon->SetParent(pInven);

			m_pMyWareInv[i] = NULL;

			CN3UIArea* pArea;

			pArea = pInven->GetChildAreaByiOrder(UI_AREA_TYPE_INV, i);
			if ( pArea )
			{
				spItem->pUIIcon->SetRegion(pArea->GetRegion());
				spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
			}			

			pInven->m_pMyInvWnd[i] = spItem;
		}
	}
}

void CUIWareHouseDlg::AddItemInWare(int iItem, int iDurability, int iCount, int iIndex)
{

	if (!iItem) return;
	std::string szIconFN;
	__IconItemSkill*	spItem = NULL;
	__TABLE_ITEM_BASIC*	pItem = NULL;													// ������ ���̺� ����ü ������..
	__TABLE_ITEM_EXT*	pItemExt = NULL;

	pItem = CGameBase::s_pTbl_Items_Basic->Find(iItem/1000*1000);	// �� ������ ���..
	if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
		pItemExt = CGameBase::s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItem%1000);	// �� ������ ���..
	if ( NULL == pItem || NULL == pItemExt )
	{
		__ASSERT(0, "NULL Item!!!");
		CLogWriter::Write("WareHouse - Ware - Unknown Item %d, IDNumber", iItem);
		return; // �������� ������..
	}

	e_PartPosition ePart;
	e_PlugPosition ePlug;
	e_ItemType eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart, ePlug); // �����ۿ� ���� ���� �̸��� ����
	if(ITEM_TYPE_UNKNOWN == eType) CLogWriter::Write("MyInfo - slot - Unknown Item");
	__ASSERT(ITEM_TYPE_UNKNOWN != eType, "Unknown Item");
	
	spItem = new __IconItemSkill;
	spItem->pItemBasic	= pItem;
	spItem->pItemExt	= pItemExt;
	spItem->szIconFN	= szIconFN; // ������ ���� �̸� ����..
	spItem->iCount		= iCount;
	spItem->iDurability = iDurability;

	m_pMyWare[iIndex/MAX_ITEM_TRADE][iIndex%MAX_ITEM_TRADE] = spItem;
	//TRACE("Init Inv Msg Inve %d, iOrder %d \n", iItem, iIndex);
}

void CUIWareHouseDlg::GoldCountToWareOK()	//���� �ִ� ���..
{
	char szGold[32];
	int iGold, iMyMoney, iWareMoney;			// �κ��丮�� ��..
	std::string str;

	// ���� �����Կ� �����ϴ� ���..
	iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();

	// Gold Offset Backup..
	m_iGoldOffsetBackup = iGold;

	// ���� ���� ���� ���� ��� �´�..
	iMyMoney = CGameBase::s_pPlayer->m_InfoExt.iGold;

	// �������� ���� ���´�..
	CN3UIString* pStr = NULL;
	pStr = (CN3UIString* )GetChildByID("string_wareitem_name");	 
	__ASSERT(pStr, "NULL UI Component!!");
	str = pStr->GetString();
	iWareMoney = atoi(str.c_str());

	if ( iGold <= 0 ) return;
	if ( iGold > iMyMoney ) return;

	m_bSendedItemGold = true;		// ���� �������� ���̴�.. !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// ���� ���� ��Ų��..
	iMyMoney -= iGold;
	CGameBase::s_pPlayer->m_InfoExt.iGold = iMyMoney;

	iWareMoney += iGold;

	// �� ǥ��.. Ware..
	pStr->SetStringAsInt(iWareMoney);
	// �� ǥ��.. �κ��丮..
	sprintf(szGold, "%d", iMyMoney);	pStr = NULL;
	str = szGold;
	pStr = (CN3UIString* )CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold"); __ASSERT(pStr, "NULL UI Component!!");
	if(pStr)	pStr->SetString(str);	pStr = NULL;	
	// �� ǥ��.. Inv..
	pStr = (CN3UIString* )GetChildByID("string_item_name"); __ASSERT(pStr, "NULL UI Component!!");
	if(pStr) pStr->SetStringAsInt(iMyMoney);

	// �������� ��Ŷ ���� ����..
	SendToServerToWareMsg(dwGold, 0xff, 0xff, 0xff, iGold);

	// ���¸� ��ȭ��Ű��.. â�� �ݰ�..
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = true;
	CN3UIWndBase::m_pCountableItemEdit->Close();

	PlayGoldSound();
}

void CUIWareHouseDlg::GoldCountFromWareOK()		// ���� ���� ���..
{
	char szGold[32];
	int iGold, iMyMoney, iWareMoney;			// �κ��丮�� ��..
	std::string str;

	// ���� �����Կ��� ���� ���..
	iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();

	// Gold Offset Backup..
	m_iGoldOffsetBackup = iGold;

	// ���� ���� ���� ���� ��� �´�..
	iMyMoney = CGameBase::s_pPlayer->m_InfoExt.iGold;

	// �������� ���� ���´�..
	CN3UIString* pStr = NULL;
	pStr = (CN3UIString* )GetChildByID("string_wareitem_name");	 
	__ASSERT(pStr, "NULL UI Component!!");
	str = pStr->GetString();
	iWareMoney = atoi(str.c_str());

	if ( iGold <= 0 ) return;
	if ( iGold > iWareMoney ) return;

	m_bSendedItemGold = true;		// ���� �������� ���̴�.. !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// ���� ���� ��Ų��..
	iMyMoney += iGold;
	CGameBase::s_pPlayer->m_InfoExt.iGold = iMyMoney;

	iWareMoney -= iGold;

	// �� ǥ��.. Ware..
	pStr->SetStringAsInt(iWareMoney);
	// �� ǥ��.. �κ��丮..
	sprintf(szGold, "%d", iMyMoney);	pStr = NULL;
	str = szGold;
	pStr = (CN3UIString* )CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold"); __ASSERT(pStr, "NULL UI Component!!");
	if(pStr)	pStr->SetString(str);	pStr = NULL;	
	// �� ǥ��.. Inv..
	pStr = (CN3UIString* )GetChildByID("string_item_name"); __ASSERT(pStr, "NULL UI Component!!");
	if(pStr) pStr->SetStringAsInt(iMyMoney);

	// �������� ��Ŷ ���� ����..
	SendToServerFromWareMsg(dwGold, 0xff, 0xff, 0xff, iGold);

	// ���¸� ��ȭ��Ű��.. â�� �ݰ�..
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = true;
	CN3UIWndBase::m_pCountableItemEdit->Close();

	PlayGoldSound();
}

void CUIWareHouseDlg::GoldCountToWareCancel()
{
	// ���� �����Կ� �����ϴ� ��� ���..
	// Sound..
	PlayGoldSound();

	// ���..
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
	CN3UIWndBase::m_sRecoveryJobInfo.pItemSource		= NULL;
	CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget		= NULL;

	CN3UIWndBase::m_pCountableItemEdit->Close();
}

void CUIWareHouseDlg::GoldCountFromWareCancel()
{
	// ���� �����Կ��� ������ ��� ���..
	// Sound..
	PlayGoldSound();

	// ���..
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer  = false;
	CN3UIWndBase::m_sRecoveryJobInfo.pItemSource		= NULL;
	CN3UIWndBase::m_sRecoveryJobInfo.pItemTarget		= NULL;

	CN3UIWndBase::m_pCountableItemEdit->Close();
}

void CUIWareHouseDlg::ReceiveResultGoldToWareFail()
{
	m_bSendedItemGold = false;			// ���� ���..

	char szGold[32];
	int iGold, iMyMoney, iWareMoney;			// �κ��丮�� ��..
	std::string str;

	// ���� �����Կ��� ���� ���..
	iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();

	// Gold Offset Backup..
	m_iGoldOffsetBackup = iGold;

	// ���� ���� ���� ���� ��� �´�..
	iMyMoney = CGameBase::s_pPlayer->m_InfoExt.iGold;

	// �������� ���� ���´�..
	CN3UIString* pStr = NULL;
	pStr = (CN3UIString* )GetChildByID("string_wareitem_name");	 
	__ASSERT(pStr, "NULL UI Component!!");
	str = pStr->GetString();
	iWareMoney = atoi(str.c_str());

	// ���� ���� ��Ų��..
	iMyMoney += iGold;
	CGameBase::s_pPlayer->m_InfoExt.iGold = iMyMoney;

	iWareMoney -= iGold;

	// �� ǥ��.. Ware..
	pStr->SetStringAsInt(iWareMoney);
	// �� ǥ��.. �κ��丮..
	sprintf(szGold, "%d", iMyMoney);	pStr = NULL;
	str = szGold;
	pStr = (CN3UIString* )CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold"); __ASSERT(pStr, "NULL UI Component!!");
	if(pStr)	pStr->SetString(str);	pStr = NULL;	
	// �� ǥ��.. Inv..
	pStr = (CN3UIString* )GetChildByID("string_item_name"); __ASSERT(pStr, "NULL UI Component!!");
	if(pStr) pStr->SetStringAsInt(iMyMoney);
}

void CUIWareHouseDlg::ReceiveResultGoldFromWareFail()
{
	m_bSendedItemGold = false;		// ���� ���..

	char szGold[32];
	int iGold, iMyMoney, iWareMoney;			// �κ��丮�� ��..
	std::string str;

	// ���� �����Կ� �����ϴ� ���..
	iGold = CN3UIWndBase::m_pCountableItemEdit->GetQuantity();

	// Gold Offset Backup..
	m_iGoldOffsetBackup = iGold;

	// ���� ���� ���� ���� ��� �´�..
	iMyMoney = CGameBase::s_pPlayer->m_InfoExt.iGold;

	// �������� ���� ���´�..
	CN3UIString* pStr = NULL;
	pStr = (CN3UIString* )GetChildByID("string_wareitem_name");	 
	__ASSERT(pStr, "NULL UI Component!!");
	str = pStr->GetString();
	iWareMoney = atoi(str.c_str());

	// ���� ���� ��Ų��..
	iMyMoney -= iGold;
	CGameBase::s_pPlayer->m_InfoExt.iGold = iMyMoney;

	iWareMoney += iGold;

	// �� ǥ��.. Ware..
	pStr->SetStringAsInt(iWareMoney);
	// �� ǥ��.. �κ��丮..
	sprintf(szGold, "%d", iMyMoney);	pStr = NULL;
	str = szGold;
	pStr = (CN3UIString* )CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold"); __ASSERT(pStr, "NULL UI Component!!");
	if(pStr)	pStr->SetString(str);	pStr = NULL;	
	// �� ǥ��.. Inv..
	pStr = (CN3UIString* )GetChildByID("string_item_name"); __ASSERT(pStr, "NULL UI Component!!");
	if(pStr) pStr->SetStringAsInt(iMyMoney);
}

//this_ui_add_start
bool CUIWareHouseDlg::OnKeyPress(int iKey)
{
	switch(iKey)
	{
	case SDL_SCANCODE_PAGEUP://DIK_PRIOR:
		ReceiveMessage(m_pBtnPageUp, UIMSG_BUTTON_CLICK);
		return true;
	case SDL_SCANCODE_PAGEDOWN://DIK_NEXT:
		ReceiveMessage(m_pBtnPageDown, UIMSG_BUTTON_CLICK);
		return true;
	case SDL_SCANCODE_ESCAPE://DIK_ESCAPE:
		ReceiveMessage(m_pBtnClose, UIMSG_BUTTON_CLICK);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

bool CUIWareHouseDlg::Load(HANDLE hFile)
{
	if(CN3UIBase::Load(hFile)==false) return false;

	m_pBtnGold			= (CN3UIButton*)(this->GetChildByID("btn_gold"));			__ASSERT(m_pBtnGold, "NULL UI Component!!");
	m_pBtnGoldWareHouse = (CN3UIButton*)(this->GetChildByID("btn_gold_warehouse"));	__ASSERT(m_pBtnGoldWareHouse, "NULL UI Component!!");
	m_pBtnClose			= (CN3UIButton*)(this->GetChildByID("btn_close"));			__ASSERT(m_pBtnClose, "NULL UI Component!!");
	m_pBtnPageUp		= (CN3UIButton*)(this->GetChildByID("btn_page_up"));		__ASSERT(m_pBtnPageUp, "NULL UI Component!!");
	m_pBtnPageDown		= (CN3UIButton*)(this->GetChildByID("btn_page_down"));		__ASSERT(m_pBtnPageDown, "NULL UI Component!!");

	return true;
}

void CUIWareHouseDlg::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}

void CUIWareHouseDlg::SetVisibleWithNoSound(bool bVisible, bool bWork, bool bReFocus)
{
	CN3UIBase::SetVisibleWithNoSound(bVisible, bWork, bReFocus);

	if(bWork)
	{
		if(CN3UIWndBase::m_pCountableItemEdit && CN3UIWndBase::m_pCountableItemEdit->IsVisible())
			CN3UIWndBase::m_pCountableItemEdit->SetVisibleWithNoSound(bVisible, bWork, bReFocus);

		if (GetState() == UI_STATE_ICON_MOVING)
			IconRestore();
		SetState(UI_STATE_COMMON_NONE);
		CN3UIWndBase::AllHighLightIconFree();

		// �� �������� inv ������ �������� �� �κ��丮 �������� inv�������� �ű��..	
		ItemMoveFromThisToInv();

		if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
		if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();
	}
}
//this_ui_add_end
