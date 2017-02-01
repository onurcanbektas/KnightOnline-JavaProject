// UIManager.cpp: implementation of the CUIManager class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "LocalInput.h"
#include "N3UIWndBase.h"
#include "UIManager.h"
#include "GameProcedure.h"
#include "GameProcMain.h"
#include "UIInventory.h"
#include "UITransactionDlg.h"
#include "SubProcPerTrade.h"
#include "N3UIWndBase.h"
#include "CountableItemEditDlg.h"
#include "UIItemExchange.h"
#include "UIWareHouseDlg.h"

#include "N3UITooltip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUIManager::__RenderStateForUI CUIManager::s_sRSFU; // RenderStateForUI

CUIManager::CUIManager()
{
	m_dwMouseFlagsCur = 0;
	m_bEnableOperation = true;					// UI ������ ������ �����ΰ�?
	m_pUIFocused = NULL;

	m_bDoneSomething = false;					// UI ���� ������ �ߴ�...
}

CUIManager::~CUIManager()
{
}

void CUIManager::Release()
{
	CN3UIBase::Release();
}

DWORD CUIManager::MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	m_dwMouseFlagsCur = UI_MOUSEPROC_NONE;
	if (!m_bVisible || !m_bEnableOperation) return m_dwMouseFlagsCur;

	if (s_pTooltipCtrl)	s_pTooltipCtrl->MouseProc(dwFlags, ptCur, ptOld);	// �������� ���콺 �޼��� ����.

	// child���� �޼��� ����
	for(UIListItor itor = m_Children.begin(); m_Children.end() != itor; )
	{
		CN3UIBase* pChild = (*itor);
		// ��ŷ� ���̸� ������ �Ŵ��� �����츸 �۵�..
		if ( CGameProcedure::s_pProcMain && CGameProcedure::s_pProcMain->m_pUITransactionDlg && 
			(CGameProcedure::s_pProcMain->m_pUITransactionDlg->IsVisible()))// && (pChild->UIType() != UI_TYPE_ICON_MANAGER) )
		{	
			if ( CN3UIWndBase::m_pCountableItemEdit->IsLocked() )
			{
				if ( pChild->m_szID.compare("base_tradeedit") != 0 )
					{	++itor; continue;	}
			}
		}
		// �����Կ� �������̸� ������ �Ŵ��� �����츸 �۵�..
		if ( CGameProcedure::s_pProcMain && CGameProcedure::s_pProcMain->m_pUIWareHouseDlg && 
			(CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->IsVisible()))// && (pChild->UIType() != UI_TYPE_ICON_MANAGER) )
		{	
			if ( CN3UIWndBase::m_pCountableItemEdit->IsLocked() )
			{
				if ( pChild->m_szID.compare("base_tradeedit") != 0 )
					{	++itor; continue;	}
			}
		}
		// ���ΰ� �ŷ����̸� ������ �Ŵ��� �����츸 �۵�.. �� �׼��� ȭ�� �������� �Է��ϴ� ���̸�.. �Է� �����츸 �۵�..
		if ( CGameProcedure::s_pProcMain && CGameProcedure::s_pProcMain->m_pSubProcPerTrade &&
			(CGameProcedure::s_pProcMain->m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE) )
		{	
			if (CGameProcedure::s_pProcMain->m_pSubProcPerTrade->m_ePerTradeState == PER_TRARE_STATE_EDITTING) 
			{
				if ( pChild->m_szID.compare("base_tradeedit") != 0 )
					{	++itor; continue;	}
			}

			if ( CN3UIWndBase::m_pCountableItemEdit->IsLocked() )
			{
				if ( pChild->m_szID.compare("base_tradeedit") != 0 )
					{	++itor; continue;	}
			}
		}

		if(pChild->m_pChildUI && pChild->m_pChildUI->IsVisible())
		{
			DWORD dwRet = pChild->m_pChildUI->MouseProc(dwFlags, ptCur, ptOld);
			if (UI_MOUSEPROC_DONESOMETHING & dwRet)
			{	// �̰�쿡�� �հ� ��Ŀ���� ���� ����̴�.
				pChild->MouseProc(0, ptCur, ptOld);
				m_dwMouseFlagsCur |= (UI_MOUSEPROC_DONESOMETHING|UI_MOUSEPROC_CHILDDONESOMETHING);

				SetFocusedUI(pChild);//this_ui

				return m_dwMouseFlagsCur;
			}
			else if ( (	UI_MOUSE_LBCLICK & dwFlags) && (UI_MOUSEPROC_INREGION & dwRet) )
			{	// ���� ���� Ŭ�� ������ �հ� ���� �ߴٰ� �ϰ� �����ع�����.
				pChild->MouseProc(0, ptCur, ptOld);
				m_dwMouseFlagsCur |= (UI_MOUSEPROC_DIALOGFOCUS);
				
				SetFocusedUI(pChild);//this_ui

				return m_dwMouseFlagsCur;
			}
		}

		DWORD dwChildRet = pChild->MouseProc(dwFlags, ptCur, ptOld);
		if (UI_MOUSEPROC_DONESOMETHING & dwChildRet)
		{	// �̰�쿡�� �հ� ��Ŀ���� ���� ����̴�.
			m_dwMouseFlagsCur |= (UI_MOUSEPROC_DONESOMETHING|UI_MOUSEPROC_CHILDDONESOMETHING);

			SetFocusedUI(pChild);//this_ui

			return m_dwMouseFlagsCur;
		}
		else if ( (	UI_MOUSE_LBCLICK & dwFlags) && (UI_MOUSEPROC_INREGION & dwChildRet) )
		{	// ���� ���� Ŭ�� ������ �հ� ���� �ߴٰ� �ϰ� �����ع�����.
			m_dwMouseFlagsCur |= (UI_MOUSEPROC_DIALOGFOCUS);
			
			SetFocusedUI(pChild);//this_ui

			return m_dwMouseFlagsCur;
		}
		else ++itor;
		//else if (UI_MOUSE_LBCLICKED|UI_MOUSE_MBCLICK|UI_MOUSE_MBCLICKED|UI_MOUSE_RBCLICK|UI_MOUSE_RBCLICKED)

		m_dwMouseFlagsCur |= dwChildRet;
	}

