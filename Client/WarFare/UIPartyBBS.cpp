// UIPartyBBS.cpp: implementation of the CUIPartyBBS class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "resource.h"
#include "UIPartyBBS.h"
#include "GameProcMain.h"
#include "UIVarious.h"
#include "GameProcedure.h"
#include "PlayerMySelf.h"
#include "UIManager.h"

#include "N3UIList.h"
#include "N3UIString.h"
#include "N3UIButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PARTY_BBS_MAXSTRING	69
#define PARTY_BBS_MAXLINE	23
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIPartyBBS::CUIPartyBBS()
{
//	m_pList_Infos			= NULL;		
	m_pBtn_PageUp			= NULL;
	m_pBtn_PageDown			= NULL;
	m_pBtn_Refresh			= NULL;
	
	m_pBtn_Close			= NULL;
	m_pBtn_Register			= NULL;
	m_pBtn_RegisterCancel	= NULL;
	m_pBtn_Whisper			= NULL;
	m_pBtn_Party			= NULL;
	m_pText_Page			= NULL;

	m_iCurPage				= 0;
	m_bProcessing			= false;
	m_fTime					= 0.0f;
	m_iCurIndex				= -1;

	for(int i = 0 ; i < PARTY_BBS_MAXSTRING ; i++)
	{
		m_pText[i] = NULL;
	}

}

CUIPartyBBS::~CUIPartyBBS()
{
}

bool CUIPartyBBS::Load(HANDLE hFile)
{
	if(CN3UIBase::Load(hFile)==false) return false;

//	m_pList_Infos = (CN3UIList*)(this->GetChildByID("List_Friends"));					__ASSERT(m_pList_Infos, "NULL UI Component!!!");
	m_pBtn_PageUp = (CN3UIButton*)(this->GetChildByID("btn_page_up"));					__ASSERT(m_pBtn_PageUp, "NULL UI Component!!!");
	m_pBtn_PageDown = (CN3UIButton*)(this->GetChildByID("btn_page_down"));				__ASSERT(m_pBtn_PageDown, "NULL UI Component!!!");
	m_pBtn_Refresh = (CN3UIButton*)(this->GetChildByID("btn_refresh"));					__ASSERT(m_pBtn_Refresh, "NULL UI Component!!!");
	
	m_pBtn_Close = (CN3UIButton*)(this->GetChildByID("btn_exit"));						__ASSERT(m_pBtn_Close, "NULL UI Component!!!");
	m_pBtn_Register = (CN3UIButton*)(this->GetChildByID("btn_add"));					__ASSERT(m_pBtn_Register, "NULL UI Component!!!");
	m_pBtn_RegisterCancel = (CN3UIButton*)(this->GetChildByID("btn_delete"));			__ASSERT(m_pBtn_RegisterCancel, "NULL UI Component!!!");
	m_pBtn_Whisper = (CN3UIButton*)(this->GetChildByID("btn_whisper"));					__ASSERT(m_pBtn_Whisper, "NULL UI Component!!!");
	m_pBtn_Party = (CN3UIButton*)(this->GetChildByID("btn_Party"));						__ASSERT(m_pBtn_Party, "NULL UI Component!!!");


	m_pText_Page = (CN3UIString*)(this->GetChildByID("string_page"));					__ASSERT(m_pText_Page, "NULL UI Component!!!");

	char szBuf[64];
	for(int i = 0 ; i < PARTY_BBS_MAXSTRING ; i++)
	{
		sprintf(szBuf,"text_%.2d",i);
		m_pText[i] = (CN3UIString*)(this->GetChildByID(szBuf));
	}

	m_iCurPage = 0; // ���� ������..

	return true;
}

bool CUIPartyBBS::SelectedString(CN3UIBase* pSender, int& iID)
{
	int iIndex = -1;
	for(int i = 0; i < PARTY_BBS_MAXSTRING ; i++)
	{
		if(pSender == m_pText[i])
		{
			iIndex = i % PARTY_BBS_MAXLINE;
			if( iIndex >= m_Datas.size() )
				return false;

			iID = iIndex;
//			SetStringColor( iID, 0xffff0000);
			return true;
		}
	}

	return false;
}

