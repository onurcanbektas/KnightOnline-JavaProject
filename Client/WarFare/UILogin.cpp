// UILogIn.cpp: implementation of the CUILogIn class.
//
//////////////////////////////////////////////////////////////////////

#include "UILogIn.h"
#include "GameProcLogIn.h"

#include "N3UIEdit.h"
#include "N3UIButton.h"
#include "N3UIList.h"
#include "UIMessageBoxManager.h"

#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUILogIn::CUILogIn()
{
	m_pEdit_id = NULL;
	m_pEdit_pw = NULL;
	
	m_pBtn_LogIn = NULL;
	m_pBtn_Connect = NULL;
	m_pBtn_Cancel = NULL;
	m_pBtn_Option = NULL;

	m_pGroup_ServerList = NULL;
	m_pGroup_LogIn = NULL;
	
	m_pImg_MGameLogo = NULL;
	m_pImg_DaumLogo = NULL;

	m_pList_Server = NULL;
	
	m_bOpenningNow = false;
	m_fMoveDelta = 0;

	m_bLogIn = false;
}

CUILogIn::~CUILogIn()
{

}

bool CUILogIn::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false)
	{
		return false;
	}

	m_pGroup_LogIn = GetChildByID("Group_LogIn");
	__ASSERT(m_pGroup_LogIn, "NULL UI Component!!");

	if (m_pGroup_LogIn)
	{
		//init buttons
		m_pBtn_LogIn = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("Btn_Login");
		__ASSERT(m_pBtn_LogIn, "NULL UI Component!!");

		m_pBtn_Cancel = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("Btn_Cancel");
		__ASSERT(m_pBtn_Cancel, "NULL UI Component!!");

		m_pBtn_Option = (CN3UIButton*)m_pGroup_LogIn->GetChildByID("Btn_Option");
		__ASSERT(m_pBtn_Option, "NULL UI Component!!");

		//init text boxes
		m_pEdit_id = (CN3UIEdit*)m_pGroup_LogIn->GetChildByID("Edit_ID");
		__ASSERT(m_pEdit_id, "NULL UI Component!!");

		m_pEdit_pw = (CN3UIEdit*)m_pGroup_LogIn->GetChildByID("Edit_PW");
		__ASSERT(m_pEdit_pw, "NULL UI Component!!");
	}

	m_pGroup_ServerList = GetChildByID("Group_ServerList");
	__ASSERT(m_pGroup_ServerList, "NULL UI Component!!");

	m_pImg_MGameLogo = GetChildByID("Img_MGame");
	__ASSERT(m_pImg_MGameLogo, "NULL UI Component!!");
	
	m_pImg_DaumLogo = GetChildByID("Img_Daum");
	__ASSERT(m_pImg_DaumLogo, "NULL UI Component!!");

	if (m_pImg_MGameLogo)
	{
		m_pImg_MGameLogo->SetVisible(false);
	}

	if (m_pImg_DaumLogo)
	{
		m_pImg_DaumLogo->SetVisible(false);
	}

	if (m_pGroup_ServerList)
	{
		m_pList_Server = (CN3UIList*)(m_pGroup_ServerList->GetChildByID("List_Server"));
		__ASSERT(m_pList_Server, "NULL UI Component!!");

		m_pBtn_Connect = (CN3UIButton*)m_pGroup_ServerList->GetChildByID("Btn_Connect");
		__ASSERT(m_pBtn_Connect, "NULL UI Component!!");

		m_pGroup_ServerList->SetVisible(false);
	}

	return true;
}

void CUILogIn::Tick()
{
	CN3UIBase::Tick();

	if (m_pGroup_ServerList)
	{
		//sliding animation of the server list
		if (m_bOpenningNow)
		{
			CN3UIBase* pMsgBox = CGameProcedure::s_pMsgBoxMgr->GetFocusMsgBox();

			if (pMsgBox)
			{
				if (pMsgBox->IsVisible())
				{
					return;
				}
			}

			POINT ptCur = m_pGroup_ServerList->GetPos();
			RECT rc = m_pGroup_ServerList->GetRegion();
			float fHeight = (float)(rc.bottom - rc.top);

			float fDelta = 5000.0f * CN3Base::s_fSecPerFrm;
			fDelta *= (fHeight - m_fMoveDelta) / fHeight;
			if (fDelta < 2.0f) fDelta = 2.0f;
			m_fMoveDelta += fDelta;

			int iYLimit = 0;
			ptCur.y = (int)(m_fMoveDelta - fHeight);
			
			//server list reached its final location (stop moving it)
			if (ptCur.y >= iYLimit)
			{
				ptCur.y = iYLimit;
				m_bOpenningNow = false;
			}

			m_pGroup_ServerList->SetPos(ptCur.x, ptCur.y);
		}
	}
}

