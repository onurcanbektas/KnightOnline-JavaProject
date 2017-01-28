// UIKnightsOperation.h: interface for the CUIKnightsOperation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIKNIGHTSOPERATION_H__1AE95FA6_E4DE_45DF_9EEA_6AEDE400ECA7__INCLUDED_)
#define AFX_UIKNIGHTSOPERATION_H__1AE95FA6_E4DE_45DF_9EEA_6AEDE400ECA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"

#include <list>
#include <string>

struct __KnightsInfoBase
{
	int iID;
	std::string szName;
};

struct __KnightsInfoExt : public __KnightsInfoBase
{
	std::string szChiefName;
	int iMemberCount;
	int iPoint;
};

typedef std::map<int, __KnightsInfoBase>::iterator it_KIB;
typedef std::map<int, __KnightsInfoBase>::value_type val_KIB;

typedef std::list<__KnightsInfoExt>::iterator it_KIE;

class CUIKnightsOperation : public CN3UIBase  
{
protected:
	int				m_iPageCur;
	std::map<int, __KnightsInfoBase>	m_KnightsMapBase;
	std::list<__KnightsInfoExt>			m_KnightsListExt;

	CN3UIButton*	m_pBtn_Up;
	CN3UIButton*	m_pBtn_Down;
	CN3UIButton*	m_pBtn_Close;

	CN3UIList*		m_pList_Knights; //
	CN3UIButton*	m_pBtn_Join;
	CN3UIButton*	m_pBtn_Create;
	CN3UIButton*	m_pBtn_Destroy;
	CN3UIButton*	m_pBtn_Withdraw;
	CN3UIEdit*		m_pEdit_KnightsName;

public:
	void	ChangeUIByDuty(enum e_KnightsDuty eDuty); // ���ѿ� ���� UI ����..
	void	Open(enum e_KnightsDuty eDuty);
	void	Close();

	__KnightsInfoBase*	KnightsInfoFind(int iID);
	void				KnightsInfoInsert(int iID, const std::string& szName);
	void				KnightsInfoDelete(int iID);

	void	KnightsListAdd(	int iID,
							const std::string& szName,
							const std::string& szChiefName,
							int iMemberCount,
							int iKnightsPoint);
	void	KnightsListUpdate();
	void	KnightsListClear();

	bool	MsgRecv_KnightsList(class DataPack* pDataPack, int& iOffset);
	void	MsgSend_KnightsCreate();
	void	MsgSend_KnightsDestroy();
	void	MsgSend_KnightsJoin();
	void	MsgSend_KnightsWithdraw(); // Ż��. 
	void	MsgSend_KnightsList(int iPage); // ������ ������ ���ܵ� ���� ��û.. 10 ���� �´�.

	void	EnableKnightsUIs(bool bEnable);
	
	virtual		bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	virtual		bool Load(HANDLE hFile);

	virtual void Release();
	CUIKnightsOperation();
	virtual ~CUIKnightsOperation();

};

#endif // !defined(AFX_UIKNIGHTSOPERATION_H__1AE95FA6_E4DE_45DF_9EEA_6AEDE400ECA7__INCLUDED_)
