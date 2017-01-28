// AniKeyPos.h: interface for the CAniKeyPos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3AnimKey_h__INCLUDED_)
#define AFX_N3AnimKey_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

enum ANIMATION_KEY_TYPE { 
	KEY_VECTOR3 = 0,
	KEY_QUATERNION = 1,
	KEY_UNKNOWN = 0xffffffff
}; 

class CN3AnimKey : CN3Base
{
	friend class CN3Joint;
protected:
	ANIMATION_KEY_TYPE m_eType; // Key Type - Position Rotation Scale
	int		m_nCount; // Ű ī��Ʈ
	float	m_fSamplingRate; // Sampling Rate - ǥ���� 30 Frame Per Sec �̴�..
	void*	m_pDatas;

public:
	ANIMATION_KEY_TYPE Type() { return m_eType; } // Ű ����, ���Ϳ� ���ʹϾ� ���°� �ִ�..
	void	Add(CN3AnimKey& AKSrc, int nIndexS, int nIndexE);
//	void	Translate(int nIndexStart, int nIndexEnd, float fDelta, int nKFlags);
	bool DataGet(float fFrm, __Vector3& v)
	{
		if(KEY_VECTOR3 != m_eType) return false;
		if(m_nCount <= 0) return false;
		
		float fD = 30.0f / m_fSamplingRate;
		
		int nIndex = (int)(fFrm * (m_fSamplingRate / 30.0f));
		if(nIndex < 0 || nIndex > m_nCount) return false;
		
		float fDelta = 0;
		if(nIndex == m_nCount) { nIndex = m_nCount - 1; fDelta = 0; }
		else fDelta = (fFrm - nIndex * fD)/fD;
		
		if(0.0f != fDelta) { v = (((__Vector3*)m_pDatas)[nIndex] * (1.0f - fDelta)) + (((__Vector3*)m_pDatas)[nIndex+1] * fDelta); }
		else v = ((__Vector3*)m_pDatas)[nIndex];
		return true;
	}
	bool DataGet(float fFrm, __Quaternion& q)
	{
		if(KEY_QUATERNION != m_eType) return false;
		if(m_nCount <= 0) return false;

		float fD = 30.0f / m_fSamplingRate;

		int nIndex = (int)(fFrm * (m_fSamplingRate / 30.0f));
		if(nIndex < 0 || nIndex > m_nCount) return false;

		float fDelta = 0;
		if(nIndex == m_nCount) { nIndex = m_nCount - 1; fDelta = 0; }
		else fDelta = (fFrm - nIndex * fD)/fD;

		if(0.0f != fDelta) q.Slerp(((__Quaternion*)m_pDatas)[nIndex], ((__Quaternion*)m_pDatas)[nIndex+1], fDelta);
		else q = ((__Quaternion*)m_pDatas)[nIndex];
		return true;
	}
	
	bool Load(HANDLE hFile);

#ifdef _N3TOOL
	void*	DataGet(int index) 
	{
		if(index < 0 || index >= m_nCount) return NULL;
		else if(KEY_VECTOR3 == m_eType) return &(((__Vector3*)m_pDatas)[index]);
		else if(KEY_QUATERNION == m_eType) return &(((__Quaternion*)m_pDatas)[index]);
		else return NULL;
	}
	bool Save(HANDLE hFile);
#endif // end of
	
	int Count() { return m_nCount; }
	float SamplingRate() { return m_fSamplingRate; }
	void Release();
	void Alloc(int nCount, float fSamplingRate = 30.0f, ANIMATION_KEY_TYPE eType = KEY_VECTOR3);
	
	void Duplicate(CN3AnimKey* pSrc);
	void*	GetDatas() { return m_pDatas; }
	void MultiplyDataVector(__Vector3 vM);
	
	CN3AnimKey();
	virtual ~CN3AnimKey();
};

#endif // !defined(AFX_N3AnimKey_h__INCLUDED_)
