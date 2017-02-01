// UIInventory.h: interface for the CUIInventory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIInventory_H__863D2E52_65A8_47E1_BCBE_6281692DDCA2__INCLUDED_)
#define AFX_UIInventory_H__863D2E52_65A8_47E1_BCBE_6281692DDCA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "N3UIArea.h"
#include "N3UIImage.h"
#include "GameProcedure.h"
#include "GameDef.h"
#include "N3UIWndBase.h"
#include "ItemRepairMgr.h"

const int	ITEM_SLOT_POS_EAR_RIGHT			= 0;
const int	ITEM_SLOT_POS_HEAD				= 1;
const int	ITEM_SLOT_POS_EAR_LEFT			= 2;
const int	ITEM_SLOT_POS_NECK				= 3;
const int	ITEM_SLOT_POS_UPPER				= 4;
const int	ITEM_SLOT_POS_SHOULDER			= 5;
const int	ITEM_SLOT_POS_HAND_RIGHT		= 6;
const int	ITEM_SLOT_POS_BELT				= 7;
const int	ITEM_SLOT_POS_HAND_LEFT			= 8;
const int	ITEM_SLOT_POS_RING_RIGHT		= 9;
const int	ITEM_SLOT_POS_LOWER				= 10;
const int	ITEM_SLOT_POS_RING_LEFT			= 11;
const int	ITEM_SLOT_POS_GLOVES			= 12;
const int	ITEM_SLOT_POS_SHOES				= 13;

const int	ITEM_ATTACH_POS_DUAL			= 0;
const int	ITEM_ATTACH_POS_HAND_RIGHT		= 1;
const int	ITEM_ATTACH_POS_HAND_LEFT		= 2;
const int	ITEM_ATTACH_POS_TWOHAND_RIGHT	= 3;
const int	ITEM_ATTACH_POS_TWOHAND_LEFT	= 4;
const int	ITEM_ATTACH_POS_UPPER			= 5;
const int	ITEM_ATTACH_POS_LOWER			= 6;
const int	ITEM_ATTACH_POS_HEAD			= 7;
const int	ITEM_ATTACH_POS_ARM				= 8;
const int	ITEM_ATTACH_POS_FOOT			= 9;
const int	ITEM_ATTACH_POS_EAR				= 10;
const int	ITEM_ATTACH_POS_NECK			= 11;	
const int	ITEM_ATTACH_POS_FINGER			= 12;
const int	ITEM_ATTACH_POS_CLOAK			= 13;
const int	ITEM_ATTACH_POS_BELT			= 14;
const int	ITEM_ATTACH_POS_INVENTORY		= 15;


enum e_InvenState { INV_STATE_NORMAL = 1, INV_STATE_REPAIR, };		// �κ��丮 ����.. ������ ���¿� �����ϱ� ����.. ���� ���°� �߰��ƴ�..


//////////////////////////////////////////////////////////////////////

class CUIInventory : public CN3UIWndBase  
{
	bool				m_bDestoyDlgAlive;

protected:
	e_InvenState		  m_eInvenState;
	CItemRepairMgr		m_cItemRepairMgr;

	bool					m_bOpenningNow; // ������ �ִ�..
	bool					m_bClosingNow;	// ������ �ִ�..
	float					 m_fMoveDelta; // �ε巴�� ������ ������ ����� ���ؼ� ������ġ ��꿡 �ε��Ҽ����� ����..

	CN3UIArea*			m_pArea_User;
	CN3UIArea*			m_pArea_Destroy;

	CN3UIString*		m_pText_Weight;

	CUIImageTooltipDlg*	m_pUITooltipDlg;

	int							m_iRBtnDownOffs;
	bool					 m_bRBtnProcessing;

public:
	__IconItemSkill*	m_pMySlot[ITEM_SLOT_COUNT];
	__IconItemSkill*	m_pMyInvWnd[MAX_ITEM_INVENTORY];

protected:
	e_UIWND_DISTRICT	GetWndDistrict(__IconItemSkill* spItem);
	int					GetItemiOrder(__IconItemSkill* spItem, e_UIWND_DISTRICT eWndDist);

	RECT				GetSampleRect();

	// �˻��ϴ� ��ƾ..
	bool				IsValidRaceAndClass(__TABLE_ITEM_BASIC* pItem, __TABLE_ITEM_EXT* pItemExt); 
	bool				IsValidPosFromInvToArm(int iOrder);
	bool				IsValidPosFromArmToArm(int iOrder);
	bool				IsValidPosFromArmToArmInverse(int iOrder);

	void				ItemAdd(__TABLE_ITEM_BASIC* pItem, __TABLE_ITEM_EXT* pItemExt, e_ItemSlot eSlot);
	void				ItemDelete(__TABLE_ITEM_BASIC* pItem, __TABLE_ITEM_EXT* pItemExt, e_ItemSlot eSlot);

	inline	bool		InvOpsSomething(__IconItemSkill* spItem);
	int						GetArmDestinationIndex(__IconItemSkill* spItem);
	int						GetInvDestinationIndex(__IconItemSkill* spItem);

// Operations
public:

	//this_ui_add_start
	int					GetIndexItemCount(DWORD dwIndex);
	void				SetVisibleWithNoSound(bool bVisible, bool bWork = false, bool bReFocus = false);
	void				SetVisible(bool bVisible);
	bool				OnKeyPress(int iKey);
	//this_ui_add_end

	void				Release();
	void				ReleaseItem();

	void				Open(e_InvenState eIS = INV_STATE_NORMAL);
	void				Close(bool bByKey = false);

	void				Tick();
	void				Render();

	void				InitIconWnd(e_UIWND eWnd);	
	void				InitIconUpdate();

	DWORD				MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool				ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	__IconItemSkill*	GetHighlightIconItem(CN3UIIcon* pUIIcon);

	bool				CheckIconDropIfSuccessSendToServer(__IconItemSkill* spItem);
	void				IconRestore();
	
	bool				ReceiveIconDrop(__IconItemSkill* spItem, POINT ptCur);

	void				CancelIconDrop(__IconItemSkill* spItem);
	void				AcceptIconDrop(__IconItemSkill* spItem);

	void				SendInvMsg(BYTE bDir, int iItemID, int SrcPos, int DestPos);

	void				ReceiveResultFromServer(BYTE bResult = 0x01);
	void				ReceiveResultItemRemoveFromServer(int iResult);

	// Duration Change..
	void				DurabilityChange(e_ItemSlot eSlot, int iDurability);	
	e_InvenState		GetInvState()	{ return m_eInvenState; }

	void				ReceiveResultFromServer(int iResult, int iUserGold);

	int					GetCountInInvByID(int iID);

	// �Ҹ� �������� �Һ��� ��� or Not..
	void				ItemCountChange(int iDistrict, int iIndex, int iCount, int iID);

	// child dlg func..
	void				ItemDestroyOK();
	void				ItemDestroyCancel();

	bool				CheckWeightValidate(__IconItemSkill* spItem);
	void				GoldUpdate();

	bool				HasAnyItemInSlot();
	void				UpdateWeight(std::string str);

	int					  GetIndexInArea(POINT pt);

	CUIInventory();
	virtual	~CUIInventory();
};

#endif // !defined(AFX_UIInventory_H__863D2E52_65A8_47E1_BCBE_6281692DDCA2__INCLUDED_)