bool CUIPartyBBS::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	int iID = -1;
	if( dwMsg == UIMSG_BUTTON_CLICK )
	{
		if(pSender == m_pBtn_Refresh)
		{
			this->MsgSend_RefreshData(m_iCurPage);
		}
		else if(pSender == m_pBtn_PageUp)
		{
			int iCurPage = m_iCurPage;
			iCurPage--;
			if(iCurPage >= 0)
			{
				this->MsgSend_RefreshData(iCurPage);
			}
		}
		else if(pSender == m_pBtn_PageDown)
		{
			int iCurPage = m_iCurPage;
			iCurPage++;
			if(iCurPage < m_iMaxPage)
			{
				this->MsgSend_RefreshData(iCurPage);
			}
		}
		else if(pSender == m_pBtn_Close)
		{
			m_iCurPage = 0;
			m_fTime = 0.0f;
			this->SetVisible(false);
		}
		else if(pSender == m_pBtn_Register)
		{
			std::string szMsg;
			szMsg = "IDS_PARTY_BBS_REGISTER";//::_LoadStringFromResource(IDS_PARTY_BBS_REGISTER, szMsg);
			CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_PARTY_BBS_REGISTER); // ���� ��ü �����..
		}
		else if(pSender == m_pBtn_RegisterCancel)
		{
			std::string szMsg;
			szMsg = "IDS_PARTY_BBS_REGISTER_CANCEL";//::_LoadStringFromResource(IDS_PARTY_BBS_REGISTER_CANCEL, szMsg);
			CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_PARTY_BBS_REGISTER_CANCEL); // ���� ��ü �����..
		}
		else if(pSender == m_pBtn_Whisper)
		{
			RequestWhisper();
		}
		else if(pSender == m_pBtn_Party) // ��Ƽ ��û
		{
			RequestParty();
		}
	}
	else if( dwMsg == UIMSG_STRING_LCLICK )
	{
		if(SelectedString(pSender, iID))
		{
			m_iCurIndex = iID;
		}
	}

	return true;
}

void CUIPartyBBS::MsgSend_RefreshData(int iCurPage)
{
	if(m_bProcessing) return; //���� ���� ��Ŷ ������ ������

	float fTime = CN3Base::TimeGet();
	if( fTime - m_fTime < 3.0f )
		return;
	m_fTime = fTime;

	BYTE byBuff[4];
	int iOffset=0;

	short sPage = m_iCurPage;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_PARTY_BBS);
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_BBS_DATA);
	CAPISocket::MP_AddShort(byBuff, iOffset, sPage);
	CGameProcedure::s_pSocket->Send(byBuff, iOffset);

	m_bProcessing = true;
}

void CUIPartyBBS::MsgRecv_RefreshData(DataPack* pDataPack, int& iOffset)
{
	m_bProcessing = false;

	BYTE byType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	BYTE byResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	if(byResult != 0x01) return; //�����ߴٸ�

	switch( byType )
	{
	case N3_SP_PARTY_REGISTER:
		{
			if(!IsVisible())
				SetVisible(true);
		}
	case N3_SP_PARTY_REGISTER_CANCEL:
		{
			PartyStringSet(byType);
			if(!IsVisible()) return;
		}
		break;
	case N3_SP_PARTY_BBS_DATA:
		{
			if(!IsVisible())
				SetVisible(true);
		}
		break;
	}

	m_Datas.clear();

	for( int i = 0 ; i < PARTY_BBS_MAXLINE ; i++ )
	{
		__InfoPartyBBS Info;
		int iNameLen	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, Info.szID, iNameLen);
		Info.iLevel		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		Info.eClass		= (e_Class)CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		if( iNameLen > 0 )
			m_Datas.push_back(Info);
	}

	short sPage = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	short sTotal = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	m_iCurPage = sPage;
	m_iMaxPage = sTotal / PARTY_BBS_MAXLINE;
	if( (sTotal % PARTY_BBS_MAXLINE) > 0 )
		m_iMaxPage++;

	RefreshPage();
}


void CUIPartyBBS::MsgSend_Register()
{
	if(m_bProcessing) return; //���� ���� ��Ŷ ������ ������

	BYTE byBuff[4];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_PARTY_BBS);	
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_REGISTER);
	CGameProcedure::s_pSocket->Send(byBuff, iOffset);

	m_bProcessing = true;
}

