// UIDead.cpp: implementation of the CUIDead class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "N3UITooltip.h"
#include "N3UIString.h"
//#include "resource.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "GameProcedure.h"
#include "GameProcMain.h"
#include "UIManager.h"
#include "PlayerMySelf.h"
#include "UIDead.h"
#include "UIInventory.h"
#include "UIStateBar.h"
#include "MagicSkillMng.h"
#include "N3FXMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

#define CHILD_UI_REVIVE_MSG				1
#define CHILD_UI_LACK_LIVE_STONE_MSG	2
#define CHILD_UI_LOW_LEVEL				3

#define TIMES_LIFE_STONE				3
#define LIFE_STONE_INDEX				379006000

#define REVIVAL_TYPE_RETURN_TOWN		1
#define REVIVAL_TYPE_LIFE_STONE			2
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIDead::CUIDead()
{
	m_pTextAlive	= NULL;
	m_pTextTown		= NULL;
	m_bProcessing	= false;
}

CUIDead::~CUIDead()
{

}

bool CUIDead::Load(HANDLE hFile)
{
	if(false == CN3UIBase::Load(hFile)) return false;

	m_pTextAlive	= (CN3UIString*)(this->GetChildByID("Text_Alive"));	__ASSERT(m_pTextAlive, "NULL UI Component!!!");
	m_pTextTown		= (CN3UIString*)(this->GetChildByID("Text_Town"));	__ASSERT(m_pTextTown, "NULL UI Component!!!");


	std::string szMsg;
	szMsg = "IDS_DEAD_REVIVAL";//::_LoadStringFromResource(IDS_DEAD_REVIVAL, szMsg);
	if(m_pTextAlive) m_pTextAlive->SetString(szMsg);

	szMsg = "IDS_DEAD_RETURN_TOWN";//::_LoadStringFromResource(IDS_DEAD_RETURN_TOWN, szMsg);
	if(m_pTextTown) m_pTextTown->SetString(szMsg);

	__TABLE_UI_RESRC*	pTblUI	= NULL;
	pTblUI = CGameBase::s_pTbl_UI->Find(NationEnum::ELMORAD.getValue());

	m_MsgBox.LoadFromFile(pTblUI->szMessageBox);

	RECT rt = m_MsgBox.GetRegion();
	POINT pt;
	pt.x = (CN3Base::s_CameraData.vp.Width - (rt.right - rt.left)) / 2;
	pt.y = (CN3Base::s_CameraData.vp.Height - (rt.bottom - rt.top)) / 2;
	m_MsgBox.SetPos(pt.x, pt.y);

	return true;
}

bool CUIDead::ReceiveMessage(CN3UIBase *pSender, DWORD dwMsg)
{
	if( dwMsg == UIMSG_STRING_LCLICK )
	{
		if(pSender == m_pTextAlive)
		{
			int iItemCnt = 0, iLevel = 0, iNeedItemCnt = 0;
			if(CGameProcedure::s_pProcMain->m_pUIInventory)
				iItemCnt = CGameProcedure::s_pProcMain->m_pUIInventory->GetIndexItemCount(LIFE_STONE_INDEX);

			iLevel = CGameProcedure::s_pPlayer->m_InfoBase.iLevel;
			iNeedItemCnt = iLevel * TIMES_LIFE_STONE;
			char szBuf[256] = "";
			std::string szMsg;

			if(iLevel < 6)
			{
				szMsg = "IDS_DEAD_LOW_LEVEL";//::_LoadStringFromResource(IDS_DEAD_LOW_LEVEL, szMsg);
				sprintf(szBuf, szMsg.c_str(), iNeedItemCnt);

				m_MsgBox.SetBoxStyle(MB_OK);
				m_MsgBox.m_eBehavior = BEHAVIOR_NOTHING;
				m_MsgBox.SetTitle("");
				m_MsgBox.SetText(szBuf);
				m_MsgBox.ShowWindow(CHILD_UI_LOW_LEVEL,this);
			}
			else if(iItemCnt >= iNeedItemCnt)
			{
				szMsg = "IDS_DEAD_REVIVAL_MESSAGE";//::_LoadStringFromResource(IDS_DEAD_REVIVAL_MESSAGE, szMsg);
				sprintf(szBuf, szMsg.c_str(), iNeedItemCnt);

				m_MsgBox.SetBoxStyle(MB_YESNO);
				m_MsgBox.m_eBehavior = BEHAVIOR_NOTHING;
				m_MsgBox.SetTitle("");
				m_MsgBox.SetText(szBuf);
				m_MsgBox.ShowWindow(CHILD_UI_REVIVE_MSG,this);
			}
			else
			{
				szMsg = "IDS_DEAD_LACK_LIFE_STONE";//::_LoadStringFromResource(IDS_DEAD_LACK_LIFE_STONE, szMsg);

				m_MsgBox.SetBoxStyle(MB_OK);
				m_MsgBox.m_eBehavior = BEHAVIOR_NOTHING;
				m_MsgBox.SetTitle("");
				m_MsgBox.SetText(szMsg);
				m_MsgBox.ShowWindow(CHILD_UI_LACK_LIVE_STONE_MSG,this);
			}
		}
		else if(pSender == m_pTextTown)
		{
			MsgSend_Revival(REVIVAL_TYPE_RETURN_TOWN);
		}
	}

	return true;
}

