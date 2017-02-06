// GameProcCharacterSelect.cpp: implementation of the CGameProcCharacterSelect class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "GameProcCharacterSelect.h"

//#include "Resource.h"
#include "PacketDef.h"
#include "GameEng.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "PlayerMySelf.h"

#include "UIManager.h"
#include "UICharacterSelect.h"
#include "UIMessageBox.h"
#include "UILoading.h"

#include "N3SndObjStream.h"
#include "N3Shape.h"
#include "N3Camera.h"
#include "N3Light.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcCharacterSelect::CGameProcCharacterSelect()
{
	baseInit();
}

CGameProcCharacterSelect::~CGameProcCharacterSelect()
{
	delete m_pCamera;

	for (int i = 0; i < 8; i++)
	{
		delete m_pLights[i];
	}

	for (int i = 0; i < MAX_AVAILABLE_CHARACTER; i++)
	{
		delete m_pChrs[i];
	}

	delete m_pActiveBg;
	delete m_pUICharacterSelect;

	::ShowCursor(TRUE);
}

#pragma region graphicsHandlers
void CGameProcCharacterSelect::baseInit()
{
	m_pCamera = NULL;

	for (int i = 0; i < 8; i++)
	{
		m_pLights[i] = NULL;
	}

	for (int i = 0; i < MAX_AVAILABLE_CHARACTER; i++)
	{
		m_pChrs[i] = NULL;
	}

	m_pActiveBg = NULL;

	m_eCurPos = &SelectCharacterPositionEnum::CENTER;
	m_eDestPos = &SelectCharacterPositionEnum::CENTER;
	m_eCurProcess = PROCESS_PRESELECT;
	m_fCurTheta = 0.0f;
	m_fFadeOut = 0.0f;
	m_bFadeOutRender = false;

	m_bReceivedCharacterSelect = false;
	m_pUICharacterSelect = NULL;

	m_pSnd_Rotate = NULL;
}

void CGameProcCharacterSelect::Init()
{
	baseInit();

	CGameProcedure::Init();

	CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Rotate);
	m_pSnd_Rotate = CN3Base::s_SndMgr.CreateObj(ID_SOUND_CHR_SELECT_ROTATE);
	s_pUIMgr->EnableOperationSet(false);

	m_pCamera = new CN3Camera();

	for (int i = 0; i < 8; i++)
	{
		m_pLights[i] = new CN3Light();
	}

	CGameProcedure::s_iChrSelectIndex = SelectCharacterPositionEnum::CENTER.getValue();

	NationEnum * eNation = s_pPlayer->m_InfoBase.eNation;
	__TABLE_UI_RESRC* pTbl = s_pTbl_UI->Find(eNation->getValue());

	m_pUICharacterSelect = new CUICharacterSelect();
	m_pUICharacterSelect->Init(s_pUIMgr);

	if (pTbl)
	{
		m_pUICharacterSelect->LoadFromFile(pTbl->szCharacterSelect);
	}

	m_pUICharacterSelect->SetPos(0,0);
	s_pUIMgr->SetFocusedUI((CN3UIBase*)m_pUICharacterSelect);

	if (*eNation == NationEnum::KARUS)
	{
		m_vEye.Set(0.0f, -0.2f, 7.4f);
	}

	else if (*eNation == NationEnum::ELMORAD)
	{
		m_vEye.Set(0.0f, -0.2f, 7.0f);
	}

	m_vAt.Set(0.0f, -0.4f, -0.0f);
	m_vUp.Set(0.0f, 1.0f, 0.0f);

	m_vEyeBackup = m_vEye;

	m_pActiveBg = new CN3Shape;	
	memset(&m_lgt[SelectCharacterPositionEnum::CENTER.getValue()], 0, sizeof(D3DLIGHT9));	
	memset(&m_lgt[SelectCharacterPositionEnum::LEFT.getValue()], 0, sizeof(D3DLIGHT9));	
	memset(&m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()], 0, sizeof(D3DLIGHT9));	

	m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Type = m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Type = m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Type = D3DLIGHT_SPOT;
	m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Attenuation0 = m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Attenuation0 = m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Attenuation0 = 0.1f;
	m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Attenuation1 = m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Attenuation1 = m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Attenuation1 = 0.0f;
	m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Attenuation2 = m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Attenuation2 = m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Attenuation2 = 0.0f;
	m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Range = m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Range = m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Range = 6.0f;
	m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Diffuse.r = m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Diffuse.r = m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Diffuse.r = 255/255.0f; 
	m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Diffuse.g = m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Diffuse.g = m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Diffuse.g = 255/255.0f; 
	m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Diffuse.b = m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Diffuse.b = m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Diffuse.b = 255/255.0f;
	m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Falloff = m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Falloff = m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Falloff = 20.0f;

	__Vector3 vTemp; 

	if (*eNation == NationEnum::KARUS)
	{
		m_pActiveBg->LoadFromFile("ChrSelect\\ka_chairs.n3shape");
		
		// Light..
		m_pLights[SelectCharacterPositionEnum::CENTER.getValue()]->LoadFromFile("ChrSelect\\ka_light_0.n3light");
		m_pLights[SelectCharacterPositionEnum::LEFT.getValue()]->LoadFromFile("ChrSelect\\ka_light_1.n3light");
		m_pLights[SelectCharacterPositionEnum::RIGHT.getValue()]->LoadFromFile("ChrSelect\\ka_light_2.n3light");

		m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Position = m_vEye;
		m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Position.y += 2.0f;
		vTemp.Set(0.0f, 0.0f, 3.5f);	
		vTemp -= m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Position;
		m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Direction = vTemp;
		m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Phi = 0.6f;

		m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Position = __Vector3(5.87f, 2.4f, 4.73f);
		vTemp.Set(2.32f, 0.0f, 2.54f);
		vTemp -= m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Position;
		m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Direction = vTemp;
		m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Phi = 0.6f;

		m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Position = __Vector3(-5.87f, 2.4f, 4.73f);
		vTemp.Set(-2.32f, 0.0f, 2.54f);
		vTemp -= m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Position;
		m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Direction = vTemp;
		m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Phi = 0.6f;
	}

	else if (*eNation == NationEnum::ELMORAD)
	{
		m_pActiveBg->LoadFromFile("ChrSelect\\el_chairs.n3shape");

		// Light..
		m_pLights[SelectCharacterPositionEnum::CENTER.getValue()]->LoadFromFile("ChrSelect\\el_light_0.n3light");
		m_pLights[SelectCharacterPositionEnum::LEFT.getValue()]->LoadFromFile("ChrSelect\\el_light_1.n3light");
		m_pLights[SelectCharacterPositionEnum::RIGHT.getValue()]->LoadFromFile("ChrSelect\\el_light_2.n3light");

		m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Position = m_vEye;	
		m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Position.y += 2.0f;
		vTemp.Set(0.0f, -0.1f, 3.0f);
		vTemp -= m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Position;
		m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Direction = vTemp;
		m_lgt[SelectCharacterPositionEnum::CENTER.getValue()].Phi = 0.45f;

		m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Position = __Vector3(5.6f, 2.4f, 4.68f);
		vTemp.Set(2.2f, -0.1f, 2.36f);	vTemp -= m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Position;
		m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Direction = vTemp;
		m_lgt[SelectCharacterPositionEnum::LEFT.getValue()].Phi = 0.45f;

		m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Position = __Vector3(-5.6f, 2.4f, 4.68f);
		vTemp.Set(-2.4f, -0.1f, 2.23f);	vTemp -= m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Position;
		m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Direction = vTemp;
		m_lgt[SelectCharacterPositionEnum::RIGHT.getValue()].Phi = 0.45f;
	}

	this->MsgSend_RequestAllCharacterInfo();
}

