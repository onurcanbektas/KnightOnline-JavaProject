// GameProcNationSelect.cpp: implementation of the CGameProcNationSelect class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include "PacketDef.h"
#include "GameEng.h"
#include "APISocket.h"

#include "GameProcNationSelect.h"
#include "PlayerMySelf.h"
#include "UINationSelectDlg.h"
#include "UIManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcNationSelect::CGameProcNationSelect()
{
	m_pUINationSelectDlg = NULL;
	s_pPlayer->m_InfoBase.eNation = &NationEnum::NO_NATION;
}

CGameProcNationSelect::~CGameProcNationSelect()
{
	delete m_pUINationSelectDlg; m_pUINationSelectDlg = NULL;
}

#pragma region graphicsHandlers
void CGameProcNationSelect::Release()
{
	CGameProcedure::Release();

	delete m_pUINationSelectDlg; m_pUINationSelectDlg = NULL;
}

void CGameProcNationSelect::Init()
{
	CGameProcedure::Init();

	std::string szTemp = "UI\\Co_NationSelect.uif";
	__TABLE_UI_RESRC* pTbl = s_pTbl_UI->Find(NationEnum::ELMORAD.getValue());
	if(pTbl)
	{
		szTemp = pTbl->szNationSelect;
	}

	m_pUINationSelectDlg = new CUINationSelectDlg();
	m_pUINationSelectDlg->Init(s_pUIMgr);
	m_pUINationSelectDlg->LoadFromFile(szTemp);
	m_pUINationSelectDlg->m_pProcNationSelectRef = this;

	s_pPlayer->m_InfoBase.eNation = &NationEnum::NO_NATION;
}

void CGameProcNationSelect::Tick()
{
	CGameProcedure::Tick();

	if (NationEnum::NO_NATION != *s_pPlayer->m_InfoBase.eNation)
	{
		CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcCharacterSelect);
	}
}

void CGameProcNationSelect::Render()
{
	DWORD color = 0x00000000;
	s_pEng->Clear(color);
	s_pEng->s_lpD3DDev->BeginScene();

	CGameProcedure::Render(); 

	s_pEng->s_lpD3DDev->EndScene();
	s_pEng->Present(CN3Base::s_hWndBase);
}
#pragma endregion graphicsHandlers

void CGameProcNationSelect::MsgSendNationSelect(NationEnum & eNation)
{
	BYTE byBuff[4];										// 패킷 버퍼..
	int iOffset=0;										// 버퍼의 오프셋..

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_NATION_SELECT);	// 커멘드.
	CAPISocket::MP_AddByte(byBuff, iOffset, (BYTE)eNation.getValue());		// 아이디 길이..
		
	s_pSocket->Send(byBuff, iOffset);								// 보낸다

	s_pUIMgr->EnableOperationSet(false); // 응답 패킷을 받기 전까지 아무짓 못하게 한다..
}

bool CGameProcNationSelect::ProcessPacket(DataPack* pDataPack, int& iOffset)
{
	int iOffsetPrev = iOffset;
	if(false == CGameProcedure::ProcessPacket(pDataPack, iOffset)) iOffset = iOffsetPrev;
	else return true;

	int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 커멘드 파싱..
	switch ( iCmd )										// 커멘드에 다라서 분기..
	{
		case N3_NATION_SELECT:							// 캐릭터 선택 메시지..
		{
			int iNation = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 국가 - 0 실패.. 1 - 카루스 2 - 엘모라드..

			s_pPlayer->m_InfoBase.eNation = &NationEnum::forValue(iNation);
		}
		return true;
	}
	
	return false;
}