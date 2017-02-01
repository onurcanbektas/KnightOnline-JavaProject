// UIMessageWnd.h: interface for the CUIMessageWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIMESSAGEWND_H__F825624B_A51E_4889_9ADC_BEBF022D010B__INCLUDED_)
#define AFX_UIMESSAGEWND_H__F825624B_A51E_4889_9ADC_BEBF022D010B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "GameDef.h"
#include "UIChat.h"

class CUIMessageWnd : public CN3UIBase  
{
protected:
	CN3UIString*	m_pChatOut;		// ä���� ��µǴ� UIString ����������(���� m_Child�� ����), �۾�ü�� �ʱ� ������ �����Ѵ�.
	CN3UIScrollBar* m_pScrollbar;	// scrollbar ����������(���� m_Child�� ����)

	ChatList		m_ChatBuffer;		// ä�� packet�������� �� buffer
	ChatList		m_LineBuffer;		// Line �������� �� buffer
	
	int				m_iChatLineCount;	// ä��â�� ��µǴ� line�� ��(ä��â ����� �������� �ٽ� ���������.)
	RECT			m_rcChatOutRegion;	// ä���� ��µǴ� ����
	CN3UIString**	m_ppUILines;		// ä���� ��µǴ� UIString �迭������(ä��â ����� ���ϹǷ� �迭�� ���Ѵ�.

protected:
	void			SetTopLine(int iTopLine);		// �� ������ �������ش�.
	void			AddLineBuffer(const std::string& szString, D3DCOLOR color);	// line ���۸� ������ش�.(�ʹ� ��� �˾Ƽ� 2�ٷ� ������ش�.)
	void			RecalcLineBuffer();		// ä��â ����� �������� ȣ�����ָ� line buffer�� �ٽ� ����ؼ� �־��ش�.
	void			CreateLines();

// Operations
public:
	bool OnKeyPress(int iKey);
	BOOL	MoveOffset(int iOffsetX, int iOffsetY);	// offsetY��ŭ �̵����ش�.(region, children, move rect �̵�)
	bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	void	Release();
	bool	Load(HANDLE hFile);
	void	SetRegion(const RECT& Rect);	// ���� ����(����� ���Ҷ� ȣ��ȴ�. �ܼ� �̵��� ȣ����� �ʴ´�.(�ܼ��̵��� MoveOffset�� ȣ��))

	void	AddMsg(const std::string& szString, D3DCOLOR color = 0xffffffff);		// ä�� �޼����� �����ϰ� �˸��� ���·� ȭ�鿡 ������ش�.

	CUIMessageWnd();
	virtual ~CUIMessageWnd();
};

#endif // !defined(AFX_UIMESSAGEWND_H__F825624B_A51E_4889_9ADC_BEBF022D010B__INCLUDED_)
