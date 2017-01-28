// Catapult.h: interface for the CCatapult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CATAPULT_H__9D404701_4B6D_4DFE_9BDD_7125FA9B3CE8__INCLUDED_)
#define AFX_CATAPULT_H__9D404701_4B6D_4DFE_9BDD_7125FA9B3CE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MachineBase.h"

class CCatapult : public CMachineBase
{
public:
	CCatapult();
	virtual ~CCatapult();

// Structure
public:
	struct __Thrower
	{
		// ���Ͽ��� �о���� ����
		CN3SPart*	pThrowerPart;			// Thrower�� ��Ʈ ������
		float	fLimitRadian;			// �Ѱ� ȸ�� ����
		float	fRadianAccel;			// ȸ�� ���ӵ�
		float	fRadianSpeed2Reload;	// ������ ȸ�� �ӵ�
		float	fReloadDelayTime;		// ���� �߻��� ������ �����ϱ������ �ð� delay
		float	fRecoilRadian;			// �߻��� �ݵ��ϴ� ����
		class CN3Shape*	pStone;			// �߻��ϴ� ������ shape ������
		__Vector3		vStoneOffset;	// ������ offset (thrower�� pivot ������ ������ �Ÿ�)

		// ���Ͽ��� ���� ������ ���� ��������� ����
		float	fReleaseTime;			// �߻��� �Ѱ� ȸ�� ������ �����ϱ������ �ð� [ sqrtf(fLimitRadian/fRadianAccel) ]

		// ���� �߿� ���Ǵ� ����
		float	fTime;					// �߻� ��� �ð�(��) �߽� �ð�0�� �������� ������.
		BOOL	bFire;					// �߻����ΰ�?
		float	fCurRadian;				// ���� Thrower�� ���ư� ȸ����
		BOOL	bDontRenderStone;		// �߻��ϴ� ���� �ȱ׸� ���ΰ�?
	};
// Attributes
public:
protected:
	__Thrower		m_Thrower;			// �߻��

// Operations
public:
	virtual void	Release();
	virtual void	Tick(float fFrm);
	virtual void	LoadMachine(FILE* stream);
	virtual void	Render();
protected:
	virtual void	ReCalcMatrix4AnimatedPart();
	void			ThrowStone(__Vector3& vPos, __Vector3& vVel, CN3Shape* pShape) const;

// �ӽ� ���� ����
public:
	virtual void	Fire();		// �߻�
};

#endif // !defined(AFX_CATAPULT_H__9D404701_4B6D_4DFE_9BDD_7125FA9B3CE8__INCLUDED_)
