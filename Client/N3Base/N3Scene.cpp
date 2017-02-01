// N3Scene.cpp: implementation of the CN3Scene class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3Scene.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Scene::CN3Scene()
{
	m_dwType |= OBJ_SCENE;

	memset(m_pCameras, 0, sizeof(m_pCameras));
	memset(m_pLights, 0, sizeof(m_pLights));

	m_nCameraActive = 0;

	m_fFrmCur = 0.0f; // Animation Frame;
	m_fFrmStart = 0.0f; // ��ü ������.
	m_fFrmEnd = 1000.0f; // �⺻�� ������.

	m_nCameraCount = 0; 
	m_nLightCount = 0;

	m_bDisableDefaultLight = false;

	m_AmbientLightColor = 0;
}

CN3Scene::~CN3Scene()
{
	int i = 0;
	for(i = 0; i < MAX_SCENE_CAMERA; i++) { if(m_pCameras[i]) { delete m_pCameras[i]; m_pCameras[i] = NULL; } }
	for(i = 0; i < MAX_SCENE_LIGHT; i++) { if(m_pLights[i]) { delete m_pLights[i]; m_pLights[i] = NULL; } }
	
	this->ShapeRelease();
	this->ChrRelease();
}

void CN3Scene::Release()
{
	m_nCameraActive = 0;

	m_fFrmCur = 0.0f; // Animation Frame;
	m_fFrmStart = 0.0f; // ��ü ������.
	m_fFrmEnd = 1000.0f; // �⺻�� ������.

	int i = 0;
	for(i = 0; i < MAX_SCENE_CAMERA; i++) { if(m_pCameras[i]) { delete m_pCameras[i]; m_pCameras[i] = NULL; } }
	for(i = 0; i < MAX_SCENE_LIGHT; i++) { if(m_pLights[i]) { delete m_pLights[i]; m_pLights[i] = NULL; } }
	this->ShapeRelease();
	this->ChrRelease();

	m_nCameraCount = 0; 
	m_nLightCount = 0;

	m_bDisableDefaultLight = false;
}

bool CN3Scene::Load(HANDLE hFile)
{
	DWORD dwRWC = 0;
	
	ReadFile(hFile, &m_nCameraActive, 4, &dwRWC, NULL);
	ReadFile(hFile, &m_fFrmCur, 4, &dwRWC, NULL); // Animation Frame;
	ReadFile(hFile, &m_fFrmStart, 4, &dwRWC, NULL); // ��ü ������.
	ReadFile(hFile, &m_fFrmEnd, 4, &dwRWC, NULL); // ��ü ������.

	int i = 0, nL = 0;
	char szName[512] = "";

	int nCC = 0;
	ReadFile(hFile, &nCC, 4, &dwRWC, NULL); // ī�޶�..
	for(i = 0; i < nCC; i++)
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Camera* pCamera = new CN3Camera();
		if(false == pCamera->LoadFromFile(szName))
		{
			delete pCamera;
			continue;
		}

		this->CameraAdd(pCamera);
	}

	int nLC = 0;
	ReadFile(hFile, &nLC, 4, &dwRWC, NULL); // ī�޶�..
	for(i = 0; i < nLC; i++) 
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Light* pLight = new CN3Light();
		if(false == pLight->LoadFromFile(szName))
		{
			delete pLight;
			continue;
		}
		
		this->LightAdd(pLight);
	}

	int nSC = 0;
	ReadFile(hFile, &nSC, 4, &dwRWC, NULL); // Shapes..
	for(i = 0; i < nSC; i++)
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Shape* pShape = new CN3Shape();
		if(false == pShape->LoadFromFile(szName))
		{
			delete pShape;
			continue;
		}

		this->ShapeAdd(pShape);
	}

	int nChrC = 0;
	ReadFile(hFile, &nChrC, 4, &dwRWC, NULL); // ĳ����
	for(i = 0; i < nChrC; i++)
	{
		ReadFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		ReadFile(hFile, szName, nL, &dwRWC, NULL);
		szName[nL] = NULL;

		CN3Chr* pChr = new CN3Chr();
		if(false == pChr->LoadFromFile(szName))
		{
			delete pChr;
			continue;
		}

		this->ChrAdd(pChr);
	}

	if(m_nCameraCount <= 0) this->DefaultCameraAdd();
	if(m_nLightCount <= 0) this->DefaultLightAdd();

	return true;
}

