// GameProcedure.h: interface for the CGameProcedure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GameProcedure_H__0BEC53F2_1282_402C_9A28_FB98CC131F64__INCLUDED_)
#define AFX_GameProcedure_H__0BEC53F2_1282_402C_9A28_FB98CC131F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum e_Version { W95 = 1, W98, WME, WNT4, W2K };
enum e_LogInClassification { LIC_KNIGHTONLINE = 0, LIC_MGAME = 1, LIC_DAUM = 2 };

#include "GameDef.h"
#include "GameBase.h"

#include "N3WorldManager.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_net.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#define		UI_POST_WND_CHAT		"Wnd_Chat"
#define		UI_POST_WND_HOTKEY		"Wnd_Hotkey"
#define		UI_POST_WND_PARTY		"Wnd_Party"
#define		UI_POST_WND_HELP		"Wnd_Help"

const int MAX_MSG_BOX = 4;

struct __WndInfo
{
	char			szName[16];
	bool			bVisible;
	POINT			ptPosition;

	__WndInfo()	{ memset(this, 0, sizeof(__WndInfo));	}
};

class CHotkeyData 
{
public:
	int	row;
	int column;
	int iID;

	CHotkeyData() {	memset(this, 0, sizeof(CHotkeyData)); }
	CHotkeyData(int rw, int cl, int id) 
	{
		CHotkeyData();
		row = rw;	column = cl;	iID = id;
	}
};

class DataPack;

class CGameProcedure : public CGameBase
{
public:
	static class CKnightChrMgr*		s_pKnightChr;		// ����Ʈ ĳ����..		
	static class CN3SndObjStream*	s_pSnd_BGM;			// ���� ������� ������..
	static class CLocalInput*		s_pLocalInput;		// ���콺�� Ű���� �Է� ��ü .. Direct Input �� ���.
	static class CAPISocket*		s_pSocket;			// ���� ���� ��ü
	static class CAPISocket*		s_pSocketSub;		// ���� ���� ��ü..
	static class CGameEng*			s_pEng;				// 3D Wrapper Engine
	static class CN3FXMgr*			s_pFX;
	
	static class CUIManager*		s_pUIMgr;					// UI Manager
	static class CUILoading*		s_pUILoading;				// �ε���..
	static class CUIMessageBoxManager*	s_pMsgBoxMgr;			// MessageBox Manager

	static class CGameProcLogIn*			s_pProcLogIn;
	static class CGameProcNationSelect*		s_pProcNationSelect;
	static class CGameProcCharacterCreate*	s_pProcCharacterCreate;
	static class CGameProcCharacterSelect*	s_pProcCharacterSelect;
	static class CGameProcMain*				s_pProcMain;
	static class CGameProcOption*			s_pProcOption;

	static class CGameProcedure*			s_pProcPrev;
	static class CGameProcedure*			s_pProcActive;

	static class CGameCursor*				s_pGameCursor;

	static SDL_Cursor* s_hCursorNormal;
	static SDL_Cursor* s_hCursorNormal1;
	static SDL_Cursor* s_hCursorClick;
	static SDL_Cursor* s_hCursorClick1;
	static SDL_Cursor* s_hCursorAttack;
	static SDL_Cursor* s_hCursorPreRepair;
	static SDL_Cursor* s_hCursorNowRepair;

	static e_Version				s_eVersion;
	static e_LogInClassification	s_eLogInClassification; // ������ ����.. MGame, Daum, KnightOnLine ....
	static std::string				s_szAccount; // ���� ���ڿ�..
	static std::string				s_szPassWord; // ���� ���..
	static std::string				s_szServer; // �����̸� ���ڿ�..
	static int						s_iChrSelectIndex; // �̰����� ���° ĳ������...??

	static bool		m_bCursorLocked;
	static SDL_Cursor*	m_hPrevGameCursor;
	static HWND		s_hWndSubSocket; // ���� ���Ͽ� ������ �ڵ�..

	static bool		s_bNeedReportConnectionClosed; // ���������� �������� �����ؾ� �ϴ���..
	static bool		s_bWindowed; // â��� ����??
	static bool		s_bKeyPress;	//Ű�� ���������� ui���� �ش��ϴ� ���۵����� �ִٸ�
	static bool		s_bKeyPressed;	//Ű�� �ö����� ui���� �ش��ϴ� ���۵����� �ִٸ�

	// NOTE: adding boolean to check if window has focus or not
	static bool		s_bIsWindowInFocus;

public:
	static std::string MessageBoxPost(const std::string& szMsg, const std::string& szTitle, int iStyle, e_Behavior eBehavior = BEHAVIOR_NOTHING);
	static void MessageBoxClose(const std::string& szMsg);
	static void MessageBoxClose(int iMsgBoxIndex);

	static void ProcActiveSet(CGameProcedure* pProc);
	static void ReConnect();
	
	static void ReportServerConnectionFailed(const std::string& szServerName, int iErrCode, bool bNeedQuitGame);
	static void ReportServerConnectionClosed(bool bNeedQuitGame);
	static void ReportDebugStringAndSendToServer(const std::string& szDebug);

	virtual bool	MsgRecv_VersionCheck(DataPack* pDataPack, int& iOffset); // ��ȣȭ Ű�� ���� �޴´�..
	virtual bool	MsgRecv_GameServerLogIn(DataPack* pDataPack, int& iOffset); // virtual - ������ȣ�� �����Ѵ�.
	virtual bool	MsgRecv_CharacterSelect(DataPack* pDataPack, int& iOffset);

	static void		MsgSend_GameServerLogIn();
	void			MsgSend_VersionCheck();
	virtual void	MsgSend_CharacterSelect();
	void			MsgSend_AliveCheck();
	void			MsgRecv_CompressedPacket(DataPack* pDataPack, int& iOffset);


	
	virtual void Release(); // ���ҽ� Ǯ���ֱ�..
	virtual void Init(); // �ʿ��� ��ҵ��� �ʱ�ȭ �� �ε�
	virtual void Tick();  // ���ν��� �ε����� �����Ѵ�. 0 �̸� �״�� ����
	virtual void Render();
	virtual void ProcessUIKeyInput(bool bEnable = true);

	static bool	IsUIKeyOperated();
	static void	StaticMemberInit(SDL_Window* pWindow);
	static void StaticMemberRelease();

	static void TickActive();
	static void RenderActive();

	static bool RegPutSetting( const char *ValueName, void *pValueData, long length );
	static bool RegGetSetting( const char *ValueName, void *pValueData, long length );

	static void	UIPostData_Read(const std::string& szKey, class CN3UIBase* pUI, int iDefaultX, int iDefaultY);
	static void	UIPostData_Write(const std::string& szKey, CN3UIBase* pUI);

	static bool CaptureScreenAndSaveToFile(const std::string& szFN);

	static void	SetGameCursor(SDL_Cursor* hCursor, bool bLocked = false);
	static void	SetGameCursor(e_Cursor eCursor, bool bLocked = false);
	static void RestoreGameCursor();

protected:
	virtual bool ProcessPacket(DataPack* pDataPack, int& iOffset);

private:
	static std::string GetStrRegKeySetting();

public:
	void LoadingUIChange(int iVictoryNation);
	CGameProcedure();
	virtual ~CGameProcedure();
};


#endif // !defined(AFX_GameProcedure_H__0BEC53F2_1282_402C_9A28_FB98CC131F64__INCLUDED_)
