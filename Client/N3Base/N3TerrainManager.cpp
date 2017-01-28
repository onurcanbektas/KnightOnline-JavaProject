// N3TerrainManager.cpp: implementation of the CN3TerrainManager class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include "N3SkyMng.h"
#include "N3ShapeMgr.h"
#include "LogWriter.h"

#include "..\WarFare\N3TerrainManager.h"

#include "..\WarFare\N3Terrain.h"
#include "..\WarFare\BirdMng.h"
//#include "GrassMng.h"

#include "..\WarFare\GameProcedure.h"
#include "..\WarFare\PlayerMySelf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3TerrainManager::CN3TerrainManager()
{
	// N3Terrain..
	m_pTerrain = new CN3Terrain;

	// Shape..
	m_pShapes = new CN3ShapeMgr();

	// Sky..
	m_pSky = new CN3SkyMng();

	// Bird..
	m_pBirdMng = new CBirdMng();

//	// Grass..
//	m_pGrasses = new CGrassMng();
}

CN3TerrainManager::~CN3TerrainManager()
{
	// N3Terrain..
	delete m_pTerrain;  m_pTerrain = NULL;

	// Shape..
#ifdef _N3GAME
	CLogWriter::Write("CN3TerrainManager::~CN3TerrainManager -> Pre delete m_pShape"); // TmpLog1122
#endif
	delete m_pShapes; m_pShapes = NULL;

	// Sky..
#ifdef _N3GAME
	CLogWriter::Write("CN3TerrainManager::~CN3TerrainManager -> Pre delete m_pSky"); // TmpLog1122
#endif
	delete m_pSky;		m_pSky = NULL;

	// Bird..
#ifdef _N3GAME
	CLogWriter::Write("CN3TerrainManager::~CN3TerrainManager -> Pre delete m_pBirdMng"); // TmpLog1122
#endif
	delete m_pBirdMng; m_pBirdMng = NULL;

	// Grass..
//	CLogWriter::Write("CN3TerrainManager::~CN3TerrainManager -> Pre delete m_pGrasses"); // TmpLog1122
//	delete m_pGrasses; m_pGrasses = NULL;

#ifdef _N3GAME
	CLogWriter::Write("CN3TerrainManager::~CN3TerrainManager -> delete finished"); // TmpLog1122
#endif
}

/////////////////////////////////////////////////////////////////////

void CN3TerrainManager::InitWorld(int iZoneID, const __Vector3& vPosPlayer)
{
	__TABLE_ZONE* pZone = s_pTbl_Zones->Find(s_pPlayer->m_InfoExt.iZoneCur);
	if(NULL == pZone) { CLogWriter::Write("Null Zone Data : %d", iZoneID); return; }

	CLogWriter::Write("CN3TerrainManager::InitWorld Pre Load\t%d", m_pTerrain); // TmpLog_11_22


	/*if(iZoneID == 1) m_pTerrain->LoadFromFile(pZone->szTerrainFN, N3FORMAT_VER_1068);//N3FORMAT_VER_1298);//pZone->dwVersion);
	else*/ m_pTerrain->LoadFromFile(pZone->szTerrainFN, N3FORMAT_VER_1298);


	CLogWriter::Write("CN3TerrainManager::InitWorld Pre Load ColorMap"); // TmpLog_11_22
	m_pTerrain->LoadColorMap(pZone->szColorMapFN);		// �÷��� �ε�..
	CLogWriter::Write("CN3TerrainManager::InitWorld Pre Release Shapes\t%d", m_pShapes); // TmpLog_11_22
	m_pShapes->Release();
	CLogWriter::Write("CN3TerrainManager::InitWorld Pre Load Shapes"); // TmpLog_11_22


	/*if(iZoneID == 1) m_pShapes->LoadFromFile(pZone->szObjectPostDataFN, N3FORMAT_VER_1068);
	else*/ m_pShapes->LoadFromFile(pZone->szObjectPostDataFN, N3FORMAT_VER_1298);//, pZone->dwVersion);	// ������Ʈ ������ �ε�..
	

	char szFName[_MAX_PATH];
	_splitpath(pZone->szTerrainFN.c_str(), NULL, NULL, szFName, NULL);
	char szFName2[_MAX_PATH];
	char szFullPathName[_MAX_PATH];
	sprintf(szFName2,"%s_Bird",szFName);
	_makepath(szFullPathName, NULL, "misc\\bird", szFName2, "lst");
	CLogWriter::Write("CN3TerrainManager::InitWorld Pre Load Birds\t%d", m_pBirdMng); // TmpLog_11_22
	m_pBirdMng->LoadFromFile(szFullPathName);

//	CLogWriter::Write("CN3TerrainManager::InitWorld Pre Load Grasses\t%d", m_pGrasses); // TmpLog_11_22
//	m_pGrasses->Init(vPosPlayer);
	CLogWriter::Write("CN3TerrainManager::InitWorld Pre Load Sky\t%d", m_pSky); // TmpLog_11_22
	m_pSky->LoadFromFile(pZone->szSkySetting); // �ϴ�, ����, �¾�, ���� ��ȭ�� ���� �� �ؽ�ó �ε�..
	m_pSky->SunAndMoonDirectionFixByHour(pZone->iFixedSundDirection); // ��, �� ������ �����ϵ簡 Ȥ�� 0 �̸� �������� �ʴ´�.
}