bool CN3Scene::Save(HANDLE hFile)
{
	::CreateDirectory("Data", NULL);
	::CreateDirectory("Chr", NULL);
	::CreateDirectory("Object", NULL);
	::CreateDirectory("Item", NULL);

	DWORD dwRWC = 0;
	
	WriteFile(hFile, &m_nCameraActive, 4, &dwRWC, NULL);
	WriteFile(hFile, &m_fFrmCur, 4, &dwRWC, NULL); // Animation Frame;
	WriteFile(hFile, &m_fFrmStart, 4, &dwRWC, NULL); // ��ü ������.
	WriteFile(hFile, &m_fFrmEnd, 4, &dwRWC, NULL); // ��ü ������.

	int i = 0, nL = 0;
	
	WriteFile(hFile, &m_nCameraCount, 4, &dwRWC, NULL); // ī�޶�..
	for(i = 0; i < m_nCameraCount; i++)
	{
		nL = m_pCameras[i]->FileName().size();
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		WriteFile(hFile, m_pCameras[i]->FileName().c_str(), nL, &dwRWC, NULL);
		m_pCameras[i]->SaveToFile();
	}

	WriteFile(hFile, &m_nLightCount, 4, &dwRWC, NULL); // ī�޶�..
	for(i = 0; i < m_nLightCount; i++) 
	{
		nL = m_pLights[i]->FileName().size();
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		WriteFile(hFile, m_pLights[i]->FileName().c_str(), nL, &dwRWC, NULL);
		m_pLights[i]->SaveToFile();
	}

	int iSC = m_Shapes.size();
	WriteFile(hFile, &iSC, 4, &dwRWC, NULL); // Shapes..
	for(i = 0; i < iSC; i++)
	{
		nL = m_Shapes[i]->FileName().size();
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		WriteFile(hFile, m_Shapes[i]->FileName().c_str(), nL, &dwRWC, NULL);
		m_Shapes[i]->SaveToFile();
	}

	int iCC = m_Chrs.size();
	WriteFile(hFile, &iCC, 4, &dwRWC, NULL); // ĳ����
	for(i = 0; i < iCC; i++)
	{
		nL = m_Chrs[i]->FileName().size();
		WriteFile(hFile, &nL, 4, &dwRWC, NULL);
		if(nL <= 0) continue;

		WriteFile(hFile, m_Chrs[i]->FileName().c_str(), nL, &dwRWC, NULL);
		m_Chrs[i]->SaveToFile();
	}

	CN3Base::SaveResrc(); // Resource �� ���Ϸ� �����Ѵ�..

	return true;
}

void CN3Scene::Render()
{
	int i = 0;
//	for(i = 0; i < m_nCameraCount; i++)
//	{
//		__ASSERT(m_pCameras[i], "Camera pointer is NULL");
//		if(m_nCameraActive != i) m_pCameras[i]->Render();
//	}

//	for(i = 0; i < m_nLightCount; i++)
//	{
//		__ASSERT(m_pLights[i], "Light pointer is NULL");
//		m_pLights[i]->Render(NULL, 0.5f);
//	}
	s_lpD3DDev->SetRenderState(D3DRS_AMBIENT, m_AmbientLightColor);

	int iSC = m_Shapes.size();
	for(i = 0; i < iSC; i++)
	{
		m_Shapes[i]->Render();
	}

	int iCC = m_Chrs.size();
	for(i = 0; i < iCC; i++)
	{
		m_Chrs[i]->Render();
	}
}

void CN3Scene::Tick(float fFrm)
{
	if(FRAME_SELFPLAY == fFrm || fFrm <  m_fFrmStart || fFrm > m_fFrmEnd)
	{
		m_fFrmCur += 30.0f / CN3Base::s_fFrmPerSec; // �����ϰ� �����̵��� �ð��� ���� �����̴� ���� ����..
		if(m_fFrmCur > m_fFrmEnd) m_fFrmCur = m_fFrmStart;
	}
	else
	{
		m_fFrmCur = fFrm;
	}

	this->TickCameras(m_fFrmCur);
	this->TickLights(m_fFrmCur);
	this->TickShapes(m_fFrmCur);
	this->TickChrs(m_fFrmCur);
}

