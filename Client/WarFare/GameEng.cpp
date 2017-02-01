// GameEng.cpp: implementation of the CGameEng class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "GameEng.h"
#include "GameBase.h"
#include "N3WorldManager.h"

#include "N3Camera.h"
#include "N3Light.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

const float LIGHTNING_DURATION = 2.0f;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameEng::CGameEng()
{
	m_pActiveCam	= NULL;

	// ���α׷��� ����� ���..
	char szBuf[_MAX_PATH];
	char szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
	::GetModuleFileName(NULL, szBuf, _MAX_PATH);
	_splitpath(szBuf, szDrive, szDir, NULL, NULL);
	_makepath(szBuf, szDrive, szDir, NULL, NULL);

	///////////////////////////////////////////////////////////////
	// �⺻ ī�޶� ����..
	CN3Camera* pCamera			= new CN3Camera();
	pCamera->m_bFogUse			= TRUE;
	pCamera->m_Data.fFOV		= D3DXToRadian(70);
	pCamera->m_Data.fNP			= 0.7f;
	pCamera->m_Data.fFP			= 512.0f;
	this->CameraAdd(pCamera);
	this->SetActiveCamera(pCamera);

	m_vEyeToReach = pCamera->EyePos();
	m_vAtToReach = pCamera->AtPos();

//	m_eViewPoint = VP_BACKWARD; // �⺻�� 1.5��Ī(�÷��̾� �ڿ��� �ٶ� ���)���� ����.
	m_eViewPoint = VP_THIRD_PERSON; // �⺻�� 3��Ī���� ����.
	m_fRotPitchFirstPerson = 0.0f; // ����Ī �����϶� ī�޶� ��ġ ����.
	m_fRotPitchBackward = 0.0f; // ����Ī �����϶� ī�޶� ��ġ ����.
	m_fRotPitchFoward = 0.0f; // �տ��� �� �����϶�  ī�޶� ��ġ ����.
	m_fRotPitchThirdFirson = D3DXToRadian(25.0f);
	m_fOffsetVPGod = 7.0f; // ������ �������϶� ī�޶� �Ÿ�..
	m_fRotYawVPGod = 0.0f; // ������ �ٶ� ������ ��쿡 ī�޶� ȸ������..
	m_fZoomBackwardOrFoward = 1.0f; // 1.5��Ī, �տ��� �� �����϶� ī�޶� Zoom 1.0f �� �����̴�.
	// �⺻ ī�޶� ����..
	///////////////////////////////////////////////////////////////

	m_fLightningTimeRemain = 0.0f; // ���� ĥ�� Ÿ�̸�..

	///////////////////////////////////////////////////////////////
	// �⺻ ����Ʈ ����
	/*
	D3DCOLORVALUE crLgt;

	crLgt.a = 0.0f, crLgt.r = crLgt.g = crLgt.b = 0.8f;
	CN3Light* pLightGlobal = new CN3Light(); // ��ü�� ���� ����Ʈ..
	pLightGlobal->m_Data.InitDirection(0, __Vector3(0,-1,0), crLgt);
	this->LightAdd(pLightGlobal);

	crLgt.a = 0.0f, crLgt.r = crLgt.g = crLgt.b = 0.5f;
	CN3Light* pLightGlobal2 = new CN3Light(); // �ݴ� ���� ��ü�� ���� ����Ʈ..
	pLightGlobal2->m_Data.InitDirection(1, __Vector3(0,1,0), crLgt);
	this->LightAdd(pLightGlobal2);

	crLgt.a = 0.0f, crLgt.r = crLgt.g = crLgt.b = 0.3f;
	CN3Light* pLight = new CN3Light(); // ī�޶�� �پ� �ٴѴ�...
	pLight->m_Data.InitPoint(2, __Vector3(0,0,0), crLgt, 32.0f);
	this->LightAdd(pLight);
	*/
	// �⺻ ����Ʈ ����
	///////////////////////////////////////////////////////////////
	m_pRefLightSun = NULL;
	m_pRefLightSupport = NULL;
	m_pRefLightCam = NULL;

	m_fFPDeltaCur = 1.0f; // ���� 
	m_fFPDeltaToReach = 1.0f; // �̰��� ��ǥ�� �ؼ� ���Ѵ�.

//	m_fLightDeltaCur = 1.0f;
//	m_fLightDeltaToReach = 1.0f; // �̰��� ��ǥ�� �ؼ� ���Ѵ�.
}

