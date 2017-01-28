// CPlayerBase.h: interface for the CPlayerBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PlayerBase_H__B8B8986B_3635_462D_8C38_A052CA75B331__INCLUDED_)
#define AFX_PlayerBase_H__B8B8986B_3635_462D_8C38_A052CA75B331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameBase.h"
#include "GameDef.h"
#include "Bitset.h"
#include "N3Chr.h"
#include <deque>

//	By : Ecli666 ( On 2002-07-22 ���� 9:59:19 )
//
#define SHADOW_SIZE 32			// 2�� �¼��� �ȴ�..
#define SHADOW_PLANE_SIZE 4.6f	
#define SHADOW_COLOR 0xa;		// 16���� ���ڸ�.. ����
//	~(By Ecli666 On 2002-07-22 ���� 9:59:19 )

const float TIME_CORPSE_REMAIN = 90.0f;	// ��ü�� ���� �ð�..
const float TIME_CORPSE_REMOVE = 10.0f; // ���������鼭 ���ٽð�..

class CDFont;
class CN3SndObj;

class CPlayerBase : public CGameBase
{
	friend class CPlayerBase;
	friend class CPlayerOtherMgr;

protected:
	e_PlayerType				m_ePlayerType; // Player Type ... Base, NPC, OTher, MySelf
	
	std::deque<e_Ani>			m_AnimationDeque;		// ���ϸ��̼� ť... ����� ���� ������ tick �� ���鼭 ���ʴ�� �Ѵ�..
	bool						m_bAnimationChanged;	// ť�� ���� ���ϸ��̼��� ���ϴ� ������ ���õȴ�..

	CN3Chr						m_Chr;									// ĳ���� �⺻ ��ü...
	__TABLE_PLAYER_LOOKS*		m_pLooksRef;							// �⺻ ���� ���̺� - ĳ���Ϳ� ���� ���ҽ� ����, ���� ��ġ, ���� ���ϵ��..
	__TABLE_ITEM_BASIC*			m_pItemPartBasics[PART_POS_COUNT];	// ĳ���Ϳ� ���� �����..
	__TABLE_ITEM_EXT*			m_pItemPartExts[PART_POS_COUNT];		// ĳ���Ϳ� ���� �����..
	__TABLE_ITEM_BASIC*			m_pItemPlugBasics[PLUG_POS_COUNT];	// ĳ���Ϳ� ���� �����..
	__TABLE_ITEM_EXT*			m_pItemPlugExts[PLUG_POS_COUNT];		// ĳ���Ϳ� ���� �����..

	// ID 
	CDFont*				m_pClanFont;			// clan or knights..�̸� ��µ� ���� Font.. -.-;
	CDFont*				m_pIDFont;				// ID ��µ� ���� Font.. -.-;
	CDFont*				m_pInfoFont;			// ��Ƽ�� ������.. ��Ÿ ���� ǥ��..
	CDFont*				m_pBalloonFont;			// ǳ���� ǥ��...
	float				m_fTimeBalloon;			// ǳ���� ǥ�� �ð�..

	e_StateAction		m_eState;				// �ൿ ����..
	e_StateAction		m_eStatePrev;			// ������ ���õ� �ൿ ����..
	e_StateAction		m_eStateNext;			// ������ ���õ� �ൿ ����..
	e_StateMove			m_eStateMove;			// �����̴� ����..
	e_StateDying		m_eStateDying;			// ������ ��� �״°�..??
	float				m_fTimeDying;			// �״� ����� ���ϴ� �ð�..

	// by tigger
//	By : Ecli666 ( On 2002-03-29 ���� 4:22:25 )
//
//	CN3Texture*			m_pTexShadow;			// �׸��� �ؽ�ó...
//	__VertexXyzT1		m_vShadows[4];			// �׸��� ������..

//	~(By Ecli666 On 2002-03-29 ���� 4:22:25 )
	
	D3DCOLORVALUE	m_cvDuration;			// ���� �÷� ��
	float			m_fDurationColorTimeCur;// ���� �ð�..
	float			m_fDurationColorTime;	// ���ӽð�..

	float 			m_fFlickeringFactor;	// ���ڰŸ�..
	float			m_fFlickeringTime;		// ���ڰŸ� �ð�..

	float			m_fRotRadianPerSec;		// �ʴ� ȸ�� ���Ȱ�
	float			m_fMoveSpeedPerSec;		// �ʴ� ������ ��.. �̰��� �⺻���̰� ����(�ȱ�, �޸���, �ڷ�, ���ֵ�) �� ���� �����ؼ� ����..