bool CUILogIn::OnKeyPress(int iKey)
{
	//if the user hasn't logged in yet
	if (!m_bLogIn)
	{
		switch (iKey)
		{
		case SDL_SCANCODE_TAB://DIK_TAB:
			FocusCircular();
			return true;
		}
	}

	//if the server list is visible and done loading animation
	else if (!m_bOpenningNow && m_pGroup_ServerList && m_pGroup_ServerList->IsVisible())
	{
		switch (iKey)
		{
		case SDL_SCANCODE_UP://DIK_UP:
		{
			if (NULL == m_pList_Server)
			{
				return false;
			}

			int iIndex = m_pList_Server->GetCurSel() - 1;

			if (iIndex < 0)
			{
				iIndex = m_pList_Server->GetCount() - 1;
			}

			m_pList_Server->SetCurSel(iIndex);
			return true;
		}

		case SDL_SCANCODE_DOWN://DIK_DOWN:
		{
			if (NULL == m_pList_Server)
			{
				return false;
			}

			//go up by 1, if reached limit reset to 0
			int iIndex = (m_pList_Server->GetCurSel() + 1) % m_pList_Server->GetCount();
			
			m_pList_Server->SetCurSel(iIndex);
			return true;
		}
		
		case SDL_SCANCODE_KP_ENTER://DIK_NUMPADENTER:
		case SDL_SCANCODE_RETURN://DIK_RETURN:
			ReceiveMessage(m_pBtn_Connect, UIMSG_BUTTON_CLICK);
			return true;
		}
	}

	return CN3UIBase::OnKeyPress(iKey);
}

bool CUILogIn::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if (NULL == pSender)
	{
		return false;
	}

	//s_CameraData.vp;
	//DWORD mm = s_CameraData.vp.Height;
	//DWORD ss = s_CameraData.vp.Width;	

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == m_pBtn_LogIn && m_pEdit_id && m_pEdit_pw)
		{
			CGameProcedure::s_pProcLogIn->MsgSend_AccountLogIn(LIC_KNIGHTONLINE);
		}

		else if (pSender == m_pBtn_Connect)
		{
			CN3UIBase* pMsgBox = CGameProcedure::s_pMsgBoxMgr->GetFocusMsgBox();

			if (pMsgBox)
			{
				if (pMsgBox->IsVisible())
				{
					return true;
				}
			}

			CGameProcedure::s_pProcLogIn->ConnectToGameServer();
		}

		else if (pSender == m_pBtn_Cancel)
		{
			CGameBase::s_bRunning = false;
		}

		else if (pSender == m_pBtn_Option)
		{
			std::string szMsg;
			szMsg = StringConstants::OPEN_OPTIONS_VALIDATION;
			CGameProcedure::MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_EXECUTE_OPTION);
		}
	}

	else if (UIMSG_LIST_DBLCLK == dwMsg)
	{
		CGameProcedure::s_pProcLogIn->ConnectToGameServer();
	}

	else if (dwMsg == UIMSG_EDIT_RETURN)
	{
		// TEMP(srmeier): there is a weird issue where the key inputs aren't going
		// through CGameProcedure::ProcessUIKeyInput() so CUILogIn::OnKeyPress() isn't
		// being called...
		if (!m_bLogIn && m_pEdit_id && m_pEdit_pw)
		{
			CN3UIBase* pMsgBox = CGameProcedure::s_pMsgBoxMgr->GetFocusMsgBox();

			if (!(pMsgBox && pMsgBox->IsVisible()))
			{
				CGameProcedure::s_pProcLogIn->MsgSend_AccountLogIn(LIC_KNIGHTONLINE);
			}
		}

		else
		{
			ReceiveMessage(m_pBtn_Connect, UIMSG_BUTTON_CLICK);
		}
	}
	else if (dwMsg == UIMSG_EDIT_TAB)
	{
		// TEMP(srmeier): there is a weird issue where the key inputs aren't going
		// through CGameProcedure::ProcessUIKeyInput() so CUILogIn::OnKeyPress() isn't
		// being called...
		FocusCircular();
	}

	return true;
}

