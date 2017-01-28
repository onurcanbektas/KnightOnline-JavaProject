// MachineBase.h: interface for the CMachineBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MACHINEBASE_H__CA5C8B1C_9A41_465B_875E_979A9BCD0483__INCLUDED_)
#define AFX_MACHINEBASE_H__CA5C8B1C_9A41_465B_875E_979A9BCD0483__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

const DWORD MS_STOP		= 0x00000000;
const DWORD MS_FORWARD	= 0x00000001;
const DWORD MS_BACKWARD = 0x00000002;
const DWORD MS_TURNRIGHT= 0x00000004;
const DWORD MS_TURNLEFT = 0x00000008;

const DWORD MACHINE_TYPE_CATAPULT = 0x21;

class CN3Texture;
class CN3PMeshInstance;

#include "N3Shape.h"

class CMachineBase : public CN3Shape
{
public:
	CMachineBase();
	virtual ~CMachineBase();

// Structure
public:
	enum {WHEEL_FL=0, WHEEL_FR=1, WHEEL_BL=2, WHEEL_BR=3, NUM_WHEEL=4};
	struct __Wheel
	{
		// ���� ����
//		__Vector3			m_vPivot;	// Local ��
		CN3SPart*				pPart;	// Shape�� ���� Part������

		// ���� ���� �Է¹��� �� ������ִ� ����.
		float				fRadius;	// ������ ������
		float				fRotateRatio;// machine�� 1.0f(rad)ȸ���Ҷ� ������ ���ư��� ����(rad) ����

		// ���� ����� �ٲ�� ����.
		float				fRadian;	// ���� ������ ���ư� ����. (x������ ������.)
	};

	std::string 	m_szID;				// id string

protected:
	__Wheel			m_Wheel[NUM_WHEEL];
	float			m_fSpeed;			// �ʴ� �̵��ӵ�(������)
	float			m_fRotateSpeed;		// �ʴ� ȸ�� ���ӵ�
	//__Vector3		m_vDir;				// machine�� �������
	float			m_fDirRadian;		// ������� ����(z���� ������ 0.0f);
	DWORD			m_dwMachineState;
	BOOL*			m_bSkipCalcPartMtx;		// ��Ʈ ��Ʈ���� ��� �ǳʶٱ�(part������ŭ �Ҵ��)
	__Vector3		m_vBalancePoint[4];		// ������ ������� 4���� ��ġ(�����¿�) ������ǥ��.

	float			m_fFireRadian;		// �߻簢
	float			m_fFireSpeed;		// �߻�ӵ�(����)

// Attributes
public:

	void			SetFireRadian(float fFireRadian) {m_fFireRadian = fFireRadian;}
	void			SetFireSpeed(float fFireSpeed) {m_fFireSpeed = fFireSpeed;}
	void			SetDirRadian(float fDirRadian) {m_fDirRadian = fDirRadian; ReCalcMatrix();}
	DWORD			GetMachineState() const {return m_dwMachineState;}

// Operations
public:
	virtual void	Release();
	virtual void	Render();
	virtual void	ReCalcMatrix();
	virtual void	Tick(float fFrm);
	virtual void	LoadMachine(FILE* stream);

protected:
	CN3SPart*		GetPartByPMeshName(const std::string& szName);
	virtual void	ReCalcMatrix4AnimatedPart();
// �ӽ� ���� ����
public:
	void Stop() {m_dwMachineState = MS_STOP;}
	void ToggleMoveForward(BOOL bOn) {if (bOn) m_dwMachineState |= MS_FORWARD; else m_dwMachineState &= (~MS_FORWARD);}
	void ToggleMoveBackward(BOOL bOn) {if (bOn) m_dwMachineState |= MS_BACKWARD; else m_dwMachineState &= (~MS_BACKWARD);}
	void ToggleTurnRight(BOOL bOn) {if (bOn) m_dwMachineState |= MS_TURNRIGHT; else m_dwMachineState &= (~MS_TURNRIGHT);}
	void ToggleTurnLeft(BOOL bOn) {if (bOn) m_dwMachineState |= MS_TURNLEFT; else m_dwMachineState &= (~MS_TURNLEFT);}

	virtual void Fire();		// �߻�
};

#endif // !defined(AFX_MACHINEBASE_H__CA5C8B1C_9A41_465B_875E_979A9BCD0483__INCLUDED_)
