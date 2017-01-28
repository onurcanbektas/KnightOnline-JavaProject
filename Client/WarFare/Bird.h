// Bird.h: interface for the CBird class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIRD_H__409CEF6D_6577_4CFB_90C9_6A8AA710D298__INCLUDED_)
#define AFX_BIRD_H__409CEF6D_6577_4CFB_90C9_6A8AA710D298__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

class CN3Shape;
class CN3SndObj;

class CBird : public CN3Base  
{
public:
	CBird();
	virtual ~CBird();

// Attributes
public:
protected:
	// ���Ͽ��� �о���� ����
	CN3Shape*	m_pShape;		// Shape pointer
	__Vector3	m_vPivot;		// �߽���(�߽������� �ݰ� m_fRadius��ŭ ���������� �����δ�.
	float		m_fRadius;		// �ݰ�

//	__Vector3	m_vVelocity;	// ���� �ӵ�(���ǵ�+����)
	float		m_fRadianSpeed;		// �ӷ�
	float		m_fRadian;		// ���� Ÿ���� ��ġ��
	float		m_fFactor1,	m_fFactor2;
	float		m_fFactorSpeed1,	m_fFactorSpeed2;

	float		m_fRadiusY;		// ���� �ݰ�
	float		m_fFactorY1, m_fFactorY2;	// ���� ��ȭ ���
	float		m_fFactorYSpeed1, m_fFactorYSpeed2;	// ���� ��ȭ ��� ��ȭ��..

	CN3SndObj*	m_pSnd;
	float		m_fSndInterval;

// Operations
public:
	virtual void Release();
	void	Tick();
	void	Render();

	int		LoadBird(const std::string& szFN);
protected:

};

#endif // !defined(AFX_BIRD_H__409CEF6D_6577_4CFB_90C9_6A8AA710D298__INCLUDED_)
