// N3TerrainManager.h: interface for the CN3TerrainManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3TERRAINMANAGER_H__4FA514AC_F8F8_4E09_88DC_AC31F2C9B21D__INCLUDED_)
#define AFX_N3TERRAINMANAGER_H__4FA514AC_F8F8_4E09_88DC_AC31F2C9B21D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3WorldBase.h"

class CN3TerrainManager : public CN3WorldBase
{
	friend class CN3WorldManager;

	class CN3Terrain*		m_pTerrain;										  // ���� Ŭ����
	class CN3ShapeMgr*		m_pShapes;										// ��ü Ŭ����
	class CN3SkyMng*		m_pSky;											  // �ϴ� Ŭ����
	class CBirdMng*			m_pBirdMng;									     // �ϴÿ� ����ٴϴ� ���� ����..
//	class CGrassMng*		m_pGrasses;									  // Ǯ �׸�

	// Function..
	void InitWorld(int iZoneID, const __Vector3& vPosPlayer);
	void Tick();

//////////////////////////////////////////////////////////////////////
	CN3Terrain* GetTerrainRef();
	CN3SkyMng* GetSkyRef();

	// Terrain..
	bool CheckCollisionCameraWithTerrain(__Vector3& vEyeResult, const __Vector3& vAt, float fNP);
	float GetHeightWithTerrain(float x, float z, bool bWarp = false);
	BOOL PickWideWithTerrain(int x, int y, __Vector3& vPick);
	bool CheckCollisionWithTerrain(__Vector3& vPos, __Vector3& vDir, float fVelocity, __Vector3* vCol);
	void GetNormalWithTerrain(float x, float z, __Vector3& vNormal);
	float GetWidthByMeterWithTerrain();
	bool IsInTerrainWithTerrain(float x, float z, __Vector3 vPosBefore = __Vector3());
	bool CheckInclineWithTerrain(const __Vector3& vPos, const __Vector3& vDir, float fIncline);

	// Shapes..
	bool CheckCollisionCameraWithShape(__Vector3& vEyeResult, const __Vector3& vAt, float fNP);
	float GetHeightNearstPosWithShape(const __Vector3& vPos, float fDist, __Vector3* pvNormal = NULL); 
	void RenderCollisionWithShape(__Vector3 &vPos);
	float GetHeightWithShape(float fX, float fZ, __Vector3* pvNormal = NULL); 
	CN3Shape* ShapeGetByIDWithShape(int iID);
	CN3Shape* PickWithShape(int iXScreen, int iYScreen, bool bMustHaveEvent, __Vector3* pvPick = NULL);
	bool CheckCollisionWithShape(	const __Vector3& vPos,				 // �浹 ��ġ
														const __Vector3& vDir,				   // ���� ����
														float fSpeedPerSec,					    // �ʴ� �����̴� �ӵ�
														__Vector3* pvCol = NULL,			 // �浹 ����
														__Vector3* pvNormal = NULL,		 // �浹�Ѹ��� ��������
														__Vector3* pVec = NULL);			// �浹�� �� �� ������ __Vector3[3]

	// Sky..
	D3DCOLOR GetSkyColorWithSky();
	float GetSunAngleByRadinWithSky();
	void RenderWeatherWithSky();
	void SetGameTimeWithSky(int iYear, int iMonth, int iDay, int iHour, int iMin);
	void SetWeatherWithSky(int iWeather, int iPercentage);
	D3DCOLOR GetLightDiffuseColorWithSky(int iIndex);
	D3DCOLOR GetLightAmbientColorWithSky(int iIndex);
	D3DCOLOR GetFogColorWithSky();
	//CN3Sun*	GetSunPointerWithSky();

	// Bird..

	// Grass..
	void InitWithGrass(__Vector3 CamPo);

	// Rendering..
	void RenderTerrain();
	void RenderShape();	
	void RenderSky();
	void RenderGrass();
	void RenderBirdMgr();
	void RenderSkyWeather();

public:
	CN3TerrainManager();
	virtual ~CN3TerrainManager();
};

#endif // !defined(AFX_N3TERRAINMANAGER_H__4FA514AC_F8F8_4E09_88DC_AC31F2C9B21D__INCLUDED_)