void CUIPartyBBS::MsgSend_RegisterCancel()
{
	if(m_bProcessing) return; //���� ���� ��Ŷ ������ ������

	BYTE byBuff[4];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_PARTY_BBS);	
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_REGISTER_CANCEL);	
	CGameProcedure::s_pSocket->Send(byBuff, iOffset);

	m_bProcessing = true;
}


void CUIPartyBBS::RefreshPage()
{
	if(m_pText_Page) m_pText_Page->SetStringAsInt(m_iCurPage+1); // ������ ǥ��..

	ResetContent();

	it_PartyBBS it = m_Datas.begin();

	std::string szDuty, szClass;
	for( int i = 0 ; i < PARTY_BBS_MAXLINE ; i++ )
	{
		if(it==m_Datas.end()) return;

		__InfoPartyBBS IPB = (*it);
		CGameProcedure::GetTextByClass(IPB.eClass, szClass);
		SetContentString(i, IPB.szID.c_str(), IPB.iLevel, szClass.c_str());
		it++;
	}
}

void CUIPartyBBS::PartyStringSet(BYTE byType)
{
	switch(byType)
	{
	case N3_SP_PARTY_REGISTER:
		CGameProcedure::s_pPlayer->m_bRecruitParty = true;
		break;
	case N3_SP_PARTY_REGISTER_CANCEL:
		CGameProcedure::s_pPlayer->m_bRecruitParty = false;
		break;
	}

	if(CGameProcedure::s_pPlayer->m_bRecruitParty)
	{
		int iLevel = CGameProcedure::s_pPlayer->m_InfoBase.iLevel;
		int iLMin = iLevel - 8;
		if(iLMin < 0) iLMin = 0;
		int iLMax = iLevel + 8;
		if(iLMax > 60) iLMax = 60;

		char szBuff[128];
		std::string szMsg;
		szMsg = "IDS_WANT_PARTY_MEMBER";//::_LoadStringFromResource(IDS_WANT_PARTY_MEMBER, szMsg);
		sprintf(szBuff, szMsg.c_str(), iLMin, iLMax);
		CGameProcedure::s_pPlayer->InfoStringSet(szBuff, 0xff00ff00);
		CGameProcedure::s_pProcMain->MsgSend_StateChange(N3_SP_STATE_CHANGE_RECRUIT_PARTY, 0x02); // ��Ƽ ��û.. ���
	}
	else
	{
		CGameProcedure::s_pPlayer->InfoStringSet("", 0);
		CGameProcedure::s_pProcMain->MsgSend_StateChange(N3_SP_STATE_CHANGE_RECRUIT_PARTY, 0x01); // ��Ƽ ��û..
	}
}

void CUIPartyBBS::ResetContent()
{
	if(m_Datas.size()>0)
		m_iCurIndex = 0;
	else
		m_iCurIndex = -1;

	for(int i = 0 ; i < PARTY_BBS_MAXSTRING ; i++)
	{
		if(m_pText[i])
		{
			m_pText[i]->SetString("");
			m_pText[i]->SetColor(0xffffffff);
		}
	}
}

void CUIPartyBBS::SetContentString(int iIndex, std::string szID, int iLevel, std::string szClass)
{
	if(m_pText[iIndex])
		m_pText[iIndex]->SetString(szID);

	if(m_pText[iIndex + PARTY_BBS_MAXLINE])
	{
		char szBuf[20];
		sprintf(szBuf,"%d",iLevel);
		m_pText[iIndex + PARTY_BBS_MAXLINE]->SetString(szBuf);
	}

	if(m_pText[iIndex + PARTY_BBS_MAXLINE*2])
		m_pText[iIndex + PARTY_BBS_MAXLINE*2]->SetString(szClass);
}

void CUIPartyBBS::Render()
{
	if(!IsVisible()) return;

	CN3UIBase::Render();
	RenderSelectContent();
}