void CGameProcCharacterSelect::Tick()
{
	CGameProcedure::Tick();

	__Vector3 vDir = CN3Base::s_CameraData.vAt - CN3Base::s_CameraData.vEye;
	vDir.Normalize();

	__Vector3 vEye = CN3Base::s_CameraData.vEye; 
	__Vector3 vUp(0,1,0);
	CN3SndObj::SetListenerPos(&vEye);
	CN3SndObj::SetListenerOrientation(&vDir, &vUp);

	m_pActiveBg->Tick();

	if(s_pUIMgr->m_bDoneSomething == false && s_pUIMgr->EnableOperation())
	{
		s_pUIMgr->SetFocusedUI(m_pUICharacterSelect);
		int nMFlags = s_pLocalInput->MouseGetFlag(); // Mouse
	
		if(nMFlags & MOUSE_LBCLICK)
		{
			if (m_eCurProcess != PROCESS_ROTATEING)
			{
				D3DVIEWPORT9 vp;
				CN3Base::s_lpD3DDev->GetViewport(&vp);

				RECT rc = { vp.Width * 0.36f, vp.Height * 0.44f, vp.Width * 0.64f, vp.Height * 0.86f };
				POINT pt = s_pLocalInput->MouseGetPos();

				if (::PtInRect(&rc, pt))
				{
					CharacterSelectOrCreate();
				}
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		s_pEng->s_lpD3DDev->LightEnable(i, FALSE);
	}
	
	for(int i = 0; i < 2; i++)
	{
		m_pLights[i]->Tick(m_pLights[i]->m_fFrmCur);
		m_pLights[i]->Apply();
	}
	
	CheckJobState();

	if (!IsUIKeyOperated())
	{
		if (s_pLocalInput->IsKeyPress(SDL_SCANCODE_RETURN))
		{
			ProcessOnReturn();
		}
	}
}

void CGameProcCharacterSelect::Render()
{
	D3DCOLOR crEnv = 0x00000000;
	s_pEng->Clear(crEnv);
	s_pEng->s_lpD3DDev->BeginScene();

	__Matrix44 mtxWorld;
	mtxWorld.Identity();
	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld); 

	m_pCamera->EyePosSet(m_vEye);
	m_pCamera->AtPosSet(m_vAt);
	m_pCamera->UpVectorSet(0,1,0);
	m_pCamera->m_Data.fNP = 0.1f;
	m_pCamera->m_Data.fFP = 100.0f;
	m_pCamera->m_Data.fFOV = 0.96f;

	m_pCamera->Tick();
	m_pCamera->Apply();

	m_pActiveBg->Render();

	for (int i = 0; i < MAX_AVAILABLE_CHARACTER; i++)
	{
		if (m_pChrs[i])
		{
			m_pChrs[i]->m_nLOD = 0;
			m_pChrs[i]->Render();
		}
	}

	CUIManager::RenderStateSet();
	m_pUICharacterSelect->Render();
	CUIManager::RenderStateRestore();

	CGameProcedure::Render(); // UI

	if (m_bFadeOutRender)
	{
		FadeOutRender();
	}

	s_pEng->s_lpD3DDev->EndScene();
	s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcCharacterSelect::Release()
{
	CGameProcedure::Release();

	delete m_pCamera; m_pCamera = NULL;
	
	for (int i = 0; i < 8; i++)
	{
		delete m_pLights[i];
		m_pLights[i] = NULL;
	}

	for (int i = 0; i < MAX_AVAILABLE_CHARACTER; i++)
	{
		delete m_pChrs[i];
		m_pChrs[i] = NULL;

		m_InfoChrs[i].clear();
	}

	delete m_pActiveBg;
	m_pActiveBg = NULL;

	delete m_pUICharacterSelect;
	m_pUICharacterSelect = NULL;

	CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Rotate);

	::ShowCursor(TRUE);
}

void CGameProcCharacterSelect::IncreseLightFactor()
{
	if (*s_pPlayer->m_InfoBase.eNation == NationEnum::KARUS)
	{
		m_fCurTheta += KARUS_INCRESE_OFFSET;

		if (m_fCurTheta > KARUS_THETA_MAX)
		{
			m_fCurTheta = KARUS_THETA_MAX;
		}
	}

	else if (*s_pPlayer->m_InfoBase.eNation == NationEnum::ELMORAD)
	{
		m_fCurTheta += ELMORAD_INCRESE_OFFSET;

		if (m_fCurTheta > ELMORAD_THERA_MAX)
		{
			m_fCurTheta = ELMORAD_THERA_MAX;
		}
	}
}

void CGameProcCharacterSelect::DecreseLightFactor()
{
	if (*s_pPlayer->m_InfoBase.eNation == NationEnum::KARUS)
	{
		m_fCurTheta -= KARUS_INCRESE_OFFSET;
	}

	else if (*s_pPlayer->m_InfoBase.eNation == NationEnum::ELMORAD)
	{
		m_fCurTheta -= ELMORAD_INCRESE_OFFSET;
	}

	if (m_fCurTheta < 0.0f)
	{
		m_fCurTheta = 0.0f;
	}
}

void CGameProcCharacterSelect::FadeOutProcess()
{
	m_fFadeOut += (30.0f / CN3Base::s_fFrmPerSec)*1.5f;

	if (m_fFadeOut > 255)
	{
		m_fFadeOut = 255;
	}
}

void CGameProcCharacterSelect::FadeOutRender()
{
	__VertexTransformedColor pVertices[4];

	pVertices[0].Set(0.0f, 0.0f, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut, 0x00, 0x00, 0x00));
	pVertices[1].Set(s_CameraData.vp.Width, 0.0f, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut, 0x00, 0x00, 0x00));
	pVertices[2].Set(s_CameraData.vp.Width, s_CameraData.vp.Height, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut, 0x00, 0x00, 0x00));
	pVertices[3].Set(0.0f, s_CameraData.vp.Height, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut, 0x00, 0x00, 0x00));

	DWORD dwUsefog = TRUE;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwUsefog);

	DWORD dwUseLighting = TRUE;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_LIGHTING, &dwUseLighting);

	DWORD dwUseColorVertex = FALSE;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_COLORVERTEX, &dwUseColorVertex);

	unsigned long  bUseAlphaBlend = TRUE;
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &bUseAlphaBlend);

	int	bLight[8];
	for (int i = 0; i < 8; i++)
	{
		CN3Base::s_lpD3DDev->GetLightEnable(i, &bLight[i]);
	}

	if (bUseAlphaBlend == FALSE)
	{
		CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	}

	if (dwUseLighting)
	{
		CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	if (dwUsefog)
	{
		CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}

	// set render states
	if (dwUseColorVertex == FALSE)
	{
		CN3Base::s_lpD3DDev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	}

	for (int i = 0; i < 8; i++)
	{
		CN3Base::s_lpD3DDev->LightEnable(i, FALSE);
	}

	DWORD dwTexStageCO, dwTexStageCARG1, dwTexStageAO, dwTexStageAARG1, dwRSSB, dwRSDB;

	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwTexStageCO);
	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLORARG1, &dwTexStageCARG1);
	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwTexStageAO);
	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwTexStageAARG1);

	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwRSSB);
	CN3Base::s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwRSDB);

	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);
	CN3Base::s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(__VertexTransformedColor));

	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, dwTexStageCO);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, dwTexStageCARG1);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwTexStageAO);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwTexStageAARG1);

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwRSSB);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwRSDB);

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_COLORVERTEX, dwUseColorVertex);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, bUseAlphaBlend);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, dwUseLighting);
	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwUsefog);

	for (int i = 0; i < 8; i++)
	{
		CN3Base::s_lpD3DDev->LightEnable(i, bLight[i]);
	}
}

