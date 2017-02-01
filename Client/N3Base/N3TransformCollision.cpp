// N3TransformCollision.cpp: implementation of the CN3TransformCollision class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3TransformCollision.h"
#include "N3VMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3TransformCollision::CN3TransformCollision()
{
	m_dwType |= OBJ_TRANSFORM_COLLISION;

	m_fRadius = 0;
	m_vMin.Set(0,0,0);
	m_vMax.Set(0,0,0);

	m_pMeshCollision = NULL;
	m_pMeshClimb = NULL;
}

CN3TransformCollision::~CN3TransformCollision()
{
	s_MngVMesh.Delete(&m_pMeshCollision);
	s_MngVMesh.Delete(&m_pMeshClimb);
}

void CN3TransformCollision::Release()
{
	m_fRadius = 0;
	m_vMin.Set(0,0,0);
	m_vMax.Set(0,0,0);

	s_MngVMesh.Delete(&m_pMeshCollision);
	s_MngVMesh.Delete(&m_pMeshClimb);

	CN3Transform::Release();
}

bool CN3TransformCollision::Load(HANDLE hFile)
{
	CN3Transform::Load(hFile);

	int nL = 0;
	char szFN[512] = "";

	DWORD dwRWC;
	ReadFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	if(nL > 0)
	{
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // �޽� ���� �̸�..
		m_pMeshCollision = s_MngVMesh.Get(szFN);
	}

	ReadFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	if(nL > 0)
	{
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL; // �޽� ���� �̸�..
		m_pMeshClimb = s_MngVMesh.Get(szFN);
	}
	return true;
}

#ifdef _N3TOOL
bool CN3TransformCollision::Save(HANDLE hFile)
{
	CN3Transform::Save(hFile);

	DWORD dwRWC;

	int nL = 0;
	if(m_pMeshCollision) nL = m_pMeshCollision->FileName().size();
	WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	if(nL > 0) 
	{
		if(m_pMeshCollision->FileName().find("object\\") < 0) // �ӽ÷� ��θ� �ٲٷ��� �־���.. ���߿� �ʿ���� �����..
		{
			char szFNTmp[256];
			wsprintf(szFNTmp, "Object\\%s.N3VMesh", m_pMeshCollision->m_szName.c_str());
			m_pMeshCollision->FileNameSet(szFNTmp);

			SetFilePointer(hFile, -4, 0, FILE_CURRENT);
			nL = m_pMeshCollision->FileName().size();
			WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
		}

		WriteFile(hFile, m_pMeshCollision->FileName().c_str(), nL, &dwRWC, NULL);
	}

	nL = 0;
	if(m_pMeshClimb) nL = m_pMeshClimb->FileName().size();
	WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
	if(nL > 0) 
	{
		if(-1 == m_pMeshClimb->FileName().find("object\\")) // �ӽ÷� ��θ� �ٲٷ��� �־���.. ���߿� �ʿ���� �����..
		{
			char szFNTmp[256];
			wsprintf(szFNTmp, "Object\\%s.N3VMesh", m_pMeshClimb->m_szName.c_str());
			m_pMeshClimb->FileNameSet(szFNTmp);

			SetFilePointer(hFile, -4, 0, FILE_CURRENT);
			nL = m_pMeshClimb->FileName().size();
			WriteFile(hFile, &nL, 4, &dwRWC, NULL); // Mesh FileName
		}

		WriteFile(hFile, m_pMeshClimb->FileName().c_str(), nL, &dwRWC, NULL);
	}
	return true;
}
#endif // end of _N3TOOL

void CN3TransformCollision::CollisionMeshSet(const std::string& szFN)
{
	s_MngVMesh.Delete(&m_pMeshCollision);
	m_pMeshCollision = s_MngVMesh.Get(szFN);
	if(m_pMeshCollision) this->FindMinMax();
}

void CN3TransformCollision::ClimbMeshSet(const std::string& szFN)
{
	s_MngVMesh.Delete(&m_pMeshClimb);
	m_pMeshClimb = s_MngVMesh.Get(szFN);
	if(m_pMeshClimb) m_pMeshClimb->FindMinMax();
}

int CN3TransformCollision::CheckCollisionPrecisely(bool bIgnoreBoxCheck, int ixScreen, int iyScreen, __Vector3* pVCol, __Vector3* pVNormal)
{
	__Vector3 vPos, vDir; // 2D ��ǥ�� 3D ��ǥ�� �ٲٰ�..
	::_Convert2D_To_3DCoordinate(ixScreen, iyScreen, s_CameraData.mtxView, s_CameraData.mtxProjection, s_CameraData.vp, vPos, vDir);

	if(false == m_pMeshCollision->Pick(m_Matrix, vPos, vDir, pVCol, pVNormal)) return -1;
	else return 0;
}

#ifdef _DEBUG
void CN3TransformCollision::RenderCollisionMesh()
{
	if(NULL == m_pMeshCollision) return;
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

	m_pMeshCollision->Render(0xffff0000); // ������.
}

void CN3TransformCollision::RenderClimbMesh()
{
	if(NULL == m_pMeshClimb) return;
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

	m_pMeshClimb->Render(0xff0000ff); // �Ķ���..
}
#endif