CGameEng::~CGameEng()
{
	it_Camera itCam = m_Cameras.begin();
	int iSize = m_Cameras.size();
	for(int i = 0; i < iSize; i++, itCam++)
	{
		delete *itCam; 
	}
	m_Cameras.clear();

/*	it_Light itLgt = m_Lights.begin();
	iSize = m_Lights.size();
	for(i = 0; i < iSize; i++, itLgt++)
	{
		delete *itLgt; 
	}
	m_Lights.clear();

	itLgt = m_LightsBackup.begin();
	iSize = m_LightsBackup.size();
	for(i = 0; i < iSize; i++, itLgt++)
	{
		delete *itLgt; 
	}
	m_LightsBackup.clear();
*/
}

/*
void CGameEng::RestoreLighting()
{
	m_Lights.clear();	
	it_Light itLgt = m_LightsBackup.begin();
	while( itLgt != m_LightsBackup.end() )
	{
		CN3Light* pLgt = *itLgt++;
		m_Lights.push_back(pLgt);
	}
	m_LightsBackup.clear();
}

void CGameEng::BackupReleaseLighting()
{
	m_LightsBackup.clear();
	it_Light itLgt = m_Lights.begin();
	while( itLgt != m_Lights.end() )
	{
		CN3Light* pLgt = *itLgt++;
		m_LightsBackup.push_back(pLgt);
	}
	m_Lights.clear();
}
*/

