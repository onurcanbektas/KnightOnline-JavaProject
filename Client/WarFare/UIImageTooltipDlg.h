// UIImageTooltipDlg.h: interface for the CUIImageTooltipDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIIMAGETOOLTIPDLG_H__CC12484D_5DC0_4F7A_ABF7_0506B2F36292__INCLUDED_)
#define AFX_UIIMAGETOOLTIPDLG_H__CC12484D_5DC0_4F7A_ABF7_0506B2F36292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "N3UIString.h"

#include "GameDef.h"

#include "GameProcedure.h"
#include "N3UIWndBase.h"

//////////////////////////////////////////////////////////////////////

#define MAX_TOOLTIP_COUNT 30

class CUIImageTooltipDlg    : public CN3UIBase  
{
	const D3DCOLOR		m_CYellow;	// ����...
	const D3DCOLOR		m_CBlue;	// ����...	
	const D3DCOLOR		m_CGold;	// ����ũ...
	const D3DCOLOR		m_CIvory;	// ���׷��̵�..
	const D3DCOLOR		m_CGreen;	// �ɼ�...	���׷�, �ɷ�ġ ���, �����Ӽ�..
	const D3DCOLOR		m_CWhite;	// �Ϲ�...
	const D3DCOLOR		m_CRed;		// ���� ���ѿ� �ɸ���..

public:
	CN3UIString*		m_pStr[MAX_TOOLTIP_COUNT];			// ��Ʈ��.. ^^
	std::string			m_pstdstr[MAX_TOOLTIP_COUNT];
	CN3UIImage*			m_pImg;

	int					m_iPosXBack, m_iPosYBack;
	__IconItemSkill*	m_spItemBack;

protected:
	bool				SetTooltipTextColor(int iMyValue, int iTooltipValue);
	bool				SetTooltipTextColor(e_Race eMyValue, e_Race eTooltipValue);
	bool				SetTooltipTextColor(e_Class eMyValue, e_Class eTooltipValue);
	int					CalcTooltipStringNumAndWrite(__IconItemSkill* spItem, bool bPrice, bool bBuy);
	void				SetPosSomething(int xpos, int ypos, int iNum);

public:
	CUIImageTooltipDlg();
	virtual ~CUIImageTooltipDlg();
	void	Release();
	void	InitPos();

	void	DisplayTooltipsEnable(int xpos, int ypos, __IconItemSkill* spItem, bool bPrice = false, bool bBuy = true);
	void	DisplayTooltipsDisable();
};

#endif // !defined(AFX_UIIMAGETOOLTIPDLG_H__CC12484D_5DC0_4F7A_ABF7_0506B2F36292__INCLUDED_)
