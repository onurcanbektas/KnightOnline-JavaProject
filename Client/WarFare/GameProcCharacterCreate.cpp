// GameProcCharacterCreate.cpp: implementation of the CGameProcCharacterCreate class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "GameProcCharacterCreate.h"

//#include "Resource.h"
#include "GameEng.h"
#include "UICharacterCreate.h"
#include "UIMessageBox.h"
#include "UIMessageBoxManager.h"
#include "PlayerMySelf.h"
#include "UIManager.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "PacketDef.h"
#include "GameCursor.h"

#include "N3UIString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameProcCharacterCreate::CGameProcCharacterCreate()
{
	m_pUICharacterCreate = NULL;

	s_pPlayer->m_InfoBase.eRace = RACE_UNKNOWN;
	s_pPlayer->m_InfoBase.eClass = CLASS_UNKNOWN;
}

CGameProcCharacterCreate::~CGameProcCharacterCreate()
{
	delete m_pUICharacterCreate;
}

void CGameProcCharacterCreate::Release()
{
	CGameProcedure::Release();

	delete m_pUICharacterCreate; m_pUICharacterCreate = NULL;

	SetRect(&m_rcChr, 0, 0, 0, 0);
	m_Tbl_InitValue.Release();

	s_pPlayer->m_ChrInv.Release();
	s_pPlayer->m_ChrInv.PartAlloc(PART_POS_COUNT);
	s_pPlayer->m_ChrInv.PlugAlloc(PLUG_POS_COUNT);
}

void CGameProcCharacterCreate::Init()
{
	CGameProcedure::Init();

	SetRect(&m_rcChr, 0, 0, 0, 0);

	m_Tbl_InitValue.LoadFromFile("Data\\NewChrValue.tbl");

	s_pPlayer->m_InfoBase.eRace = RACE_UNKNOWN;
	s_pPlayer->m_InfoBase.eClass = CLASS_UNKNOWN;
	s_pPlayer->m_ChrInv.Release();
	s_pPlayer->m_ChrInv.PartAlloc(PART_POS_COUNT);
	s_pPlayer->m_ChrInv.PlugAlloc(PLUG_POS_COUNT);

	__TABLE_UI_RESRC* pTblUI = s_pTbl_UI->Find(s_pPlayer->m_InfoBase.eNation->getValue());
	m_pUICharacterCreate = new CUICharacterCreate();
	m_pUICharacterCreate->Init(s_pUIMgr);
	if(pTblUI) m_pUICharacterCreate->LoadFromFile(pTblUI->szCharacterCreate);
}

