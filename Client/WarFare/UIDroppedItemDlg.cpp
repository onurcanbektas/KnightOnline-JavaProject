// UIDroppedItemDlg.cpp: implementation of the UIDroppedItemDlg class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"

#include "UIDroppedItemDlg.h"

#include "PacketDef.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "GameProcMain.h"
#include "PlayerMyself.h"

#include "N3UIWndBase.h"
#include "UIImageTooltipDlg.h"
#include "UIInventory.h"
#include "UITransactionDlg.h"

#include "SubProcPerTrade.h"
#include "PlayerOtherMgr.h"
#include "PlayerNPC.h"

#include "UIHotKeyDlg.h"
#include "UISkillTreeDlg.h"

//#include "Resource.h"

#include "N3UIArea.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIDroppedItemDlg::CUIDroppedItemDlg()
{
	for( int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )	m_pMyDroppedItem[i] = NULL;
	for( int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )	m_bSendedIconArray[i] = false;

	m_iItemBundleID = 0;
	m_pUITooltipDlg = NULL;
}

CUIDroppedItemDlg::~CUIDroppedItemDlg()
{
	Release();
}

void CUIDroppedItemDlg::Release()
{
	CN3UIBase::Release();

	for( int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )
	{
		if ( m_pMyDroppedItem[i] != NULL )
		{
			delete m_pMyDroppedItem[i];
			m_pMyDroppedItem[i] = NULL;
		}
	}
}
void CUIDroppedItemDlg::Init(CN3UIBase* pParent)
{
	CN3UIBase::Init(pParent);

}

