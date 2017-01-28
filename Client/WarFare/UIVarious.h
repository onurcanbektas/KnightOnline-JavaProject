// UIVarious.h: interface for the CUIState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIVarious_H__094F9A3E_1620_4E07_8726_82881B21B48C__INCLUDED_)
#define AFX_UIVarious_H__094F9A3E_1620_4E07_8726_82881B21B48C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"
#include "N3UIBase.h"
#include <string>
#include <list>

class CUIState : public CN3UIBase  
{
public:
	CN3UIString*	m_pText_ID;
	CN3UIString*	m_pText_Level;
	CN3UIString*	m_pText_RealmPoint;

	CN3UIString*	m_pText_Class;
	CN3UIString*	m_pText_Race;
	CN3UIString*	m_pText_Nation;

	CN3UIString*	m_pText_HP;
	CN3UIString*	m_pText_MP;
	CN3UIString*	m_pText_Exp;
	CN3UIString*	m_pText_AP;			// ���ݷ�
	CN3UIString*	m_pText_GP;			// ����
	CN3UIString*	m_pText_Weight;

	CN3UIString*	m_pText_BonusPoint;

	CN3UIButton*	m_pBtn_Strength;
	CN3UIButton*	m_pBtn_Stamina;
	CN3UIButton*	m_pBtn_Dexterity;
	CN3UIButton*	m_pBtn_MagicAttak;
	CN3UIButton*	m_pBtn_Intelligence;

	CN3UIString*	m_pText_Strength;
	CN3UIString*	m_pText_Stamina;
	CN3UIString*	m_pText_Dexterity;
	CN3UIString*	m_pText_MagicAttak;
	CN3UIString*	m_pText_Intelligence;

	CN3UIString*	m_pText_RegistFire;
	CN3UIString*	m_pText_RegistMagic;
	CN3UIString*	m_pText_RegistIce;
	CN3UIString*	m_pText_RegistCurse;
	CN3UIString*	m_pText_RegistLight;
	CN3UIString*	m_pText_RegistPoison;

	CN3UIBase*		m_pImg_MAP;
	CN3UIBase*		m_pImg_Sta;
	CN3UIBase*		m_pImg_Int;
	CN3UIBase*		m_pImg_Dex;
	CN3UIBase*		m_pImg_Str;

public:
	void	UpdateBonusPointAndButtons(int iBonusPointRemain); // ���ʽ� ����Ʈ ������ �����Ѱ�??

	void	UpdateID(const std::string& szID);
	void	UpdateLevel(int iVal);
	void	UpdateRealmPoint(int iVal); // ���� �⿩���� 10�� ����� ǥ��

	void	UpdateHP(int iVal, int iValMax);
	void	UpdateMSP(int iVal, int iValMax);
	void	UpdateExp(int iVal, int iValMax);
	void	UpdateAttackPoint(int iVal, int iDelta);
	void	UpdateGuardPoint(int iVal, int iDelta);
	void	UpdateWeight(int iVal, int iValMax);
	void	UpdateStrength(int iVal, int iDelta);
	void	UpdateStamina(int iVal, int iDelta);
	void	UpdateDexterity(int iVal, int iDelta);
	void	UpdateIntelligence(int iVal, int iDelta);
	void	UpdateMagicAttak(int iVal, int iDelta);

	void	UpdateRegistFire(int iVal, int iDelta);
	void	UpdateRegistCold(int iVal, int iDelta);
	void	UpdateRegistLight(int iVal, int iDelta);
	void	UpdateRegistMagic(int iVal, int iDelta);
	void	UpdateRegistCurse(int iVal, int iDelta);
	void	UpdateRegistPoison(int iVal, int iDelta);

	void			MsgSendAblityPointChange(BYTE byType, short siValueDelta); // �ɷ�ġ ��ȭ ��Ŷ���� ������..

	virtual bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	virtual bool	Load(HANDLE hFile);

	virtual void	Release();
	CUIState();
	virtual ~CUIState();
};


struct __KnightsMemberInfo // ��� �ܿ� ����..
{
	std::string		szName;
	int				iLevel; // �÷��̾� ����..
	e_Class			eClass; // ����.
	e_KnightsDuty	eDuty;	// ���� ����.
	int				iConnected;
};

typedef std::list<__KnightsMemberInfo>::iterator it_KMI;
#define MAX_CLAN_GRADE	5

class CUIKnights : public CN3UIBase  
{
protected:
	int								m_iPageCur;
	std::list<__KnightsMemberInfo>	m_MemberList;

	CN3UIString*	m_pText_Name;
	CN3UIString*	m_pText_Duty;
	CN3UIString*	m_pText_Page;
	CN3UIString*	m_pText_MemberCount;
	CN3UIString*	m_pText_Grade;
	CN3UIString*	m_pText_Rank; 

	CN3UIImage*		m_pImage_Grade[MAX_CLAN_GRADE];
	CN3UIList*		m_pList_Members;

	CN3UIButton*	m_pBtn_Admit;
	CN3UIButton*	m_pBtn_Appoint;
	CN3UIButton*	m_pBtn_Remove;
	CN3UIButton*	m_pBtn_Refresh;

public:	
	float			m_fTimeLimit_Refresh;
	float			m_fTimeLimit_Appoint;
	float			m_fTimeLimit_Remove;
	float			m_fTimeLimit_Admit;
	
public:
	void	Clear();
	void	UpdateKnightsName(const std::string& szName);
	void	UpdateKnightsDuty(e_KnightsDuty eDuty);
	void	UpdateKnightsGrade(int iVal);
	void	UpdateKnightsRank(int iVal);

