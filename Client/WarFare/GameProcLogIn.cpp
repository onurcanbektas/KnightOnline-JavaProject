// GameProcLogIn.cpp: implementation of the CGameProcLogIn class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "Resource.h"
#include "GameEng.h"
#include "GameProcLogIn.h"
#include "UILogIn.h"
#include "PlayerMySelf.h"
#include "UIManager.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "PacketDef.h"

#include "N3Camera.h"
#include "N3Light.h"
#include "N3Chr.h"
#include "N3SndObj.h"
#include "N3SndObjStream.h"
#include "N3SndMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcLogIn::CGameProcLogIn()
{
	m_pUILogIn	= NULL;
	m_pChr		= NULL;
	m_pTexBkg	= NULL;
	m_pCamera	= NULL;
	
	for (int i = 0; i < 3; i++)
	{
		m_pLights[i] = NULL;
	}

	m_bLogIn = false;
}

CGameProcLogIn::~CGameProcLogIn()
{
	delete m_pUILogIn;
	delete m_pChr;
	delete m_pTexBkg;

	delete m_pCamera;

	for (int i = 0; i < 3; i++)
	{
		delete m_pLights[i];
	}
}

#pragma region graphicsHandlers
void CGameProcLogIn::Init()
{
	CGameProcedure::Init();

	m_pTexBkg = new CN3Texture();
	m_pTexBkg->LoadFromFile("Intro\\Moon.dxt");

	m_pChr = new CN3Chr();
	m_pChr->LoadFromFile("Intro\\Intro.N3Chr");
	m_pChr->AniCurSet(0);
	
	m_pCamera = new CN3Camera();
	m_pCamera->EyePosSet(0.22f, 0.91f, -1.63f);
	m_pCamera->AtPosSet(-0.19f, 1.1f, 0.09f);
	m_pCamera->m_Data.fNP = 0.1f;
	m_pCamera->m_Data.fFP = 32.0f;
	m_pCamera->m_bFogUse = false;

	for (int i = 0; i < 3; i++)
	{
		m_pLights[i] = new CN3Light();
	}

	m_pLights[0]->LoadFromFile("Intro\\0.N3Light");
	m_pLights[1]->LoadFromFile("Intro\\1.N3Light");
	m_pLights[2]->LoadFromFile("Intro\\2.N3Light");

	s_pEng->s_SndMgr.ReleaseStreamObj(&(CGameProcedure::s_pSnd_BGM));
	CGameProcedure::s_pSnd_BGM = s_pEng->s_SndMgr.CreateStreamObj(35);

	m_pUILogIn = new CUILogIn();
	m_pUILogIn->Init(s_pUIMgr);
	
	__TABLE_UI_RESRC* pTbl = s_pTbl_UI->GetIndexedData(0);

	// TODO(srmeier): the UIE saves in the old format...
	if (pTbl) m_pUILogIn->LoadFromFile(pTbl->szLogIn);//, N3FORMAT_VER_1298);

	RECT rc = m_pUILogIn->GetRegion();
	int iX = (CN3Base::s_CameraData.vp.Width - (rc.right - rc.left))/2;
	int iY = CN3Base::s_CameraData.vp.Height - (rc.bottom - rc.top);
	m_pUILogIn->SetPos(iX, iY);
	rc.left = 0; rc.top = 0; rc.right = CN3Base::s_CameraData.vp.Width; rc.bottom = CN3Base::s_CameraData.vp.Height;
	m_pUILogIn->SetRegion(rc);
	s_pUIMgr->SetFocusedUI((CN3UIBase*)m_pUILogIn);

	char szIniPath[_MAX_PATH] = "";
	lstrcpy(szIniPath, CN3Base::PathGet().c_str());
	lstrcat(szIniPath, "Server.Ini");
	int iServerCount = GetPrivateProfileInt("Server", "Count", 0, szIniPath);

	//read server list from the client ini file
	char szIPs[256][32];
	memset(szIPs, 0, sizeof(szIPs));

	for(int i = 0; i < iServerCount; i++)
	{
		char szKey[32] = "";
		sprintf(szKey, "IP%d", i);
		GetPrivateProfileString("Server", szKey, "", szIPs[i], 32, szIniPath);
	}

	//try to pick on of the given server ips
	int iServer = -1;
	
	if (iServerCount > 0)
	{
		iServer = rand() % iServerCount;
	}
	
	//check connection
	if(	iServer >= 0 && lstrlen(szIPs[iServer]) )
	{
		s_bNeedReportConnectionClosed = false;
		int iErr = s_pSocket->Connect(GetActiveWindow(), szIPs[iServer], SOCKET_PORT_LOGIN);
		s_bNeedReportConnectionClosed = true;
		
		if (iErr)
		{
			this->ReportServerConnectionFailed(StringConstants::LOGIN_SERVER, iErr, true);
		}

		else
		{
			m_pUILogIn->FocusToID();
		}
	}
	else
	{
		this->MessageBoxPost(StringConstants::NO_SERVER_LIST, StringConstants::LOGIN_SERVER, MB_OK, BEHAVIOR_EXIT);
	}

	if(LIC_KNIGHTONLINE != s_eLogInClassification)
	{
		this->MsgSend_AccountLogIn(s_eLogInClassification);
	}
}

