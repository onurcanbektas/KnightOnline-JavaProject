// UIMessageBoxManager.cpp: implementation of the CUIMessageBoxManager class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "UIMessageBoxManager.h"
#include "GameProcedure.h"
#include "UIManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIMessageBoxManager::CUIMessageBoxManager()
{
	m_pMsgBoxLatestRef = NULL;
}

CUIMessageBoxManager::~CUIMessageBoxManager()
{
	Release();
}

int CUIMessageBoxManager::GetCount()
{
	return m_UBMs.size();
}

std::string CUIMessageBoxManager::MessageBoxPost(const std::string& szMsg, const std::string& szTitle, int iStyle, e_Behavior eBehavior)
{
	CUIMessageBox*		pMB		= NULL;
	__TABLE_UI_RESRC*	pTblUI	= NULL;

	it_UBM it, it_e;

	it		= m_UBMs.find(szMsg);
	it_e	= m_UBMs.end();

	if( it == it_e )
	{
		pMB = new CUIMessageBox();
		if( pMB == NULL )
			return szMsg;

		pTblUI = CGameBase::s_pTbl_UI->Find(NATION_ELMORAD);

		pMB->LoadFromFile(pTblUI->szMessageBox );
		pMB->SetVisible(false);
		m_UBMs.insert(val_UBM(szMsg, pMB));
	}
	else
	{
		pMB = it->second;
	}

	RECT rt = pMB->GetRegion();
	POINT pt;
	pt.x = (CN3Base::s_CameraData.vp.Width - (rt.right - rt.left)) / 2;
	pt.y = (CN3Base::s_CameraData.vp.Height - (rt.bottom - rt.top)) / 2;
	
	pMB->SetBoxStyle(iStyle);
	if(BEHAVIOR_DELETE_CHR == eBehavior) pMB->SetVisibleEditControl(true); // Edit Control Visible
	else pMB->SetVisibleEditControl(false);


	pMB->m_eBehavior = eBehavior;
	pMB->SetText(szMsg);
	pMB->SetTitle(szTitle);
	
	pMB->SetPos(pt.x, pt.y);
	pMB->SetVisible(true);

	m_pMsgBoxLatestRef = pMB; // �������� ��� ��ȭ����.. Z ������ ���ؼ� �� �����͸� �������ش�..

	it		= m_UBMs.begin();
	it_e	= m_UBMs.end();
	while(it!=it_e)
	{
		pMB = it->second;
		if( pMB && !pMB->IsVisible() )
		{
			T_Delete(pMB);
			it = m_UBMs.erase(it);
			continue;
		}
		it++;
	}

	return szMsg;
}

void CUIMessageBoxManager::MessageBoxClose(const std::string& szMsg)
{
	it_UBM it_f, it_e;
	it_f = m_UBMs.find(szMsg);
	it_e = m_UBMs.end();

	if(it_f != it_e)
	{
		if(m_pMsgBoxLatestRef == it_f->second)
			m_pMsgBoxLatestRef = NULL;

		CUIMessageBox* pMB = it_f->second;
		if(pMB) pMB->SetVisible(false);
	}
}

void CUIMessageBoxManager::Render()
{
	it_UBM it = m_UBMs.begin(), it_e = m_UBMs.end();
	for(; it != it_e; it++)
	{
		CUIMessageBox* pMB = it->second;
		if(pMB == NULL || pMB == m_pMsgBoxLatestRef) continue;
		if(!pMB->IsVisible()) continue;
		CUIManager::RenderStateSet();
		pMB->Render(); // �޽��� �ڽ� ������..
		CUIManager::RenderStateRestore();
	}

	if(m_pMsgBoxLatestRef && m_pMsgBoxLatestRef->IsVisible())
	{
		CUIManager::RenderStateSet();
		m_pMsgBoxLatestRef->Render(); // �޽��� �ڽ� ������..
		CUIManager::RenderStateRestore();
	}
}

void CUIMessageBoxManager::MessageBoxCloseAll()
{
	it_UBM it = m_UBMs.begin(), it_e = m_UBMs.end();
	for(; it != it_e; it++)
	{
		CUIMessageBox* pMB = it->second;
		if(pMB) pMB->SetVisible(false);
	}

	m_pMsgBoxLatestRef = NULL;
}

DWORD CUIMessageBoxManager::MouseProcAndTick(DWORD &dwFlags, const POINT &ptCur, const POINT &ptOld)
{
	DWORD dwRet = 0;
	CUIMessageBox *pMB;

	if(m_pMsgBoxLatestRef && m_pMsgBoxLatestRef->IsVisible())
	{
		dwRet = m_pMsgBoxLatestRef->MouseProc(dwFlags, ptCur, ptOld);
		if(dwRet != UI_MOUSEPROC_NONE) dwFlags = 0;
		if(m_pMsgBoxLatestRef) m_pMsgBoxLatestRef->Tick();
	}

	it_UBM it = m_UBMs.begin(), it_e = m_UBMs.end();
	for(; it != it_e; it++)
	{
		pMB = it->second;
		if(false == pMB->IsVisible())	continue;
		if(m_pMsgBoxLatestRef == pMB)	continue;

		dwRet = pMB->MouseProc(dwFlags, ptCur, ptOld);
		if(dwRet != UI_MOUSEPROC_NONE) dwFlags = 0;
		pMB->Tick();
	}

	return dwRet;
}

void CUIMessageBoxManager::Release()
{
	it_UBM it = m_UBMs.begin(), it_e = m_UBMs.end();
	for(; it != it_e; it++)
	{
		T_Delete(it->second);
	}
	m_UBMs.clear();

	m_pMsgBoxLatestRef = NULL;
}

CUIMessageBox* CUIMessageBoxManager::ReFocusMsgBox()
{
	for(rit_UBM itor = m_UBMs.rbegin(); m_UBMs.rend() != itor; ++itor)
	{
		CUIMessageBox* pMsgBox = itor->second;
		if(pMsgBox && pMsgBox->IsVisible())
		{
			m_pMsgBoxLatestRef = pMsgBox;
			return m_pMsgBoxLatestRef;
		}
	}

	m_pMsgBoxLatestRef = NULL;
	return NULL;
}
