// UILogIn.h: interface for the CUILogIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UILogIn_H__5D51312E_7B98_43C2_871B_12A9CC0C2096__INCLUDED_)
#define AFX_UILogIn_H__5D51312E_7B98_43C2_871B_12A9CC0C2096__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "N3UIBase.h"
#include "PremiumEnum.h"

struct __GameServerInfo : public binary_function<__GameServerInfo, __GameServerInfo, bool>
{
	std::string szName;
	std::string szIP;
	int	iConcurrentUserCount;
	int freeLimit;
	int premiumLimit;

	void Init()
	{
		szName = "";
		szIP = "";
		iConcurrentUserCount = 0;
		freeLimit = 0;
		premiumLimit = 0; 
	}

	bool operator () (const __GameServerInfo& x, const __GameServerInfo& y) const 
	{
		return (x.iConcurrentUserCount >= y.iConcurrentUserCount);
	}

	__GameServerInfo()
	{
		this->Init(); 
	}
};

class CUILogIn : public CN3UIBase  
{
protected:
	CN3UIEdit*	m_pEdit_id;
	CN3UIEdit*	m_pEdit_pw;
	
	CN3UIButton* m_pBtn_LogIn;
	CN3UIButton* m_pBtn_Connect;
	CN3UIButton* m_pBtn_Cancel;
	CN3UIButton* m_pBtn_Option;

	CN3UIBase*	m_pGroup_ServerList;
	CN3UIBase*	m_pGroup_LogIn;

	CN3UIBase* m_pImg_MGameLogo;
	CN3UIBase* m_pImg_DaumLogo;

	CN3UIList*	m_pList_Server;

	std::vector<__GameServerInfo> m_ListServerInfos;

	bool	m_bOpenningNow;
	float 	m_fMoveDelta;
	bool	m_bLogIn;

public:
	CUILogIn();
	virtual ~CUILogIn();

	bool Load(HANDLE hFile);

	void Tick();

	bool OnKeyPress(int iKey);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	void FocusToID();
	void FocusCircular();
	
	void AccountIDGet(std::string& szID);
	void AccountPWGet(std::string& szPW);

	void OpenServerList();
	void showLoginUI(bool show);

	bool ServerInfoAdd(const __GameServerInfo& GSI);
	void ServerInfoUpdate(PremiumEnum * premium);
	bool ServerInfoGetCur(__GameServerInfo& GSI);
	bool ServerInfoGet(int iIndex, __GameServerInfo& GSI);

	void ConnectButtonSetEnable(bool bEnable);
};

#endif // !defined(AFX_UILogIn_H__5D51312E_7B98_43C2_871B_12A9CC0C2096__INCLUDED_)