void CGameProcLogIn::Tick()
{
	CGameProcedure::Tick();

	for (int i = 0; i < 3; i++)
	{
		m_pLights[i]->Tick();
	}

	m_pChr->Tick();

	static float fTmp = 0;

	if(fTmp == 0)
	{
		if(CGameProcedure::s_pSnd_BGM) CGameProcedure::s_pSnd_BGM->Play();
	}

	fTmp += CN3Base::s_fSecPerFrm;

	if(fTmp > 21.66f)
	{
		fTmp = 0;
		
		if (CGameProcedure::s_pSnd_BGM)
		{
			CGameProcedure::s_pSnd_BGM->Stop();
		}
	}
}

void CGameProcLogIn::Render()
{
	D3DCOLOR crEnv = 0x00000000;
	s_pEng->Clear(crEnv);
	s_pEng->s_lpD3DDev->BeginScene();

//	__Vector3 vEye(0.22f, 0.91f, -1.63f), vAt(-0.19f, 1.1048f, 0.0975f), vUp(0,1,0);
//	__Matrix44 mtxView, mtxPrj, mtxWorld;
//	mtxWorld.Identity();

	m_pCamera->Tick();
	m_pCamera->Apply();
/*	D3DVIEWPORT8 vp;
	CN3Base::s_lpD3DDev->GetViewport(&vp);
	float fLens = D3DXToRadian(55.0f);
	float fAspect = (float)vp.Width / (float)vp.Height;
	float fNear = 0.1f;
	float fFar = 100.0f;

	::D3DXMatrixLookAtLH(&mtxView, &vEye, &vAt, &vUp);
	::D3DXMatrixPerspectiveFovLH(&mtxPrj, fLens, fAspect, fNear, fFar);
	CN3Base::s_lpD3DDev->SetTransform(D3DTS_VIEW, &mtxView); 
	CN3Base::s_lpD3DDev->SetTransform(D3DTS_PROJECTION, &mtxPrj); 
	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld); 
*/

	for (int i = 0; i < 8; i++)
	{
		CN3Base::s_lpD3DDev->LightEnable(i, FALSE);
	}

	for (int i = 0; i < 3; i++)
	{
		m_pLights[i]->Apply();
	}

/*	D3DLIGHT8 lgt0, lgt1, lgt2;
	
	memset(&lgt0, 0, sizeof(D3DLIGHT8));
	lgt0.Type = D3DLIGHT_POINT;
	lgt0.Attenuation0 = 0.5f;
	lgt0.Range = 100000.0f;
	lgt0.Position = __Vector3(-500, 100, -50);
	lgt0.Diffuse.r = 232/255.0f; lgt0.Diffuse.g = 226/255.0f; lgt0.Diffuse.b = 215/255.0f;

	memset(&lgt1, 0, sizeof(D3DLIGHT8));
	lgt1.Type = D3DLIGHT_POINT;
	lgt1.Attenuation0 = 1.0f;
	lgt1.Range = 100000.0f;
	lgt1.Position = __Vector3(1000, -300, -50);
//	lgt1.Ambient.r = 56/255.0f; lgt1.Ambient.g = 58/255.0f; lgt1.Ambient.b = 129/255.0f;
	lgt1.Diffuse.r = 66/255.0f; lgt1.Diffuse.g = 68/255.0f; lgt1.Diffuse.b = 168/255.0f;

	memset(&lgt2, 0, sizeof(D3DLIGHT8));
	lgt2.Type = D3DLIGHT_POINT;
	lgt2.Attenuation0 = 1.0f;
	lgt2.Range = 100000.0f;
	lgt2.Position = __Vector3(-200, -800, 350);
//	lgt2.Ambient.r = 52/255.0f; lgt2.Ambient.g = 56/255.0f; lgt2.Ambient.b = 107/255.0f;
	lgt2.Diffuse.r = 124/255.0f; lgt2.Diffuse.g = 45/255.0f; lgt2.Diffuse.b = 31/255.0f;

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	for(int i = 0; i < 8; i++)
		CN3Base::s_lpD3DDev->LightEnable(0, FALSE);
	CN3Base::s_lpD3DDev->LightEnable(0, TRUE);
	CN3Base::s_lpD3DDev->LightEnable(1, TRUE);
	CN3Base::s_lpD3DDev->LightEnable(2, TRUE);
	CN3Base::s_lpD3DDev->SetLight(0, &lgt0);
	CN3Base::s_lpD3DDev->SetLight(1, &lgt1);
	CN3Base::s_lpD3DDev->SetLight(2, &lgt2);
*/

	////////////////////////////////////////////
	D3DVIEWPORT9 vp;
	CN3Base::s_lpD3DDev->GetViewport(&vp);

	float fMW = (m_pTexBkg->Width() * vp.Width / 1024.0f)*1.3f;
	float fMH = (m_pTexBkg->Height() * vp.Height / 768.0f)*1.3f;
	float fX = 100.0f * vp.Width / 1024.0f;
	float fY = 50.0f * vp.Height / 768.0f;

	float fRHW = 1.0f;
	__VertexTransformed vMoon[4];
	vMoon[0].Set(fX,     fY,     0, fRHW, 0xffffffff, 0.0f, 0.0f);
	vMoon[1].Set(fX+fMW, fY,     0, fRHW, 0xffffffff, 1.0f, 0.0f);
	vMoon[2].Set(fX+fMW, fY+fMH, 0, fRHW, 0xffffffff, 1.0f, 1.0f);
	vMoon[3].Set(fX,     fY+fMH, 0, fRHW, 0xffffffff, 0.0f, 1.0f);
	
	DWORD dwZWrite;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ZWRITEENABLE, &dwZWrite);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CN3Base::s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CN3Base::s_lpD3DDev->SetTexture(0, m_pTexBkg->Get());
	CN3Base::s_lpD3DDev->SetFVF(FVF_TRANSFORMED);
	CN3Base::s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vMoon, sizeof(__VertexTransformed));

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ZWRITEENABLE, dwZWrite);
	////////////////////////////////////////////

	m_pChr->Render();

	CGameProcedure::Render();

	s_pEng->s_lpD3DDev->EndScene();	
	s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcLogIn::Release()
{
	CGameProcedure::Release();

	delete m_pUILogIn;
	m_pUILogIn = NULL;

	delete m_pChr;
	m_pChr = NULL;

	delete m_pTexBkg;
	m_pTexBkg = NULL;

	delete m_pCamera;
	m_pCamera = NULL;

	for (int i = 0; i < 3; i++)
	{
		delete m_pLights[i];
		m_pLights[i] = NULL;
	}
}
#pragma endregion graphicsHandlers