void CN3TerrainManager::Tick()
{
	m_pTerrain->Tick();
	m_pShapes->Tick();
//	m_pGrasses->Tick((CGameProcedure* )CGameProcedure::s_pProcMain);
	m_pSky->Tick();
	m_pBirdMng->Tick();
}

CN3Terrain* CN3TerrainManager::GetTerrainRef()
{
	return m_pTerrain;
}

CN3SkyMng* CN3TerrainManager::GetSkyRef()
{
	return m_pSky;
}

void CN3TerrainManager::RenderTerrain()
{
	if (m_pTerrain)		m_pTerrain->Render();
}

void CN3TerrainManager::RenderShape()
{
	if (m_pShapes)		m_pShapes->Render();
}

void CN3TerrainManager::RenderSky()
{
	if (m_pSky)		m_pSky->Render();
}

void CN3TerrainManager::RenderGrass()
{
//	if (m_pGrasses)		m_pGrasses->Render();
}

void CN3TerrainManager::RenderBirdMgr()
{
	if (m_pBirdMng)		m_pBirdMng->Render();
}

void CN3TerrainManager::RenderSkyWeather()
{
	if (m_pSky)		m_pSky->RenderWeather();
}

//////////////////////////////////////////////////////////////////////////////////

// Terrain..
bool CN3TerrainManager::CheckCollisionCameraWithTerrain(__Vector3& vEyeResult, const __Vector3& vAt, float fNP)
{
	if (m_pTerrain)		
		return m_pTerrain->CheckCollisionCamera(vEyeResult, vAt, fNP);
	else
		return false;
}

float CN3TerrainManager::GetHeightWithTerrain(float x, float z, bool bWarp)
{

	if (m_pTerrain)		
		return m_pTerrain->GetHeight(x, z);
	else
		return -FLT_MAX;
}

BOOL CN3TerrainManager::PickWideWithTerrain(int x, int y, __Vector3& vPick)
{
	if (m_pTerrain)
		return m_pTerrain->PickWide(x, y, vPick);
	else
		return FALSE;
}

bool CN3TerrainManager::CheckCollisionWithTerrain(__Vector3& vPos, __Vector3& vDir, float fVelocity, __Vector3* vCol)
{
	if (m_pTerrain)
		return m_pTerrain->CheckCollision(vPos, vDir, fVelocity, vCol);
	else
		return false;
}

void CN3TerrainManager::GetNormalWithTerrain(float x, float z, __Vector3& vNormal)
{
	if (m_pTerrain)	m_pTerrain->GetNormal(x, z, vNormal);
}

float CN3TerrainManager::GetWidthByMeterWithTerrain()
{
	if (m_pTerrain)
		return m_pTerrain->GetWidthByMeter();
	else
		return -FLT_MAX;
}

bool CN3TerrainManager::IsInTerrainWithTerrain(float x, float z, __Vector3 vPosBefore)
{
	if (m_pTerrain)
		return m_pTerrain->IsInTerrain(x, z);
	else
		return false;	
}

bool CN3TerrainManager::CheckInclineWithTerrain(const __Vector3& vPos, const __Vector3& vDir, float fIncline)
{
	if (m_pTerrain)
		return m_pTerrain->CheckIncline(vPos, vDir, fIncline);
	else
		return false;	
}