void CUIDroppedItemDlg::Render()
{
	if (!m_bVisible) return;	// ������ ������ �ڽĵ��� render���� �ʴ´�.
	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();
	m_pUITooltipDlg->DisplayTooltipsDisable();

	bool bTooltipRender = false;
	__IconItemSkill* spItem;

	for(UIListReverseItor itor = m_Children.rbegin(); m_Children.rend() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		pChild->Render();
		if ( (pChild->UIType() == UI_TYPE_ICON) && (pChild->GetStyle() & UISTYLE_ICON_HIGHLIGHT) )
		{
			bTooltipRender = true;
			spItem = GetHighlightIconItem( (CN3UIIcon* )pChild );
		}
	}

	if ( bTooltipRender )
		m_pUITooltipDlg->DisplayTooltipsEnable(ptCur.x, ptCur.y, spItem );

	// ���� ǥ�õǾ� �� ������ ���� ǥ��..
	CN3UIString *pStr = NULL;
	for( int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )
	{
		if ( m_pMyDroppedItem[i] && ( (m_pMyDroppedItem[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE) || 
			(m_pMyDroppedItem[i]->pItemBasic->byContable == UIITEM_TYPE_COUNTABLE_SMALL) ) )
		{
			// string ���..
			pStr = GetChildStringByiOrder(i);
			if(pStr) 
			{
				if ( (GetState() == UI_STATE_ICON_MOVING) && (m_pMyDroppedItem[i] == CN3UIWndBase::m_sSelectedIconInfo.pItemSelect) )
				{
					pStr->SetVisible(false);
				}
				else
				{
					if ( m_pMyDroppedItem[i]->pUIIcon->IsVisible() )
					{
						pStr->SetVisible(true);
						pStr->SetStringAsInt(m_pMyDroppedItem[i]->iCount);
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
			pStr = GetChildStringByiOrder(i);
			if(pStr) 
				pStr->SetVisible(false);
		}
	}
}

void CUIDroppedItemDlg::InitIconWnd(e_UIWND eWnd)
{
	__TABLE_UI_RESRC* pTblUI = CGameBase::s_pTbl_UI->Find(CGameBase::s_pPlayer->m_InfoBase.eNation);

	m_pUITooltipDlg = new CUIImageTooltipDlg();
	m_pUITooltipDlg->Init(this);
	m_pUITooltipDlg->LoadFromFile(pTblUI->szItemInfo);
	m_pUITooltipDlg->InitPos();
	m_pUITooltipDlg->SetVisible(FALSE);	

	CN3UIWndBase::InitIconWnd(eWnd);
}

void CUIDroppedItemDlg::InitIconUpdate()
{
	CN3UIArea* pArea;
	float fUVAspect = (float)45.0f/(float)64.0f;
	int i;

	for( i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )
	{
		if ( m_pMyDroppedItem[i] )
		{
			m_pMyDroppedItem[i]->pUIIcon = new CN3UIIcon;
			m_pMyDroppedItem[i]->pUIIcon->Init(this);
			m_pMyDroppedItem[i]->pUIIcon->SetTex(m_pMyDroppedItem[i]->szIconFN);
			m_pMyDroppedItem[i]->pUIIcon->SetUVRect(0,0, fUVAspect, fUVAspect);
			m_pMyDroppedItem[i]->pUIIcon->SetUIType(UI_TYPE_ICON);
			m_pMyDroppedItem[i]->pUIIcon->SetStyle(UISTYLE_ICON_ITEM|UISTYLE_ICON_CERTIFICATION_NEED);
			pArea = CN3UIWndBase::GetChildAreaByiOrder(UI_AREA_TYPE_DROP_ITEM, i);
			if ( pArea )
			{
				m_pMyDroppedItem[i]->pUIIcon->SetRegion(pArea->GetRegion());
				m_pMyDroppedItem[i]->pUIIcon->SetMoveRect(pArea->GetRegion());
			}
		}
	}
}

__IconItemSkill* CUIDroppedItemDlg::GetHighlightIconItem(CN3UIIcon* pUIIcon)
{
	for( int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )
	{
		if ( (m_pMyDroppedItem[i] != NULL) && (m_pMyDroppedItem[i]->pUIIcon == pUIIcon) )
			return m_pMyDroppedItem[i];
	}
	return NULL;
}

void CUIDroppedItemDlg::EnterDroppedState(int xpos, int ypos)
{
	if ( !IsVisible() )
		SetVisible(true);

	SetPos(xpos, ypos-150);

	int i;
	for( i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )
	{
		m_bSendedIconArray[i] = false;

		if ( m_pMyDroppedItem[i] )
		{
			if ( m_pMyDroppedItem[i]->pUIIcon )
			{
				RemoveChild(m_pMyDroppedItem[i]->pUIIcon);
				m_pMyDroppedItem[i]->pUIIcon->Release();
				delete m_pMyDroppedItem[i]->pUIIcon;
				m_pMyDroppedItem[i]->pUIIcon = NULL;
			}
			delete m_pMyDroppedItem[i];	
			m_pMyDroppedItem[i] = NULL;
		}
	}

	CN3UIWndBase::AllHighLightIconFree();
}


void CUIDroppedItemDlg::LeaveDroppedState()
{
	if ( IsVisible() )
		SetVisible(false);

	for( int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )	m_bSendedIconArray[i] = false;
}

DWORD CUIDroppedItemDlg::MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;
	if ( !IsVisible() ) { dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);  return dwRet; }

	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

void CUIDroppedItemDlg::AddToItemTable(int iItemID, int iItemCount, int iOrder)
{
	__IconItemSkill*	spItem;
	__TABLE_ITEM_BASIC*	pItem = NULL;								// ������ ���̺� ����ü ������..
	__TABLE_ITEM_EXT*	pItemExt = NULL;							// ������ ���̺� ����ü ������..
	std::string szIconFN;

	pItem = CGameBase::s_pTbl_Items_Basic->Find(iItemID/1000*1000);	// �� ������ ���..
	if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
		pItemExt = CGameBase::s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItemID%1000);
	if(NULL == pItem || NULL == pItemExt)
	{
		__ASSERT(0, "������ ������ ���̺� ����!!");
		CLogWriter::Write("CUIDroppedItemDlg::AddToItemTable - Invalidate ItemID : %d", iItemID);
		return;
	}

	//TRACE("Dropped item from server to ItemDlg %d \n", iItemID);
	e_PartPosition ePart;
	e_PlugPosition ePlug;
	e_ItemType eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart, ePlug); // �����ۿ� ���� ���� �̸��� ����
	if ( ITEM_TYPE_UNKNOWN == eType ) return;
	
	spItem = new __IconItemSkill;
	spItem->pItemBasic = pItem;
	spItem->pItemExt = pItemExt;
	spItem->szIconFN = szIconFN; // ������ ���� �̸� ����..
	spItem->iCount	= iItemCount;
	spItem->iDurability = pItem->siMaxDurability+pItemExt->siMaxDurability;

	m_pMyDroppedItem[iOrder] = spItem;
}

void CUIDroppedItemDlg::AddToItemTableToInventory(int iItemID, int iItemCount, int iOrder)
{
	CN3UIArea* pArea;
	__IconItemSkill*	spItem;
	__TABLE_ITEM_BASIC*	pItem = NULL;								// ������ ���̺� ����ü ������..
	__TABLE_ITEM_EXT*	pItemExt = NULL;							// ������ ���̺� ����ü ������..
	std::string szIconFN;
	float fUVAspect = (float)45.0f/(float)64.0f;

	pItem = CGameBase::s_pTbl_Items_Basic->Find(iItemID/1000*1000);	// �� ������ ���..
	if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
		pItemExt = CGameBase::s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItemID%1000);
	if(NULL == pItem || NULL == pItemExt)
	{
		__ASSERT(0, "������ ������ ���̺� ����!!");
		CLogWriter::Write("CUIDroppedItemDlg::AddToItemTableToInventory - Invalidate ItemID : %d", iItemID);
		return;
	}

	//TRACE("Dropped item from server to ItemDlg %d \n", iItemID);
	e_PartPosition ePart;
	e_PlugPosition ePlug;
	e_ItemType eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart, ePlug); // �����ۿ� ���� ���� �̸��� ����
	if ( ITEM_TYPE_UNKNOWN == eType ) return;
	
	spItem = new __IconItemSkill;
	spItem->pItemBasic = pItem;
	spItem->pItemExt = pItemExt;
	spItem->szIconFN = szIconFN; // ������ ���� �̸� ����..
	spItem->iCount	= iItemCount;
	spItem->iDurability = pItem->siMaxDurability+pItemExt->siMaxDurability;

	spItem->pUIIcon = new CN3UIIcon;
	spItem->pUIIcon->Init(CGameProcedure::s_pProcMain->m_pUIInventory);
	spItem->pUIIcon->SetTex(spItem->szIconFN);
	spItem->pUIIcon->SetUVRect(0,0, fUVAspect, fUVAspect);
	spItem->pUIIcon->SetUIType(UI_TYPE_ICON);
	spItem->pUIIcon->SetStyle(UISTYLE_ICON_ITEM|UISTYLE_ICON_CERTIFICATION_NEED);
	spItem->pUIIcon->SetVisible(true);
	pArea = NULL;
	pArea = CGameProcedure::s_pProcMain->m_pUIInventory->GetChildAreaByiOrder(UI_AREA_TYPE_INV, iOrder);
	if ( pArea )
	{
		spItem->pUIIcon->SetRegion(pArea->GetRegion());
		spItem->pUIIcon->SetMoveRect(pArea->GetRegion());
	}

	CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[iOrder] = spItem;
	PlayItemSound(pItem);
}

