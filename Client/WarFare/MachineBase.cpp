// MachineBase.cpp: implementation of the CMachineBase class.
//
//////////////////////////////////////////////////////////////////////

//#include "StdAfx.h"
#include "MachineBase.h"
#include "N3Terrain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMachineBase::CMachineBase()
{
	m_fRotateSpeed = 0.15f;
	m_fSpeed = 1.0f;
	m_bSkipCalcPartMtx = NULL;
	Release();
}

CMachineBase::~CMachineBase()
{
	Release();
}

void CMachineBase::Release()
{
	CN3Shape::Release();
	ZeroMemory( m_Wheel, sizeof(m_Wheel) );
	m_dwMachineState = MS_STOP;
	m_fDirRadian = 0.0f;
	m_szID = "";
	if (m_bSkipCalcPartMtx) {delete [] m_bSkipCalcPartMtx; m_bSkipCalcPartMtx = NULL;}

	m_fFireRadian = 0.0f;
	m_fFireSpeed = 0.0f;
}

void CMachineBase::ReCalcMatrix4AnimatedPart()
{
	// ���� ��Ʈ�� ��Ʈ������ �ٽ� ����� �ش�..
	int iPC = m_Parts.size();
	for(int i = 0; i < iPC; i++)
	{
		for (int j=0; j<NUM_WHEEL; ++j)
		{
			if (m_Parts[i] == m_Wheel[j].pPart)
			{
				m_Parts[i]->m_Matrix.RotationX(m_Wheel[j].fRadian);
				m_Parts[i]->m_Matrix.PosSet(m_Parts[i]->m_vPivot);
				m_Parts[i]->m_Matrix *= m_Matrix;
				m_bSkipCalcPartMtx[j] = TRUE;
				break;
			}
		}
	}

}

void CMachineBase::ReCalcMatrix()
{
//	CN3Transform::ReCalcMatrix(); // Transfomr Matrix �� ��� ���ְ�..
/*
	// ������� ���� ���
	static __Matrix44 mtx1;	static __Matrix44 mtx2;
	mtx1.Scale(m_vScale);	mtx2.Rotation(0, m_fDirRadian, 0);
	mtx1 *= mtx2;	mtx1.PosSet(m_vPos);

	float fPitch=0, fRoll=0;
	CN3Terrain* pTerrain =  CN3GameBase::s_pArith->m_pTerrain;
	__Vector3 vPos1 = m_vBalancePoint[0]*mtx1;
	__Vector3 vPos2 = m_vBalancePoint[1]*mtx1;
	vPos1.y = pTerrain->GetHeight(vPos1.x, vPos1.z);
	vPos2.y = pTerrain->GetHeight(vPos2.x, vPos2.z);
	if (vPos1.y != -FLT_MAX && vPos2.y != -FLT_MAX) fPitch = asinf((vPos2.y - vPos1.y)/(vPos1-vPos2).Magnitude());	// �������� ���϶� ���
	vPos1 = m_vBalancePoint[2]*mtx1;
	vPos2 = m_vBalancePoint[3]*mtx1;
	vPos1.y = pTerrain->GetHeight(vPos1.x, vPos1.z);
	vPos2.y = pTerrain->GetHeight(vPos2.x, vPos2.z);
	if (vPos1.y != -FLT_MAX && vPos2.y != -FLT_MAX) fRoll = asinf((vPos2.y - vPos1.y)/(vPos1-vPos2).Magnitude());	// �������� ���϶��� ���

	// machine�� ������, ȸ��, ��ġ ���
	D3DXMatrixRotationYawPitchRoll(&mtx1, -m_fDirRadian, fPitch, fRoll);
	// -m_fDirRadian : -�� �ٿ��ִ� ������ N3Base�� ���ǵ� matrix����� d3d�� ������ �ݴ�� �Ǿ� �־.. 

	m_Matrix.Scale(m_vScale);
	m_Matrix *= mtx1;
	m_Matrix.PosSet(m_vPos);

	ReCalcMatrix4AnimatedPart();

	// Animate ���� �ʴ� ������ ��Ʈ�� ��Ʈ������ �ٽ� ���.
	int iPC = m_Parts.size();
	for(int i = 0; i < iPC; i++)
	{
		if (m_bSkipCalcPartMtx[count] == FALSE)	m_Parts[i]->RecalcMatrix(m_Matrix);
		++count;
	}
*/
}