void CN3Scene::TickCameras(float fFrm)
{
	for(int i = 0; i < m_nCameraCount; i++)
	{
		m_pCameras[i]->Tick(m_fFrmCur);
		if(m_nCameraActive == i) m_pCameras[i]->Apply(); // ī�޶� ������ ���� �����Ѵ�..
	}
}

void CN3Scene::TickLights(float fFrm)
{
	for(int i = 0; i < 8; i++) s_lpD3DDev->LightEnable(i, FALSE); // �ϴ� ����Ʈ �� ����..
	
	for(int i = 0; i < m_nLightCount; i++)
	{
		m_pLights[i]->Tick(m_fFrmCur);
		m_pLights[i]->Apply(); // ����Ʈ ����
	}

	// ����Ʈ�� �׻� ī�޶� ������� �����..
	if(false == m_bDisableDefaultLight)
	{
		__Vector3 vDir = s_CameraData.vAt - s_CameraData.vEye;
		vDir.Normalize();
		D3DCOLORVALUE crLgt = { 1.0f, 1.0f, 1.0f, 1.0f };

		CN3Light::__Light lgt;
		lgt.InitDirection(7, vDir, crLgt);

		s_lpD3DDev->LightEnable(7, TRUE);
		s_lpD3DDev->SetLight(7, &lgt);
	}

	// Ambient Light �ٲٱ�..
//	DWORD dwAmbient =	0xff000000 | 
//						(((DWORD)(m_pLights[i]->m_Data.Diffuse.r * 255 * 0.5f)) << 16) | 
//						(((DWORD)(m_pLights[i]->m_Data.Diffuse.g * 255 * 0.5f)) << 8) | 
//						(((DWORD)(m_pLights[i]->m_Data.Diffuse.b * 255 * 0.5f)) << 0);
//	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_AMBIENT, dwAmbient);
}

void CN3Scene::TickShapes(float fFrm)
{
	int iSC = m_Shapes.size();
	for(int i = 0; i < iSC; i++)
	{
		m_Shapes[i]->Tick(m_fFrmCur);
	}
}

void CN3Scene::TickChrs(float fFrm)
{
	int iCC = m_Chrs.size();
	for(int i = 0; i < iCC; i++)
	{
		m_Chrs[i]->Tick(m_fFrmCur);
	}
}

int CN3Scene::CameraAdd(CN3Camera *pCamera)
{
	if(m_nCameraCount >= MAX_SCENE_CAMERA) return -1;
	if(NULL == pCamera) return -1;

	if(m_pCameras[m_nCameraCount]) delete m_pCameras[m_nCameraCount];
	m_pCameras[m_nCameraCount] = pCamera;

	m_nCameraCount++;
	return m_nCameraCount;
}

void CN3Scene::CameraDelete(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_nCameraCount) return;

	delete m_pCameras[iIndex];
	m_pCameras[iIndex] = NULL;
	
	m_nCameraCount--;
	for(int i = iIndex; i < m_nCameraCount; i++) m_pCameras[i] = m_pCameras[i+1];
	m_pCameras[m_nCameraCount] = NULL;
}

void CN3Scene::CameraDelete(CN3Camera *pCamera)
{
	for(int i = 0; i < m_nCameraCount; i++) 
	{
		if(m_pCameras[i] == pCamera) 
		{
			this->CameraDelete(i);
			break;
		}
	}
}

void CN3Scene::CameraSetActive(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_nCameraCount) return;
	
	m_nCameraActive = iIndex;
}

int CN3Scene::LightAdd(CN3Light *pLight)
{
	if(NULL == pLight) return -1;

	if(m_pLights[m_nLightCount]) delete m_pLights[m_nLightCount];
	m_pLights[m_nLightCount] = pLight;

	m_nLightCount++;
	return m_nLightCount;
}

void CN3Scene::LightDelete(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_nLightCount) return;

	delete m_pLights[iIndex];
	m_pLights[iIndex] = NULL;
	
	m_nLightCount--;
	for(int i = iIndex; i < m_nLightCount; i++) m_pLights[i] = m_pLights[i+1];
	m_pLights[m_nLightCount] = NULL;
}