bool CUIDroppedItemDlg::ReceiveIconDrop(__IconItemSkill* spItem, POINT ptCur)
{
	CN3UIArea* pArea = NULL;

	if (!m_bVisible) return false;

	// �˻��ؼ� ���õ� �������� ���� �����쿡�� ����� �˷���� �Ѵ�..
	switch ( CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd )
	{
		// �κ��丮 ������κ��� �� ���̶��..
		case UIWND_INVENTORY:
			CGameProcedure::s_pProcMain->m_pUIInventory->CancelIconDrop(spItem);
			break;

		// ��ŷ� ������κ��� �� ���̶��...
		case UIWND_TRANSACTION:
			CGameProcedure::s_pProcMain->m_pUITransactionDlg->CancelIconDrop(spItem);
			break;
	}

	return false;
}

int CUIDroppedItemDlg::GetInventoryEmptyInviOrder(__IconItemSkill* spItem)
{
	int i;
	if (spItem == NULL)
	{
		for ( i = 0; i < MAX_ITEM_INVENTORY; i++ )
		{
			if ( !CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[i] )
				return i;
		}
	}
	else
	{
		for ( i = 0; i < MAX_ITEM_INVENTORY; i++ )
		{
			if ( CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[i] && 
				(CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[i]->pItemBasic->dwID == spItem->pItemBasic->dwID) )
				return i;
		}

		for ( i = 0; i < MAX_ITEM_INVENTORY; i++ )
		{
			if ( !CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[i] )
				return i;
		}
	}

	return -1;
}