DWORD CUIDead::MouseProc(DWORD dwFlags, const POINT &ptCur, const POINT &ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;
	if (!m_bVisible) return dwRet;

	// UI �����̴� �ڵ�
	if (UI_STATE_COMMON_MOVE == m_eState)
	{
		if (dwFlags&UI_MOUSE_LBCLICKED)
		{
			SetState(UI_STATE_COMMON_NONE);
		}
		else
		{
			MoveOffset(ptCur.x - ptOld.x, ptCur.y - ptOld.y);
		}
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	if(false == IsIn(ptCur.x, ptCur.y))	// ���� ���̸�
	{
		if(false == IsIn(ptOld.x, ptOld.y))
		{
			return dwRet;// ���� ��ǥ�� ���� ���̸� 
		}
		dwRet |= UI_MOUSEPROC_PREVINREGION;	// ���� ��ǥ�� ���� ���̾���.
	}
	else
	{
		// tool tip ����
		if (s_pTooltipCtrl) s_pTooltipCtrl->SetText(m_szToolTip);
	}
	dwRet |= UI_MOUSEPROC_INREGION;	// �̹� ��ǥ�� ���� ���̴�.

	if(m_pChildUI && m_pChildUI->IsVisible())
		return dwRet;

	// child���� �޼��� ����
	for(UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		DWORD dwChildRet = 0;

		dwChildRet = pChild->MouseProc(dwFlags, ptCur, ptOld);
		if( pChild->IsVisible() && UI_TYPE_STRING == pChild->UIType() )
		{
			if(pChild->IsIn(ptCur.x, ptCur.y) && (dwFlags & UI_MOUSE_LBCLICKED) )	
			{
				dwChildRet |= UI_MOUSEPROC_DONESOMETHING;
			}

			if(pChild->IsIn(ptCur.x, ptCur.y) && (dwFlags & UI_MOUSE_LBDBLCLK) )	
			{
				dwChildRet |= UI_MOUSEPROC_DONESOMETHING;
			}
		}

		if (UI_MOUSEPROC_DONESOMETHING & dwChildRet)
		{	// �̰�쿡�� �հ� ��Ŀ���� ���� ����̴�.
			// (�Ʒ� �ڵ�� dialog�� �����ϴ� ������ �ؾ� �Ѵ�. ���� ���Ƴ���)
//			m_Children.erase(itor);			// �켱 ����Ʈ���� �����
//			m_Children.push_front(pChild);	// �Ǿտ� �ִ´�. �׸��� ������ �� ���߿� �׸����� �Ϸ���

			dwRet |= (UI_MOUSEPROC_CHILDDONESOMETHING|UI_MOUSEPROC_DONESOMETHING);
			return dwRet;
		}
	}

	// UI �����̴� �ڵ�
	if (UI_STATE_COMMON_MOVE != m_eState && 
			PtInRect(&m_rcMovable, ptCur) && (dwFlags&UI_MOUSE_LBCLICK) )
	{
		SetState(UI_STATE_COMMON_MOVE);
		dwRet |= UI_MOUSEPROC_DONESOMETHING;
		return dwRet;
	}

	return dwRet;
}

void CUIDead::CallBackProc(int iID, DWORD dwFlag)
{
	//TRACE("OnButton ID:%d Btn %d\n",iID, dwFlag);

	if(iID == CHILD_UI_REVIVE_MSG)
	{
		if(dwFlag == 1)//OK
		{
			MsgSend_Revival(REVIVAL_TYPE_LIFE_STONE);
		}
	}
	else if(iID == CHILD_UI_LACK_LIVE_STONE_MSG)
	{
	}
	else if(iID == CHILD_UI_LOW_LEVEL)
	{
	}
}

void CUIDead::MsgSend_Revival(BYTE byType)
{
	if(m_bProcessing) return;

	if(CGameProcedure::s_pPlayer->m_iSendRegeneration >= 2) return; // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���

	BYTE byBuff[4];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_REGENE);
	CAPISocket::MP_AddByte(byBuff, iOffset, byType);
	CGameProcedure::s_pSocket->Send(byBuff, iOffset); // ������..

	CLogWriter::Write("Send Regeneration");
	CGameProcedure::s_pPlayer->m_iSendRegeneration = 2; // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
	//TRACE("���� - �ٽ� ��Ƴ���\n");

	m_bProcessing = true;
}

