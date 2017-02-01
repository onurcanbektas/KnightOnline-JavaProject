// UIPartyBBS.h: interface for the CUIPartyBBS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIPartyBBS_H__7B2732B7_C9CA_46A3_89BC_C59934ED3F13__INCLUDED_)
#define AFX_UIPartyBBS_H__7B2732B7_C9CA_46A3_89BC_C59934ED3F13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "N3UIBase.h"
#include <list>

typedef std::list<__InfoPartyBBS>::iterator it_PartyBBS;

class CUIPartyBBS : public CN3UIBase // ��Ƽ�� ���� UI, �δ�� ���� Ŭ������ ����..
{
protected:
	class CN3UIButton*		m_pBtn_PageUp;
	class CN3UIButton*		m_pBtn_PageDown;
	class CN3UIButton*		m_pBtn_Refresh;
	
	class CN3UIButton*		m_pBtn_Close;
	class CN3UIButton*		m_pBtn_Register;
	class CN3UIButton*		m_pBtn_RegisterCancel;
	class CN3UIButton*		m_pBtn_Whisper;
	class CN3UIButton*		m_pBtn_Party;

	class CN3UIString*		m_pText_Page;
	class CN3UIString*		m_pText[69];

	std::list<__InfoPartyBBS>	m_Datas; // BBS Data
	int			m_iCurPage; // ���� ������..
	int			m_iMaxPage;	// �� ������..
	int			m_iCurIndex;
	bool		m_bProcessing;
	float		m_fTime;

public:
	bool OnKeyPress(int iKey);
	void SetVisible(bool bVisible);
	void RequestParty();
	void RequestWhisper();
	void SetStringColor(int iIndex, DWORD dwColor);
	void RenderSelectContent();
	void Render();
	void SetContentString(int iIndex, std::string szID, int iLevel, std::string szClass);
	void ResetContent();
	void MsgSend_Register();
	void MsgSend_RegisterCancel();
	void MsgSend_RefreshData(int iCurPage);

	void MsgRecv_RefreshData(class DataPack* pDataPack, int& iOffset);

	void PartyStringSet(BYTE byType);
	void RefreshPage();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(class CN3UIBase* pSender, DWORD dwMsg);
	bool SelectedString(CN3UIBase* pSender, int& iID);
	
	
	CUIPartyBBS();
	virtual ~CUIPartyBBS();
};

#endif // !defined(AFX_UIPartyBBS_H__7B2732B7_C9CA_46A3_89BC_C59934ED3F13__INCLUDED_)