#pragma endregion graphicsHandlers

void CGameProcCharacterSelect::CheckJobState()
{
	switch (m_eCurProcess)
	{
	case PROCESS_PRESELECT:
		DoProcPreselect();
		break;

	case PROCESS_ROTATEING:
		handleRotation();
		break;

	case PROCESS_SELECTED:
		DoSelectedChrProc();
		break;
	}
}

void CGameProcCharacterSelect::DoProcPreselect()
{
	D3DVIEWPORT9 vp;
	CN3Base::s_lpD3DDev->GetViewport(&vp);

	float left = vp.Width * 0.36f;
	float right = vp.Width * 0.64f;
	float top = vp.Height * 0.44f;
	float bottom = vp.Height * 0.86f;

	POINT pt = s_pLocalInput->MouseGetPos();

	int	iPosIndex = m_eCurPos->getValue();

	//check if mouse is on the character
	if ((pt.x > left) && (pt.x < right) && (pt.y < bottom) && (pt.y > top))
	{
		// Doing..
		if (m_pChrs[iPosIndex])
		{
			m_pChrs[iPosIndex]->AniCurSet(SELECT_ANIM_PRE_SELECT, true);
			m_pChrs[iPosIndex]->Tick();
		}

		IncreseLightFactor();

		m_pUICharacterSelect->DisplayChrInfo(&m_InfoChrs[iPosIndex]);
	}
	else
	{
		// Doing..
		if (m_pChrs[iPosIndex])
		{
			m_pChrs[iPosIndex]->AniCurSet(SELECT_ANIM_DUMMY, true);
			m_pChrs[iPosIndex]->Tick();
		}

		DecreseLightFactor();

		m_pUICharacterSelect->DontDisplayInfo();
	}

	// Light..
	for (int i = 0; i < 3; i++)
	{
		m_lgt[i].Theta = 0.0f;
	}

	m_lgt[iPosIndex].Theta = m_fCurTheta;

	if (*s_pPlayer->m_InfoBase.eNation == NationEnum::KARUS)
	{
		if (m_lgt[iPosIndex].Theta != 0.0f)
		{
			s_pEng->s_lpD3DDev->LightEnable(iPosIndex + 4, TRUE);
			s_pEng->s_lpD3DDev->SetLight(iPosIndex + 4, &m_lgt[iPosIndex]);
		}

		else
		{
			s_pEng->s_lpD3DDev->LightEnable(iPosIndex + 4, FALSE);
		}
	}

	else if (*s_pPlayer->m_InfoBase.eNation == NationEnum::ELMORAD)
	{
		if (m_lgt[iPosIndex].Theta != 0.0f)
		{
			s_pEng->s_lpD3DDev->LightEnable(iPosIndex + 4, TRUE);
			s_pEng->s_lpD3DDev->SetLight(iPosIndex + 4, &m_lgt[iPosIndex]);
		}

		else
		{
			s_pEng->s_lpD3DDev->LightEnable(iPosIndex + 4, FALSE);
		}
	}
}