	float			m_fYawCur;				// ���� ȸ����..
	float			m_fYawToReach;			// �� ȸ������ ��ǥ�� Tick ���� ȸ���Ѵ�..

	float			m_fYNext;				// ������Ʈ Ȥ�� ������ �浹 üũ�� ���� ���̰�..
	float			m_fGravityCur;			// �߷°�..

	float			m_fScaleToSet;			// ���� ������ ����ȭ..
	float			m_fScalePrev;

public:
	class CN3ShapeExtra*	m_pShapeExtraRef;			// �� NPC �� �����̳� ���� ������Ʈ�� �����̸� �� �����͸� �����ؼ� ��,��..

	int					m_iMagicAni;
	int					m_iIDTarget;			// Ÿ�� ID...
	int					m_iDroppedItemID;		// ������ ����Ʈ�� ������
	bool				m_bGuardSuccess;		// �� �����ߴ����� ���� �÷���..
	bool				m_bVisible;				// ���̴���??

	__InfoPlayerBase	m_InfoBase;				// ĳ���� ����..
	__Vector3			m_vPosFromServer;		// �ֱٿ� �������Լ� ���� ���� ��ġ..	

	float				m_fTimeAfterDeath;		// ������ �����ð� - 5�������� �����Ѱ�?? ������ ������ ������ �ٷ� �״´�.

	int					m_iSkillStep;			// ���� ��ų�� ���� �ִٸ� 0 �� �ƴѰ��̴�...
	float				m_fAttackDelta;			// ��ų�̳� ������ ���� ���ϴ� ���� �ӵ�.. 1.0 �� �⺻�̰� Ŭ���� �� ���� �����Ѵ�.
	float				m_fMoveDelta;			// ��ų�̳� ������ ���� ���ϴ� �̵� �ӵ� 1.0 �� �⺻�̰� Ŭ���� �� ���� �����δ�.
	__Vector3			m_vDirDying;			// ������ �и��� ����..

	//sound..
	bool			m_bSoundAllSet;
	CN3SndObj*		m_pSnd_Attack_0;
//	CN3SndObj*		m_pSnd_Attack_1;
	CN3SndObj*		m_pSnd_Move;
	CN3SndObj*		m_pSnd_Struck_0;
//	CN3SndObj*		m_pSnd_Struck_1;
//	CN3SndObj*		m_pSnd_Dead_0;
//	CN3SndObj*		m_pSnd_Dead_1;
	CN3SndObj*		m_pSnd_Breathe_0;
//	CN3SndObj*		m_pSnd_Breathe_1;

	CN3SndObj*		m_pSnd_Blow;

	float			m_fCastFreezeTime;

// �Լ�...
//	By : Ecli666 ( On 2002-03-29 ���� 1:32:12 )
//	
	CBitset				m_bitset[SHADOW_SIZE];			// Used in Quake3.. ^^
	__VertexT1			m_pvVertex[4];
	unsigned short		m_pIndex[6];
	__VertexT1			m_vTVertex[4];
	float				m_fShaScale;
	CN3Texture			m_N3Tex; 
	static CN3SndObj*	m_pSnd_MyMove;
		
	const __Matrix44 CalcShadowMtxBasicPlane(__Vector3 vOffs);
	void			CalcPart(CN3CPart* pPart, int nLOD, __Vector3 vLP);
	void			CalcPlug(CN3CPlugBase* pPlug, const __Matrix44* pmtxJoint, __Matrix44& mtxMV, __Vector3 vLP);

protected:
	void			RenderShadow(float fSunAngle);
//	~(By Ecli666 On 2002-03-29 ���� 1:32:12 )

	virtual bool	ProcessAttack(CPlayerBase* pTarget); // ���� ��ƾ ó��.. Ÿ�� �����͸� ���ϰ� �浹üũ���� �ϸ� �浹�ϸ� ���� ����..

public:
	const __Matrix44*	JointMatrixGet(int nJointIndex) { return m_Chr.MatrixGet( nJointIndex); }
	bool 				JointPosGet(int iJointIdx, __Vector3& vPos);
	
