// N3Star.h: interface for the CN3Star class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3STAR_H__09250C74_48BB_4F81_9903_45EAE7605CB6__INCLUDED_)
#define AFX_N3STAR_H__09250C74_48BB_4F81_9903_45EAE7605CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

const int	MAX_STAR = 200;

class CN3Star : public CN3Base  
{
public:
	CN3Star();
	virtual ~CN3Star();

// Attributes
public:
	int		GetNumStar() const {return m_iCurNumStar;}
protected:
	int		m_iCurNumStar;		// ���� ���ִ� ���� ��
	int		m_iNextNumStar;		// ������ ���� �� ����
	float	m_fAddTime;			// �� �ϳ��� �߰��ǰų� �������� �ð� ����
	BOOL	m_bIncrease;		// ���� ������ ���ΰ� ������ ���ΰ�
	__VertexXyzColor	m_Stars[MAX_STAR];
// Operations
public:
	void		Init();
	virtual void Release();
	virtual void Tick();
	virtual void Render();

	void		SetStar(int iNum, float fSec);
};

#endif // !defined(AFX_N3STAR_H__09250C74_48BB_4F81_9903_45EAE7605CB6__INCLUDED_)