#pragma region packets
bool CGameProcLogIn::ProcessPacket(DataPack* pDataPack, int& iOffset)
{
	int iOffsetPrev = iOffset;
	
	if (false == CGameProcedure::ProcessPacket(pDataPack, iOffset))
	{
		iOffset = iOffsetPrev;
	}

	else
		return true;
	
	int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch (iCmd)
	{
	case N3_GAMESERVER_GROUP_LIST:
		this->MsgRecv_GameServerGroupList(pDataPack, iOffset);
		return true;

	case N3_ACCOUNT_LOGIN:
		this->MsgRecv_AccountLogIn(pDataPack, iOffset);
		return true;

	case N3_NEWS:
		this->MsgRecv_GetNews(pDataPack, iOffset);
		return true;
	}

	return false;
}

#pragma region out
bool CGameProcLogIn::MsgSend_AccountLogIn(e_LogInClassification eLIC)
{
	if(LIC_KNIGHTONLINE == eLIC) 
	{
		m_pUILogIn->AccountIDGet(s_szAccount);
		m_pUILogIn->AccountPWGet(s_szPassWord);
	}

	//basic validation that the given login details are valid
	if (s_szAccount.empty() || s_szPassWord.empty() || s_szAccount.size() > 50 || s_szPassWord.size() > 150)
	{
		return false;
	}

	//hide login ui
	m_pUILogIn->showLoginUI(false);
	m_bLogIn = true;

	BYTE byBuff[256];
	int iOffset=0;

	BYTE byCmd = N3_ACCOUNT_LOGIN;

//	else if(LIC_DAUM == eLIC) byCmd = N3_ACCOUNT_LOGIN_DAUM;

	CAPISocket::MP_AddByte(byBuff, iOffset, byCmd);
	CAPISocket::MP_AddShort(byBuff, iOffset, s_szAccount.size());
	CAPISocket::MP_AddString(byBuff, iOffset, s_szAccount);
	CAPISocket::MP_AddShort(byBuff, iOffset, s_szPassWord.size());
	CAPISocket::MP_AddString(byBuff, iOffset, s_szPassWord);
		
	s_pSocket->Send(byBuff, iOffset);

	return true;
}