#pragma region rotation
void CGameProcCharacterSelect::DoJobRotate(e_Side side)
{
	//busy rotating
	if (m_eCurProcess == PROCESS_ROTATEING)
	{
		return;
	}

	//calc desire position
	int position = m_eCurPos->getValue();

	if (side == e_Side::left)
	{
		position--;
	}

	else
	{
		position++;
	}

	//can't rotate to that side
	if (position < 0 || position >= MAX_AVAILABLE_CHARACTER)
	{
		return;
	}

	//reset light
	if (m_fCurTheta != 0.0f)
	{
		m_fCurTheta = 0.0f;
		m_fFadeOut = 0.0f;
		m_bFadeOutRender = false;
	}

	//play sound
	if (m_pSnd_Rotate)
	{
		m_pSnd_Rotate->Play();
	}

	//change state to rotating
	m_eCurProcess = PROCESS_ROTATEING;

	//update the desired position
	m_eDestPos = &SelectCharacterPositionEnum::forValue(position);
	CGameProcedure::s_iChrSelectIndex = m_eDestPos->getValue();
}

void CGameProcCharacterSelect::handleRotation()
{
	e_Side rotateSide;

	//need to go right to reach destination
	if (m_eCurPos->getValue() < m_eDestPos->getValue())
	{
		rotateSide = e_Side::right;
	}

	//need to go left to reach destination
	else
	{
		rotateSide = e_Side::left;
	}

	rotate(rotateSide);
}

void CGameProcCharacterSelect::rotate(e_Side side)
{
	//if already reached destination no need in rotating - update state
	if (isRotateDone(side))
	{
		m_eCurProcess = PROCESS_PRESELECT;
		m_eCurPos = m_eDestPos;
	}

	//do the rotation
	else
	{
		float rotateAmount;
		
		switch (side)
		{
		case e_Side::left:
			rotateAmount = CN3Base::s_fSecPerFrm*1.2f;
			break;
		case e_Side::right:
			rotateAmount = -CN3Base::s_fSecPerFrm*1.2f;
			break;
		default:
			break;
		}
		
		__Matrix44 mtxrot;
		mtxrot.Identity();
		__Vector3 vOffs;

		mtxrot.RotationY(rotateAmount);

		vOffs = m_vEye - m_vAt;
		vOffs *= mtxrot;
		m_vEye = vOffs + m_vAt;
	}
}

bool CGameProcCharacterSelect::isRotateDone(e_Side side)
{
	//move from the center
	if (*m_eCurPos == SelectCharacterPositionEnum::CENTER)
	{
		float rotationY;
		float diff = 0;

		switch (side)
		{
		case e_Side::left:
			rotationY = 0.01f;
			break;
		case e_Side::right:
			rotationY = -0.01f;
			break;
		}

		if (*s_pPlayer->m_InfoBase.eNation == NationEnum::KARUS)
		{
			diff = 0.75f;
		}

		else if (*s_pPlayer->m_InfoBase.eNation == NationEnum::ELMORAD)
		{
			diff = 0.74f;
		}

		__Matrix44 mtxrot; mtxrot.Identity();
		__Vector3 vOffs, v1, v2, v3;
		__Vector3 vEyeTemp;

		vEyeTemp = m_vEye;
		mtxrot.RotationY(rotationY);
		vOffs = vEyeTemp - m_vAt;
		vOffs *= mtxrot;
		vEyeTemp = vOffs + m_vAt;

		v1 = m_vEyeBackup - m_vAt;
		v2 = vEyeTemp - m_vAt;
		v1.Normalize(); v2.Normalize();

		//check if destination reached
		return v1.Dot(v2) < diff;
	}

	else
	{
		bool done;

		//move from the left to the center
		if (*m_eCurPos == SelectCharacterPositionEnum::LEFT)
		{
			done = m_vEye.x < 0.0f;
		}

		//move from the right to the center
		else if (*m_eCurPos == SelectCharacterPositionEnum::RIGHT)
		{
			done = m_vEye.x > 0.0f;
		}

		if (done)
		{
			m_vEye = m_vEyeBackup;
		}

		return done;
	}
}
#pragma endregion rotation

void CGameProcCharacterSelect::CharacterSelectOrCreate()
{
	CGameProcedure::MessageBoxClose(-1);

	int iIndex = CGameProcedure::s_iChrSelectIndex;

	//empty sit -> need to create char
	if (NULL == m_pChrs[iIndex])
	{
		CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcCharacterCreate);
	}

	//select the character
	else
	{
		s_pPlayer->m_InfoExt.iZoneInit = 0x01;
		s_pPlayer->m_InfoExt.iZoneCur = m_InfoChrs[iIndex].iZone;
		s_pPlayer->IDSet(-1, m_InfoChrs[iIndex].szID, 0xffffffff);

		this->MsgSend_CharacterSelect();
	}
}

