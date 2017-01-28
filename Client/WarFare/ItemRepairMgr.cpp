// ItemRepairMgr.cpp: implementation of the CItemRepairMgr class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "resource.h"
#include "ItemRepairMgr.h"

#include "PacketDef.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "UIInventory.h"
#include "GameProcMain.h"
#include "PlayerMySelf.h"

#include "UIRepairTooltipDlg.h"

#include "N3UIString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemRepairMgr::CItemRepairMgr()
{
	m_pspItemBack = NULL;	
}

CItemRepairMgr::~CItemRepairMgr()
{

}

void CItemRepairMgr::Tick()
{
	CUIInventory *pInv	= CGameProcedure::s_pProcMain->m_pUIInventory;
	if (!pInv) return;
	if (!pInv->IsVisible())	return;
	if (pInv->GetInvState() != INV_STATE_REPAIR) return;
	if (CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer) return;

	CUIRepairTooltipDlg *pDlg = CGameProcedure::s_pProcMain->m_pUIRepairTooltip;
	if (pDlg)	pDlg->m_bBRender = false;

	POINT ptCur			= CGameProcedure::s_pLocalInput->MouseGetPos();

	// ��ġ�� ���ؼ� 
	int i;	int iArm = 0x00; int iOrder = -1; __IconItemSkill* spItem = NULL;
	for (i = 0; i < ITEM_SLOT_COUNT; i++)
	{
		if (spItem) break;
		if (pInv->m_pMySlot[i])
		{
			if (pInv->m_pMySlot[i]->pUIIcon->IsIn(ptCur.x, ptCur.y))
			{
				iArm = 0x01;
				spItem = pInv->m_pMySlot[i];
				iOrder = i;
			}
		}
	}

	if (!spItem)
	{
		for (i = 0; i < MAX_ITEM_INVENTORY; i++)
		{
			if (spItem) break;
			if (pInv->m_pMyInvWnd[i])
			{
				if (pInv->m_pMyInvWnd[i]->pUIIcon->IsIn(ptCur.x, ptCur.y))
				{
					iArm = 0x02;
					spItem = pInv->m_pMyInvWnd[i];
					iOrder = i;
				}
			}
		}
	}

	// ������ ���� ������..  
	int iRepairGold = 0;
	if (spItem)
	{
		iRepairGold = CalcRepairGold(spItem);

		// ���� ���� ���� ǥ��..
		if (pDlg)
		{
			pDlg->m_bBRender		= true;
			pDlg->m_iBxpos			= ptCur.x;
			pDlg->m_iBypos			= ptCur.y;
			pDlg->m_pBspItem		= spItem;
			pDlg->m_iBRequiredGold	= iRepairGold;
		}

		// ���� ���� �� ���� ���� ����� ��θ�.. 
		if (iRepairGold > s_pPlayer->m_InfoExt.iGold)
		{
			// ������ ǥ��.. 
			if (pDlg)
				pDlg->m_bBHaveEnough = false;
		}
		else
		{
			//���̸� ���� ����..
			if (pDlg)
				pDlg->m_bBHaveEnough = true;
		}
	}

	DWORD dwMouseFlags	= CGameProcedure::s_pLocalInput->MouseGetFlag();	// ���콺 ��ư �÷��� - LocalInput.h ����
	if (dwMouseFlags & MOUSE_LBCLICK)		// ���� ��ư�� ������..
	{
		m_pspItemBack	= spItem;
		m_iArm			= iArm;
		m_iiOrder		= iOrder;
	}
	else if(dwMouseFlags & MOUSE_LBCLICKED)
	{
		if (m_pspItemBack && spItem && (m_pspItemBack == spItem) )
		{
			// Send To Server..
			if (iRepairGold > 0)										// ���� ������ ������..
			{
				// ���� ���� �� ���� ���� ����� ��θ�.. 
				if (iRepairGold > s_pPlayer->m_InfoExt.iGold)
				{
					// �������� ������ �ʰ� �޽��� ǥ��.. 
					std::string szMsg = "IDS_REPAIR_LACK_GOLD"; //::_LoadStringFromResource(IDS_REPAIR_LACK_GOLD, szMsg);
					CGameProcedure::s_pProcMain->MsgOutput(szMsg, 0xffff00ff);
				}
				else
				{
					BYTE byBuff[8];															// ��Ŷ ����..
					int iOffset=0;															// ��Ŷ ������..

					CAPISocket::MP_AddByte(byBuff, iOffset,  N3_ITEM_REPAIR_REQUEST);			// ���� ��ŸƮ ��Ŷ Ŀ���..
					CAPISocket::MP_AddByte(byBuff, iOffset,  iArm);							// ���̵� ���� ��Ŷ�� �ֱ�..
					CAPISocket::MP_AddByte(byBuff, iOffset,  iOrder);							// ���̵� ���� ��Ŷ�� �ֱ�..
					CAPISocket::MP_AddDword(byBuff, iOffset, spItem->pItemBasic->dwID+spItem->pItemExt->dwID);	// ���̵� ���ڿ� ��Ŷ�� �ֱ�..

					CGameProcedure::s_pSocket->Send(byBuff, iOffset);	

					// ������ ��ٸ�..
					CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = true;

					// Change To Cursor..
					CGameProcedure::SetGameCursor(CGameProcedure::s_hCursorNowRepair, true);
				}
			}
		}
	}
}

