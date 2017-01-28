// PvsMgr.h: interface for the CPvsMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PVSMGR_H__7E562A54_E3B8_4484_A861_7ADD71D4411D__INCLUDED_)
#define AFX_PVSMGR_H__7E562A54_E3B8_4484_A861_7ADD71D4411D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include "N3BaseFileAccess.h"
#include "N3Shape.h"

#include "PortalVolume.h"
#include <string>

const int ciVersion = 1;

typedef std::list<CPortalVolume*>::iterator iter;

class CPvsMgr : public CN3BaseFileAccess
{
	friend class CDungeonManager;
	friend class CPortalVolume;

	const std::string m_IndoorFolder;
	const float m_fVolumeOffs;		// Volume üũ ����..

	std::list<CPortalVolume*> m_pPvsList;

	//.. Current Portal Volume.. ^^
	CPortalVolume* m_pCurVol;

public:
	static CN3Mng <class CN3Shape> s_MngShape;
	static CN3Mng <class CN3ShapeExtra> s_MngShapeExt;
	static std::list<ShapeInfo* > s_plShapeInfoList;
	static ShapeInfo* GetShapeInfoByManager(int iID);

private:
	void Tick(bool bWarp = false, __Vector3 vPos = __Vector3());
	void Render();

	void DeleteAllPvsObj();

	// String Cryptograph.. ^^
	static std::string ReadDecryptString(HANDLE hFile);
	std::string GetIndoorFolderPath() {	return m_IndoorFolder; }

	CPortalVolume* GetPortalVolPointerByID(int iID);

	////////////////////////////////////////////////////////////////
	bool CheckCollisionCameraWithTerrain(__Vector3& vEyeResult, const __Vector3& vAt, float fNP);
	bool CheckCollisionCameraWithShape(__Vector3& vEyeResult, const __Vector3& vAt, float fNP);
	float GetHeightWithTerrain(float x, float z, bool bWarp = false );
	float GetHeightNearstPosWithShape(const __Vector3& vPos, float fDist, __Vector3* pvNormal = NULL); 
	bool IsInTerrainWithTerrain(float x, float z, __Vector3 vPosBefore = __Vector3());
	float GetHeightWithShape(float fX, float fZ, __Vector3* pvNormal = NULL); 
	BOOL PickWideWithTerrain(int x, int y, __Vector3& vPick);
	CN3Shape* PickWithShape(int iXScreen, int iYScreen, bool bMustHaveEvent, __Vector3* pvPick = NULL);
	CN3Shape* ShapeGetByIDWithShape(int iID);
	bool CheckCollisionWithShape(	const __Vector3& vPos,				 // �浹 ��ġ
														const __Vector3& vDir,				   // ���� ����
														float fSpeedPerSec,					    // �ʴ� �����̴� �ӵ�
														__Vector3* pvCol,						 // �浹 ����
														__Vector3* pvNormal,				  // �浹�Ѹ��� ��������
														__Vector3* pVec);						// �浹�� �� �� ������ __Vector3[3]

public:
	CPvsMgr();
	virtual ~CPvsMgr();

	//..
	bool Load(HANDLE hFile);
	bool LoadOldVersion(HANDLE hFile, int iVersionFromData);
};

#endif // !defined(AFX_PVSMGR_H__7E562A54_E3B8_4484_A861_7ADD71D4411D__INCLUDED_)