void CMachineBase::Render()
{
	CN3Shape::Render();
	if (m_bSkipCalcPartMtx) ZeroMemory(m_bSkipCalcPartMtx, sizeof(m_bSkipCalcPartMtx[0])*PartCount());		// �׸��� part matrix��� �÷��� �ʱ�ȭ
}

void CMachineBase::Tick(float fFrm)
{
/*	CN3Shape::Tick(fFrm);
	if (m_bDontRender) return;

	// ȸ�� ���
	if (!((m_dwMachineState & MS_TURNRIGHT) && (m_dwMachineState & MS_TURNLEFT)))	// �¿� ���ð� �ƴϸ�
	{
		float fAddRadian = 0.0f;
		if (m_dwMachineState & MS_TURNRIGHT)
		{
			fAddRadian = -m_fRotateSpeed*s_fSecPerFrm;
			m_fDirRadian += fAddRadian;
			if (m_fDirRadian < 0) m_fDirRadian += (2 * D3DX_PI);
		}
		else if (m_dwMachineState & MS_TURNLEFT)
		{
			fAddRadian = m_fRotateSpeed*s_fSecPerFrm;
			m_fDirRadian += fAddRadian;
			if (m_fDirRadian > (2 * D3DX_PI)) m_fDirRadian -= (2 * D3DX_PI);
		}
		// ���� ȸ���� ���.
		int i;
		for (i=0; i<NUM_WHEEL; ++i)
		{
			m_Wheel[i].fRadian += fAddRadian*m_Wheel[i].fRotateRatio;
		}
	}

	float f90d = D3DXToRadian(90.0f);
	__Vector3 vDir;	vDir.Set(cosf(f90d+m_fDirRadian), 0, sinf(f90d+m_fDirRadian));
	__Vector3 vPosDiff;	vPosDiff.Set(0,0,0);

	// ���� �̵� ���.
	if (!((m_dwMachineState & MS_FORWARD) && (m_dwMachineState & MS_BACKWARD)))	// �յ� ���ð� �ƴϸ�
	{
		float fDistance = 0.0f;
		if (m_dwMachineState & MS_FORWARD) fDistance = (m_fSpeed*s_fSecPerFrm);
		else if (m_dwMachineState & MS_BACKWARD) fDistance = (-m_fSpeed*s_fSecPerFrm);

		if (fDistance != 0.0f)
		{
			vPosDiff = vDir*fDistance;

			// ������ ���� y��ǥ ���
			__Vector3 vPos = m_vPos + vPosDiff;
			vPos.y = CN3GameBase::s_pArith->m_pTerrain->GetHeight(vPos.x, vPos.z);
			m_vPos = vPos;

			// ���� ȸ���� ���.
			int i;
			for (i=0; i<NUM_WHEEL; ++i)
			{
				m_Wheel[i].fRadian -= (fDistance/m_Wheel[i].fRadius);
			}
		}
	}

	// ���� ȸ���� 0~2pi ���̷� ����
	int i;
	for (i=0; i<NUM_WHEEL; ++i)
	{
		if (m_Wheel[i].fRadian > D3DX_PI*2)	m_Wheel[i].fRadian -= (D3DX_PI*2);
		else if (m_Wheel[i].fRadian < 0.0f)	m_Wheel[i].fRadian += (D3DX_PI*2);
	}

	// Rotate ���
//	m_vRot.y = m_fDirRadian;

	// �̵� �� matrix���.
	ReCalcMatrix();
*/
}

CN3SPart* CMachineBase::GetPartByPMeshName(const std::string& szName)
{
	if(szName.empty()) return NULL;

	int iPC = m_Parts.size();
	for(int i = 0; i < iPC; i++)
	{
		CN3PMesh* pPMesh = m_Parts[i]->Mesh();
		if (pPMesh == NULL) continue;
		if (pPMesh->m_szName == szName) return m_Parts[i];
	}
	
	return NULL;
}