void CGameProcCharacterSelect::AddChr(SelectCharacterPositionEnum & eCP, __CharacterSelectInfo* pInfo)
{
	int	iPosIndex = eCP.getValue();

	if (NULL == m_pChrs[iPosIndex])
	{
		m_pChrs[iPosIndex] = new CN3Chr();
	}

	else
	{
		m_pChrs[iPosIndex]->Release();
	}

	__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks->Find(pInfo->eRace);	// User Player Character Skin
	__ASSERT(pLooks, "Table"); //TODO fix this

	m_pChrs[iPosIndex]->PartAlloc(PART_POS_COUNT);
	m_pChrs[iPosIndex]->PlugAlloc(PLUG_POS_COUNT);

	std::string szJointFN, szAniFN, szPlug0FN, szPlug1FN;
	getRaceGraphics(pInfo->eRace, pInfo->eClass, szJointFN, szAniFN, szPlug0FN, szPlug1FN);
	
	m_pChrs[iPosIndex]->JointSet(szJointFN);
	m_pChrs[iPosIndex]->AniCtrlSet(szAniFN);
	m_pChrs[iPosIndex]->PlugSet(0, szPlug0FN);
	m_pChrs[iPosIndex]->PlugSet(1, szPlug1FN);

	//add chest to the character
	this->AddChrPart(iPosIndex, pLooks, PART_POS_UPPER, pInfo->dwItemUpper, pInfo->iItemUpperDurability);
	
	__TABLE_ITEM_BASIC* pItemUpper = this->s_pTbl_Items_Basic->Find(pInfo->dwItemUpper);
	
	//if the chest armor type is 'robe' no need to show pants
	if (pItemUpper && pItemUpper->byIsRobeType)
	{
		m_pChrs[iPosIndex]->PartSet(PART_POS_LOWER, "");
	}

	//add pants to the character
	else
	{
		this->AddChrPart(iPosIndex, pLooks, PART_POS_LOWER, pInfo->dwItemLower, pInfo->iItemLowerDurability);
	}
	
	//add gloves to the character
	this->AddChrPart(iPosIndex, pLooks, PART_POS_HANDS, pInfo->dwItemGloves, pInfo->iItemGlovesDurability);

	//add boots to the character
	this->AddChrPart(iPosIndex, pLooks, PART_POS_FEET, pInfo->dwItemShoes, pInfo->iItemShoesDurability);

	char szBuff[256] = "";
	std::string szResrcFN;
 
	if(!pLooks->szPartFNs[PART_POS_FACE].empty())
	{
		char szBuff[256] = "";
		char szDir[128] = "";
		char szFName[128] = "";
		char szExt[16] = "";

		::_splitpath(pLooks->szPartFNs[PART_POS_FACE].c_str(), NULL, szDir, szFName, szExt);
		sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, pInfo->iFace, szExt);
		m_pChrs[iPosIndex]->PartSet(PART_POS_FACE, szBuff);
	}
 
	__TABLE_ITEM_BASIC* pItemHelmet = this->s_pTbl_Items_Basic->Find(pInfo->dwItemHelmet);

	//add helmet to the character if needed (some are invisible, like mage helmet)
	if(pItemHelmet && pItemHelmet->dwIDResrc)
	{
		this->AddChrPart(iPosIndex, pLooks, PART_POS_HAIR_HELMET, pInfo->dwItemHelmet, pInfo->iItemHelmetDurability);
	}

	else if(!pLooks->szPartFNs[PART_POS_HAIR_HELMET].empty())
	{
		char szBuff[256] = "";
		char szDir[128] = "";
		char szFName[128] = "";
		char szExt[16] = "";

		::_splitpath(pLooks->szPartFNs[PART_POS_HAIR_HELMET].c_str(), NULL, szDir, szFName, szExt);
		sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, pInfo->iHair, szExt);
		m_pChrs[iPosIndex]->PartSet(PART_POS_HAIR_HELMET, szBuff);
	}

	else 
	{
		m_pChrs[iPosIndex]->PartSet(PART_POS_HAIR_HELMET, "");
	}

	__Quaternion qt;

	float x, y, z, r;

	if (*s_pPlayer->m_InfoBase.eNation == NationEnum::KARUS)
	{
		y = -1.16f;

		if (eCP == SelectCharacterPositionEnum::LEFT)
		{
			x = 1.86f;
			z = 2.1f;
			r = 42.0f;
		}

		else if (eCP == SelectCharacterPositionEnum::CENTER)
		{
			x = 0.0f;
			z = 2.72f;
			r = 0.0f; 
		}

		else if (eCP == SelectCharacterPositionEnum::RIGHT)
		{
			x = -1.9f;
			z = 2.1f;
			r = -42.0f;
		}
	}

	else if (*s_pPlayer->m_InfoBase.eNation == NationEnum::ELMORAD)
	{
		y = -1.20f;

		if (eCP == SelectCharacterPositionEnum::LEFT)
		{
			x = 1.86f;
			z = 2.0f;
			r = 42.0f;
		}

		else if (eCP == SelectCharacterPositionEnum::CENTER)
		{
			x = 0.0f;
			z = 2.74f;
			r = 0.0f;
		}

		else if (eCP == SelectCharacterPositionEnum::RIGHT)
		{
			x = -1.9f;
			z = 2.0f;
			r = -46.0f;
		}
	}

	m_pChrs[iPosIndex]->PosSet(x, y, z);
	qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(r));
	m_pChrs[iPosIndex]->RotSet(qt);

	for(int i = 0; i < 3; i++ )
	{
		if ( m_pChrs[i] )
		{
			m_pChrs[i]->AniCurSet(SELECT_ANIM_PRE_SELECT, true, 0);
			m_pChrs[i]->Tick();
		}
	}
}