//	if(UI_MOUSE_LBCLICK & dwFlags) m_pUIFocused = NULL; // ��Ŀ�� ���� UI ���.. �ƹ��͵� ���ϸ�.. ���̴�..

	return m_dwMouseFlagsCur;
}

void CUIManager::ReorderChildList()	// ���̾˷α� ���� ���ġ
{
	int iChildCount = m_Children.size();
	if (iChildCount<=0) return;
	CN3UIBase** ppBuffer = new CN3UIBase*[iChildCount];
	int iAlwaysTopChildCount = 0;

	for(UIListItor itor = m_Children.begin(); m_Children.end() != itor; )
	{
		CN3UIBase* pChild = (*itor);
		if (pChild->GetStyle() & UISTYLE_ALWAYSTOP)
		{
			itor = m_Children.erase(itor);			// �켱 ����Ʈ���� �����
			ppBuffer[iAlwaysTopChildCount++] = pChild;
		}
		else ++itor;
	}
	int i;
	for (i=iAlwaysTopChildCount-1; i>=0; --i)
	{
		m_Children.push_front(ppBuffer[i]);	// �Ǿտ� �ִ´�. �׸��� ������ �� ���߿� �׸����� �ϰ� �޼����� �� ���� �ް� �Ϸ���
	}
	delete [] ppBuffer;
}

void CUIManager::Tick()
{
	CN3UIBase::Tick();
	if (s_pTooltipCtrl)	s_pTooltipCtrl->Tick();
}