	e_PlayerType	PlayerType() { return m_ePlayerType; }
	e_Race			Race() { return m_InfoBase.eRace; }
	e_Nation		Nation() { return m_InfoBase.eNation; }
	virtual void	SetSoundAndInitFont(DWORD dwFontFlag = 0UL);
	void			SetSoundPlug(__TABLE_ITEM_BASIC* pItemBasic);
	void			ReleaseSoundAndFont();
	void			RegenerateCollisionMesh(); // �ִ� �ּҰ��� �ٽ� ã�� �浹�޽ø� �ٽ� �����..
	e_StateAction	State() { return m_eState; } // �ൿ ����...
	e_StateMove		StateMove() { return m_eStateMove; } // �����̴� ����

	e_ItemClass		ItemClass_RightHand()
	{
		if(m_pItemPlugBasics[PLUG_POS_RIGHTHAND]) return (e_ItemClass)(m_pItemPlugBasics[PLUG_POS_RIGHTHAND]->byClass); // ������ Ÿ�� - ������
		else return ITEM_CLASS_UNKNOWN;
	}
	e_ItemClass		ItemClass_LeftHand()
	{
		if(m_pItemPlugBasics[PLUG_POS_LEFTHAND]) return (e_ItemClass)(m_pItemPlugBasics[PLUG_POS_LEFTHAND]->byClass); // ������ Ÿ�� - ������
		else return ITEM_CLASS_UNKNOWN;
	}
	
	e_Ani	JudgeAnimationBreath(); // ������ ��� �Ǵ��ϱ�.. ���� �����۰� Ÿ���� �ִ³Ŀ� ���� �ٸ� ���ϸ��̼� �ε����� ����.
	e_Ani	JudgeAnimationWalk(); // �ȱ� ����Ǵ��ϱ�.. ���� �����۰� Ÿ���� �ִ³Ŀ� ���� �ٸ� ���ϸ��̼� �ε����� ����.
	e_Ani	JudgeAnimationRun(); // �ȱ� ����Ǵ��ϱ�.. ���� �����۰� Ÿ���� �ִ³Ŀ� ���� �ٸ� ���ϸ��̼� �ε����� ����.
	e_Ani	JudgeAnimationWalkBackward(); // �ȱ� ����Ǵ��ϱ�.. ���� �����۰� Ÿ���� �ִ³Ŀ� ���� �ٸ� ���ϸ��̼� �ε����� ����.
	e_Ani	JudgeAnimationAttack(); // ���� ��� �Ǵ��ϱ�.. ���� �����ۿ� ���� �ٸ� ���ϸ��̼� �ε����� ����.
	e_Ani	JudgeAnimationStruck(); // ���� NPC �� ������ �����ؼ� ���ϸ��̼� �ε����� ����
	e_Ani	JudgeAnimationGuard(); // ���� ���� �Ǵ��ϱ�.  ���� NPC �� ������ �����ؼ� ���ϸ��̼� �ε����� ����
	e_Ani	JudgeAnimationDying(); // ���� NPC �� ������ �����ؼ� ���ϸ��̼� �ε����� ����
	e_Ani	JudgetAnimationSpellMagic(); // ���� ���� 

	bool			IsDead() { return ( PSA_DYING == m_eState || PSA_DEATH == m_eState ); } // �׾��ִ���?
	bool			IsAlive() { return !IsDead(); } // ����ִ���?
	bool			IsMovingNow() { if(PSM_WALK == m_eStateMove || PSM_RUN == m_eStateMove || PSM_WALK_BACKWARD == m_eStateMove) return true; return false; } // �����̰� �ִ���?

	void			AnimationAdd(e_Ani eAni, bool bImmediately);
	void			AnimationClear() { m_AnimationDeque.clear(); }
	int				AnimationCountRemain() { return m_AnimationDeque.size() + 1; }
	bool			IsAnimationChange() { return m_bAnimationChanged; }	// ť�� ���� ���ϸ��̼��� ���ϴ� ������ ���õȴ�..

	bool			Action(e_StateAction eState, bool bLooping, CPlayerBase* pTarget = NULL, bool bForceSet = false); // �ൿ ���̺� ���� �ൿ�� �Ѵ�..
	bool			ActionMove(e_StateMove eMove); // �����̱�..
	void			ActionDying(e_StateDying eSD, const __Vector3& vDir); // �״� ��� �����ϱ�..