void CGameProcCharacterSelect::AddChrPart(int iPosIndex, const __TABLE_PLAYER_LOOKS* pLooks, e_PartPosition ePartPos, DWORD dwItemID, int iItemDurability)
{
	if(NULL == m_pChrs[iPosIndex] || NULL == pLooks)
	{
		__ASSERT(0, "Invalid Chr Part");
		return;
	}

	__TABLE_ITEM_BASIC* pItem = s_pTbl_Items_Basic->Find(dwItemID/1000*1000);
	if(dwItemID && NULL == pItem)
	{
		__ASSERT(0, "NULL Item!!!");
		return;
	}

	std::string szResrcFN;
	CN3CPart* pPart = NULL;
	e_PartPosition ePartPosTmp = PART_POS_UNKNOWN;
	e_PlugPosition ePlugPosTmp = PLUG_POS_UNKNOWN;

	CGameProcedure::MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, m_InfoChrs[iPosIndex].eRace);
	
	if (szResrcFN.empty())
	{
		pPart = m_pChrs[iPosIndex]->PartSet(ePartPos, pLooks->szPartFNs[ePartPos]);
	}

	else
	{
		pPart = m_pChrs[iPosIndex]->PartSet(ePartPos, szResrcFN);
	}

	if(pPart && pItem && pItem->siMaxDurability > 0)
	{
		int iPercentage = iItemDurability * 100 / pItem->siMaxDurability;

		if (iPercentage <= 30)
		{
			pPart->TexOverlapSet("Misc\\Dust_Hard.dxt");
		}

		else if (iPercentage <= 70)
		{
			pPart->TexOverlapSet("Misc\\Dust_Soft.dxt");
		}

		else
		{
			pPart->TexOverlapSet("");
		}
	}
}

void CGameProcCharacterSelect::getRaceGraphics(e_Race race, e_Class speciality, std::string & szJointFN, std::string & szAniFN, std::string & szPlug0FN, std::string & szPlug1FN)
{
	switch (race)
	{
	case RACE_EL_BABARIAN:
		szJointFN = "ChrSelect\\upc_el_ba_wa.n3joint";
		szAniFN = "ChrSelect\\upc_el_ba_wa.n3anim";
		szPlug0FN = "ChrSelect\\wea_el_great_sword.n3cplug";
		szPlug1FN = "";
		break;
	case RACE_EL_WOMEN:
		switch (speciality)
		{
		case CLASS_EL_WARRIOR:
		case CLASS_EL_BLADE:
		case CLASS_EL_PROTECTOR:
			szJointFN = "ChrSelect\\upc_el_rf_wa.n3joint";
			szAniFN = "ChrSelect\\upc_el_rf_wa.n3anim";
			szPlug0FN = "ChrSelect\\wea_el_long_sword_left.n3cplug";
			szPlug1FN = "";
			break;
		case CLASS_EL_ROGUE:
		case CLASS_EL_RANGER:
		case CLASS_EL_ASSASIN:
			szJointFN = "ChrSelect\\upc_el_rf_rog.n3joint";
			szAniFN = "ChrSelect\\upc_el_rf_wa.n3anim";
			szPlug0FN = "ChrSelect\\wea_el_rf_rog_bow.n3cplug";
			szPlug1FN = "ChrSelect\\wea_el_quiver.n3cplug";
			break;
		case CLASS_EL_WIZARD:
		case CLASS_EL_MAGE:
		case CLASS_EL_ENCHANTER:
			szJointFN = "ChrSelect\\upc_el_rf_wiz.n3joint";
			szAniFN = "ChrSelect\\upc_el_rf_wa.n3anim";
			szPlug0FN = "ChrSelect\\upc_el_rf_wiz.n3cplug";
			szPlug1FN = "";
			break;
		case CLASS_EL_PRIEST:
		case CLASS_EL_CLERIC:
		case CLASS_EL_DRUID:
			szJointFN = "ChrSelect\\upc_el_rf_pri.n3joint";
			szAniFN = "ChrSelect\\upc_el_rf_wa.n3anim";
			szPlug0FN = "ChrSelect\\wea_el_wand.n3cplug";
			szPlug1FN = "";
			break;
		}
		break;
	case RACE_EL_MAN:
		switch (speciality)
		{
		case CLASS_EL_WARRIOR:
		case CLASS_EL_BLADE:
		case CLASS_EL_PROTECTOR:
			szJointFN = "ChrSelect\\upc_el_rm_wa.n3joint";
			szAniFN = "ChrSelect\\upc_el_rm_wa.n3anim";
			szPlug0FN = "ChrSelect\\wea_el_long_sword.n3cplug";
			break;
		case CLASS_EL_ROGUE:
		case CLASS_EL_RANGER:
		case CLASS_EL_ASSASIN:
			szJointFN = "ChrSelect\\upc_el_rm_rog.n3joint";
			szAniFN = "ChrSelect\\upc_el_rm_rog.n3anim";
			szPlug0FN = "ChrSelect\\upc_el_rm_rog_bow.n3cplug";
			szPlug1FN = "ChrSelect\\wea_el_quiver.n3cplug";
			break;
		case CLASS_EL_WIZARD:
		case CLASS_EL_MAGE:
		case CLASS_EL_ENCHANTER:
			szJointFN = "ChrSelect\\upc_el_rm_ma.n3joint";
			szAniFN = "ChrSelect\\upc_el_rm_rog.n3anim";
			szPlug0FN = "ChrSelect\\upc_el_rm_wiz.n3cplug";
			szPlug1FN = "";
			break;
		case CLASS_EL_PRIEST:
		case CLASS_EL_CLERIC:
		case CLASS_EL_DRUID:
			szJointFN = "ChrSelect\\upc_el_rm_pri.n3joint";
			szAniFN = "ChrSelect\\upc_el_rm_rog.n3anim";
			szPlug0FN = "ChrSelect\\wea_el_wand.n3cplug";
			szPlug1FN = "";
			break;
		}
		break;
	case RACE_KA_ARKTUAREK:
		szJointFN = "ChrSelect\\upc_ka_at_wa.n3joint";
		szAniFN = "ChrSelect\\upc_ka_at_wa.n3anim";
		szPlug0FN = "ChrSelect\\wea_ka_great_axe.n3cplug";
		szPlug1FN = "";
		break;
	case RACE_KA_TUAREK:
		switch (speciality)
		{
		case CLASS_KA_ROGUE:
		case CLASS_KA_HUNTER:
		case CLASS_KA_PENETRATOR:
			szJointFN = "ChrSelect\\upc_ka_tu_rog.n3joint";
			szAniFN = "ChrSelect\\upc_ka_at_wa.n3anim";
			szPlug0FN = "ChrSelect\\wea_ka_bow.n3cplug";
			szPlug1FN = "ChrSelect\\wea_ka_quiver.n3cplug";
			break;
		case CLASS_KA_PRIEST:
		case CLASS_KA_SHAMAN:
		case CLASS_KA_DARKPRIEST:
			szJointFN = "ChrSelect\\upc_ka_tu_pri.n3joint";
			szAniFN = "ChrSelect\\upc_ka_at_wa.n3anim";
			szPlug0FN = "ChrSelect\\wea_ka_mace.n3cplug";
			szPlug1FN = "";
			break;
		}
		break;
	case RACE_KA_WRINKLETUAREK:
		szJointFN = "ChrSelect\\upc_ka_wt_ma.n3joint";
		szAniFN = "ChrSelect\\upc_ka_at_wa.n3anim";
		szPlug0FN = "ChrSelect\\wea_ka_staff.n3cplug";
		szPlug1FN = "";
		break;
	case RACE_KA_PURITUAREK:
		szJointFN = "ChrSelect\\upc_el_rf_pri.n3joint";
		szAniFN = "ChrSelect\\upc_el_rf_wa.n3anim";
		szPlug0FN = "ChrSelect\\wea_ka_mace.n3cplug";
		szPlug1FN = "";
		break;
	default:
		return;
	}
}

