// PlayerMgr.h: interface for the CPlayerMySelf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYERMGR_H__036554E3_CDCC_4B13_97B2_34C5D4D63E8C__INCLUDED_)
#define AFX_PLAYERMGR_H__036554E3_CDCC_4B13_97B2_34C5D4D63E8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "PlayerBase.h"

//////////////////////////////////////////////////////////////////////
class CPlayerMySelf : public CPlayerBase
{
protected:
	bool			m_bRunning; // �ٴ���..
	float			m_fAttackTimeRecent;	// �ֱٿ� ������ �ð�..

	__Vector3		m_vCollisionOffsets[3]; // �㸮 �κ� 2���� �浹 üũ + �Ӹ� �κ� 1���� �浹 üũ..

public:	
	__InfoPlayerMySelf	m_InfoExt;
	CN3Chr				m_ChrInv;				// ĳ���� �⺻ ��ü...
	bool				m_bMoveContinous;		// ��� �����̴���..
	bool				m_bAttackContinous;		// ��� �����ϴ� ��������..
	bool				m_bSitDown;				// �ɾ��ִ� ��������....
	bool				m_bRecruitParty;		// ��Ƽ ������??

	bool				m_bStun;				// ����..
	float				m_fStunTime;			// ������ �ð�..

	int					m_iSendRegeneration;	// �ѹ� ������ �ٽ� ���������� �Ⱥ����� �÷��� 0 �϶��� �޽��� �ڽ��� ����..
	bool				m_bTempMoveTurbo;		// ������ ���� �ӽ÷� ���� �ڵ�.. ���� ������ �����̰� �Ѵ�.. Tick ����.

	DWORD				m_dwMagicID;			// ���� �ִ� ����..
	float				m_fCastingTime;			// ���� �ֹ� �Ŵ� �ð�..
	
	class CN3Shape*		m_pObjectTarget;		// Ÿ�� ������Ʈ ������..

	bool				m_bTargetOrPosMove;		// Ÿ���̳� ���� �̵� ����
	int					m_iMoveTarget;			// Ÿ�� ���̵�
	__Vector3			m_vTargetPos;			// �̵��� ���� ��ġ
	void				SetMoveTargetID(int iID);
	void				SetMoveTargetPos(__Vector3 vPos);

public:	
	void			TargetOrPosMove();
	void			Stun(float fTime); // ������ �ð����� ���� ��Ű��.
	void			StunRelease(); // ���� Ǯ��..

	float			MoveSpeedCalculationAndCheckCollision(); // �ӵ��� ���ϰ� �� �ӵ��� �浹 üũ�� �Ѵ�. ���ϰ��� 0 �̸� �浹�̴�..

	void			InitFace(); // ���� �ִ� ������ ���� �ʱ�ȭ �Ѵ�..
	void			InitHair(); // ���� �ִ� ������ �Ӹ�ī���� �ʱ�ȭ �Ѵ�..
	void			KnightsInfoSet(int iID, const std::string& szName, int iGrade, int iRank);
	void			SetSoundAndInitFont(DWORD dwFontFlag = 0UL);

	float			AttackableDistance(CPlayerBase* pTarget);
	float			DistanceExceptRadius(CPlayerBase* pTarget);
	bool			IsAttackableTarget(CPlayerBase* pTarget, bool bMesureAngle = true); // ���� ������ ������ �ִ���..
	bool			IsRunning() { return m_bRunning; }
	bool			CheckCollision();		// �����̴� ó���� �浹üũ�� �Ѵ�. �浹�Ǵ°� ������ �������� �ʴ´�.
	//.. 
	bool			InitChr(__TABLE_PLAYER_LOOKS* pTblUPC);
	CN3CPart*		PartSet(e_PartPosition ePos, const std::string& szFN, __TABLE_ITEM_BASIC* pItemBasic, __TABLE_ITEM_EXT* pItemExt);
	CN3CPlugBase*	PlugSet(e_PlugPosition ePos, const std::string& szFN, __TABLE_ITEM_BASIC* pItemBasic, __TABLE_ITEM_EXT* pItemExt);

	void			Tick();
	void			Render(float fSunAngle);

	__Vector3		NextPos(float fTimeAfter);
	void			RotAdd(const float fRotRadianPerSec);			// y ���� �������� �ʴ� ȸ���ϴ� �ӵ��� �־��ش�.
	
	bool			ToggleAttackContinous();
	bool			ToggleRunMode();
	void			ToggleMoveMode();

	// Inventory..
	void			InventoryChrRender(const RECT& Rect);
	void			ChrCreateChrRender(const RECT& Rect);
	void			InventoryChrTick();
	void			InventoryChrAnimationInitialize();


	void			Release();
	CPlayerMySelf();
	virtual	~CPlayerMySelf();
};


#endif // !defined(AFX_PLAYERMGR_H__036554E3_CDCC_4B13_97B2_34C5D4D63E8C__INCLUDED_)