void CUIPartyBBS::RenderSelectContent()
{
	if(!IsVisible())	return;
	if(m_iCurIndex < 0)	return;
	if(m_iCurIndex >= PARTY_BBS_MAXLINE) return;

	RECT rc, rc1;
	if(m_pText[m_iCurIndex])
	{
		rc = m_pText[m_iCurIndex]->GetRegion();
		if(m_pText[m_iCurIndex + PARTY_BBS_MAXLINE*2])
		{
			rc1 = m_pText[m_iCurIndex + PARTY_BBS_MAXLINE*2]->GetRegion();
			rc.right = rc1.right;
		}
	}
	else
		return;

	__VertexTransformedColor vLines[5];
	vLines[0].Set(rc.left, rc.top, UI_DEFAULT_Z, UI_DEFAULT_RHW, 0xff00ff00);
	vLines[1].Set(rc.right, rc.top, UI_DEFAULT_Z, UI_DEFAULT_RHW, 0xff00ff00);
	vLines[2].Set(rc.right, rc.bottom, UI_DEFAULT_Z, UI_DEFAULT_RHW, 0xff00ff00);
	vLines[3].Set(rc.left, rc.bottom, UI_DEFAULT_Z, UI_DEFAULT_RHW, 0xff00ff00);
	vLines[4] = vLines[0];

	DWORD dwZ, dwFog, dwAlpha, dwCOP, dwCA1, dwSrcBlend, dwDestBlend, dwVertexShader, dwAOP, dwAA1;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZ);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlpha);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
	CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwCOP);
	CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLORARG1, &dwCA1);
	CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAOP);
	CN3Base::s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwAA1);
	CN3Base::s_lpD3DDev->GetFVF(&dwVertexShader);

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	CN3Base::s_lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);
	CN3Base::s_lpD3DDev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vLines, sizeof(__VertexTransformedColor));
	
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZ);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlpha);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, dwCOP);
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, dwCA1);
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwAOP);
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwAA1);
	CN3Base::s_lpD3DDev->SetFVF(dwVertexShader);
}

void CUIPartyBBS::SetStringColor(int iIndex, DWORD dwColor)
{
	if(m_pText[iIndex])
		m_pText[iIndex]->SetColor(dwColor);

	if(m_pText[iIndex + PARTY_BBS_MAXLINE])
		m_pText[iIndex + PARTY_BBS_MAXLINE]->SetColor(dwColor);

	if(m_pText[iIndex + PARTY_BBS_MAXLINE*2])
		m_pText[iIndex + PARTY_BBS_MAXLINE*2]->SetColor(dwColor);
}

void CUIPartyBBS::RequestWhisper()
{
	if(m_iCurIndex <= -1)
		return;

	it_PartyBBS it = m_Datas.begin();

	for( int i = 0 ; i < PARTY_BBS_MAXLINE ; i++, it++ )
	{
		if( it == m_Datas.end() ) break;
		if( i == m_iCurIndex )
		{
			__InfoPartyBBS IPB = (*it);
			if(0 != lstrcmpi(IPB.szID.c_str(), CGameProcedure::s_pPlayer->m_InfoBase.szID.c_str()))
			{//�� �ڽſ��Դ� �ӼӸ��� ���ϰ� �Ѵ�...
				CGameProcedure::s_pProcMain->MsgSend_ChatSelectTarget(IPB.szID);
			}
			break;
		}
	}
}

void CUIPartyBBS::RequestParty()
{
	if(m_iCurIndex <= -1)
		return;

	it_PartyBBS it = m_Datas.begin();

	for( int i = 0 ; i < PARTY_BBS_MAXLINE ; i++, it++ )
	{
		if( it == m_Datas.end() ) break;
		if( i == m_iCurIndex )
		{
			__InfoPartyBBS IPB = (*it);

			if(0 != lstrcmpi(IPB.szID.c_str(), CGameProcedure::s_pPlayer->m_InfoBase.szID.c_str()))
			{//�� �ڽſ��Դ� ��Ƽ ��û�� ���ϰ� �Ѵ�...
				std::string szMsg;
				if (CGameProcedure::s_pProcMain->MsgSend_PartyOrForceCreate(0, IPB.szID))
					szMsg = "IDS_PARTY_INVITE";//::_LoadStringFromResource(IDS_PARTY_INVITE, szMsg); // ��Ƽ
				else
					szMsg = "IDS_PARTY_INVITE_FAILED";//::_LoadStringFromResource(IDS_PARTY_INVITE_FAILED, szMsg); // ��Ƽ �ʴ� ����
				CGameProcedure::s_pProcMain->MsgOutput(IPB.szID + szMsg, 0xffffff00);
				break;
			}
		}
	}
}

void CUIPartyBBS::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}

bool CUIPartyBBS::OnKeyPress(int iKey)
{
	switch(iKey)
	{
	case SDL_SCANCODE_ESCAPE://DIK_ESCAPE:
		SetVisible(false);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}
