// CountableItemEditDlg.h: interface for the CCountableItemEditDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COUNTABLEITEMEDITDLG_H__55E83755_6471_4A3D_84D7_857B0FF88833__INCLUDED_)
#define AFX_COUNTABLEITEMEDITDLG_H__55E83755_6471_4A3D_84D7_857B0FF88833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "N3UIArea.h"
#include "N3UIImage.h"
#include "N3UIString.h"
#include "N3UIButton.h"

#include "N3UIWndBase.h"
#include "GameProcedure.h"

#include "GameDef.h"


//////////////////////////////////////////////////////////////////////

class CCountableItemEditDlg   : public CN3UIBase  
{
	bool				m_bLocked;
	e_UIWND				m_eCallerWnd;
	e_UIWND_DISTRICT	m_eCallerWndDistrict;
	bool				m_bWareGold;

public:
	CN3UIArea*			m_pArea;
	CN3UIImage*			m_pImageOfIcon;
	CN3UIEdit*			m_pEdit;

	CN3UIButton*		m_pBtnOk;
	CN3UIButton*		m_pBtnCancel;


public:
	bool OnKeyPress(int iKey);
	bool Load(HANDLE hFile);
	void				SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
	void				SetVisible(bool bVisible);
	int					GetQuantity(); // "edit_trade" Edit Control ���� �������� ����´�..
	void				SetQuantity(int iQuantity); // "edit_trade" Edit Control ���� �������� ���ڿ��� �����Ѵ�..

	CCountableItemEditDlg();
	virtual ~CCountableItemEditDlg();

	virtual void		Release();
	virtual bool		ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	virtual void		Open(e_UIWND eUW, e_UIWND_DISTRICT eUD, bool bCountGold, bool bWareGold = false);
	virtual void		Close();

	bool				IsLocked()	{	return m_bLocked;	}

	e_UIWND				GetCallerWnd()	{	return m_eCallerWnd;	}
	e_UIWND_DISTRICT	GetCallerWndDistrict()	{	return m_eCallerWndDistrict;	}
};

#endif // !defined(AFX_COUNTABLEITEMEDITDLG_H__55E83755_6471_4A3D_84D7_857B0FF88833__INCLUDED_)