void CUIManager::Render()
{
	this->RenderStateSet();

	CN3UIBase::Render();	// �ڽĵ� render
	if (s_pTooltipCtrl) s_pTooltipCtrl->Render();	// tooltip render

	/*
	NOTE: there is a very weird issue with setting the render state and displaying text.
	- when the debug info is being displayed and you change window focus weird shit happens
	- currently just sticking this in here. it will work here but will end up showing during
	every game procedure which is somewhat unwanted right now...
	*/
	////////////////////////////////////////////////////////
#ifdef _DEBUG
	CDFont* m_pDFont = new CDFont("����", 10);	// default �� ���� 16���� ����
	m_pDFont->InitDeviceObjects(CN3Base::s_lpD3DDev);
	m_pDFont->RestoreDeviceObjects();

	static char szDebugs[4][256] = { "", "", "", "" };

	sprintf(szDebugs[0], "nTerrain_Polygon(%d), nTerrain_Tile_Polygon(%d), nShape(%d), nShape_Part(%d), nShape_Polygon(%d)",
		CN3Base::s_RenderInfo.nTerrain_Polygon,
		CN3Base::s_RenderInfo.nTerrain_Tile_Polygon,
		CN3Base::s_RenderInfo.nShape,
		CN3Base::s_RenderInfo.nShape_Part,
		CN3Base::s_RenderInfo.nShape_Polygon
	);

	sprintf(szDebugs[1], "nChr(%d), nChr_Part(%d), nChr_Polygon(%d), nChr_Plug(%d), nChr_Plug_Polygon(%d)",
		CN3Base::s_RenderInfo.nChr,
		CN3Base::s_RenderInfo.nChr_Part,
		CN3Base::s_RenderInfo.nChr_Polygon,
		CN3Base::s_RenderInfo.nChr_Plug,
		CN3Base::s_RenderInfo.nChr_Plug_Polygon
	);

	sprintf(szDebugs[2], "Camera : FieldOfView(%.1f), NearPlane(%.1f) FarPlane(%.1f)",
		D3DXToDegree(CN3Base::s_CameraData.fFOV),
		CN3Base::s_CameraData.fNP,
		CN3Base::s_CameraData.fFP
	);

	if (CGameProcedure::s_pProcMain && CGameBase::ACT_WORLD && CGameBase::ACT_WORLD->GetSkyRef()) {
		int iYear = 0, iMonth = 0, iDay = 0, iH = 0, iM = 0;
		CGameBase::ACT_WORLD->GetSkyRef()->GetGameTime(&iYear, &iMonth, &iDay, &iH, &iM);
		sprintf(szDebugs[3], "%.2f:FPS, %d/%d/%d : %d:%d", CN3Base::s_fFrmPerSec, iYear, iMonth, iDay, iH, iM);
	}
	else szDebugs[3][0] = NULL;

	for (int i = 0; i<4; ++i) {
		m_pDFont->SetText(szDebugs[i]);
		m_pDFont->DrawText(0, 0 + i * 18, 0xFFFFFFFF, 0);
	}

	delete m_pDFont;
#endif
	////////////////////////////////////////////////////////

	this->RenderStateRestore();
}

void CUIManager::RenderStateSet()
{
	if(NULL == s_lpD3DDev) return;

#ifdef _DEBUG
	__ASSERT(FALSE == s_sRSFU.bSet, "������ RenderStateSet()�Լ��� ȣ���ϰ� RenderStateRestore()�Լ��� ȣ����� ���� �����Դϴ�.");
	s_sRSFU.bSet = TRUE;
#endif

	// back up old state
	s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &(s_sRSFU.dwZEnable));
	s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &(s_sRSFU.dwAlphaBlend));
	s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &(s_sRSFU.dwSrcBlend));
	s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &(s_sRSFU.dwDestBlend));
	s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &(s_sRSFU.dwFog));
	/*
	s_lpD3DDev->GetTextureStageState(0, D3DTSS_MAGFILTER,   &(s_sRSFU.dwMagFilter));
	s_lpD3DDev->GetTextureStageState(0, D3DTSS_MINFILTER,   &(s_sRSFU.dwMinFilter));
	s_lpD3DDev->GetTextureStageState(0, D3DTSS_MIPFILTER,   &(s_sRSFU.dwMipFilter));
	*/
	s_lpD3DDev->GetSamplerState(0, D3DSAMP_MAGFILTER, &(s_sRSFU.dwMagFilter));
	s_lpD3DDev->GetSamplerState(0, D3DSAMP_MINFILTER, &(s_sRSFU.dwMinFilter));
	s_lpD3DDev->GetSamplerState(0, D3DSAMP_MIPFILTER, &(s_sRSFU.dwMipFilter));

	// set state
	if (D3DZB_FALSE != s_sRSFU.dwZEnable) s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	if (TRUE != s_sRSFU.dwAlphaBlend) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	if (D3DBLEND_SRCALPHA != s_sRSFU.dwSrcBlend) s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	if (D3DBLEND_INVSRCALPHA != s_sRSFU.dwDestBlend) s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (FALSE != s_sRSFU.dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE   , FALSE);	// 2d�� fog�� �Դ´� ��.��;
	if (D3DTEXF_POINT != s_sRSFU.dwMagFilter ) s_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER,   D3DTEXF_POINT);
	if (D3DTEXF_POINT != s_sRSFU.dwMinFilter ) s_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER,   D3DTEXF_POINT);
	if (D3DTEXF_NONE != s_sRSFU.dwMipFilter ) s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER,   D3DTEXF_NONE);
}