bool CGameProcLogIn::MsgSend_GameServerGroupList()
{
	int iOffset = 0;
	BYTE byBuffs[4];
	CAPISocket::MP_AddByte(byBuffs, iOffset, N3_GAMESERVER_GROUP_LIST);
	s_pSocket->Send(byBuffs, iOffset);

	return true;
}

bool CGameProcLogIn::MsgSend_GetNews()
{
	int iOffset = 0;
	BYTE byBuffs[4];
	CAPISocket::MP_AddByte(byBuffs, iOffset, N3_NEWS);
	s_pSocket->Send(byBuffs, iOffset);

	return true;
}
#pragma endregion out

#pragma region in
void CGameProcLogIn::MsgRecv_AccountLogIn(DataPack* pDataPack, int& iOffset)
{
	std::string szTitle;
	std::string szMsg;

	int iResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	if (LoginResultCodeEnum::AUTH_SUCCESS == iResult)
	{
		//read premium type
		PremiumEnum * premium = &PremiumEnum::forValue(CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset));
		s_pPlayer->m_InfoBase.premium = premium;

		int iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, s_pPlayer->m_InfoBase.premiumExpirationDate, iLen);

		NationEnum * nation = &NationEnum::forValue(CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset));
		s_pPlayer->m_InfoBase.eNation = nation;

		this->MessageBoxClose(-1);

		//show premium
		if (*premium != PremiumEnum::NONE)
		{
			szTitle = StringParser::parse(StringConstants::PREMIUM_TYPE, premium->getName().c_str());
			szMsg = StringParser::parse(StringConstants::EXPIRATION_DATE, s_pPlayer->m_InfoBase.premiumExpirationDate);

			this->MessageBoxPost(szMsg, szTitle, MB_OK);
		}

		this->MsgSend_GetNews();
	}

	else
	{
		szTitle = StringConstants::ERROR_TITLE;

		if (LoginResultCodeEnum::AUTH_NOT_FOUND == iResult)
		{
			szMsg = StringConstants::AUTHENTICATION_FAILED;
		}

		else if (LoginResultCodeEnum::AUTH_BANNED == iResult)
		{
			szMsg = StringConstants::BANNED_ACCOUNT;
		}

		else
		{
			szMsg = StringConstants::SERVER_ERROR;
		}

		this->MessageBoxPost(szMsg, szTitle, MB_OK);

		//show the login UI again
		m_pUILogIn->showLoginUI(true);		
		m_bLogIn = false;
	}
}

