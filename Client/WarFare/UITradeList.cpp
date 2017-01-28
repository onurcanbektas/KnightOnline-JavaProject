// UITradeList.cpp: implementation of the CUITradeList class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "resource.h"
#include "UITradeList.h"

#include "GameDef.h"
#include "GameProcedure.h"

#include "N3UIString.h"
#include "N3UIScrollBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUITradeList::CUITradeList()
{
	m_pScrollbar = NULL;
}

CUITradeList::~CUITradeList()
{

}

void CUITradeList::Release()
{
	CN3UIBase::Release();

	m_pScrollbar = NULL;
}

bool CUITradeList::Load(HANDLE hFile)
{
	if(CN3UIBase::Load(hFile)==false) return false;

	m_pScrollbar = (CN3UIScrollBar*)GetChildByID("scroll");//	__ASSERT(m_pScrollbar, "NULL UI Component!!");

	m_pStr_List[0] = (CN3UIString*)GetChildByID("string_list0");//	__ASSERT(m_pStr_List[0], "NULL UI Component!!");
	m_pStr_List[1] = (CN3UIString*)GetChildByID("string_list1");//	__ASSERT(m_pStr_List[1], "NULL UI Component!!");
	m_pStr_List[2] = (CN3UIString*)GetChildByID("string_list2");//	__ASSERT(m_pStr_List[2], "NULL UI Component!!");
	m_pStr_List[3] = (CN3UIString*)GetChildByID("string_list3");//	__ASSERT(m_pStr_List[3], "NULL UI Component!!");
	m_pStr_List[4] = (CN3UIString*)GetChildByID("string_list4");//	__ASSERT(m_pStr_List[4], "NULL UI Component!!");
	m_pStr_List[5] = (CN3UIString*)GetChildByID("string_list5");//	__ASSERT(m_pStr_List[5], "NULL UI Component!!");
	m_pStr_List[6] = (CN3UIString*)GetChildByID("string_list6");//	__ASSERT(m_pStr_List[6], "NULL UI Component!!");
	m_pStr_List[7] = (CN3UIString*)GetChildByID("string_list7");//	__ASSERT(m_pStr_List[7], "NULL UI Component!!");
	m_pStr_List[8] = (CN3UIString*)GetChildByID("string_list8");//	__ASSERT(m_pStr_List[8], "NULL UI Component!!");

	return true;
}

bool CUITradeList::ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)					
	{
		if(pSender->m_szID == "btn_close")
			Close();
	}

	else if (dwMsg == UIMSG_SCROLLBAR_POS)
	{
		// ��ũ�ѹٿ� �´� ä�� Line ����
		int iCurLinePos = m_pScrollbar->GetCurrentPos();
		SetTopLine(iCurLinePos);
	}

	return true;
}

void CUITradeList::Open(int iIDTarget)
{
	SetVisible(true);

	if (iIDTarget == -1) return;

	__TABLE_EXCHANGE_QUEST* pQuest = NULL;
	__TABLE_PLAYER_LOOKS* pPlayer = NULL;
	__TABLE_ITEM_BASIC* pItem = NULL;

	// ���̵� = ���� �ڵ�*1000 + 001����.. (���� �ڵ�+1)*100 + 001����..
	int i, iIDFirst, iIDIndexFirst, iIDIndexLast, iDivide, iTotalCount;
	iIDFirst = iIDTarget*1000+1;
	iIDIndexFirst = CGameBase::s_pTbl_Exchange_Quest->IDToIndex(iIDFirst);

	if ( iIDIndexFirst == -1 ) 
		return;		// �ƹ��� ����Ʈ�� ������ ���� �ʴ�..

	iTotalCount = CGameBase::s_pTbl_Exchange_Quest->GetSize();
	iIDIndexLast = 0;

	for( i = iIDIndexFirst; i < iTotalCount; i++ )
	{
		pQuest = CGameBase::s_pTbl_Exchange_Quest->GetIndexedData(i);
		if (!pQuest) 
		{
			iIDIndexLast = i;
			break;
		}

		iDivide = pQuest->dwID / 1000;
		if ( iDivide != (iIDFirst / 1000) )
		{
			iIDIndexLast = i;
			break;
		}
	}
	
	if (!iIDIndexLast) iIDIndexLast = iTotalCount;

	// �޽��� �ڽ� �ؽ�Ʈ ǥ��..
	char pszID[32];
	for( i = iIDIndexFirst; i < iIDIndexFirst + 40; i++ )
	{
		pQuest						= CGameBase::s_pTbl_Exchange_Quest->GetIndexedData(i);
		if (pQuest)
		{
			iDivide						= pQuest->dwID / 1000;
			pPlayer						= CGameBase::s_pTbl_NPC_Looks->Find(iDivide);
			m_sStr[i-iIDIndexFirst]		= pPlayer->szName;
			m_sStr[i-iIDIndexFirst]		+= "���� ";
			
			pItem						= CGameBase::s_pTbl_Items_Basic->Find(pQuest->iCondition0);
			if(pItem) m_sStr[i-iIDIndexFirst] += pItem->szName;
			else m_sStr[i-iIDIndexFirst] += "NULL Item";
			m_sStr[i-iIDIndexFirst]		+= "�� ";	
			
			pItem						= CGameBase::s_pTbl_Items_Basic->Find(pQuest->iCondition1);
			if(pItem) m_sStr[i-iIDIndexFirst] += pItem->szName;
			else m_sStr[i-iIDIndexFirst] += "NULL Item";
			m_sStr[i-iIDIndexFirst]		+= "�� ";	
			sprintf(pszID, "%d",i);
			m_sStr[i-iIDIndexFirst]		+= pszID;
			m_sStr[i-iIDIndexFirst]		+= "\n";
		}
		else
			m_sStr[i-iIDIndexFirst]		= "";
	}

	SetTopLine(m_pScrollbar->GetCurrentPos());
}

void CUITradeList::Close()
{
	SetVisible(false);
}

void CUITradeList::SetTopLine(int iTopLine)		// �� ������ �������ش�.
{
	// 0 ~ 10 ���̰�.. ���� �� ������ ����..
	int i, iTotalCount, iFirstIndex, iLastIndex;
	for(i = 0; i < 40; i++)
	{
		if (m_sStr[i].empty())
			break;
	}
	iTotalCount = i;

	int iRatio = (int)( ((float)iTopLine)*0.1f*((float)(iTotalCount-9.0f)));
	iFirstIndex = iRatio;
	iLastIndex  = iRatio + 9;
	
	if (iLastIndex > iTotalCount)
	{
		iLastIndex = iTotalCount;
		if ((iLastIndex - 9) < 0)
			iFirstIndex = 0;
		else
			iFirstIndex = iLastIndex - 9;
	}

	for (i = 0; i < 9; i++)
	{
		if(NULL == m_pStr_List[i]) continue;
		m_pStr_List[i]->SetString(m_sStr[iFirstIndex+i]);
	}

}