void CMachineBase::LoadMachine(FILE* stream)
{
	if (stream == NULL) return;

	Release();
	char szSrcName[_MAX_PATH];	// shape source���� �̸�
	char szWheel[NUM_WHEEL][_MAX_PATH];	// ���� pmesh�̸�

	int result;
	result = fscanf(stream, "Speed = %f\n", &m_fSpeed);	__ASSERT(result != EOF, "�߸��� Machine ���� ����");
	result = fscanf(stream, "RotateSpeed = %f\n", &m_fRotateSpeed);	__ASSERT(result != EOF, "�߸��� Machine ���� ����");
	result = fscanf(stream, "Shape_Name = %s\n", szSrcName);					__ASSERT(result != EOF, "�߸��� Machine ���� ����");
	result = fscanf(stream, "Wheel_FL = %s\n", szWheel[WHEEL_FL]);			__ASSERT(result != EOF, "�߸��� Machine ���� ����");
	result = fscanf(stream, "Wheel_FR = %s\n", szWheel[WHEEL_FR]);			__ASSERT(result != EOF, "�߸��� Machine ���� ����");
	result = fscanf(stream, "Wheel_BL = %s\n", szWheel[WHEEL_BL]);			__ASSERT(result != EOF, "�߸��� Machine ���� ����");
	result = fscanf(stream, "Wheel_BR = %s\n", szWheel[WHEEL_BR]);			__ASSERT(result != EOF, "�߸��� Machine ���� ����");
//	result = fscanf(stream, "WheelRadius_FL = %f\n", &(m_Wheel[WHEEL_FL].fRadius));	__ASSERT(result != EOF, "�߸��� Machine ���� ����");
//	result = fscanf(stream, "WheelRadius_FR = %f\n", &(m_Wheel[WHEEL_FR].fRadius));	__ASSERT(result != EOF, "�߸��� Machine ���� ����");
//	result = fscanf(stream, "WheelRadius_BL = %f\n", &(m_Wheel[WHEEL_BL].fRadius));	__ASSERT(result != EOF, "�߸��� Machine ���� ����");
//	result = fscanf(stream, "WheelRadius_BR = %f\n", &(m_Wheel[WHEEL_BR].fRadius));	__ASSERT(result != EOF, "�߸��� Machine ���� ����");

	// shape load�ϱ�
	this->Load(szSrcName);

	__ASSERT(m_bSkipCalcPartMtx == NULL, "Machine���� �޸� �� ���ɼ�");
	int iPartCount = PartCount();
	if (iPartCount>0) m_bSkipCalcPartMtx = new BOOL[iPartCount];
	ZeroMemory(m_bSkipCalcPartMtx, sizeof(m_bSkipCalcPartMtx[0])*iPartCount);

	// ������ ���� CN3SPart ������ ã��
	int i;
	for (i=0; i<NUM_WHEEL; ++i)
	{
		m_Wheel[i].pPart = GetPartByPMeshName(szWheel[i]);
		__ASSERT(m_Wheel[i].pPart, "Machine�� ���� ��Ʈ�� NULL�Դϴ�.");
		// ���� ������ ���ϱ�
		CN3PMesh* pPMesh = m_Wheel[i].pPart->Mesh();
		__ASSERT(pPMesh, "machine ������ PMesh�� �����.");
		m_Wheel[i].fRadius = (pPMesh->Max().y - pPMesh->Min().y)/2.0f;
	}

	// machine�� 1.0f(rad)ȸ���Ҷ� ������ ���ư��� ����(rad) ���� ����ϱ�
	for (i=0; i<NUM_WHEEL; ++i)
	{
		if (i == WHEEL_FL || i == WHEEL_BL)
			m_Wheel[i].fRotateRatio = m_Wheel[i].pPart->m_vPivot.Magnitude() / m_Wheel[i].fRadius;
		else
			m_Wheel[i].fRotateRatio = -m_Wheel[i].pPart->m_vPivot.Magnitude() / m_Wheel[i].fRadius;
	}

	// ������ ������� �� ����ϱ�
	m_vBalancePoint[0] =	m_Wheel[WHEEL_FL].pPart->m_vPivot +				// ��
							0.5*(m_Wheel[WHEEL_FR].pPart->m_vPivot - m_Wheel[WHEEL_FL].pPart->m_vPivot);
	m_vBalancePoint[1] =	m_Wheel[WHEEL_BL].pPart->m_vPivot +				// ��
							0.5*(m_Wheel[WHEEL_BR].pPart->m_vPivot - m_Wheel[WHEEL_BL].pPart->m_vPivot);
	m_vBalancePoint[2] =	m_Wheel[WHEEL_FL].pPart->m_vPivot +				// ��
							0.5*(m_Wheel[WHEEL_BL].pPart->m_vPivot - m_Wheel[WHEEL_FL].pPart->m_vPivot);
	m_vBalancePoint[3] =	m_Wheel[WHEEL_FR].pPart->m_vPivot +				// ��
							0.5*(m_Wheel[WHEEL_BR].pPart->m_vPivot - m_Wheel[WHEEL_FR].pPart->m_vPivot);
}

void CMachineBase::Fire()
{
}