void CGameEng::SetDefaultLight(CN3Light* pSun, CN3Light* pSupport, CN3Light* pCam)
{
	m_pRefLightSun = pSun;
	m_pRefLightSupport = pSupport;
	m_pRefLightCam = pCam;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

void CGameEng::Tick(const D3DCOLOR* crDiffuses,			// Diffuse ����Ʈ ����.. 3 �� ����.
					const D3DCOLOR* crAmbients,			// Ambient ����Ʈ ����.. 3 �� ����.
					const D3DCOLOR crFog,				// �Ȱ� ����..
					const __Vector3& vPosPlayer,		// �÷��̾� ��ġ
					const __Quaternion& qtPlayer,		// ȸ�� ���ʹϾ�
					float fHeightPlayer,				// Ű�� �μ��� ������ ī�޶�� ����Ʈ ó��..
					float fSunRadianZ)					// ���� Z ����..
{
	if(NULL == m_pActiveCam) return;

	float fRadius = fHeightPlayer * 2.0f;
	float fYaw = 0;
	__Quaternion qtRot = qtPlayer;
	__Vector3 vAxis(0,1,0);
	qtRot.AxisAngle(vAxis, fYaw);
	
	if(vAxis.y < 0) // ȸ������ �����̸�.
	{
		vAxis.y *= -1.0f;
		fYaw *= -1.0f;
	}

	switch(m_eViewPoint)
	{
		case VP_BACKWARD:
		{
			::D3DXQuaternionRotationYawPitchRoll(&qtRot, fYaw, m_fRotPitchBackward, 0);
			__Matrix44 mtxRot = qtRot;

			m_vEyeToReach.Set(0, 0, -(fRadius / s_CameraData.fFOV) * m_fZoomBackwardOrFoward);
			m_vAtToReach = vPosPlayer; m_vAtToReach.y += fHeightPlayer * 0.8f;
			m_vEyeToReach = m_vAtToReach + (m_vEyeToReach * mtxRot);
		}
		break;
		case VP_FIRST_PERSON:
		{
			::D3DXQuaternionRotationYawPitchRoll(&qtRot, fYaw, m_fRotPitchFirstPerson, 0);
			__Matrix44 mtxRot = qtRot;

			m_vEyeToReach = vPosPlayer; m_vEyeToReach.y += fHeightPlayer - 0.1f;
			m_vAtToReach.Set(0,0,1);
			m_vAtToReach = m_vEyeToReach + (m_vAtToReach * mtxRot);
		}
		break;
		case VP_FOWARD:
		{
			::D3DXQuaternionRotationYawPitchRoll(&qtRot, fYaw, -m_fRotPitchFoward, 0);
			__Matrix44 mtxRot = qtRot;

			m_vEyeToReach.Set(0, 0, fRadius * m_fZoomBackwardOrFoward);
			m_vAtToReach = vPosPlayer; m_vAtToReach.y += fHeightPlayer * 0.8f;
			m_vEyeToReach = m_vAtToReach + (m_vEyeToReach * mtxRot);
		}
		break;
		case VP_THIRD_PERSON:
		{
			::D3DXQuaternionRotationYawPitchRoll(&qtRot, m_fRotYawVPGod, m_fRotPitchThirdFirson, 0);
			__Matrix44 mtxRot = qtRot;

			m_vAtToReach = vPosPlayer; m_vAtToReach.y += fHeightPlayer * 0.8f;
			m_vEyeToReach.Set(0,0,-m_fOffsetVPGod);
			m_vEyeToReach = m_vAtToReach + (m_vEyeToReach * mtxRot);
		}
		break;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// ī�޶� �浹 üũ...
	if(VP_FIRST_PERSON == m_eViewPoint) // ����Ī���� �浹üũ ���Ѵ�.
	{
		m_pActiveCam->LookAt(m_vEyeToReach, m_vAtToReach, __Vector3(0,1,0)); // ó�ٺ���..
	}
	else
	{
		__Vector3 vEyeResult = m_vEyeToReach;
		float fNP = m_pActiveCam->m_Data.fNP;
		CGameBase::ACT_WORLD->CheckCollisionCameraWithTerrain(vEyeResult, m_vAtToReach, fNP); // ������ �浹üũ
		CGameBase::ACT_WORLD->CheckCollisionCameraWithShape(vEyeResult, m_vAtToReach, fNP); // ������Ʈ�� �浹üũ..
		m_pActiveCam->LookAt(vEyeResult, m_vAtToReach, __Vector3(0,1,0)); // ó�ٺ���..
	}
	// ī�޶� �浹 üũ...
	////////////////////////////////////////////////////////////////////////////////////


	// �� �÷��� ���� ����..
	// ApplyCameraAndLight ���� ������ �Ȱ����� ���� �����Ѵ�.
	if(m_fFPDeltaCur != m_fFPDeltaToReach)
	{
		float fFPChange = (m_fFPDeltaToReach - m_fFPDeltaCur) * s_fSecPerFrm / 5.0f; // 5�ʵ��� ���ϰ� �Ѵ�.
		m_fFPDeltaCur += fFPChange;

		if(fFPChange < 0 && m_fFPDeltaCur < m_fFPDeltaToReach) m_fFPDeltaCur = m_fFPDeltaToReach;
		if(fFPChange > 0 && m_fFPDeltaCur > m_fFPDeltaToReach) m_fFPDeltaCur = m_fFPDeltaToReach;
	}
	float fFPToRestore = m_pActiveCam->m_Data.fFP;
	m_pActiveCam->m_Data.fFP = s_Options.iViewDist * m_fFPDeltaCur;
	
	m_pActiveCam->m_FogColor = crFog; // �Ȱ����� �����..
	m_pActiveCam->Tick(); // ����� ���ü����� ������ ���..

	__Matrix44 mtxRotSun;
	mtxRotSun.RotationZ(fSunRadianZ); // ���� ������ �����..

	/*
	it_Light itLgt = m_Lights.begin();
	int iSize = m_Lights.size();
	for(int i = 0; i < iSize; i++, itLgt++)
	{
		CN3Light* pLight = *itLgt;
		__ASSERT(pLight, "Light pointer is NULL!!!");
		
		if(0 == pLight->m_Data.nNumber) // �⺻ �𷺼ų� ����Ʈ
		{
			// View Matrix ������ ������ �����..
//			__Vector3 vDir(0.0f,-1.5f,1.0f);
//			vDir.Normalize();
//			__Matrix44 mtxVI = s_CameraData.mtxViewInverse;
//			mtxVI.PosSet(0,0,0);
//			pLight->m_Data.Direction = vDir * mtxVI;

			// �ؿ� ������ �����..
			__Matrix44 mtxRot; mtxRot.RotationZ(fSunRadianZ);
			__Vector3 vDir(-1,0,1);
			vDir *= mtxRot;
			vDir.Normalize();
			pLight->m_Data.Direction = vDir;
			
			// ����Ʈ �÷� ����..
			pLight->m_Data.Diffuse = ::_D3DCOLOR_To_D3DCOLORVALUE(crDiffuses[0]);
			pLight->m_Data.Ambient = ::_D3DCOLOR_To_D3DCOLORVALUE(crAmbients[0]);
		}
		else if(1 == pLight->m_Data.nNumber)
		{
			__Vector3 vDir(2,-3, 2); // ������ �Ʒ��� ...
			vDir.Normalize();
			pLight->m_Data.Direction = vDir;
			
			// ����Ʈ �÷� ����..
			pLight->m_Data.Diffuse = ::_D3DCOLOR_To_D3DCOLORVALUE(crDiffuses[1]);
			pLight->m_Data.Ambient = ::_D3DCOLOR_To_D3DCOLORVALUE(crAmbients[1]);
		}
		else if(2 == pLight->m_Data.nNumber)
		{
			__Vector3 vPos = s_CameraData.vEye;
			vPos.y += 16.0f;
			pLight->PosSet(vPos); // ī�޶� ���� ���� �Ѵ�..
			
			// ����Ʈ �÷� ����..
			pLight->m_Data.Diffuse = ::_D3DCOLOR_To_D3DCOLORVALUE(crDiffuses[2]);
			pLight->m_Data.Ambient = ::_D3DCOLOR_To_D3DCOLORVALUE(crAmbients[2]);
		}


		// ���� ó��..
		if(m_fLightningTimeRemain > 0)
		{
			float fLightningDelta = 0;

			if(m_fLightningTimeRemain > LIGHTNING_DURATION * 0.8f)
				fLightningDelta = (m_fLightningTimeRemain - LIGHTNING_DURATION * 0.8f) / (LIGHTNING_DURATION * 0.2f);
			else
				fLightningDelta = m_fLightningTimeRemain / (LIGHTNING_DURATION * 0.8f);

			pLight->m_Data.Diffuse.r += (1.0f - pLight->m_Data.Diffuse.r) * fLightningDelta * 0.4f;
			pLight->m_Data.Diffuse.g += (1.0f - pLight->m_Data.Diffuse.g) * fLightningDelta * 0.5f;
			pLight->m_Data.Diffuse.b += (1.0f - pLight->m_Data.Diffuse.b) * fLightningDelta;

			m_fLightningTimeRemain -= CN3Base::s_fSecPerFrm;
			if(m_fLightningTimeRemain < 0) m_fLightningTimeRemain = 0;
		}

		pLight->Tick();
	}
	*/
	if(m_pRefLightSun)
	{
		// �ؿ� ������ �����..
		__Matrix44 mtxRot; mtxRot.RotationZ(fSunRadianZ);
		__Vector3 vDir(-1,0,1);
		vDir *= mtxRot;
		vDir.Normalize();
		m_pRefLightSun->m_Data.Direction = vDir;
		
		// ����Ʈ �÷� ����..
		m_pRefLightSun->m_Data.Diffuse = ::_D3DCOLOR_To_D3DCOLORVALUE(crDiffuses[0]);
		m_pRefLightSun->m_Data.Ambient = ::_D3DCOLOR_To_D3DCOLORVALUE(crAmbients[0]);

		// ���� ó��..
		if(m_fLightningTimeRemain > 0)
		{
			float fLightningDelta = 0;

			if(m_fLightningTimeRemain > LIGHTNING_DURATION * 0.8f)
				fLightningDelta = (m_fLightningTimeRemain - LIGHTNING_DURATION * 0.8f) / (LIGHTNING_DURATION * 0.2f);
			else
				fLightningDelta = m_fLightningTimeRemain / (LIGHTNING_DURATION * 0.8f);

			m_pRefLightSun->m_Data.Diffuse.r += (1.0f - m_pRefLightSun->m_Data.Diffuse.r) * fLightningDelta * 0.4f;
			m_pRefLightSun->m_Data.Diffuse.g += (1.0f - m_pRefLightSun->m_Data.Diffuse.g) * fLightningDelta * 0.5f;
			m_pRefLightSun->m_Data.Diffuse.b += (1.0f - m_pRefLightSun->m_Data.Diffuse.b) * fLightningDelta;

			m_fLightningTimeRemain -= CN3Base::s_fSecPerFrm;
			if(m_fLightningTimeRemain < 0) m_fLightningTimeRemain = 0;
		}
	}
	if(m_pRefLightSupport)
	{
		__Vector3 vDir(2,-3, 2); // ������ �Ʒ��� ...
		vDir.Normalize();
		m_pRefLightSupport->m_Data.Direction = vDir;
		
		// ����Ʈ �÷� ����..
		m_pRefLightSupport->m_Data.Diffuse = ::_D3DCOLOR_To_D3DCOLORVALUE(crDiffuses[1]);
		m_pRefLightSupport->m_Data.Ambient = ::_D3DCOLOR_To_D3DCOLORVALUE(crAmbients[1]);

		// ���� ó��..
		if(m_fLightningTimeRemain > 0)
		{
			float fLightningDelta = 0;

			if(m_fLightningTimeRemain > LIGHTNING_DURATION * 0.8f)
				fLightningDelta = (m_fLightningTimeRemain - LIGHTNING_DURATION * 0.8f) / (LIGHTNING_DURATION * 0.2f);
			else
				fLightningDelta = m_fLightningTimeRemain / (LIGHTNING_DURATION * 0.8f);

			m_pRefLightSupport->m_Data.Diffuse.r += (1.0f - m_pRefLightSupport->m_Data.Diffuse.r) * fLightningDelta * 0.4f;
			m_pRefLightSupport->m_Data.Diffuse.g += (1.0f - m_pRefLightSupport->m_Data.Diffuse.g) * fLightningDelta * 0.5f;
			m_pRefLightSupport->m_Data.Diffuse.b += (1.0f - m_pRefLightSupport->m_Data.Diffuse.b) * fLightningDelta;

			m_fLightningTimeRemain -= CN3Base::s_fSecPerFrm;
			if(m_fLightningTimeRemain < 0) m_fLightningTimeRemain = 0;
		}
	}
	if(m_pRefLightCam)
	{
		__Vector3 vPos = s_CameraData.vEye;
		vPos.y += 16.0f;
		m_pRefLightCam->PosSet(vPos); // ī�޶� ���� ���� �Ѵ�..
		
		// ����Ʈ �÷� ����..
		m_pRefLightCam->m_Data.Diffuse = ::_D3DCOLOR_To_D3DCOLORVALUE(crDiffuses[2]);
		m_pRefLightCam->m_Data.Ambient = ::_D3DCOLOR_To_D3DCOLORVALUE(crAmbients[2]);

		// ���� ó��..
		if(m_fLightningTimeRemain > 0)
		{
			float fLightningDelta = 0;

			if(m_fLightningTimeRemain > LIGHTNING_DURATION * 0.8f)
				fLightningDelta = (m_fLightningTimeRemain - LIGHTNING_DURATION * 0.8f) / (LIGHTNING_DURATION * 0.2f);
			else
				fLightningDelta = m_fLightningTimeRemain / (LIGHTNING_DURATION * 0.8f);

			m_pRefLightCam->m_Data.Diffuse.r += (1.0f - m_pRefLightCam->m_Data.Diffuse.r) * fLightningDelta * 0.4f;
			m_pRefLightCam->m_Data.Diffuse.g += (1.0f - m_pRefLightCam->m_Data.Diffuse.g) * fLightningDelta * 0.5f;
			m_pRefLightCam->m_Data.Diffuse.b += (1.0f - m_pRefLightCam->m_Data.Diffuse.b) * fLightningDelta;

			m_fLightningTimeRemain -= CN3Base::s_fSecPerFrm;
			if(m_fLightningTimeRemain < 0) m_fLightningTimeRemain = 0;
		}
	}
}


void CGameEng::ApplyCameraAndLight()
{
	if(m_pActiveCam)
	{
		m_pActiveCam->Apply(); // ���� �� ��ſ� �ϳ��� ����...
	}
/*
	for(it_Light it = m_Lights.begin(), itEnd = m_Lights.end(); it != itEnd; it++)
	{
		CN3Light* pLight = *it;
		__ASSERT(pLight, "Light pointer is NULL!!!");
		
		pLight->Apply();
	}
*/
}

void CGameEng::ViewPointChange(e_ViewPoint eVP)
{
	if(eVP < VP_BACKWARD || eVP > VP_THIRD_PERSON)
	{
		if(m_eViewPoint < VP_BACKWARD) m_eViewPoint = VP_BACKWARD;
		if(m_eViewPoint >= VP_THIRD_PERSON) m_eViewPoint = VP_THIRD_PERSON;

		int iVP = (int)(m_eViewPoint) + 1;
		if ( iVP > (VP_THIRD_PERSON) ) iVP = VP_BACKWARD;
		m_eViewPoint = (e_ViewPoint)iVP;						// ���� ����..
	}
	else
	{
		m_eViewPoint = eVP;						// ���� ����..
	}
}

void CGameEng::CameraPitchAdd(float fRotXPerSec)
{
	if(NULL == m_pActiveCam) return;
	float fPitchMax = D3DXToRadian(70.0f);
	float fPitchMin = -D3DXToRadian(50.0f);
	if(VP_BACKWARD == m_eViewPoint)
	{
		m_fRotPitchBackward += fRotXPerSec * s_fSecPerFrm;
		if(m_fRotPitchBackward > fPitchMax) m_fRotPitchBackward = fPitchMax; // �Ʒ� ���°�
		else if(m_fRotPitchBackward < fPitchMin) m_fRotPitchBackward = fPitchMin; // ������ ��..
	}
	else if(VP_FIRST_PERSON == m_eViewPoint)
	{
		m_fRotPitchFirstPerson += fRotXPerSec * s_fSecPerFrm;
		if(m_fRotPitchFirstPerson > fPitchMax) m_fRotPitchFirstPerson = fPitchMax; // �Ʒ� ���°�
		else if(m_fRotPitchFirstPerson < fPitchMin) m_fRotPitchFirstPerson = fPitchMin; // ������ ��..
	}
	else if(VP_FOWARD == m_eViewPoint)
	{
		m_fRotPitchFoward += fRotXPerSec * s_fSecPerFrm;
		if(m_fRotPitchFoward > fPitchMax) m_fRotPitchFoward = fPitchMax; // �Ʒ� ���°�
		else if(m_fRotPitchFoward < fPitchMin) m_fRotPitchFoward = fPitchMin; // ������ ��..
	}
	else if(VP_THIRD_PERSON == m_eViewPoint) // ������ ���� �� �Ÿ��� �ÿ��� �ٿ��� �Ѵ�..
	{
		fPitchMin = D3DXToRadian(10.0f); // �� �ؿ��� �Ѱ�

		m_fRotPitchThirdFirson += fRotXPerSec * s_fSecPerFrm;
		if(m_fRotPitchThirdFirson > fPitchMax) m_fRotPitchThirdFirson = fPitchMax; // �Ʒ� ���°�
		else if(m_fRotPitchThirdFirson < fPitchMin) m_fRotPitchThirdFirson = fPitchMin; // ������ ��..
	}
}

void CGameEng::CameraYawAdd(float fRotYPerSec)
{
	if(VP_THIRD_PERSON == m_eViewPoint) // ������ ���� �� ..
	{
		m_fRotYawVPGod += fRotYPerSec * s_fSecPerFrm; // ������ �ٶ� ������ ��쿡 ī�޶� ȸ������..
		return; // ���ư���!
	}
}

void CGameEng::CameraZoom(float fDelta)
{
	if(NULL == m_pActiveCam) return;
	if(VP_BACKWARD == m_eViewPoint || VP_FOWARD == m_eViewPoint)
	{
		m_fZoomBackwardOrFoward -= fDelta * s_fSecPerFrm; // 1.5��Ī, �տ��� �� �����϶� ī�޶� Zoom 1.0f �� �����̴�.
		if(m_fZoomBackwardOrFoward < 0.4f) m_fZoomBackwardOrFoward = 0.4f; // �Ʒ� ���°�
		else if(m_fZoomBackwardOrFoward >= 1.6f) m_fZoomBackwardOrFoward = 1.6f;
	}
	else if(VP_THIRD_PERSON == m_eViewPoint) // ������ ���� �� �Ÿ��� �ÿ��� �ٿ��� �Ѵ�..
	{
		// NOTE (zoom way back): search for "zoom way back" to get here

		m_fOffsetVPGod -= 4.0f * fDelta * s_fSecPerFrm;
		if(m_fOffsetVPGod < 2.0f) m_fOffsetVPGod = 2.0f; // �ʹ��۾����� ���߰�..
		else if(m_fOffsetVPGod > 10.0f) m_fOffsetVPGod = 10.0f; // �ʹ� Ŀ���� ���߰�..
		return; // ���ư���!
	}
}

void CGameEng::FarPlaneDeltaSet(float fFPDelta, bool bUpdateImmediately)
{
	if(fFPDelta < 0.25f) fFPDelta = 0.25f;
	if(fFPDelta > 1.0f) fFPDelta = 1.0f;

	m_fFPDeltaToReach = fFPDelta;
	if(bUpdateImmediately) m_fFPDeltaCur = fFPDelta;
}

//void CGameEng::LightDeltaSet(float fLightDelta, bool bUpdateImmediately)
//{
//	if(fLightDelta < 0.25f) fLightDelta = 0.25f;
//	if(fLightDelta > 1.0f) fLightDelta = 1.0f;
//
//	m_fLightDeltaToReach = fLightDelta;
//	if(bUpdateImmediately) m_fLightDeltaCur = fLightDelta;
//}

void CGameEng::Lightning()
{
	m_fLightningTimeRemain = LIGHTNING_DURATION; // ���� ĥ�� Ÿ�̸�..
}