void CGameProcCharacterCreate::Render()
{
	s_pEng->Clear(0); // Ŭ����..
	s_pEng->s_lpD3DDev->BeginScene();			// �� ���� ����...

	s_pUIMgr->Render();

	s_pPlayer->InventoryChrRender(m_rcChr); // ĳ���� �׸���..

	s_pMsgBoxMgr->Render(); //MessageBox�� �׷��ش�.
	if(s_pGameCursor) s_pGameCursor->Render();

	s_pEng->s_lpD3DDev->EndScene();			// �� ���� ����...
	s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcCharacterCreate::SetChr()
{
	__InfoPlayerBase*	pInfoBase = &(s_pPlayer->m_InfoBase);
	__InfoPlayerMySelf*	pInfoExt = &(s_pPlayer->m_InfoExt);
	__TABLE_NEW_CHR* pTbl = m_Tbl_InitValue.Find(pInfoBase->eRace);

	if(pTbl)
	{
		pInfoExt->iStrength =		pTbl->iStr;
		pInfoExt->iStamina =		pTbl->iSta;
		pInfoExt->iDexterity =		pTbl->iDex;
		pInfoExt->iIntelligence =	pTbl->iInt;
		pInfoExt->iMagicAttak =		pTbl->iMAP;
		m_pUICharacterCreate->m_iBonusPoint	=		pTbl->iBonus;
		m_pUICharacterCreate->m_iMaxBonusPoint =	pTbl->iBonus;
	}

	__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks->Find(s_pPlayer->m_InfoBase.eRace);	// User Player Character Skin ����ü ������..;
	if(NULL == pLooks) return;

	s_pPlayer->InitChr(pLooks);
	s_pPlayer->m_ChrInv.ScaleSet(1,1,1); // �������� ������� ������.

	if(pLooks) // ��Ʈ ����..
	{
		for(int i = 0; i < PART_POS_COUNT; i++)
		{
			if(i == PART_POS_FACE) { s_pPlayer->InitFace(); continue; }
			if(i == PART_POS_HAIR_HELMET) { s_pPlayer->InitHair(); continue; }
			s_pPlayer->PartSet((e_PartPosition)i, pLooks->szPartFNs[i], NULL, NULL);
		}
	}
	
	m_pUICharacterCreate->Reset();
	m_pUICharacterCreate->UpdateRaceAndClassButtons(pInfoBase->eRace);
	m_pUICharacterCreate->UpdateClassButtons(pInfoBase->eClass);

	s_pPlayer->InventoryChrAnimationInitialize();
	s_pPlayer->Action(PSA_BASIC, true, NULL, true);
}

void CGameProcCharacterCreate::Tick()
{
//	s_pLocalInput->Tick(); // Ű����� ���콺�κ��� �Է��� �޴´�.
//	if(dwMouseFlags & MOUSE_LBDOWN) SetCursor(s_hCursorClick);
//	else SetCursor(s_hCursorNormal);

	CGameProcedure::Tick();

	DWORD dwMouseFlags = s_pLocalInput->MouseGetFlag();
	m_pUICharacterCreate->Tick();
	m_pUICharacterCreate->MouseProc(dwMouseFlags, s_pLocalInput->MouseGetPos(), s_pLocalInput->MouseGetPosOld());

	s_pEng->s_SndMgr.Tick(); // Sound Engine...

	// Network Msg ó���ϱ�
	DataPack* pDataPack = NULL;
	while ( s_pSocket->m_qRecvPkt.size() > 0 )			// ��Ŷ ����Ʈ�� ��Ŷ�� �ֳ�????
	{
		int iOffset = 0;
		pDataPack = s_pSocket->m_qRecvPkt.front();			// ť�� ù��° ���� ����..
		if (false == ProcessPacket(pDataPack, iOffset)) break;		// ��Ŷ�� ó���� ��Ȳ�� �ƴϴ�.
		delete pDataPack;
		s_pSocket->m_qRecvPkt.pop();					// ��Ŷ�� ť���� ����..
	}

	s_pPlayer->InventoryChrTick();
}

bool CGameProcCharacterCreate::MsgSendCharacterCreate()
{
	e_ErrorCharacterCreate eErrCode = ERROR_CHARACTER_CREATE_SUCCESS;
	const std::string& szID = s_pPlayer->IDString();

	int iIDLength = szID.size();
	if(iIDLength <= 0)
	{
		eErrCode = ERROR_CHARACTER_CREATE_INVALID_NAME;
	}
	else if(RACE_UNKNOWN == s_pPlayer->m_InfoBase.eRace)
	{
		eErrCode = ERROR_CHARACTER_CREATE_INVALID_RACE;
	}
//	else if(RACE_KA_WRINKLETUAREK == s_pPlayer->m_InfoBase.eRace) // ������� ���� �Ұ���..
//	{
//		eErrCode = ERROR_CHARACTER_CREATE_NOT_SUPPORTED_RACE;
//	}
	else if(CLASS_UNKNOWN == s_pPlayer->m_InfoBase.eClass)
	{
		eErrCode = ERROR_CHARACTER_CREATE_INVALID_CLASS;
	}
	else if(m_pUICharacterCreate->m_iBonusPoint > 0)
	{
		eErrCode = ERROR_CHARACTER_CREATE_REMAIN_BONUS_POINT;
	}
	else
	{
		// �̸��� ��ĭ�̳� Ư�����ڰ� ��� �ִ��� Ȯ��
		bool bHasSpecialLetter = false;
		for(int i = 0; i < iIDLength; i++)
		{
			// CompadmreString(LOCALE_USER_DEFAULT, NORM_IGNOREWIDTH, id, strlen(id), pUser->m_UserId, strlen(pUser->m_UserId) ) == CSTR_EQUAL )
			if(	'~' == szID[i] || 
				'`' == szID[i] || 
				'!' == szID[i] || 
				'@' == szID[i] || 
				'#' == szID[i] || 
				'$' == szID[i] || 
				'%' == szID[i] || 
				'^' == szID[i] || 
				'&' == szID[i] || 
				'*' == szID[i] || 
				'(' == szID[i] || 
				')' == szID[i] || 
//				'_' == szID[i] || 
				'-' == szID[i] || 
				'+' == szID[i] || 
				'=' == szID[i] || 
				'|' == szID[i] || 
				'\\' == szID[i] || 
				'<' == szID[i] || 
				'>' == szID[i] || 
				',' == szID[i] || 
				'.' == szID[i] || 
				'?' == szID[i] || 
				'/' == szID[i] || 
				'{' == szID[i] || 
				'[' == szID[i] || 
				'}' == szID[i] || 
				']' == szID[i] ||
				'\"' == szID[i] ||
				'\'' == szID[i] ||
				' ' == szID[i] ) 
			{
				bHasSpecialLetter = true;
				eErrCode = ERROR_CHARACTER_CREATE_INVALID_NAME_HAS_SPECIAL_LETTER;
				break; 
			}
		}

		if(false == bHasSpecialLetter)
		{

			__InfoPlayerBase*	pInfoBase = &(s_pPlayer->m_InfoBase);
			__InfoPlayerMySelf*	pInfoExt = &(s_pPlayer->m_InfoExt);

			BYTE byBuff[64];
			int iOffset = 0;
			CAPISocket::MP_AddByte(byBuff, iOffset,  N3_NEW_CHARACTER);					// Ŀ���.
			CAPISocket::MP_AddByte(byBuff, iOffset, CGameProcedure::s_iChrSelectIndex);	// ĳ���� �ε��� b
			CAPISocket::MP_AddShort(byBuff, iOffset, iIDLength);						// Id ���� s
			CAPISocket::MP_AddString(byBuff, iOffset, s_pPlayer->IDString());			// ID ���ڿ� str
			CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->m_InfoBase.eRace);		// ���� b
			CAPISocket::MP_AddShort(byBuff, iOffset, s_pPlayer->m_InfoBase.eClass);		// ���� b
			CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iFace);					// �󱼸�� b
			CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iHair);					// �Ӹ���� b
			CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iStrength);				// �� b
			CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iStamina);				// ������ b
			CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iDexterity);				// ��ø b
			CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iIntelligence);			// ���� b
			CAPISocket::MP_AddByte(byBuff, iOffset, pInfoExt->iMagicAttak);				// ���� b

			s_pSocket->Send(byBuff, iOffset);								// ������
			
			s_pUIMgr->EnableOperationSet(false); // ��Ŷ�� ���ö����� UI �� Disable ��Ų��...
			
			return true;
		}
	}

	ReportErrorCharacterCreate(eErrCode); // ���� ����...

	return false;
}