void CItemRepairMgr::ReceiveResultFromServer(int iResult, int iUserGold)
{
	CUIRepairTooltipDlg *pDlg = CGameProcedure::s_pProcMain->m_pUIRepairTooltip;
	CUIInventory *pInv	= CGameProcedure::s_pProcMain->m_pUIInventory;
	if (!pInv) return;
	if (!m_pspItemBack) return;

	// �����̸� npc������ Durability�� �ִ밪����..
	if(iResult == 0x01)
	{
		m_pspItemBack->iDurability = m_pspItemBack->pItemBasic->siMaxDurability+m_pspItemBack->pItemExt->siMaxDurability;

		switch (m_iArm)
		{
			case 0x01: // �����ϰ� �ִ� ������
				pInv->m_pMySlot[m_iiOrder] = m_pspItemBack;
				s_pPlayer->DurabilitySet((e_ItemSlot)m_iiOrder, m_pspItemBack->iDurability); // �������� ���� ���ش�..
				break;

			case 0x02: // �κ��丮�� �ִ� ������..
				pInv->m_pMyInvWnd[m_iiOrder] = m_pspItemBack;
				break;
		}

		// ������ ���°� UISTYLE_DURABILITY_EXHAUST �̸�..
		m_pspItemBack->pUIIcon->SetStyle(m_pspItemBack->pUIIcon->GetStyle() & (~UISTYLE_DURABILITY_EXHAUST));
		
		if (pDlg)	pDlg->m_iBRequiredGold	= 0;
		pInv->PlayRepairSound();
	}

	// �� ������Ʈ..
	UpdateUserTotalGold(iUserGold);

	// ���� ��ٸ� ����..
	CN3UIWndBase::m_sRecoveryJobInfo.m_bWaitFromServer = false;

	// Change To Cursor..
	CGameProcedure::SetGameCursor(CGameProcedure::s_hCursorPreRepair, true);
}

void CItemRepairMgr::UpdateUserTotalGold(int iGold)
{
	char szGold[32];
	CN3UIString* pStatic = NULL;

	// �� ������Ʈ..
	s_pPlayer->m_InfoExt.iGold = iGold;
	sprintf(szGold, "%d", iGold);
	pStatic = (CN3UIString* )CGameProcedure::s_pProcMain->m_pUIInventory->GetChildByID("text_gold"); __ASSERT(pStatic, "NULL UI Component!!");
	if(pStatic)
	{
		pStatic->SetString(szGold);
	}
}

int CItemRepairMgr::CalcRepairGold(__IconItemSkill* spItem)
{
	int iMaxDurability = spItem->pItemBasic->siMaxDurability + spItem->pItemExt->siMaxDurability;
	if (iMaxDurability <= 0) return 0;
	float fAllPrice = (float)spItem->pItemBasic->iPrice*(float)spItem->pItemExt->siPriceMultiply;
	float fTemp = ((fAllPrice-10.0f)/(10000.0f))+pow(fAllPrice, 0.75f);
	float fValue = fTemp*( (float)(iMaxDurability - spItem->iDurability) / (float)(iMaxDurability) );

	return (int)fValue;
}