int	CUIDroppedItemDlg::GetItemiOrder(__IconItemSkill* spItem)
{
	int i;

	for( i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )
	{
		if ( (m_pMyDroppedItem[i] != NULL) && (m_pMyDroppedItem[i] == spItem) )
			return i;
	}

	return -1;
}

bool CUIDroppedItemDlg::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
// Temp Define
#define FAIL_CODE {		\
				CN3UIWndBase::m_sSelectedIconInfo.eIconSelectState = UIICON_SELECT_NONE;	\
				return false;	\
			}

// Code Begin
	if(NULL == pSender) return false;

	int iOrder, iOrderInv = -1;

	DWORD dwBitMask = 0x000f0000;

	__TABLE_ITEM_BASIC*	pItem;
	__IconItemSkill*	spItem;
	std::string szIconFN;
	e_PartPosition ePart;
	e_PlugPosition ePlug;
	e_ItemType eType;

	// ������ ������ ���� �������̴ϱ�.. ������ ������..
	BYTE byBuff[16];
	int iOffset = 0;

	switch (dwMsg & dwBitMask)
	{
		case UIMSG_ICON_DOWN_FIRST:
			spItem = NULL;
			spItem = GetHighlightIconItem((CN3UIIcon* )pSender);
			if ( !spItem )	break;
			m_iBackupiOrder = GetItemiOrder( spItem );
			break;

		case UIMSG_ICON_UP:
			SetState(UI_STATE_COMMON_NONE);

			// �������� ������ �˻�..
			pItem = NULL;								// ������ ���̺� ����ü ������..
			spItem = NULL;
			spItem = GetHighlightIconItem((CN3UIIcon* )pSender);
			if ( !spItem )	break;
			pItem = CGameBase::s_pTbl_Items_Basic->Find(spItem->pItemBasic->dwID);	// �� ������ ���..
			if ( pItem == NULL )
			{
				__ASSERT(0, "NULL Item!!!");
				CLogWriter::Write("CUIDroppedItemDlg::ReceiveMessage - UIMSG_ICON_UP - NULL Icon : %d", spItem->pItemBasic->dwID);
				break;
			}

			eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart, ePlug); // �����ۿ� ���� ���� �̸��� ����

			// ���� ������ �迭�̶� ��.. 
			iOrder = GetItemiOrder( spItem );
			if ( m_iBackupiOrder != iOrder )	break;

			if ( m_bSendedIconArray[iOrder] == true )	// �ѹ� ���´� ��Ŷ�̸� break.. 
				break;
			else
				m_bSendedIconArray[iOrder] = true;

			CAPISocket::MP_AddByte(byBuff, iOffset, N3_ITEM_DROPPED_GET);
			CAPISocket::MP_AddDword(byBuff, iOffset, m_iItemBundleID);

			// ���� �ƴϸ� �κ��丮 ����Ʈ�� �߰�.... 
			if ( ITEM_TYPE_GOLD != eType ) 
			{
				CAPISocket::MP_AddDword(byBuff, iOffset, spItem->pItemBasic->dwID+spItem->pItemExt->dwID);
//				CAPISocket::MP_AddDword(byBuff, iOffset, (BYTE)iOrderInv);
			}
			else
			{
				CAPISocket::MP_AddDword(byBuff, iOffset, spItem->pItemBasic->dwID);
//				CAPISocket::MP_AddDword(byBuff, iOffset, 0xff);
			}
			CGameProcedure::s_pSocket->Send(byBuff, iOffset);

			// ���� ������ ���� ����..	
			CN3UIWndBase::m_sRecoveryJobInfo.pItemSource = spItem;
			CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.UIWnd = UIWND_DROPITEM;
			CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.UIWndDistrict = UIWND_DISTRICT_DROPITEM;
			CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder = iOrder;
			CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.UIWnd = UIWND_INVENTORY;
			CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.UIWndDistrict = UIWND_DISTRICT_INVENTORY_INV;
			CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceEnd.iOrder = iOrderInv;
			break;

		case UIMSG_ICON_DOWN:
			break;

		case UIMSG_ICON_DBLCLK:
			SetState(UI_STATE_COMMON_NONE);
			break;
	}

	return true;
}