	void	MemberListClear();
	void	MemberListAdd(const std::string& szName, e_KnightsDuty eDuty, e_Class eClass, int iLevel, int connected);
	void	MemberListUpdate();
	void	MemberListSort();

	void	RefreshPage(); 

	void	MsgSend_MemberInfoAll();

	void	SetVisible(bool bVisible);		
	virtual bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	virtual bool Load(HANDLE hFile);

	virtual void Release();
	CUIKnights();
	virtual ~CUIKnights();

	//���� ����....
	bool	NeedMemberListRequest() { if(m_MemberList.empty()) return true; else return false; }
	
	void	ChangeUIByDuty(e_KnightsDuty eDuty);
	//void	VisibleAppointButtons(bool bVisible); // ������ ���� �Ӹ� Interface
	
	//void	MsgSend_MemberInfoOnline(int iPage);
	bool	MsgRecv_MemberInfo(class DataPack* pDataPack, int& iOffset);
	
	//void	MsgSend_MemberJoinAdmit();
	//void	MsgSend_MemberJoinReject();
	//void	MsgSend_MemberPunish();
	//void	MsgSend_MemberRemove();
	//void	MsgSend_DutyAppoint(e_KnightsDuty eDuty);
};

struct __FriendsInfo : public binary_function<__FriendsInfo, __FriendsInfo, bool>// ��� �ܿ� ����..
{
	std::string		szName;
	int				iID; // ID
	bool			bOnLine; // �����߳�?
	bool			bIsParty; // ��Ƽ �÷������ΰ�?

	void Init() { szName = ""; iID = -1; bOnLine = false; bIsParty = false; }
	bool operator () (const __FriendsInfo& x, const __FriendsInfo& y) const 
	{
		return (x.szName >= y.szName);
	}

	__FriendsInfo() { this->Init(); }
};

typedef std::map<std::string, __FriendsInfo>::iterator it_FI;
typedef std::map<std::string, __FriendsInfo>::value_type val_FI;

class CUIFriends : public CN3UIBase  
{
protected:
	int m_iPageCur;
	std::map<std::string, __FriendsInfo> m_MapFriends;

	CN3UIList* m_pList_Friends;
	CN3UIString* m_pText_Page;

	CN3UIButton* m_pBtn_NextPage;
	CN3UIButton* m_pBtn_PrevPage;
	
	CN3UIButton* m_pBtn_Refresh;
	CN3UIButton* m_pBtn_Party;
	CN3UIButton* m_pBtn_Whisper;
	CN3UIButton* m_pBtn_Add;
	CN3UIButton* m_pBtn_Delete;

public:
	void		SaveListToTextFile(const std::string& szID);  // ���ڿ��� ������ �߰��ϰ�.. ������ ���� ����..
	void		MsgSend_MemberInfo(bool bDisableInterval); // ���� ������ ���� ��û
	void		MsgSend_MemberInfo(const std::string& szID); // �̳Ѹ� ��û..
	void		MsgRecv_MemberInfo(DataPack* pDataPack, int& iOffset);
	void		UpdateList();
	bool		MemberDelete(const std::string& szID);
	bool		MemberAdd(const std::string& szID, int iID, bool bOnLine, bool bIsParty);

	virtual bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	virtual bool Load(HANDLE hFile);
	
	CUIFriends();
	virtual ~CUIFriends();

};



class CUIQuest : public CN3UIBase
{
protected:


public:
	virtual bool Load(HANDLE hFile);

	CUIQuest();
	virtual ~CUIQuest();
};



class CUIVarious : public CN3UIBase // �ٿ뵵 UI
{
public:
	CUIState*		m_pPageState;
	CUIKnights*		m_pPageKnights;
	CUIQuest*		m_pPageQuest;
	CUIFriends*		m_pPageFriends;
	
protected:
	CN3UIButton*	m_pBtn_Knights;
	CN3UIButton*	m_pBtn_State;
	CN3UIButton*	m_pBtn_Quest;
	CN3UIButton*	m_pBtn_Friends;
	CN3UIButton*	m_pBtn_Close;

	bool		m_bOpenningNow; // ������ �ִ�..
	bool		m_bClosingNow;	// ������ �ִ�..
	float		m_fMoveDelta; // �ε巴�� ������ ������ ����� ���ؼ� ������ġ ��꿡 �ε��Ҽ����� ����..

public:
	void		SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
	void		SetVisible(bool bVisible);
	bool		OnKeyPress(int iKey);
	void		UpdatePageButtons(CN3UIButton* pButtonToActive);
	void		UpdateAllStates(const __InfoPlayerBase* pInfoBase, const __InfoPlayerMySelf* pInfoExt);
	void		UpdateKnightsInfo(); // ���� ���� ���� ������Ʈ
	void		Open();
	void		Close();
	
	virtual void	Tick();
	virtual	bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	virtual bool	Load(HANDLE hFile);

	virtual void Release();
	CUIVarious();
	virtual ~CUIVarious();
};

#endif // !defined(AFX_UIVarious_H__094F9A3E_1620_4E07_8726_82881B21B48C__INCLUDED_)