// Shapes..
bool CN3TerrainManager::CheckCollisionCameraWithShape(__Vector3& vEyeResult, const __Vector3& vAt, float fNP)
{
	if (m_pShapes) 
		return m_pShapes->CheckCollisionCamera(vEyeResult, vAt, fNP);
	else
		return false;
}

float CN3TerrainManager::GetHeightNearstPosWithShape(const __Vector3 &vPos, float fDist, __Vector3* pvNormal)
{
	if (m_pShapes) 
		return m_pShapes->GetHeightNearstPos(vPos, fDist, pvNormal);
	else
		return -FLT_MAX;
}

void CN3TerrainManager::RenderCollisionWithShape(__Vector3 &vPos)
{
	if (m_pShapes) m_pShapes->RenderCollision(vPos);
}

float CN3TerrainManager::GetHeightWithShape(float fX, float fZ, __Vector3* pvNormal)
{
	if (m_pShapes) 
		return m_pShapes->GetHeight(fX, fZ, pvNormal);
	else
		return -FLT_MAX;
}

CN3Shape* CN3TerrainManager::ShapeGetByIDWithShape(int iID)
{
	if (m_pShapes) 
		return m_pShapes->ShapeGetByID(iID);
	else
		return NULL;
}

CN3Shape* CN3TerrainManager::PickWithShape(int iXScreen, int iYScreen, bool bMustHaveEvent, __Vector3* pvPick)
{
	if (m_pShapes) 
		return m_pShapes->Pick(iXScreen, iYScreen, bMustHaveEvent, pvPick);
	else
		return NULL;
}

bool CN3TerrainManager::CheckCollisionWithShape(	  const __Vector3& vPos,				 // �浹 ��ġ
																						const __Vector3& vDir,				   // ���� ����
																						float fSpeedPerSec,					    // �ʴ� �����̴� �ӵ�
																						__Vector3* pvCol,						 // �浹 ����
																						__Vector3* pvNormal,				  // �浹�Ѹ��� ��������
																						__Vector3* pVec)						// �浹�� �� �� ������ __Vector3[3]
{
	if (m_pShapes) 
		return m_pShapes->CheckCollision( vPos, vDir, fSpeedPerSec, pvCol, pvNormal, pVec);
	else
		return false;
}

// Sky..
D3DCOLOR CN3TerrainManager::GetSkyColorWithSky()
{
	if (m_pSky)
		return m_pSky->GetSkyColor();
	else
		return 0xffffffff;
}

float CN3TerrainManager::GetSunAngleByRadinWithSky()	
{ 	
	if (m_pSky)
		return m_pSky->GetSunAngleByRadin();
	else
		return -FLT_MAX;
}

void CN3TerrainManager::RenderWeatherWithSky()
{
	if (m_pSky)	m_pSky->RenderWeather();	
}

void CN3TerrainManager::SetGameTimeWithSky(int iYear, int iMonth, int iDay, int iHour, int iMin)
{
	if (m_pSky)	m_pSky->SetGameTime(iYear, iMonth, iDay, iHour, iMin);
}

void CN3TerrainManager::SetWeatherWithSky(int iWeather, int iPercentage)
{
	if (m_pSky)	m_pSky->SetWeather((CN3SkyMng::eSKY_WEATHER)iWeather, iPercentage);
}

D3DCOLOR CN3TerrainManager::GetLightDiffuseColorWithSky(int iIndex)
{
	if (m_pSky)	
		return m_pSky->GetLightDiffuseColor(iIndex);
	else
		return 0xffffffff;
}

D3DCOLOR CN3TerrainManager::GetLightAmbientColorWithSky(int iIndex)
{
	if (m_pSky)	
		return 	m_pSky->GetLightAmbientColor(iIndex);
	else
		return 0xffffffff;
}

D3DCOLOR CN3TerrainManager::GetFogColorWithSky()
{
	if (m_pSky)	
		return m_pSky->GetFogColor();
	else
		return 0xffffffff;
}

/*
CN3Sun*	CN3TerrainManager::GetSunPointerWithSky()
{
	if (m_pSky)
		return m_pSky->GetSunPointer();
	else
		return NULL;
}
*/

// Grass..
void CN3TerrainManager::InitWithGrass(__Vector3 CamPo)
{
//	if (m_pGrasses)	m_pGrasses->Init(CamPo);
}








