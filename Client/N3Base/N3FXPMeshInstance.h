// N3FXPMeshInstance.h: interface for the CN3FXPMeshInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3FXPMESHINSTANCE_H__19060ED0_7DDA_4279_A558_B586319324F0__INCLUDED_)
#define AFX_N3FXPMESHINSTANCE_H__19060ED0_7DDA_4279_A558_B586319324F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"
#include "N3PMesh.h"
#include "N3FXPMesh.h"

#include <string>

class CN3FXPMeshInstance   : public CN3Base
{
public:
	CN3FXPMeshInstance();
	CN3FXPMeshInstance(CN3FXPMesh *pN3FXPMesh);
	CN3FXPMeshInstance(const std::string& szFN);
	virtual ~CN3FXPMeshInstance();

protected:
	__VertexXyzColorT1* m_pColorVertices;
	WORD*				m_pIndices;

	// number of triangles/vertices to use at this moment.
	int					m_iNumIndices;
	int					m_iNumVertices; 

	CN3PMesh::__EdgeCollapse*	m_pCollapseUpTo;	// ���� ������

	// A pointer to the original mesh. This is where you'll find the vertex data (which
	// is invariant between PMRInstance's)
	CN3FXPMesh*	m_pFXPMesh;

public:
	void		SetColor(DWORD dwColor = 0xffffffff);
	BOOL		IsLOD() { if(NULL == m_pCollapseUpTo) return FALSE; return TRUE; }
	bool		Create(CN3FXPMesh* pN3FXPMesh);
	bool		Create(const std::string& szFN);
	void		Release();
	void		Render();
	void		RenderTwoUV();
	// The value you pass into this could be based on distance from camera, frame rate, etc
	void		SetLOD(float value); // value = Camera Distance / Camera FOV;

	// Or you can choose to collapse/split one step at a time. These return false if there was
	// no more collapsing/splitting to do.
	bool		CollapseOne();
	bool		SplitOne();

	void		SetLODByNumVertices(int iNumVertices);
	int			GetNumVertices() const {return m_iNumVertices;};
	int			GetNumIndices() const {return m_iNumIndices;};
	CN3FXPMesh*	GetMesh() {return m_pFXPMesh;};

	__VertexXyzColorT1*	GetVertices() const;
	WORD*				GetIndices() const { return m_pIndices;};
};

#endif // !defined(AFX_N3FXPMESHINSTANCE_H__19060ED0_7DDA_4279_A558_B586319324F0__INCLUDED_)