void CGameProcCharacterCreate::ReportErrorCharacterCreate(e_ErrorCharacterCreate eErrCode)
{
	std::string szErr;

	if (ERROR_CHARACTER_CREATE_NO_MORE_CHARACTER == eErrCode)
		szErr = "You cannot create anymore characters.";//::_LoadStringFromResource(IDS_ERR_NO_MORE_CHARACTER, szErr);
	else if(ERROR_CHARACTER_CREATE_INVALID_NATION_AND_INVALID_RACE == eErrCode)
		szErr = "The selected nation and the race does not match. ";//::_LoadStringFromResource(IDS_ERR_INVALID_NATION_RACE, szErr);
	else if(ERROR_CHARACTER_CREATE_OVERLAPPED_ID == eErrCode)
		szErr = "This ID is already used on another character.";//::_LoadStringFromResource(IDS_ERR_OVERLAPPED_ID, szErr);
	else if(ERROR_CHARACTER_CREATE_DB_CREATE == eErrCode)
		szErr = "Failed creating Database";//::_LoadStringFromResource(IDS_ERR_DB_CREATE, szErr);
	else if(ERROR_CHARACTER_CREATE_INVALID_NAME == eErrCode)
		szErr = "Please enter your character ID.";//::_LoadStringFromResource(IDS_ERR_INVALID_NAME, szErr);
	else if(ERROR_CHARACTER_CREATE_INVALID_NAME_HAS_SPECIAL_LETTER == eErrCode)
		szErr = "You cannot use this character ID.";//::_LoadStringFromResource(IDS_ERR_INVALID_NAME_HAS_SPECIAL_LETTER, szErr);
	else if(ERROR_CHARACTER_CREATE_INVALID_RACE == eErrCode)
		szErr = "Please select a race.";//::_LoadStringFromResource(IDS_ERR_INVALID_RACE, szErr);
	else if(ERROR_CHARACTER_CREATE_NOT_SUPPORTED_RACE == eErrCode)
		szErr = "This race is not available yet.";//::_LoadStringFromResource(IDS_ERR_NOT_SUPPORTED_RACE, szErr);
	else if(ERROR_CHARACTER_CREATE_INVALID_CLASS == eErrCode)
		szErr = "Please select a specialty.";//::_LoadStringFromResource(IDS_ERR_INVALID_CLASS, szErr);
	else if(ERROR_CHARACTER_CREATE_REMAIN_BONUS_POINT == eErrCode)
		szErr = "There are stat points still remaining.";//::_LoadStringFromResource(IDS_ERR_REMAIN_BONUS_POINT, szErr);
	else if(ERROR_CHARACTER_CREATE_INVALID_STAT_POINT == eErrCode)
		szErr = "Unknown error.";//::_LoadStringFromResource(IDS_ERR_UNKNOWN, szErr);
	else
		szErr = "Unknown error.";//::_LoadStringFromResource(IDS_ERR_UNKNOWN, szErr);

	
	std::string szTitle = "Failed creating character"; //::_LoadStringFromResource(IDS_ERR_CHARACTER_CREATE, szTitle);
	MessageBoxPost(szErr, szTitle, MB_OK);
}