/*
#ifdef _N3TOOL
BOOL CN3TransformCollision::CheckClimb(__Vector3 &vPos, __Vector3 &vDir, __Vector3* pVCol, __Vector3* pVNormal, __Vector3* pVPolygon)
{
	if(NULL == m_pMeshClimb) return FALSE;
	
	int nIC = m_pMeshClimb->IndexCount();
	int nFC = 0;
	if(nIC > 0)
	{
		nFC = nIC / 3; // Face Count
	}
	else
	{
		nFC = m_pMeshClimb->VertexCount() / 3;
	}
	if(nFC <= 0) return FALSE;

	float t,u,v;
	__Vector3 vSrc[3];

	if(nIC > 0)
	{
		WORD* pwIndices = m_pMeshClimb->Indices();
		__Vector3* pVSrc = m_pMeshClimb->Vertices();
		for(int i = 0; i < nFC; i++)
		{
			vSrc[0] = pVSrc[pwIndices[i*3+0]] * m_Matrix;
			vSrc[1] = pVSrc[pwIndices[i*3+1]] * m_Matrix;
			vSrc[2] = pVSrc[pwIndices[i*3+2]] * m_Matrix;

			if(TRUE == ::IntersectTriangle(vPos, vDir, vSrc[0], vSrc[1], vSrc[2], t, u, v)) 
			{
				if(pVCol) *pVCol = (vDir * t) + vPos;
				if(pVNormal) { pVNormal->Cross(vSrc[1] - vSrc[0], vSrc[2] - vSrc[0]); pVNormal->Normalize(); }
				if(pVPolygon)
				{
					pVPolygon[0] = vSrc[0];
					pVPolygon[1] = vSrc[1];
					pVPolygon[2] = vSrc[2];
				}
				return TRUE;
			}
		}
	}
	else
	{
		__Vector3* pVSrc = m_pMeshClimb->Vertices();
		for(int i = 0; i < nFC; i++, pVSrc += 3)
		{
			vSrc[0] = pVSrc[0] * m_Matrix;
			vSrc[1] = pVSrc[1] * m_Matrix;
			vSrc[2] = pVSrc[2] * m_Matrix;

			if(TRUE == ::IntersectTriangle(vPos, vDir, vSrc[0], vSrc[1], vSrc[2], t, u, v)) 
			{
				if(pVCol) *pVCol = (vDir * t) + vPos;
				if(pVNormal) { pVNormal->Cross(vSrc[1] - vSrc[0], vSrc[2] - vSrc[0]); pVNormal->Normalize(); }
				if(pVPolygon)
				{
					pVPolygon[0] = vSrc[0];
					pVPolygon[1] = vSrc[1];
					pVPolygon[2] = vSrc[2];
				}
				return TRUE;
			}
		}
	}

	return FALSE;
}
#endif // end of _N3TOOL

#ifdef _N3TOOL
BOOL CN3TransformCollision::CheckClimb(int x, int y, __Vector3* pVCol, __Vector3* pVNormal, __Vector3* pVPolygon)
{
	if(NULL == m_pMeshClimb || NULL == m_pMeshClimb->Vertices()) return FALSE;

	// Compute the vector of the pick ray in screen space
	__Vector3 vTmp;
	vTmp.x =  ( ( ( 2.0f * x ) / (CN3Base::s_CameraData.vp.Width) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._11;
	vTmp.y = -( ( ( 2.0f * y ) / (CN3Base::s_CameraData.vp.Height) ) - 1 ) / CN3Base::s_CameraData.mtxProjection._22;
	vTmp.z =  1.0f;

	// Transform the screen space pick ray into 3D space
	__Matrix44* pMtxVI = &CN3Base::s_CameraData.mtxViewInverse;
	__Vector3 vPos, vDir;
	vDir.x  = vTmp.x * pMtxVI->_11 + vTmp.y * pMtxVI->_21 + vTmp.z * pMtxVI->_31;
	vDir.y  = vTmp.x * pMtxVI->_12 + vTmp.y * pMtxVI->_22 + vTmp.z * pMtxVI->_32;
	vDir.z  = vTmp.x * pMtxVI->_13 + vTmp.y * pMtxVI->_23 + vTmp.z * pMtxVI->_33;
	vPos = pMtxVI->Pos();

	return CheckClimb(vPos, vDir, pVCol, pVNormal, pVPolygon);
}
#endif // end of _N3TOOL
*/

void CN3TransformCollision::FindMinMax()
{
	m_vMin.Set(0,0,0);
	m_vMax.Set(0,0,0);
	m_fRadius = 0.0f;

	if(NULL == m_pMeshCollision || m_pMeshCollision->VertexCount() <= 0) return;

	m_vMin.Set(FLT_MAX, FLT_MAX, FLT_MAX);
	m_vMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	
	int nVC = m_pMeshCollision->VertexCount();
	__Vector3* pVs = m_pMeshCollision->Vertices();
	for(int i = 0; i < nVC; i++)
	{
		if(pVs[i].x < m_vMin.x) m_vMin.x = pVs[i].x;
		if(pVs[i].y < m_vMin.y) m_vMin.y = pVs[i].y;
		if(pVs[i].z < m_vMin.z) m_vMin.z = pVs[i].z;
		if(pVs[i].x > m_vMax.x) m_vMax.x = pVs[i].x;
		if(pVs[i].y > m_vMax.y) m_vMax.y = pVs[i].y;
		if(pVs[i].z > m_vMax.z) m_vMax.z = pVs[i].z;
	}

	// �ִ� �ּҰ��� ���� ������ ����Ѵ�..
	m_fRadius  = (m_vMax - m_vMin).Magnitude() * 0.5f;
}