void CGameProcCharacterSelect::ProcessOnReturn()
{
	if (!m_bReceivedCharacterSelect)
	{
		return;
	}

	if (m_eCurProcess != PROCESS_ROTATEING)
	{
		int iIndex = m_eCurPos->getValue();

		// Light..
		if (*s_pPlayer->m_InfoBase.eNation == NationEnum::KARUS)
		{
			m_lgt[iIndex].Theta = KARUS_THETA_MAX;
		}

		else if (*s_pPlayer->m_InfoBase.eNation == NationEnum::ELMORAD)
		{
			m_lgt[iIndex].Theta = ELMORAD_THERA_MAX;
		}

		s_pEng->s_lpD3DDev->LightEnable(iIndex + 4, TRUE);
		s_pEng->s_lpD3DDev->SetLight(iIndex + 4, &m_lgt[iIndex]);

		s_SndMgr.ReleaseStreamObj(&(CGameProcedure::s_pSnd_BGM));
		CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcMain);

		this->s_pUILoading->Render(StringConstants::LOADING_DATA, 0);
	}
}

void CGameProcCharacterSelect::DoSelectedChrProc()
{
	int iIndex = m_eCurPos->getValue();

	// Light..
	s_pEng->s_lpD3DDev->LightEnable(iIndex + 4, TRUE);
	s_pEng->s_lpD3DDev->SetLight(iIndex + 4, &m_lgt[iIndex]);

	if (!m_pChrs[iIndex]->IsAnimEnd())
	{
		m_pChrs[iIndex]->AniCurSet(SELECT_ANIM_SELECTED, true, 0.6f);
		m_pChrs[iIndex]->Tick();
		m_bFadeOutRender = true;
		FadeOutProcess();
	}

	else
	{
		s_SndMgr.ReleaseStreamObj(&(CGameProcedure::s_pSnd_BGM));
		CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcMain);
	}
}

void CGameProcCharacterSelect::CharacterSelect()
{
	int iIndex = m_eCurPos->getValue();

	if (m_eCurProcess == PROCESS_PRESELECT)
	{
		if (m_pChrs[iIndex])
		{
			m_eCurProcess = PROCESS_SELECTED;
			m_pChrs[iIndex]->AniCurSet(SELECT_ANIM_SELECTED, true, 0.6f);
			m_pChrs[iIndex]->Tick();

			if (*s_pPlayer->m_InfoBase.eNation == NationEnum::KARUS)
			{
				m_fCurTheta = KARUS_THETA_MAX;
			}

			else if (*s_pPlayer->m_InfoBase.eNation == NationEnum::ELMORAD)
			{
				m_fCurTheta = ELMORAD_THERA_MAX;
			}

			m_lgt[iIndex].Theta = m_fCurTheta;
			s_pEng->s_lpD3DDev->LightEnable(iIndex + 4, TRUE);
			s_pEng->s_lpD3DDev->SetLight(iIndex + 4, &m_lgt[iIndex]);
		}
	}

	m_bReceivedCharacterSelect = true;
	s_pUIMgr->EnableOperationSet(false);
}

void CGameProcCharacterSelect::CharacterSelectFailed()
{
	m_bReceivedCharacterSelect = false;
	std::string szErr = "IDS_ERR_CHARACTER_SELECT"; //::_LoadStringFromResource(IDS_ERR_CHARACTER_SELECT, szErr);
	CGameProcedure::MessageBoxPost(szErr, "", MB_OK, BEHAVIOR_EXIT);
	s_pUIMgr->EnableOperationSet(true);
}


bool CGameProcCharacterSelect::MsgRecv_CharacterSelect(DataPack* pDataPack, int& iOffset) // virtual
{
	bool bSuccess = CGameProcedure::MsgRecv_CharacterSelect(pDataPack, iOffset);

	if(bSuccess) this->CharacterSelect(); // 캐릭터를 일으킨다..
	else this->CharacterSelectFailed();

	return bSuccess;
}


