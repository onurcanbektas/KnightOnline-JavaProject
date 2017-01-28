// UIChat.h: interface for the CUIChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UICHAT_H__2CFECA0D_EA38_4900_86BB_BAFD4D5EE6F7__INCLUDED_)
#define AFX_UICHAT_H__2CFECA0D_EA38_4900_86BB_BAFD4D5EE6F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include <deque>

//enum e_ChatBuffer { CHAT_BUFFER_NORMAL = 0, CHAT_BUFFER_PRIVATE, CHAT_BUFFER_PARTY, CHAT_BUFFER_KNIGHTS, CHAT_BUFFER_COUNT };

struct __ChatInfo
{
	std::string szChat; // ���ڿ�
	D3DCOLOR	color; // ���ڿ� ����

	__ChatInfo() {};
	__ChatInfo(const std::string& szChat_Arg, D3DCOLOR color_Arg) { szChat = szChat_Arg; color = color_Arg; }
};

typedef deque<__ChatInfo*>			ChatList;
typedef ChatList::iterator			ChatListItor;
typedef ChatList::reverse_iterator	ChatListReverseItor;
const int MAX_CHAT_LINES = 100;

class CUIChat : public CN3UIBase  
{
protected:
//	ChatList		m_ChatBuffers[CHAT_BUFFER_COUNT];		// ä�� packet�������� �� buffer
//	ChatList		m_LineBuffers[CHAT_BUFFER_COUNT];		// Line �������� �� buffer
	ChatList		m_ChatBuffer;		// ä�� packet�������� �� buffer
	ChatList		m_LineBuffer;		// Line �������� �� buffer
	ChatList		m_ContinueMsg;		// ���������� �������ִ� �޽��� buffer
	int				m_iCurContinueMsg;

	CN3UIString*	m_pNoticeTitle;		// ä��â �����ٿ� ǥ�õ� ����...
	CN3UIString*	m_pChatOut;			// ä���� ��µǴ� UIString ����������(���� m_Child�� ����), �۾�ü�� �ʱ� ������ �����Ѵ�.
	CN3UIScrollBar* m_pScrollbar;		// scrollbar ����������(���� m_Child�� ����)
	int				m_iChatLineCount;	// ä��â�� ��µǴ� line�� ��(ä��â ����� �������� �ٽ� ���������.)
	RECT			m_rcChatOutRegion;	// ä���� ��µǴ� ����
	CN3UIString**	m_ppUILines;		// ä���� ��µǴ� UIString �迭������(ä��â ����� ���ϹǷ� �迭�� ���Ѵ�.

	CN3UIEdit*		m_pEdit;			//son, chat_in
	std::string		m_szString;			//son, chat_in

	CN3UIBase*		m_pBtn_Normal;
	CN3UIBase*		m_pBtn_Private;
	CN3UIBase*		m_pBtn_PartyOrForce;
	CN3UIBase*		m_pBtn_KnightsOrGuild;
	CN3UIBase*		m_pBtn_Shout;
	CN3UIBase*		m_pBtn_Check;

	enum e_ChatMode	m_eChatMode;

	bool			m_bChatNormal;
	bool			m_bChatPrivate;
	bool			m_bChatClan;
	bool			m_bChatShout;
	bool			m_bChatParty;

	bool			m_bKillFocus;
//	e_ChatBuffer	m_eChatBuffer; // ä�� ǥ�� ��� .. ���۰� ���������ִ�..
	
/*
	ChatList		m_MsgBuffer;		// ä�� packet�������� �� buffer
	ChatList		m_MsgLineBuffer;	// Line �������� �� buffer

	CN3UIString*	m_pMsgOut;		// ä���� ��µǴ� UIString ����������(���� m_Child�� ����)
	CN3UIScrollBar* m_pMsgScrollbar;	// scrollbar ����������(���� m_Child�� ����)
	int				m_iMsgLineCount;	// ä��â�� ��µǴ� line�� ��(ä��â ����� �������� �ٽ� ���������.)
*/

protected:
	void			SetTopLine(int iTopLine);		// �� ������ �������ش�.
//	void			AddLineBuffer(e_ChatBuffer eCB, const std::string& szString, D3DCOLOR color);	// line ���۸� ������ش�.(�ʹ� ��� �˾Ƽ� 2�ٷ� ������ش�.)
	void			AddLineBuffer(const std::string& szString, D3DCOLOR color);	// line ���۸� ������ش�.(�ʹ� ��� �˾Ƽ� 2�ٷ� ������ش�.)
	void			RecalcLineBuffers();		// ä��â ����� �������� ȣ�����ָ� line buffer�� �ٽ� ����ؼ� �־��ش�.
	void			CreateLines();

// Operations
public:
	void			SetNoticeTitle(const std::string& szString, D3DCOLOR color);
	void			ShowContinueMsg();
	void			DeleteContinueMsg();
	bool			OnKeyPress(int iKey);
	bool			GetEnableKillFocus() { return m_bKillFocus; }
	void			SetEnableKillFocus(bool bKillFocus) { m_bKillFocus = bKillFocus; }
	void			ChatListenEnable();
	void			ChangeChattingMode(e_ChatMode eCM);
	virtual BOOL	MoveOffset(int iOffsetX, int iOffsetY);	// Offset��ŭ �̵����ش�.(region, children, move rect �̵�)
	virtual bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	virtual void	Release();
	virtual bool	Load(HANDLE hFile);
	virtual void	SetRegion(const RECT& Rect);	// ���� ����(����� ���Ҷ� ȣ��ȴ�. �ܼ� �̵��� ȣ����� �ʴ´�.(�ܼ��̵��� MoveOffset�� ȣ��))

	void				SetString(const std::string& szChat);
	void				SetCaretPos(int iPos);
	const std::string&	GetString() const {return m_szString;}				//son, chat_in
	void				AddChatMsg(e_ChatMode eCM, const std::string& szString, D3DCOLOR color = 0xffffffff);		// ä�� �޼����� �����ϰ� �˸��� ���·� ȭ�鿡 ������ش�.
	void				AddContinueMsg(e_ChatMode eCM, const std::string& szString, D3DCOLOR color = 0xffffffff);
	void				AdjustScroll(); // ��ũ�� ��ġ�� ����..

	BOOL			IsChatMode();
	void			SetFocus();
	void			KillFocus();

	CUIChat();
	virtual ~CUIChat();
};

#endif // !defined(AFX_UICHAT_H__2CFECA0D_EA38_4900_86BB_BAFD4D5EE6F7__INCLUDED_)