	float			Yaw() { return m_fYawCur; } // ȸ����..
	float			MoveSpeed() { return m_fMoveSpeedPerSec; }
	__Vector3		Position() const { return m_Chr.Pos(); }
	void			PositionSet(const __Vector3& vPos, bool bForcely) { m_Chr.PosSet(vPos); if(bForcely) m_fYNext = vPos.y; }
	float			Distance(const __Vector3& vPos) { return (m_Chr.Pos() - vPos).Magnitude(); }
	__Vector3		Scale() { return m_Chr.Scale(); }
	void			ScaleSet(float fScale) { m_fScaleToSet = m_fScalePrev = fScale; m_Chr.ScaleSet(fScale, fScale, fScale); }
	void			ScaleSetGradually(float fScale) { m_fScaleToSet = fScale; m_fScalePrev = m_Chr.Scale().y; } // ���� ������ ��ȭ..
	__Vector3		Direction();
	__Quaternion	Rotation() { return m_Chr.Rot(); }
	void			RotateTo(float fYaw, bool bImmediately);
	void			RotateTo(CPlayerBase* pOther); // �̳��� �ٶ󺻴�.
	float			Height();
	float			Radius();
	__Vector3		HeadPosition(); // �׻� ���ϴ� �Ӹ���ġ�� �����´�..
	__Vector3		RootPosition() { if(!m_Chr.m_MtxJoints.empty()) return m_Chr.m_MtxJoints[0].Pos(); return __Vector3(0,0,0); }
	int				LODLevel() { return m_Chr.m_nLOD; }

	__Vector3		Max();
	__Vector3		Min();
	__Vector3		Center();

	void			DurationColorSet(const _D3DCOLORVALUE& color, float fDurationTime); // �÷��� ���ϴ� �ð���� �����ϸ鼭 ��������� ���ư���.
	void			FlickerFactorSet(float fAlpha);

	void			InfoStringSet(const std::string& szInfo, D3DCOLOR crFont);
	void			BalloonStringSet(const std::string& szBalloon, D3DCOLOR crFont);
	void			IDSet(int iID, const std::string& szID, D3DCOLOR crID);
	virtual void	KnightsInfoSet(int iID, const std::string& szName, int iGrade, int iRank);
	const std::string&	IDString() { return m_InfoBase.szID; } // ID �� Character �������� �̸����� ����Ѵ�.
	int				IDNumber() { return m_InfoBase.iID; }
	CPlayerBase*	TargetPointerCheck(bool bMustAlive);

	////////////////////
	// �浹 üũ �Լ���...
	bool				CheckCollisionByBox(const __Vector3& v0, const __Vector3& v1, __Vector3* pVCol, __Vector3* pVNormal);
	bool				CheckCollisionToTargetByPlug(CPlayerBase* pTarget, int nPlug, __Vector3* pVCol);

	virtual bool			InitChr(__TABLE_PLAYER_LOOKS* pTbl);
	virtual void			InitHair() {};
	virtual void			InitFace() {};
	CN3CPart*				Part(e_PartPosition ePos) { return m_Chr.Part(ePos); }
	CN3CPlugBase*			Plug(e_PlugPosition ePos) { return m_Chr.Plug(ePos); }
	virtual CN3CPart*		PartSet(e_PartPosition ePos, const std::string& szFN, __TABLE_ITEM_BASIC* pItemBasic, __TABLE_ITEM_EXT* pItemExt);
	virtual CN3CPlugBase*	PlugSet(e_PlugPosition ePos, const std::string& szFN, __TABLE_ITEM_BASIC* pItemBasic, __TABLE_ITEM_EXT* pItemExt);
	virtual void			DurabilitySet(e_ItemSlot eSlot, int iDurability);

	void TickYaw();				// ȸ���� ó��.
	void TickAnimation();		// ���ϸ��̼� ó��.
	void TickDurationColor();	// ĳ���� ���� ��ȭ ó��.
	void TickSound();			// Sound ó��..

	virtual void		Tick();
	virtual void		Render(float fSunAngle);
#ifdef _DEBUG
	virtual void		RenderCollisionMesh() { m_Chr.RenderCollisionMesh(); }
#endif
	void				RenderChrInRect(CN3Chr* pChr, const RECT& Rect);	// Dino �߰�, ������ �簢���ȿ� ĳ���͸� �׸���. 

	void				Release();

	CPlayerBase();
	virtual				~CPlayerBase();

	int					GetNPCOriginID() {	if (m_pLooksRef) return m_pLooksRef->dwID; else return -1;	}
};

#endif // !defined(AFX_PlayerBase_H__B8B8986B_3635_462D_8C38_A052CA75B331__INCLUDED_)