void CUIDroppedItemDlg::GetItemByIDToInventory(BYTE bResult, int iItemID, int iGold, int iPos, int iItemCount, int iStrLen, 
											   std::string strString)
{
	// ������ ����Ʈ���� �������� ã��..
	bool				bFound = false;
	__TABLE_ITEM_BASIC*	pItem = NULL;									// ������ ���̺� ����ü ������..
	__TABLE_ITEM_EXT*	pItemExt = NULL;
	__IconItemSkill*	spItem = NULL;
	int i;
	char szMsg[32];
	CN3UIString* pStatic = NULL;
	__InfoPlayerMySelf*	pInfoExt = NULL;
	std::string stdMsg;

	if (bResult == 0x00)			// ����..
	{
		int iOrderInv = GetInventoryEmptyInviOrder();
		if ( iOrderInv == -1 )
		{
			// �κ��丮�� �� ��������.. break.. ^^
			char szBuff[128] = "";
			std::string stdMsg;
			stdMsg = "IDS_INV_ITEM_FULL";//::_LoadStringFromResource(IDS_INV_ITEM_FULL, stdMsg);
			CGameProcedure::s_pProcMain->MsgOutput(stdMsg, 0xff9b9bff);
		}

		if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
		if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();

		return;
	}
	
	if (bResult == 0x02)		// ��Ƽ ���¿��� ��Ƹ� ��´�..
	{
		// �� ����..
		pStatic = NULL;
		pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

		// �� ������Ʈ..
		stdMsg = "Picked up %d Coins.";//::_LoadStringFromResource(IDS_DROPPED_NOAH_GET, stdMsg);
		sprintf(szMsg, stdMsg.c_str(), iGold - pInfoExt->iGold);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xff9b9bff);

		pInfoExt->iGold = iGold;
		//TRACE("�� ������Ʈ %d \n", iGold);
		pStatic = (CN3UIString* )CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold"); __ASSERT(pStatic, "NULL UI Component!!");
		if(pStatic) pStatic->SetStringAsInt(iGold);

		if (!IsVisible()) 
			return;

		// �� �������� ������ ���ش�..
		bFound = false;
		for( i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )
		{
			if( m_pMyDroppedItem[i] && m_pMyDroppedItem[i]->pItemBasic->dwID == dwGold )
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)	return;

		spItem = m_pMyDroppedItem[i];
		if (!spItem)	return;
		// �Ŵ������� ����..
		RemoveChild(spItem->pUIIcon);

		// ���ҽ� ����..
		spItem->pUIIcon->Release();
		delete spItem->pUIIcon;
		spItem->pUIIcon = NULL;
		delete spItem;
		spItem = NULL;
		m_pMyDroppedItem[i] = NULL;

		PlayGoldSound();

		if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
		if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();
	}

	if (bResult == 0x03)		// ��Ƽ���¿��� ���� �������� ����..
	{
		pItem = NULL;
		pItemExt = NULL;
		pItem = CGameBase::s_pTbl_Items_Basic->Find(iItemID/1000*1000);	// �� ������ ���..
		if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
			pItemExt = CGameBase::s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItemID%1000);
		if(NULL == pItem || NULL == pItemExt)
		{
			__ASSERT(0, "������ ������ ���̺� ����!!");
			CLogWriter::Write("CUIDroppedItemDlg::GetItemByIDToInventory - NULL Icon : %d", iItemID);
			return;
		}

		char szBuff[128] = "";
		stdMsg = "IDS_PARTY_ITEM_GET";//::_LoadStringFromResource(IDS_PARTY_ITEM_GET, stdMsg);
		sprintf(szBuff, stdMsg.c_str(), strString.c_str(), pItem->szName.c_str());
		CGameProcedure::s_pProcMain->MsgOutput(szBuff, 0xff9b9bff);

		if (!IsVisible()) 
			return;

		// ������ �������� ������ ���ش�..
		bFound = false;
		for( i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )
		{
			if( (m_pMyDroppedItem[i]) && (m_pMyDroppedItem[i]->pItemBasic->dwID+m_pMyDroppedItem[i]->pItemExt->dwID == iItemID) )
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)	return;

		if (m_pMyDroppedItem[i])
			PlayItemSound(m_pMyDroppedItem[i]->pItemBasic);

		spItem = m_pMyDroppedItem[i];
		if (!spItem)	return;
		// �Ŵ������� ����..
		RemoveChild(spItem->pUIIcon);

		// ���ҽ� ����..
		spItem->pUIIcon->Release();
		delete spItem->pUIIcon;
		spItem->pUIIcon = NULL;
		delete spItem;
		spItem = NULL;
		m_pMyDroppedItem[i] = NULL;

		if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
		if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();

		return;
	}

	if (bResult == 0x04)		// ��Ƽ ���¿��� �ٸ� ����� �������� ����..
	{
		spItem = m_pMyDroppedItem[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
		if (spItem)
		{
			// �Ŵ������� ����..
			RemoveChild(spItem->pUIIcon);

			// ���ҽ� ����..
			spItem->pUIIcon->Release();
			delete spItem->pUIIcon;
			spItem->pUIIcon = NULL;
			delete spItem;
			spItem = NULL;
		}

		m_pMyDroppedItem[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = NULL;

		if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
		if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();

		return;	
	}

	if (bResult == 0x05)		// ��Ƽ ���¿��� �Ϲ����� ������ ����..
	{
		if (iItemID == dwGold)
		{
			__ASSERT(0, "Invalidate Item ID From Server.. ");
			CLogWriter::Write("CUIDroppedItemDlg::GetItemByIDToInventory - ID Pos : %d", iPos);
			return;
		}

		if ( (iPos < 0) || (iPos > (MAX_ITEM_INVENTORY-1)) )
		{
			__ASSERT(0, "Invalidate Item Pos From Server.. ");
			CLogWriter::Write("CUIDroppedItemDlg::GetItemByIDToInventory - Invalidate Pos : %d", iPos);
			return;
		}

		__IconItemSkill	*spItemDest = CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[iPos];
		if (spItemDest)		// �������� �ִ�..
		{
			if ( iItemID != (spItemDest->pItemBasic->dwID+spItemDest->pItemExt->dwID) )
			{
				// ���� �������� Ŭ����..
				if (!spItemDest) return;
				RemoveChild(spItemDest->pUIIcon);

				// ������ ���ҽ� ����...
				spItemDest->pUIIcon->Release();
				delete spItemDest->pUIIcon;
				spItemDest->pUIIcon = NULL;
				delete spItemDest;
				spItemDest = NULL;

				CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[iPos] = NULL;

				//  �������� ���� ����.. ���� ����..
				AddToItemTableToInventory(iItemID, iItemCount, iPos);
			}
			else
			{
				// ���� ����..
				CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[iPos]->iCount = iItemCount;
				PlayItemSound(CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[iPos]->pItemBasic);
			}
		}
		else
		{
			// �������� ���� ��� .. ���� �����.. ���� ����..
			AddToItemTableToInventory(iItemID, iItemCount, iPos);
		}

		pItem = NULL;
		pItem = CGameBase::s_pTbl_Items_Basic->Find(iItemID/1000*1000);	// �� ������ ���..
		if(NULL == pItem)
		{
			__ASSERT(0, "������ ������ ���̺� ����!!");
			CLogWriter::Write("CUIDroppedItemDlg::GetItemByIDToInventory - NULL Icon : %d", iItemID);
			return;
		}

		stdMsg = "IDS_ITEM_GET_BY_RULE";//::_LoadStringFromResource(IDS_ITEM_GET_BY_RULE, stdMsg);
		sprintf(szMsg, stdMsg.c_str(), pItem->szName.c_str());
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xff9b9bff);

		if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
		if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();
	}

	if (bResult == 0x06)
	{
		// �޽��� �ڽ� �ؽ�Ʈ ǥ��..
		std::string szMsg = "You cannot trade or pick up items because you have either exceeded the possible quantity or the weight."; //::_LoadStringFromResource(IDS_ITEM_TOOMANY_OR_HEAVY, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
	}

	if (bResult == 0x07)
	{
		// �޽��� �ڽ� �ؽ�Ʈ ǥ��..
		std::string szMsg = "IDS_INV_ITEM_FULL"; //::_LoadStringFromResource(IDS_INV_ITEM_FULL, szMsg);
		CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff3b3b);
	}

	if (bResult == 0x01)
	{
		if (iItemID != dwGold)
		{
			if ( (iPos < 0) || (iPos > (MAX_ITEM_INVENTORY-1)) )
			{
				__ASSERT(0, "Invalidate Item Pos From Server.. ");
				CLogWriter::Write("CUIDroppedItemDlg::GetItemByIDToInventory - Invalidate Pos : %d", iPos);
				return;
			}

			spItem = NULL;
			__IconItemSkill	*spItemDest = CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[iPos];
			if (spItemDest)		// �������� �ִ�..
			{
				if ( iItemID != (spItemDest->pItemBasic->dwID+spItemDest->pItemExt->dwID) )
				{
					// ���� �������� Ŭ����..
					if (!spItemDest) return;
					RemoveChild(spItemDest->pUIIcon);

					// ������ ���ҽ� ����...
					spItemDest->pUIIcon->Release();
					delete spItemDest->pUIIcon;
					spItemDest->pUIIcon = NULL;
					delete spItemDest;
					spItemDest = NULL;

					CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[iPos] = NULL;

					//  �������� ���� ����.. ���� ����..
					AddToItemTableToInventory(iItemID, iItemCount, iPos);
				}
				else
				{
					// ���� ����..
					CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[iPos]->iCount = iItemCount;
					PlayItemSound(CGameProcedure::s_pProcMain->m_pUIInventory->m_pMyInvWnd[iPos]->pItemBasic);
				}
			}
			else
			{
				// �������� ���� ��� .. ���� �����.. ���� ����..
				AddToItemTableToInventory(iItemID, iItemCount, iPos);
			}

			spItem = m_pMyDroppedItem[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder];
			if (spItem)
			{
				// �Ŵ������� ����..
				RemoveChild(spItem->pUIIcon);

				// ���ҽ� ����..
				spItem->pUIIcon->Release();
				delete spItem->pUIIcon;
				spItem->pUIIcon = NULL;
				delete spItem;
				spItem = NULL;
			}
			m_pMyDroppedItem[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = NULL;
		}
		else
		{
			pStatic = NULL;
			pInfoExt = &(CGameBase::s_pPlayer->m_InfoExt);

			// �� ������Ʈ..
			stdMsg = "Picked up %d Coins.";//::_LoadStringFromResource(IDS_DROPPED_NOAH_GET, stdMsg);
			sprintf(szMsg, stdMsg.c_str(), iGold - pInfoExt->iGold);
			CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xff9b9bff);

			pInfoExt->iGold = iGold;
			//TRACE("�� ������Ʈ %d \n", iGold);
			pStatic = (CN3UIString* )CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold"); __ASSERT(pStatic, "NULL UI Component!!");
			if(pStatic) pStatic->SetStringAsInt(iGold);

			spItem = CN3UIWndBase::m_sRecoveryJobInfo.pItemSource;
			if ( NULL == spItem ) return;

			// �Ŵ������� ����..
			RemoveChild(spItem->pUIIcon);

			// ���ҽ� ����..
			spItem->pUIIcon->Release();
			delete spItem->pUIIcon;
			spItem->pUIIcon = NULL;
			delete spItem;
			spItem = NULL;
			m_pMyDroppedItem[CN3UIWndBase::m_sRecoveryJobInfo.UIWndSourceStart.iOrder] = NULL;

			PlayGoldSound();
		}

		if (CGameProcedure::s_pProcMain->m_pUISkillTreeDlg) CGameProcedure::s_pProcMain->m_pUISkillTreeDlg->UpdateDisableCheck();
		if (CGameProcedure::s_pProcMain->m_pUIHotKeyDlg) CGameProcedure::s_pProcMain->m_pUIHotKeyDlg->UpdateDisableCheck();
	}

	bFound = false;
	for( i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )
	{
		if( m_pMyDroppedItem[i] != NULL )
			bFound = true;
	}

	if ( !bFound )
		LeaveDroppedState();
}

//this_ui_add_start
void CUIDroppedItemDlg::SetVisibleWithNoSound(bool bVisible, bool bWork, bool bReFocus)
{
	CN3UIBase::SetVisibleWithNoSound(bVisible, bWork, bReFocus);

	for( int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++ )	m_bSendedIconArray[i] = false;
}
//this_ui_add_end