void CUILogIn::FocusToID()
{
	if (m_pEdit_id)
	{
		m_pEdit_id->SetFocus();
	}
}

void CUILogIn::FocusCircular()
{
	//make sure we have both id and password controls
	if (NULL == m_pEdit_id || NULL == m_pEdit_pw)
	{
		return;
	}

	//switch the focus between the id and pass
	if (m_pEdit_id->HaveFocus())
	{
		m_pEdit_pw->SetFocus();
	}

	else
	{
		m_pEdit_id->SetFocus();
	}
}

void CUILogIn::AccountIDGet(std::string& szID)
{
	//if control found take its value, else return empty string
	if (m_pEdit_id)
	{
		szID = m_pEdit_id->GetString();
	}

	else
	{
		szID = "";
	}
}

void CUILogIn::AccountPWGet(std::string& szPW)
{
	//if control found take its value, else return empty string
	if (m_pEdit_pw)
	{
		szPW = m_pEdit_pw->GetString();
	}

	else
	{
		szPW = "";
	}
}

void CUILogIn::OpenServerList()
{
	//already open or not exist
	if (m_bOpenningNow || NULL == m_pGroup_ServerList)
	{
		return;
	}

	m_pGroup_ServerList->SetVisible(true);

	//set base position
	RECT rc = m_pGroup_ServerList->GetRegion();
	m_pGroup_ServerList->SetPos(0, -(rc.bottom - rc.top));

	m_fMoveDelta = 0;

	//start moving (tick will handle it)
	m_bOpenningNow = true;
}

void CUILogIn::showLoginUI(bool show)
{
	if (m_pGroup_LogIn)
	{
		m_pGroup_LogIn->SetVisible(show);
	}

	m_bLogIn = !show;
}

bool CUILogIn::ServerInfoAdd(const __GameServerInfo& GSI)
{
	m_ListServerInfos.push_back(GSI);
	return true;
}

void CUILogIn::ServerInfoUpdate(PremiumEnum * premium)
{
	if (NULL == m_pList_Server)
	{
		return;
	}

	m_pList_Server->ResetContent();

	if (!m_ListServerInfos.empty())
	{
		int iSize = m_ListServerInfos.size();

		for (int i = 0; i < iSize; i++)
		{
			int limit = m_ListServerInfos[i].freeLimit;

			if (*premium != PremiumEnum::NONE)
			{
				limit = m_ListServerInfos[i].premiumLimit;
			}

			std::string server = m_ListServerInfos[i].szName + " (" + std::to_string(m_ListServerInfos[i].iConcurrentUserCount) + "/" + ::to_string(limit) + ")";
			m_pList_Server->AddString(server);
		}
	}
}

bool CUILogIn::ServerInfoGetCur(__GameServerInfo& GSI)
{
	GSI.Init();

	if (NULL == m_pList_Server)
	{
		return false;
	}

	int iIndex = m_pList_Server->GetCurSel();
	return this->ServerInfoGet(iIndex, GSI);
}

bool CUILogIn::ServerInfoGet(int iIndex, __GameServerInfo& GSI)
{
	if (NULL == m_pList_Server)
	{
		return false;
	}

	//check out of boundries
	if (iIndex < 0 || iIndex >= m_ListServerInfos.size())
	{
		return false;
	}

	GSI = m_ListServerInfos[iIndex];

	return true;
}

void CUILogIn::ConnectButtonSetEnable(bool bEnable)
{
	eUI_STATE eState1 = (bEnable ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DISABLE);
	eUI_STATE eState2 = (bEnable ? UI_STATE_LIST_ENABLE : UI_STATE_LIST_DISABLE);
	
	if (m_pBtn_Connect)
	{
		m_pBtn_Connect->SetState(eState1);
	}

	if (m_pList_Server)
	{
		m_pList_Server->SetState(eState2);
	}
}


