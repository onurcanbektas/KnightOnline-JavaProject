// PlayerNPC.cpp: implementation of the CPlayerNPC class.
//
//////////////////////////////////////////////////////////////////////

//#include "StdAfx.h"
#include "PlayerNPC.h"
#include "GameProcMain.h"
#include "N3WorldManager.h"
#include "N3Shape.h"
#include "N3ShapeMgr.h"
#include "N3SndObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayerNPC::CPlayerNPC()
{
	m_ePlayerType = PLAYER_NPC; // Player Type ... Base, NPC, OTher, MySelf
	m_vPosFromServer = m_Chr.Pos(); // �������� ���� ��ġ.
}

CPlayerNPC::~CPlayerNPC()
{
}

void CPlayerNPC::Tick()
{
	if(m_pShapeExtraRef) // ������Ʈ�̸�..
	{
		CPlayerBase::Tick();
		return;
	}

	if(m_fTimeAfterDeath > 0) // �׾���ϰų� ���� ���̸�...
	{
		if(m_fTimeAfterDeath > 3.0f)
			this->Action(PSA_DYING, false); // 5 �ʰ� ������ �״´�.
		CPlayerBase::Tick();  // ȸ��, ������ ���ϸ��̼� Tick �� ���� ���� ó��.. ���..
		return;
	}

	__Vector3 vPos = m_Chr.Pos();
	if(	m_vPosFromServer.x != vPos.x || m_vPosFromServer.z != vPos.z ) // ���� �� �����Ѵ�.
	{
		if(m_fMoveSpeedPerSec == 0)
		{
			__Vector3 vT1(m_vPosFromServer.x, 0, m_vPosFromServer.z), vT2(vPos.x, 0, vPos.z);
			m_fMoveSpeedPerSec = (vT2 - vT1).Magnitude();
		}

		__Vector3 vOffset = m_vPosFromServer - vPos; vOffset.y = 0;
		__Vector3 vDir = vOffset; vDir.Normalize();  // ����.. 

		float fSpeedAbsolute = (m_fMoveSpeedPerSec > 0) ? m_fMoveSpeedPerSec : -m_fMoveSpeedPerSec; // �ӵ� ���밪
		float fDist = vOffset.Magnitude(); // �Ÿ�
		if(fDist < fSpeedAbsolute * CN3Base::s_fSecPerFrm) // �����̴� �Ÿ��� ���� �ٿ°Ÿ�..
		{
			vPos.x = m_vPosFromServer.x; // ��ġ�� �������ְ�..
			vPos.z = m_vPosFromServer.z; // ��ġ�� �������ְ�..
//			m_fMoveSpeedPerSec = 0; // �����̴� �ӵ��� 0����!
			this->ActionMove(PSM_STOP); // �����̴� ��� ó��..
		}
		else 
		{
			float fYaw = (m_fMoveSpeedPerSec < 0) ? ::_Yaw2D(-vDir.x, -vDir.z) : ::_Yaw2D(vDir.x, vDir.z); // ������ ����ؼ�..
			this->RotateTo(fYaw, false); // ���� �������� ������..

			e_StateMove eMove = PSM_STOP; // ������...
			// �÷��̾�� �ȴ� �ӵ��� ���� �������� ��ġ�� �ݺ��..
			float fStandWalk = ((PLAYER_OTHER == m_ePlayerType) ? (MOVE_SPEED_WHEN_WALK * 2.0f) : (MOVE_SPEED_WHEN_WALK * m_Chr.Radius() * 2.0f));
			if(m_fMoveSpeedPerSec < 0) eMove = PSM_WALK_BACKWARD; // �ڷ� �ȱ�..
			else if(m_fMoveSpeedPerSec < fStandWalk) eMove = PSM_WALK; // ������ �ȱ�..
			else eMove = PSM_RUN; // if(fDN > 5.0f) // ���� ��ġ�� �Ÿ��� ���� �̻��̸� �پ��.
			this->ActionMove(eMove); // �����̴� ��� ó��..

			vPos += vDir * (fSpeedAbsolute * s_fSecPerFrm); // �̵�..
		}

		float fYTerrain = ACT_WORLD->GetHeightWithTerrain(vPos.x, vPos.z); // ������ ���̰�..
		float fYMesh = ACT_WORLD->GetHeightNearstPosWithShape(vPos, 1.0f); // �浹 üũ ������Ʈ�� ���̰�..
		if(fYMesh != -FLT_MAX && fYMesh > fYTerrain && fYMesh < m_fYNext + 1.0f) m_fYNext = fYMesh; // �ö󰥼� �ִ� ������Ʈ�̰� ���̰��� ���麸�� ������.
		else m_fYNext = fYTerrain;
		this->PositionSet(vPos, false); // ��ġ ���� ����..
	}

	if(PSA_ATTACK == m_eState || m_iSkillStep != 0) // ���� ���̰ų� ��ų ������̸�..
	{
		CPlayerBase* pTarget = this->TargetPointerCheck(false);
		CPlayerBase::ProcessAttack(pTarget); // ���ݿ� ���� ��ƾ ó��.. ���ϸ��̼� ���ð� �浹�� ó���һ� ��Ŷ�� ó�� ���Ѵ�..
	}

	CPlayerBase::Tick(); // ȸ��. �̵�, ���ϸ��̼� ƽ.. ���� �ٲ� ���� ó���Ѵ�.
}

void CPlayerNPC::MoveTo(float fPosX, float fPosY, float fPosZ, float fSpeed, int iMoveMode)
{
	m_vPosFromServer.Set(fPosX, fPosY, fPosZ);
	if(m_pShapeExtraRef) return; // ������Ʈ �����̸� �����ϼ��� ����..

	// iMoveMode : ���� �����̴� ����.. 0 -���� 1 �����ӽ��� 2 - 1�ʸ��� �ѹ� ���ӿ�����..
	if(0 == iMoveMode) 
	{
	}
	else if(iMoveMode) // ������ ����.. // ��� ������
	{
		m_fMoveSpeedPerSec = fSpeed;
		__Vector3 vPos = m_Chr.Pos(); vPos.y = 0;
		__Vector3 vPosS(fPosX, 0, fPosZ);
	
		if(fSpeed) m_fMoveSpeedPerSec *= ((vPosS - vPos).Magnitude() / (fSpeed * PACKET_INTERVAL_MOVE)) * 0.85f; // �ӵ�����.. ����ȭ������ �׷���.. �ణ �ٿ��ִ� ������ ��ĩ�ϴ°� �����ϱ� ���ؼ��̴�..
		else m_fMoveSpeedPerSec = ((vPosS - vPos).Magnitude() / (fSpeed * PACKET_INTERVAL_MOVE)) * 0.85f; // �ӵ�����.. ����ȭ������ �׷���.. �ణ �ٿ��ִ� ������ ��ĩ�ϴ°� �����ϱ� ���ؼ��̴�..
		if(fSpeed < 0) m_fMoveSpeedPerSec *= -1.0f; // �ڷ� ����..
	}
	else
	{
//		__ASSERT(0, "Invalid Move Mode");
	}
}
