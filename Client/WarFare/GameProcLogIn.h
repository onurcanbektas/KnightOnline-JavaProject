// GameProcLogIn.h: interface for the CGameProcLogIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROCLOGIN_H__C245C1E3_23FD_425C_888A_A7B412456AC3__INCLUDED_)
#define AFX_GAMEPROCLOGIN_H__C245C1E3_23FD_425C_888A_A7B412456AC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameProcedure.h"
#include "LoginResultCodes.h";

class CGameProcLogIn : public CGameProcedure
{
protected:
	bool				m_bLogIn;
	class CN3Chr *		m_pChr;
	class CN3Texture *	m_pTexBkg;
	class CUILogIn *	m_pUILogIn;

	class CN3Camera *	m_pCamera;
	class CN3Light *	m_pLights[3];

	virtual bool ProcessPacket(DataPack* pDataPack, int& iOffset);
public:
	CGameProcLogIn();
	virtual	~CGameProcLogIn();

	void				Init();
	void				Tick();
	void				Render();
	void				Release();

	void				ConnectToGameServer();

	void				MsgRecv_GameServerGroupList(DataPack* pDataPack, int& iOffset);
	void				MsgRecv_GetNews(DataPack* pDataPack, int& iOffset);
	void				MsgRecv_AccountLogIn(DataPack* pDataPack, int& iOffset);
	bool				MsgRecv_VersionCheck(DataPack* pDataPack, int& iOffset); // virtual
	bool				MsgRecv_GameServerLogIn(DataPack* pDataPack, int& iOffset);

	bool				MsgSend_AccountLogIn(enum e_LogInClassification eLIC);
	bool				MsgSend_GameServerGroupList();
	bool				MsgSend_GetNews();
};

#endif // !defined(AFX_GAMEPROCLOGIN_H__C245C1E3_23FD_425C_888A_A7B412456AC3__INCLUDED_)