void CUIManager::RenderStateRestore()
{
#ifdef _DEBUG
	__ASSERT(TRUE == s_sRSFU.bSet, "������ RenderStateSet()�Լ��� ȣ������ �ʾҽ��ϴ�.");
	s_sRSFU.bSet = FALSE;
#endif

	// restore
	if (D3DZB_FALSE != s_sRSFU.dwZEnable) s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, s_sRSFU.dwZEnable);
	if (TRUE != s_sRSFU.dwAlphaBlend) s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, s_sRSFU.dwAlphaBlend);
	if (D3DBLEND_SRCALPHA != s_sRSFU.dwSrcBlend) s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, s_sRSFU.dwSrcBlend);
	if (D3DBLEND_INVSRCALPHA != s_sRSFU.dwDestBlend) s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, s_sRSFU.dwDestBlend);
	if (FALSE != s_sRSFU.dwFog) s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE   , s_sRSFU.dwFog);
	if (D3DTEXF_POINT != s_sRSFU.dwMagFilter ) s_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER,   s_sRSFU.dwMagFilter);
	if (D3DTEXF_POINT != s_sRSFU.dwMinFilter ) s_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER,   s_sRSFU.dwMinFilter);
	if (D3DTEXF_NONE != s_sRSFU.dwMipFilter ) s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER,   s_sRSFU.dwMipFilter);
}

bool CUIManager::BroadcastIconDropMsg(__IconItemSkill* spItem)
{
	bool bFound = false;
	POINT ptCur = CGameProcedure::s_pLocalInput->MouseGetPos();

	// ��������� ���� �ٴϸ鼭 �˻�..
	for(UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		if ( bFound ) break;
		CN3UIBase* pChild = (*itor);
		if ( pChild->UIType() == UI_TYPE_ICON_MANAGER )
		{
			// �ش� �����찡 ���̰�(Ȱ��ȭ �Ǿ� �ְ�), �� ������ ���� �ȿ� ������..
			if ( ((CN3UIWndBase* )pChild)->IsVisible() && ((CN3UIWndBase* )pChild)->IsIn(ptCur.x, ptCur.y) )
				// �ش� �����쿡 ������ ��� �޽��� �Լ��� ȣ��..
				if ( ((CN3UIWndBase* )pChild)->ReceiveIconDrop(spItem, ptCur) )
					return true;
				else
					bFound = true;
		}
	}

	// ��� ������ �������� ������ ������.. �ش� �������� ���� �����쿡�� Cancel �޽����� ���� �ش�..
	if ( !bFound )
	{
		switch ( CN3UIWndBase::m_sSelectedIconInfo.UIWndSelect.UIWnd )
		{
			case UIWND_INVENTORY:
				CGameProcedure::s_pProcMain->m_pUIInventory->CancelIconDrop(spItem);
				break;

			case UIWND_TRANSACTION:
				CGameProcedure::s_pProcMain->m_pUITransactionDlg->CancelIconDrop(spItem);
				break;

			case UIWND_WARE_HOUSE:
				CGameProcedure::s_pProcMain->m_pUIWareHouseDlg->CancelIconDrop(spItem);
				break;

			case UIWND_EXCHANGE_REPAIR:
				CGameProcedure::s_pProcMain->m_pUIItemREDlg->CancelIconDrop(spItem);
				break;
		}
	}
	return false;
}

CN3UIBase* CUIManager::GetTopUI(bool bVisible)
{
	if(!bVisible) 
	{
		if(m_Children.empty()) return NULL;
		else return *(m_Children.begin());
	}

	UIListItor it = m_Children.begin(), itEnd = m_Children.end();
	for(; it != itEnd; it++)
	{
		CN3UIBase* pUI = *(it);
		if(pUI->IsVisible()) return pUI;
	}
	
	return NULL;
}

void CUIManager::SetFocusedUI(CN3UIBase* pUI)
{
	if(NULL == pUI)
	{
		m_pUIFocused = NULL;
		return;
	}

	UIListItor it = m_Children.begin(), itEnd = m_Children.end();
	it = m_Children.begin();
	for(; it != itEnd; it++)
	{
		if(pUI == *it) break;
	}
	if(it == itEnd) return;

	it = m_Children.erase(it);			// �켱 ����Ʈ���� �����
	m_Children.push_front(pUI);	// �Ǿտ� �ִ´�. �׸��� ������ �� ���߿� �׸����� �ϰ� �޼����� �� ���� �ް� �Ϸ���
	ReorderChildList();	// child list ������(�׻� ���� �ߴ� dialog ������ �ٽ� �����Ѵ�.)

	m_pUIFocused = this->GetTopUI(true);
}

