// UINotice.cpp: implementation of the CUINotice class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "UINotice.h"
#include "GameProcedure.h"
#include "UIManager.h"

#include "N3UIString.h"
#include "N3UIScrollBar.h"
#include "N3UIButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUINotice::CUINotice()
{
	m_pText_Notice = NULL;
	m_pScrollBar = NULL;
	m_pBtn_OK = NULL;
}

CUINotice::~CUINotice()
{
	m_Texts.clear();
}

void CUINotice::Release()
{
	m_Texts.clear();
	CN3UIBase::Release();
}

bool CUINotice::Load(HANDLE hFile)
{
	if(CN3UIBase::Load(hFile)==false) return false;

	m_pText_Notice = (CN3UIString*)GetChildByID("Text_Notice");
	m_pScrollBar = (CN3UIScrollBar*)GetChildByID("ScrollBar");
	m_pBtn_OK = (CN3UIButton*)GetChildByID("Btn_Quit"); //m_pBtn_OK = (CN3UIButton*)GetChildByID("Btn_OK");

	if(m_pScrollBar) 
	{
		m_pScrollBar->SetRange(0, 100);
	}

	return true;
}

bool CUINotice::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if(NULL == pSender) return false;

	//s_CameraData.vp;  //�ҷ� ���� ������ ���캻�� 
	//DWORD mm = s_CameraData.vp.Height;
	//DWORD ss = s_CameraData.vp.Width;	

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if(pSender == m_pBtn_OK)
		{
			if(m_pText_Notice) m_pText_Notice->SetString("");
			SetVisible(false);
		}
	}
	else if (dwMsg == UIMSG_SCROLLBAR_POS)
	{
		if(pSender == m_pScrollBar)
		{
			// ��ũ�ѹٿ� �´� ä�� Line ����
			int iCurLinePos = m_pScrollBar->GetCurrentPos();
		}
	}

	return true;
}

void CUINotice::GenerateText()
{
	if(NULL == m_pText_Notice) return;
	
	// ���ڼ��� ����..
	int iTextLen = 0;
	it_String it = m_Texts.begin(), itEnd = m_Texts.end();
	for(; it != itEnd; it++)
		iTextLen += it->size() + 3; // LineFeed, Carriage return

	if(iTextLen <= 0) return;

	std::vector<char> szBuff(iTextLen * 2, 0);

	// ���ڵ��� ���̰�  // LineFeed, Carriage return �� ���δ�.
	it = m_Texts.begin(); itEnd = m_Texts.end();
	for(; it != itEnd; it++)
	{
		lstrcat(&(szBuff[0]), it->c_str());
		lstrcat(&(szBuff[0]), "\n");
	}

	m_pText_Notice->SetString(&(szBuff[0])); // ���� ����..
}

bool CUINotice::OnKeyPress(int iKey)
{
	switch(iKey)
	{
	case SDL_SCANCODE_ESCAPE://DIK_ESCAPE:
	case SDL_SCANCODE_RETURN://DIK_RETURN:
		ReceiveMessage(m_pBtn_OK, UIMSG_BUTTON_CLICK);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CUINotice::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);
	if(bVisible)
		CGameProcedure::s_pUIMgr->SetVisibleFocusedUI(this);
	else
		CGameProcedure::s_pUIMgr->ReFocusUI();//this_ui
}

void CUINotice::RemoveNotice()
{
	m_Texts.clear();
}
