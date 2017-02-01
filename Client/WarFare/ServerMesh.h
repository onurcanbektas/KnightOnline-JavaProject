// ServerMesh.h: interface for the CServerMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERMESH_H__5D0D1E4F_E427_462E_9D72_CB0C4FC02C82__INCLUDED_)
#define AFX_SERVERMESH_H__5D0D1E4F_E427_462E_9D72_CB0C4FC02C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

class CN3Terrain;

class CServerMesh   : public CN3Base  
{
	__Vector3		m_vSMesh[48];
	void	AutoConcMesh(D3DCOLOR color, float left, float right, float bottom, float top, float low, float high, int iStart );

public:
	CServerMesh();
	virtual ~CServerMesh();

	void	Tick(CN3Terrain* pTerrain, const __Vector3& vPosPlayer);
	void	Render();
};

#endif // !defined(AFX_SERVERMESH_H__5D0D1E4F_E427_462E_9D72_CB0C4FC02C82__INCLUDED_)