void CN3Scene::LightDelete(CN3Light *pLight)
{
	for(int i = 0; i < m_nLightCount; i++) 
	{
		if(m_pLights[i] == pLight) 
		{
			this->LightDelete(i);
			break;
		}
	}
}

int CN3Scene::ShapeAdd(CN3Shape *pShape)
{
	if(NULL == pShape) return -1;
	m_Shapes.push_back(pShape);

	return m_Shapes.size();
}

void CN3Scene::ShapeDelete(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_Shapes.size()) return;

	it_Shape it = m_Shapes.begin(), itEnd = m_Shapes.end();
	for(int i = 0; i < iIndex; i++, it++);
	CN3Shape* pShape = *it;
	delete pShape;
	it = m_Shapes.erase(it);
}

void CN3Scene::ShapeDelete(CN3Shape* pShape)
{
	it_Shape it = m_Shapes.begin(), itEnd = m_Shapes.end();
	for(; it != itEnd; it++)
	{
		CN3Shape* pShapeSrc = *it;
		if(pShapeSrc == pShape)
		{
			delete pShapeSrc;
			it = m_Shapes.erase(it);
			return;
		}
	}
}

void CN3Scene::ShapeRelease()
{
	int iSC = m_Shapes.size();
	for(int i = 0; i < iSC; i++)
	{
		delete m_Shapes[i];
	}
	m_Shapes.clear();
}

int CN3Scene::ChrAdd(CN3Chr *pChr)
{
	if(NULL == pChr) return -1;
	m_Chrs.push_back(pChr);

	return m_Chrs.size();
}

void CN3Scene::ChrDelete(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_Chrs.size()) return;

	it_Chr it = m_Chrs.begin(), itEnd = m_Chrs.end();
	for(; it != itEnd; it++);

	CN3Chr* pChr = *it; delete pChr;
	m_Chrs.erase(it);
}

void CN3Scene::ChrDelete(CN3Chr* pChr)
{
	it_Chr it = m_Chrs.begin(), itEnd = m_Chrs.end();
	CN3Chr* pChrSrc;
	for(; it != itEnd; it++);
	{
		pChrSrc = *it;
		if(pChr == pChrSrc) 
		{
			delete pChrSrc;
			m_Chrs.erase(it);
			return;
		}
	}
}

void CN3Scene::ChrRelease()
{
	int iCC = m_Chrs.size();
	for(int i = 0; i < iCC; i++) delete m_Chrs[i];
	m_Chrs.clear();
}