void CUIDead::MsgRecv_Revival(DataPack *pDataPack, int &iOffset)
{
	m_bProcessing = false;

	__Vector3 vPosPlayer;
	vPosPlayer.x = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	vPosPlayer.z = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	vPosPlayer.y = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;
	
	CGameProcedure::s_pProcMain->InitPlayerPosition(vPosPlayer); // �÷��̾� ��ġ �ʱ�ȭ.. ������ �����, �⺻������ ���ϰ� �Ѵ�.
	CGameProcedure::s_pPlayer->RegenerateCollisionMesh(); // �浹 �޽ø� �ٽ� �����..

	CGameProcedure::s_pPlayer->m_iSendRegeneration = 0; // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
	CGameProcedure::s_pPlayer->m_fTimeAfterDeath = 0; // �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷���
	//TRACE("���� - �ٽ� ��Ƴ���(%.1f, %.1f)\n", vPosPlayer.x, vPosPlayer.z);

	//
	//���� & ȿ�� �ʱ�ȭ..
	if(CGameProcedure::s_pProcMain->m_pUIStateBarAndMiniMap) 
		CGameProcedure::s_pProcMain->m_pUIStateBarAndMiniMap->ClearMagic();
	if(CGameProcedure::s_pProcMain->m_pMagicSkillMng) 
		CGameProcedure::s_pProcMain->m_pMagicSkillMng->ClearDurationalMagic();
	if(CGameProcedure::s_pProcMain->CGameProcedure::s_pFX) 
		CGameProcedure::s_pFX->StopMine();

	CLogWriter::Write("Receive Regeneration");

	int iID = CGameProcedure::s_pPlayer->IDNumber();
	if(CGameProcedure::s_pPlayer->Nation()==NationEnum::KARUS) 
		CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_REGEN_KARUS, iID, -1);
	else if(CGameProcedure::s_pPlayer->Nation()==NationEnum::ELMORAD) 
		CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_REGEN_ELMORAD, iID, -1);

	SetVisible(false);
}

void CUIDead::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}

void CUIDead::SetVisibleWithNoSound(bool bVisible, bool bWork, bool bReFocus)
{
	if(!bWork)
	{
		CN3UIBase::SetVisibleWithNoSound(bVisible, bWork, bReFocus);
	}
}