CN3UIBase* CUIManager::ReFocusUI()
{
	SetVisibleFocusedUI(GetEnableFocusTopUI(true));
	return m_pUIFocused;
}

void CUIManager::SetVisibleFocusedUI(CN3UIBase *pUI)
{
	if(NULL == pUI)
	{
		m_pUIFocused = NULL;
		return;
	}

	if(!pUI->IsVisible())
		return;

	UIListItor it = m_Children.begin(), itEnd = m_Children.end();

	DWORD dwUIStyle, dwUIHideStyle;
	CN3UIBase* pUIHide = NULL;

	dwUIStyle = pUI->GetStyle();
	if(dwUIStyle & UISTYLE_SHOW_ME_ALONE)
	{
		for(; it != itEnd;)
		{
			pUIHide = *it;
			if(pUIHide == NULL)
			{
				it = m_Children.erase(it);
				continue;
			}

			dwUIHideStyle = pUIHide->GetStyle();
			if(pUIHide->IsVisible() && pUI != pUIHide && !(dwUIHideStyle & UISTYLE_HIDE_UNABLE))
				pUIHide->SetVisibleWithNoSound(false, true);

			it++;
		}//
	}
	else if(!(dwUIStyle & UISTYLE_HIDE_UNABLE))
	{
		for(; it != itEnd;)
		{
			pUIHide = *it;
			if(pUIHide == NULL)
			{
				it = m_Children.erase(it);
				continue;
			}

			dwUIHideStyle = pUIHide->GetStyle();
			if(pUIHide->IsVisible() && pUI != pUIHide && (dwUIHideStyle & UISTYLE_SHOW_ME_ALONE))
				pUIHide->SetVisibleWithNoSound(false, true);
/*
			if(pUIHide->IsVisible() && pUI != pUIHide)
			{
				if(dwUIHideStyle & UISTYLE_SHOW_ME_ALONE)
					pUIHide->SetVisibleWithNoSound(false, true);
				else if( (dwUIStyle & UISTYLE_POS_LEFT) && (dwUIHideStyle & UISTYLE_POS_LEFT) )
					pUIHide->SetVisibleWithNoSound(false, true);
				else if( (dwUIStyle & UISTYLE_POS_RIGHT) && (dwUIHideStyle & UISTYLE_POS_RIGHT) )
					pUIHide->SetVisibleWithNoSound(false, true);
			}
*/
			it++;
		}
	}

	it = m_Children.begin();
	itEnd = m_Children.end();

	for(; it != itEnd; it++)
	{
		if(pUI == *it) break;
	}
	if(it == itEnd) return;

	if(!(dwUIStyle & UISTYLE_FOCUS_UNABLE))
	{
		it = m_Children.erase(it);			// �켱 ����Ʈ���� �����
		m_Children.push_front(pUI);	// �Ǿտ� �ִ´�. �׸��� ������ �� ���߿� �׸����� �ϰ� �޼����� �� ���� �ް� �Ϸ���
		ReorderChildList();	// child list ������(�׻� ���� �ߴ� dialog ������ �ٽ� �����Ѵ�.)
	}

	m_pUIFocused = this->GetEnableFocusTopUI(true);
}

CN3UIBase* CUIManager::GetEnableFocusTopUI(bool bVisible)
{
	if(!bVisible) 
	{
		if(m_Children.empty()) return NULL;
		else return *(m_Children.begin());
	}

	UIListItor it = m_Children.begin(), itEnd = m_Children.end();
	for(; it != itEnd; it++)
	{
		CN3UIBase* pUI = *(it);
		if(pUI && pUI->IsVisible() &&	//���̰�
			!(pUI->GetStyle() & UISTYLE_FOCUS_UNABLE) && //��Ŀ���� �����ϰ�
			!(pUI->GetStyle() & UISTYLE_HIDE_UNABLE)) //������ �ִ�
			return pUI;
	}
	
	return NULL;
}

void CUIManager::UserMoveHideUIs()
{
	bool bHide = false;
	UIListItor it = m_Children.begin(), itEnd = m_Children.end();
	for(; it != itEnd; it++)
	{
		CN3UIBase* pUI = *(it);

		if(pUI == NULL) continue;

		if(pUI->IsVisible() && (pUI->GetStyle() & UISTYLE_USER_MOVE_HIDE))
		{
			bHide = true;
			pUI->SetVisibleWithNoSound(false, true);
		}
	}

	if(bHide) ReFocusUI();
}