void CGameProcLogIn::MsgRecv_GameServerGroupList(DataPack* pDataPack, int& iOffset)
{
	int iServerCount = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	for (int i = 0; i < iServerCount; i++)
	{
		__GameServerInfo GSI;

		int iLen = 0;
		iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, GSI.szIP, iLen);

		iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, GSI.szName, iLen);

		GSI.iConcurrentUserCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		int iFreeLimit = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		GSI.freeLimit = iFreeLimit;

		int iPremiumLimit = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		GSI.premiumLimit = iPremiumLimit;

		m_pUILogIn->ServerInfoAdd(GSI); // ServerList
	}

	//update the ui server list & open it
	m_pUILogIn->ServerInfoUpdate(s_pPlayer->m_InfoBase.premium);
	m_pUILogIn->OpenServerList();
}

void CGameProcLogIn::MsgRecv_GetNews(DataPack* pDataPack, int& iOffset)
{
	std::string szTitle;
	std::string szMsg;

	short iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szTitle, iLen);

	iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szMsg, iLen);

	this->MessageBoxPost(szMsg, szTitle, MB_OK);

	this->MsgSend_GameServerGroupList();
}

bool CGameProcLogIn::MsgRecv_VersionCheck(DataPack* pDataPack, int& iOffset) // virtual
{
	bool version = CGameProcedure::MsgRecv_VersionCheck(pDataPack, iOffset);

	if (version)
	{
		CGameProcedure::MsgSend_GameServerLogIn();
		m_pUILogIn->ConnectButtonSetEnable(false);
	}

	return version;
}

bool CGameProcLogIn::MsgRecv_GameServerLogIn(DataPack* pDataPack, int& iOffset)
{
	int iResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	if (LoginResultCodeEnum::AUTH_SUCCESS == iResult)
	{
		s_pEng->s_SndMgr.ReleaseStreamObj(&(CGameProcedure::s_pSnd_BGM));
		CGameProcedure::s_pSnd_BGM = s_pEng->s_SndMgr.CreateStreamObj(ID_SOUND_BGM_EL_BATTLE);

		if (CGameProcedure::s_pSnd_BGM)
		{
			CGameProcedure::s_pSnd_BGM->Looping(true);
			CGameProcedure::s_pSnd_BGM->Play();
		}

		//show nation select / char select
		if (NationEnum::NO_NATION == *s_pPlayer->m_InfoBase.eNation)
		{
			CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcNationSelect);
		}

		else if (NationEnum::KARUS == *s_pPlayer->m_InfoBase.eNation || NationEnum::ELMORAD == *s_pPlayer->m_InfoBase.eNation)
		{
			CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcCharacterSelect);
		}
	}

	else
	{
		if (LoginResultCodeEnum::AUTH_IN_GAME == iResult)
		{
			std::string szMsg = StringConstants::ACCOUNT_INGAME;
			std::string szTitle = StringConstants::ERROR_TITLE;
			this->MessageBoxPost(szMsg, szTitle, MB_OK);
		}

		m_pUILogIn->ConnectButtonSetEnable(true);
	}

	return LoginResultCodeEnum::AUTH_SUCCESS == iResult;
}
#pragma endregion in

#pragma endregion packets

void CGameProcLogIn::ConnectToGameServer()
{
	__GameServerInfo GSI;

	if (false == m_pUILogIn->ServerInfoGetCur(GSI))
	{
		return;
	}

	s_bNeedReportConnectionClosed = false;
	int iErr = s_pSocket->Connect(GetActiveWindow(), GSI.szIP.c_str(), SOCKET_PORT_GAME);
	s_bNeedReportConnectionClosed = true;

	//ping to game server failed
	if (iErr)
	{
		this->ReportServerConnectionFailed(GSI.szName, iErr, false);
		m_pUILogIn->ConnectButtonSetEnable(true);
	}

	//game server is alive, check the version (this one goes to the gameserver)
	else
	{
		s_szServer = GSI.szName;
		this->MsgSend_VersionCheck();
	}
}