#pragma region packets
#pragma region out
void CGameProcCharacterSelect::MsgSend_RequestAllCharacterInfo()
{
	BYTE byBuff[4];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_ALL_CHARACTER_INFO_REQUEST);
	s_pSocket->Send(byBuff, iOffset);
}

bool CGameProcCharacterSelect::MsgSend_DeleteChr(const std::string& szKey)
{
	//make sure the name is valid
	if (szKey.empty() || szKey.size() >= 45)
	{
		return false;
	}

	//make sure we are in the correct state
	if (m_eCurProcess != PROCESS_PRESELECT)
	{
		return false;
	}

	int iIndex = m_eCurPos->getValue();

	//validate that the user entered the same character name
	if (m_InfoChrs[iIndex].szID != szKey)
	{
		this->MessageBoxPost(StringConstants::CHARACTER_DELETE_NAME_MISSMATCH, StringConstants::CHARACTER_DELETE_ERROR, MB_OK);
		return false;
	}

	BYTE byBuff[64];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_DELETE_CHARACTER);
	CAPISocket::MP_AddShort(byBuff, iOffset, szKey.size());
	CAPISocket::MP_AddString(byBuff, iOffset, szKey);

	s_pSocket->Send(byBuff, iOffset);

	return true;
}

#pragma endregion out

#pragma region in
bool CGameProcCharacterSelect::ProcessPacket(DataPack* pDataPack, int& iOffset)
{
	int iOffsetPrev = iOffset;

	if (false == CGameProcedure::ProcessPacket(pDataPack, iOffset))
	{
		iOffset = iOffsetPrev;
	}

	else
	{
		return true;
	}

	int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch (iCmd)
	{
	case N3_ALL_CHARACTER_INFO_REQUEST:
		this->MsgRecv_AllCharacterInfo(pDataPack, iOffset);
		s_pUIMgr->EnableOperationSet(true);
		return true;

	case N3_DELETE_CHARACTER:
		this->MsgRecv_DeleteChr(pDataPack, iOffset);
		return true;
	}

	return false;
}

bool CGameProcCharacterSelect::MsgRecv_VersionCheck(DataPack* pDataPack, int& iOffset) // virtual
{
	bool version = CGameProcedure::MsgRecv_VersionCheck(pDataPack, iOffset);

	if (version)
	{
		this->MsgSend_CharacterSelect();
	}

	return version;
}

void CGameProcCharacterSelect::MsgRecv_AllCharacterInfo(DataPack* pDataPack, int& iOffset)
{
	int iCharacters = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	for (int i = 0; i < iCharacters; i++)
	{
		int temp = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		SelectCharacterPositionEnum * selectCharacterPosition = &SelectCharacterPositionEnum::forValue(temp);

		int iNameLength = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, m_InfoChrs[selectCharacterPosition->getValue()].szID, iNameLength);

		m_InfoChrs[selectCharacterPosition->getValue()].eRace = (e_Race)(CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset));
		m_InfoChrs[selectCharacterPosition->getValue()].eClass = (e_Class)(CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset));
		m_InfoChrs[selectCharacterPosition->getValue()].iLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		m_InfoChrs[selectCharacterPosition->getValue()].iFace = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		m_InfoChrs[selectCharacterPosition->getValue()].iHair = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		m_InfoChrs[selectCharacterPosition->getValue()].iZone = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

		//head
		m_InfoChrs[selectCharacterPosition->getValue()].dwItemHelmet = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		m_InfoChrs[selectCharacterPosition->getValue()].iItemHelmetDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		//chest
		m_InfoChrs[selectCharacterPosition->getValue()].dwItemUpper = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		m_InfoChrs[selectCharacterPosition->getValue()].iItemUpperDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		//shoulder
		m_InfoChrs[selectCharacterPosition->getValue()].dwItemCloak = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		m_InfoChrs[selectCharacterPosition->getValue()].iItemCloakDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		//leg
		m_InfoChrs[selectCharacterPosition->getValue()].dwItemLower = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		m_InfoChrs[selectCharacterPosition->getValue()].iItemLowerDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		//foot
		m_InfoChrs[selectCharacterPosition->getValue()].dwItemShoes = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		m_InfoChrs[selectCharacterPosition->getValue()].iItemShoesDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		//glove
		m_InfoChrs[selectCharacterPosition->getValue()].dwItemGloves = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		m_InfoChrs[selectCharacterPosition->getValue()].iItemGlovesDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		// NOTE(srmeier): this was added for 1298

		//right hand
		DWORD dwRightHand = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		int iItemRightHandDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		//left hand
		DWORD dwLeftHand = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		int iItemLeftHandDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		AddChr(*selectCharacterPosition, &(m_InfoChrs[selectCharacterPosition->getValue()]));
	}
}

void CGameProcCharacterSelect::MsgRecv_DeleteChr(DataPack* pDataPack, int& iOffset)
{
	BYTE byResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch (byResult)
	{
	case CHARACTER_DOESNT_EXIST:
	case CHARACTER_DELETE_FAILED:
		this->MessageBoxPost(StringConstants::CHARACTER_DELETE_ERROR, StringConstants::CHARACTER_DELETE, MB_OK);
		break;

	case CHARACTER_DELETE_SUCCESS:
		if (m_pChrs[m_eCurPos->getValue()] != NULL)
		{
			m_pChrs[m_eCurPos->getValue()]->Release();
			delete m_pChrs[m_eCurPos->getValue()];
			m_pChrs[m_eCurPos->getValue()] = NULL;

			m_InfoChrs[m_eCurPos->getValue()].clear();
		}

		if ((m_pChrs[0] == NULL) && (m_pChrs[1] == NULL) && (m_pChrs[2] == NULL))
		{
			CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcNationSelect);
		}

		break;
	}
}
#pragma endregion in
#pragma endregion packets





void CGameProcCharacterSelect::MsgSend_CharacterSelect() // virtual
{
	CGameProcedure::MsgSend_CharacterSelect();
	s_pUIMgr->EnableOperationSet(false);
}