/*
bool CN3Scene::CheckOverlappedShapesAndReport()
{
	// �̸� �ߺ� �˻�..
	CN3Transform* pShapes[8192];
	memset(pShapes, 0, 8192*4);

	int i, j, nBC = 0;
//	for(i = 0; i < m_nCameraCount; i++) pTransforms[nBC++] = m_pCameras[i];
//	for(i = 0; i < m_nLightCount; i++) pTransforms[nBC++] = m_pLights[i];
	it_Shape it = m_Shapes.begin(), itEnd = m_Shapes.end();
	for(; it != itEnd; it++)
	{
		CN3Shape* pShape = *it;
		pShapes[nBC++] = pShape;
	}
//	for(i = 0; i < m_nChrCount; i++) pTransforms[nBC++] = m_pChrs[i];

	bool bOverlapped = false;
	__Vector3 vPos1, vPos2;
	for(i = 0; i < nBC; i++)
	{
		vPos1 = pShapes[i]->Pos();
		for(j = i+1; j < nBC; j++)
		{
			vPos2 = pShapes[j]->Pos();
			if(	vPos1 == vPos2 ||
				pShapes[i]->FileName() == pShapes[j]->FileName() ) // ��ġ�� �̸��� ���� ������Ʈ�� �ִ��� ã�ƺ���.
			{
				char szErr[512];
				__Vector3 vPos = pShapes[j]->Pos();
				sprintf(szErr, "���� �̸��� ���ų� ��ġ�� ���� ������Ʈ�� �ֽ��ϴ�\nù��° ������Ʈ : \"%s\" - ��ġ(%f, %f, %f)\n�ι�° ������Ʈ : \"%s\" - ��ġ(%f, %f, %f)",
					pShapes[i]->FileName().c_str(), vPos1.x, vPos1.y, vPos1.z,
					pShapes[j]->FileName().c_str(), vPos2.x, vPos2.y, vPos2.z);
				MessageBox(::GetActiveWindow(), szErr, "Scene ������Ʈ �ߺ� ����", MB_OK);
				bOverlapped = true;
			}
		}
	}

	return bOverlapped;
}

void CN3Scene::DeleteOverlappedShapes()
{
	// �̸� �ߺ� �˻�..
	BOOL bNeedDeletes[8192];
	it_Shape itShapes[8192];
	CN3Shape* pShapes[8192];
	memset(bNeedDeletes, 0, sizeof(bNeedDeletes));
	memset(itShapes, 0, sizeof(itShapes));
	memset(itShapes, 0, sizeof(pShapes));

	int i, j, nBC = 0;
	it_Shape it = m_Shapes.begin(), itEnd = m_Shapes.end();
	for(; it != itEnd; it++)
	{
		itShapes[nBC] = it;

		CN3Shape* pShape = *it;
		pShapes[nBC++] = pShape;
	}
//	for(i = 0; i < m_nChrCount; i++) pTransforms[nBC++] = m_pChrs[i];

	int iNeedDeleteCount = 0;
	__Vector3 vPos1, vPos2;
	for(i = 0; i < nBC; i++)
	{
		vPos1 = pShapes[i]->Pos();
		for(j = i+1; j < nBC; j++)
		{
			vPos2 = pShapes[j]->Pos();
			if(	vPos1 == vPos2 ||
				pShapes[i]->FileName() == pShapes[j]->FileName() ) // ��ġ�� �̸��� ���� ������Ʈ�� �ִ��� ã�ƺ���.
			{
				bNeedDeletes[j] = true;
			}
		}
	}

	for(i = 0; i < nBC; i++)
	{
		if(bNeedDeletes[i])
		{
			CN3Shape* pShape = *(itShapes[i]);
			delete pShape;
			m_Shapes.erase(itShapes[i]);
		}
	}
}
*/
bool CN3Scene::LoadDataAndResourcesFromFile(const std::string& szFN)
{
	if(szFN.empty()) return false;

	char szPath[512] = "", szDrv[_MAX_DRIVE] = "", szDir[_MAX_DIR] = "";
	::_splitpath(szFN.c_str(), szDrv, szDir, NULL, NULL);
	::_makepath(szPath, szDrv, szDir, NULL, NULL);

	this->Release();
	this->PathSet(szPath);
	return LoadFromFile(szFN);
}

bool CN3Scene::SaveDataAndResourcesToFile(const std::string& szFN)
{
	if(szFN.empty()) return false;

	char szPath[512] = "", szDrv[_MAX_DRIVE] = "", szDir[_MAX_DIR] = "";
	::_splitpath(szFN.c_str(), szDrv, szDir, NULL, NULL);
	::_makepath(szPath, szDrv, szDir, NULL, NULL);

	this->PathSet(szPath);
	return SaveToFile(szFN);
}

void CN3Scene::DefaultCameraAdd()
{
	CN3Camera* pCamera = new CN3Camera();
	pCamera->m_szName = "DefaultCamera";
	pCamera->FileNameSet("Data\\DefaultCamera.N3Camera");
	this->CameraAdd(pCamera);
}
void CN3Scene::DefaultLightAdd()
{
	// Light �ʱ�ȭ..
	CN3Light* pLight = new CN3Light();
	pLight->m_szName = "DefaultLight";
	pLight->FileNameSet("Data\\DefaultLight.N3Light");
	int nLight = this->LightAdd(pLight) - 1;

	D3DCOLORVALUE ltColor = { 0.7f, 0.7f, 0.7f, 1.0f};
	pLight->m_Data.InitDirection(0, __Vector3(-1.0f,-1.0f,0.5f), ltColor);
	pLight->PosSet(1000.0f, 1000.0f, -1000.0f);
	pLight->m_Data.bOn = TRUE;
	pLight->m_Data.nNumber = nLight;
}