bool CGameProcCharacterCreate::ProcessPacket(DataPack* pDataPack, int& iOffset)
{
	int iOffsetPrev = iOffset;
	if(false == CGameProcedure::ProcessPacket(pDataPack, iOffset)) iOffset = iOffsetPrev;
	else return true;

	int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// Ŀ��� �Ľ�..
	switch ( iCmd )										// Ŀ��忡 �ٶ� �б�..
	{
		case N3_NEW_CHARACTER:				// ĳ���� ���� �޽���..
		{
			BYTE bySuccess = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// Ŀ��� �Ľ�..
			if(0 == bySuccess) 
			{
				ProcActiveSet((CGameProcedure*)s_pProcCharacterSelect); // ĳ���� ����â���� ����..
			}
			else // �����ϸ�.. ������ 0 �� �ƴ� ������ �´�..
			{
				this->ReportErrorCharacterCreate((e_ErrorCharacterCreate)bySuccess); // ���� �޽��� ���..
				s_pUIMgr->EnableOperationSet(false); // UI ���� �����ϰ� �Ѵ�... �ٽ� ĳ���� ������ �Ѵ�..
			}
			s_pUIMgr->EnableOperationSet(false); // ��Ŷ�� ���ö����� UI �� Disable ��Ų��...
		}
		return true;
	}
	
	return false;
}