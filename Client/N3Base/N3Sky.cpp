// N3Sky.cpp: implementation of the CN3Sky class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxBase.h"
#include "N3Sky.h"
#include "N3PMesh.h"
#include "N3PMeshInstance.h"
#include "N3Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Sky::CN3Sky()
{
//	m_pTex = NULL;
	m_SkyColor.ChangeColor(0xff081021);	//��
	m_FogColor.ChangeColor(0xff102942);	//��
}

CN3Sky::~CN3Sky()
{
//	if (m_pTex) {s_MngTex.Delete(m_pTex);	m_pTex = NULL;}
}

void CN3Sky::Release()
{
	CN3Base::Release();
//	m_SkyColor.ChangeColor(0xFF5284DE);	//��
//	m_FogColor.ChangeColor(0xFFB5C6DE);	//��
	m_SkyColor.ChangeColor(0xff081021);	//��
	m_FogColor.ChangeColor(0xff102942);	//��
//	if (m_pTex) {s_MngTex.Delete(m_pTex);	m_pTex = NULL;}
}

void CN3Sky::Tick()
{
	m_SkyColor.Tick();
	m_FogColor.Tick();
	int i;
	/* 
	* Temeporary Fix until future implementation 
	* m_SkyColor stores as FogColor because of broken sky issue. 
	* planning to implement the real fog to work properly as the newer versions.
	*/
	D3DCOLOR FogColor = m_FogColor.GetCurColor();//m_SkyColor.GetCurColor();
	for (i=0; i<4; ++i)
	{
		m_vFronts[i].color = (m_vFronts[i].color&0xff000000) | (FogColor&0x00ffffff);
		m_Bottom[i].color = FogColor;
	}
}

void CN3Sky::Render()
{
    // Set up a rotation matrix to orient the billboard towards the camera.
	__Matrix44 matWorld;
	__Vector3 vDir = s_CameraData.vEye - s_CameraData.vAt;	// ī�޶��� ����
	if (0.0f == vDir.x) matWorld.Identity();
	else if( vDir.x > 0.0f ) matWorld.RotationY(-atanf(vDir.z/vDir.x) - (D3DX_PI * 0.5f));
	else  matWorld.RotationY(-atanf(vDir.z/vDir.x) + (D3DX_PI * 0.5f));
	s_lpD3DDev->SetTransform( D3DTS_WORLD, &matWorld );

	// Set the texture stage states
	s_lpD3DDev->SetTexture(0, NULL);
	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );

	// Render the skybox
	s_lpD3DDev->SetFVF(FVF_XYZCOLOR);
	// ���� �׸���
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_Bottom, sizeof(m_Bottom[0]));
	// ���� �׸���
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_vFronts, sizeof(m_vFronts[0]));
}

void CN3Sky::Init()
{
	Release();
	
	// ���� �� ��츦 ���� ���....
	const float fWidth		= 3.5f; // ���� ���� ��
	const float fTopY		= 0.5f; // ���� ���� ����..  �������� ���������ϰ� �� �κ��� �� ���� - �� �����ʹ� �ϴ��̴�..
	const float fBottomY	= 0.1f; // ���� ���� �ٴ� ����..  �������� ���������ϰ� �� �κ��� �Ʒ� - �� �Ʒ��� ������ �Ȱ����̴�..
	const float fDistance	= 1.5f; // ���� ���� �ٴ� ����..
	const D3DCOLOR color = m_FogColor.GetCurColor();
	m_vFronts[0].Set( fWidth,	fTopY,		fDistance,		0x00ffffff&color);
	m_vFronts[1].Set( fWidth,	fBottomY,	fDistance,		color);
	m_vFronts[2].Set(-fWidth,	fBottomY,	fDistance,		color);
	m_vFronts[3].Set(-fWidth,	fTopY,		fDistance,		0x00ffffff&color);

	const float fBottomOffset = -5.0f;	// �ظ� ���� �ٴ� ����
	m_Bottom[0].Set( fWidth,	fBottomY,	fDistance,		color);
	m_Bottom[1].Set( fWidth,	fBottomOffset,	fDistance,	color);
	m_Bottom[2].Set(-fWidth,	fBottomOffset,	fDistance,	color);
	m_Bottom[3].Set(-fWidth,	fBottomY,	fDistance,		color);
}
