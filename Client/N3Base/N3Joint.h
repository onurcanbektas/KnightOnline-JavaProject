// N3Joint.h: interface for the CN3Joint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3IJoint_h__INCLUDED_)
#define AFX_N3IJoint_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include "N3Transform.h"

#include <list>
class CN3Joint;
typedef std::list<CN3Joint*>::iterator	it_Joint;

const int MAX_JOINT_TRANSFORM = 64;

class CN3Joint : public CN3Transform
{
	friend class CN3Joint; // Position, Rotation, Scale ������ �����Ӱ� �ٷ�� �־�� �Ѵ�..
public:
	__Quaternion			m_qOrient;		// Joint Orient Quaternion
	CN3AnimKey				m_KeyOrient;	// Joint Orient Ű��... NULL �̸� ���°Ŵ�..

protected:
	CN3Joint*				m_pParent;
	std::list<CN3Joint*>	m_Children;

public:
	void ChildDelete(CN3Joint* pChild);
#ifdef _N3TOOL
	void CopyExceptAnimationKey(CN3Joint* pJSrc);
	void AddKey(CN3Joint* pJSrc, int nIndexS, int nIndexE);
	void KeyDelete(CN3Joint* pJoint, int nKS, int nKE);
#endif // end of _N3TOOL

	void MatricesGet(__Matrix44 *pMtxs, int& nJointIndex);
	
	void Tick(float fFrm);
	bool TickAnimationKey(float fFrm);
	void ReCalcMatrix();
	void ReCalcMatrixBlended(float fFrm0, float fFrm1, float fWeight0);
	void ParentSet(CN3Joint* pParent);
	void ChildAdd(CN3Joint* pChild);
	CN3Joint* Parent() { return m_pParent; }
	int ChildCount() { return m_Children.size(); }
	CN3Joint* Child(int index)
	{
		if(index < 0 || index > m_Children.size()) return NULL;
		it_Joint it = m_Children.begin();
		for(int i = 0; i < index; i++, it++);
		return *it;
	}

	void NodeCount(int &nCount);
	BOOL FindPointerByID(int nID, CN3Joint *&pJoint);
#ifdef _N3TOOL
	BOOL FindIndex(const std::string& szName, int &nIndex);
	BOOL FindPointerByName(const std::string& szName, CN3Joint *&pJoint); // �̸��� ������ �ش� ����� �����͸� �����ش�..
	void RotSet(const __Quaternion& qtRot) { m_qRot = qtRot; this->ReCalcMatrix(); }
	void RotSet(float x, float y, float z, float w) { m_qRot.x = x; m_qRot.y = y; m_qRot.z = z; m_qRot.w = w; this->ReCalcMatrix(); }
	void Render(const __Matrix44* pMtxParent = NULL, float fUnitSize = 0.1f);
#endif // end of _N3TOOL

	void Release();
	bool Load(HANDLE hFile);
#ifdef _N3TOOL
	bool Save(HANDLE hFile);
#endif // end of _N3TOOL

	CN3Joint();
	virtual ~CN3Joint();

};

#endif // !defined(AFX_N3IJoint_h__INCLUDED_)
