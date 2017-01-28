// UIRepairTooltipDlg.cpp: implementation of the UIRepairTooltipDlg class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "resource.h"
#include "UIRepairTooltipDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIRepairTooltipDlg::CUIRepairTooltipDlg()
{
	m_iPosXBack = 0;
	m_iPosYBack = 0;
	m_spItemBack = NULL;

	m_bBRender = false;
	m_iBxpos = -1;
	m_iBypos = -1;
	m_pBspItem = NULL;
	m_iBRequiredGold = 0;
	m_bBHaveEnough = true;

	m_iBRequiredGoldBackup = 0;
}

CUIRepairTooltipDlg::~CUIRepairTooltipDlg()
{
	Release();
}

void CUIRepairTooltipDlg::Release()
{
	CN3UIBase::Release();
}

void CUIRepairTooltipDlg::InitPos()
{
	m_pStr[0] = (CN3UIString* )GetChildByID("string_repairgold");				//�̸� 
	__ASSERT(m_pStr[0], "NULL UI Component!!");
	m_pStr[0]->SetString("0");	
	m_pStr[1] = (CN3UIString* )GetChildByID("string_dur_max");					//�̸� 
	__ASSERT(m_pStr[1], "NULL UI Component!!");
	m_pStr[1]->SetString("0");	
	m_pStr[2] = (CN3UIString* )GetChildByID("string_dur_current");				//�̸� 
	__ASSERT(m_pStr[2], "NULL UI Component!!");
	m_pStr[2]->SetString("0");	
	m_pStr[3] = (CN3UIString* )GetChildByID("string_title");					//�̸� 
	__ASSERT(m_pStr[3], "NULL UI Component!!");
	m_pStr[3]->SetString("0");	
	BackupStrColor();
}

void CUIRepairTooltipDlg::BackupStrColor()
{
	m_pStrColor[0] = m_pStr[0]->GetColor();
	m_pStrColor[1] = m_pStr[1]->GetColor();
	m_pStrColor[2] = m_pStr[2]->GetColor();
	m_pStrColor[3] = m_pStr[3]->GetColor();
}

void CUIRepairTooltipDlg::DisplayTooltipsEnable(int xpos, int ypos, __IconItemSkill* spItem, int iRequiredGold, bool bHaveEnough)
{
	int iIndex = 0;
	if ( !spItem ) return;

	if ( !IsVisible() )
		SetVisible(TRUE);

	std::string szMsg; 

	if ( (m_iPosXBack != xpos) || (m_iPosYBack != ypos) )
	{
		int iWidth = m_rcRegion.right-m_rcRegion.left;
		int iHeight = m_rcRegion.bottom-m_rcRegion.top;

		int iRight, iTop;
		iRight = 1024; iTop = 0;

		if ( (xpos+iWidth<iRight) && ((ypos-iHeight)>iTop))
			SetPos(xpos+26, ypos-iHeight);
		else if ( (xpos+iWidth>iRight) && ((ypos-iHeight)<iTop))
			SetPos(xpos-iWidth, ypos);
		else if ( xpos+iWidth>iRight )
			SetPos(xpos-iWidth, ypos-iHeight);
		else
			SetPos(xpos+26, ypos);

		m_iPosXBack = xpos;
		m_iPosYBack = ypos;
	}

	if ( (!m_spItemBack) || (m_spItemBack->pItemBasic->dwID != spItem->pItemBasic->dwID) || 
		(m_iBRequiredGoldBackup != m_iBRequiredGold) )
	{
		m_spItemBack = spItem;

		std::string szString;
		char szBuff[64] = "";

		if ( (spItem->pItemBasic->byContable != UIITEM_TYPE_COUNTABLE) && (spItem->pItemBasic->byContable != UIITEM_TYPE_COUNTABLE_SMALL) && 
				(spItem->pItemBasic->siMaxDurability+spItem->pItemExt->siMaxDurability != 1) )
		{
			SetTooltipTextColor(bHaveEnough);
			szMsg = "IDS_TOOLTIP_NOAH";//::_LoadStringFromResource(IDS_TOOLTIP_NOAH, szMsg);
			sprintf(szBuff, szMsg.c_str(), iRequiredGold);
			if (!m_pStr[iIndex]->IsVisible())
				m_pStr[iIndex]->SetVisible(true);
			m_pStr[iIndex]->SetString(szBuff);
			iIndex++;

			szMsg = "IDS_TOOLTIP_MAX_DURABILITY";//::_LoadStringFromResource(IDS_TOOLTIP_MAX_DURABILITY, szMsg);
			sprintf(szBuff, szMsg.c_str(), spItem->pItemBasic->siMaxDurability+spItem->pItemExt->siMaxDurability);
			if (!m_pStr[iIndex]->IsVisible())
				m_pStr[iIndex]->SetVisible(true);
			m_pStr[iIndex]->SetString(szBuff);
			iIndex++;

			szMsg = "IDS_TOOLTIP_CUR_DURABILITY";//::_LoadStringFromResource(IDS_TOOLTIP_CUR_DURABILITY, szMsg);
			sprintf(szBuff, szMsg.c_str(), spItem->iDurability);
			if (!m_pStr[iIndex]->IsVisible())
				m_pStr[iIndex]->SetVisible(true);
			m_pStr[iIndex]->SetString(szBuff);
			iIndex++;

			szMsg = "IDS_TOOLTIP_REPAIR_PRICE";//::_LoadStringFromResource(IDS_TOOLTIP_REPAIR_PRICE, szMsg);
			sprintf(szBuff, szMsg.c_str());
			if (!m_pStr[iIndex]->IsVisible())
				m_pStr[iIndex]->SetVisible(true);
			m_pStr[iIndex]->SetString(szBuff);
			iIndex++;

			for( int i = iIndex; i < MAX_REPAIR_TOOLTIP_COUNT; i++ )
				m_pStr[i]->SetString("");
		}
		else
		{
			szMsg = "IDS_TOOLTIP_CANNOT";//::_LoadStringFromResource(IDS_TOOLTIP_CANNOT, szMsg);
			m_pStr[0]->SetVisible(false);
			m_pStr[1]->SetVisible(false);
			m_pStr[2]->SetVisible(false);
			sprintf(szBuff, szMsg.c_str());
			m_pStr[3]->SetString(szBuff);
			m_pStr[3]->SetVisible(true);
		}
	}

	m_iBRequiredGoldBackup = m_iBRequiredGold;
	CN3UIBase::Render();
}

void CUIRepairTooltipDlg::DisplayTooltipsDisable()
{
	m_spItemBack = NULL;
	if ( IsVisible() )
		SetVisible(FALSE);
}

void CUIRepairTooltipDlg::SetTooltipTextColor(bool bHaveEnough)
{
	if (bHaveEnough)
		m_pStr[0]->SetColor(m_pStrColor[0]);
	else
		m_pStr[0]->SetColor(0xffdd0000);
}

void CUIRepairTooltipDlg::Render()
{
	if (!m_bBRender)
		DisplayTooltipsDisable();
	else
		DisplayTooltipsEnable(m_iBxpos, m_iBypos, m_pBspItem, m_iBRequiredGold, m_bBHaveEnough);
}