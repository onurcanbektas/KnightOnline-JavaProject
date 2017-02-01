// N3VMesh.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3VMesh_h__INCLUDED_)
#define AFX_N3VMesh_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3BaseFileAccess.h"

class CN3IMesh;

class CN3VMesh : public CN3BaseFileAccess
{
protected:
	__Vector3*	m_pVertices; // �� ����
	int			m_nVC; // �� ����

	WORD*		m_pwIndices; // Index...
	int			m_nIC; // Index Count

	__Vector3	m_vMin;
	__Vector3	m_vMax;
	float		m_fRadius; // ������

public:
	__Vector3	Min() { return m_vMin; }
	__Vector3	Max() { return m_vMax; }
	bool		Pick(const __Matrix44& MtxWorld, const __Vector3& vPos, const __Vector3& vDir, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL);
	bool		CheckCollision(const __Matrix44& MtxWorld, const __Vector3& v0, const __Vector3& v1, __Vector3* pVCol = NULL, __Vector3* pVNormal = NULL); // Local ��ǥ�� ���� �浹 üũ ����..
	float		Radius() { return m_fRadius; }
	void		FindMinMax();
	void		Render(D3DCOLOR crLine);
	void		CreateVertices(int nVC);
	void		CreateIndex(int nIC);
	void		CreateCube(const __Vector3& vMin, const __Vector3& vMax);
	
	int			VertexCount() { return m_nVC; }
	__Vector3*	Vertices() { return m_pVertices; }

	int			IndexCount() { return m_nIC; }
	WORD*		Indices() { return m_pwIndices; }

	bool		Load(HANDLE hFile);
#ifdef _N3TOOL
	bool		Import(CN3IMesh *pIMesh);
	bool		Save(HANDLE hFile);
#endif // end of _N3TOOL
	
	void		Release();

	CN3VMesh();
	virtual ~CN3VMesh();

//	By : Ecli666 ( On 2002-08-09 ���� 2:47:59 )
//
	int				GetColIndexByiOrder(int iOrder);
__Vector3	  GetColVertexByIndex(int iIndex); 
	void			PartialColRender(int iCount, int* piIndices);
	void			PartialGetCollision(int iIndex, __Vector3& vec);
//	~(By Ecli666 On 2002-08-09 ���� 2:47:59 )
};

#endif // !defined(AFX_N3VMesh_h__INCLUDED_)
