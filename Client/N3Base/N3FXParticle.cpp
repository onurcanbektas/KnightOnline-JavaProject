// N3FXParticle.cpp: implementation of the CN3FXParticle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3FXParticle.h"
#include "N3FXPartParticles.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3FXParticle::CN3FXParticle()
{
	m_fLife = 0.0f;
	m_fCurrLife = 0.0f;
	m_iTexIdx = 0;

	m_pRefParent = NULL;

	m_vCreatePoint.Set(0.0f, 0.0f, 0.0f);
	m_vAxis.Set(0.0f, 0.0f, 1.0f);

	m_vLcPos.Set(0.0f, 0.0f, 0.0f);
	m_vWdPos - m_vLcPos;
	m_dwColor = 0xffffffff;

	m_vVelocity.Set(0.0f, 0.0f, 0.0f);
	m_vAccel.Set(0.0f, 0.0f, 0.0f);

	m_fRot = 0.0f;
	m_fSize = 0.0f;
	//m_fCurrSize = 0.0f;

	m_fDropY = m_fDropVel = 0.0f;

	m_pVB = NULL;
	/*
	m_pVB = new __VertexXyzColorT1[NUM_VERTEX_PARTICLE];
	m_pVB[0].Set(0,0,0,0xffffffff, 0.0f, -1.0f);
	m_pVB[1].Set(0,0,0,0xffffffff, 2.0f, 1.0f);
	m_pVB[2].Set(0,0,0,0xffffffff, 0.0f, 1.0f);	
	*/
}


CN3FXParticle::~CN3FXParticle()
{
	//delete[] m_pVB;
	m_pVB = NULL;
}

bool CN3FXParticle::Tick()
{
	if(!m_pRefParent) return false;
	if(m_pRefParent->m_bChangeColor && m_fCurrLife >= m_fLife) return false;
	if(!m_pRefParent->m_bChangeColor && m_fCurrLife >= (m_pRefParent->m_fFadeIn + m_fLife + m_pRefParent->m_fFadeOut)) return false;

	///////////////////////////////////////////////
	//����ó��..
	__Quaternion qt;
	qt.RotationAxis(m_vAxis, m_fRot);

	__Matrix44 mtxRot;
	mtxRot = qt;

	m_vWdPos = m_vCreatePoint + (m_vLcPos*mtxRot);
	m_vWdPos.y -= m_fDropY;

	__Matrix44 mtxVI = m_pRefParent->m_mtxVI;

	m_pVB = &(m_pRefParent->m_pVB[m_iID]);

	__Matrix44 mtxRotateTex;
	mtxRotateTex.Identity();
	mtxRotateTex.RotationZ(m_pRefParent->m_fTexRotateVelocity * m_fCurrLife);
	__Vector3 vScale;
	vScale.Set(m_fSize + m_pRefParent->m_fScaleVelX * m_fCurrLife,
				m_fSize + m_pRefParent->m_fScaleVelY * m_fCurrLife, m_fSize);
	if(vScale.x < 0.0f) vScale.x = 0.0f;
	if(vScale.y < 0.0f) vScale.y = 0.0f;
	if(vScale.z < 0.0f) vScale.z = 0.0f;

	for(int i=0;i<NUM_VERTEX_PARTICLE;i++)
	{
		m_pVB[i] = ((m_pRefParent->m_vUnit[i] * vScale) * mtxRotateTex * mtxVI) + m_vWdPos;

		m_pVB[i].tu = m_pRefParent->m_vUnit[i].tu;
		m_pVB[i].tv = m_pRefParent->m_vUnit[i].tv;
	}
	
	///////////////////////////////////////////////
	//�����غ�..
	m_vLcPos += m_vVelocity * CN3Base::s_fSecPerFrm;

	if(m_pRefParent->m_bChangeColor)
	{
		int idx = (int)(m_fCurrLife * (float)NUM_KEY_COLOR / m_fLife);
		if(idx>=NUM_KEY_COLOR) idx = NUM_KEY_COLOR - 1;
		m_dwColor = m_pRefParent->m_dwChangeColor[idx];
	}
	else
	{
		if(m_fCurrLife <= m_pRefParent->m_fFadeIn)
		{
			DWORD Alpha = (DWORD)(255.0f * m_fCurrLife / m_pRefParent->m_fFadeIn);
			Alpha = (Alpha<<24);
			m_dwColor = Alpha + 0x00ffffff;
		}
		else if(m_fCurrLife < (m_pRefParent->m_fFadeIn + m_fLife)) m_dwColor = 0xffffffff;
		else if(m_fCurrLife < (m_pRefParent->m_fFadeIn + m_fLife + m_pRefParent->m_fFadeOut))
		{
			DWORD Alpha = (DWORD)(255.0f * ((m_pRefParent->m_fFadeIn + m_fLife + m_pRefParent->m_fFadeOut)-m_fCurrLife) / m_pRefParent->m_fFadeOut);
			Alpha = (Alpha<<24);
			m_dwColor = Alpha + 0x00ffffff;
			//m_fSize = m_pRefParent->m_fParticleSize * AlphaRatio;		
		}
		else m_dwColor = 0x00ffffff;
	}

	for(int i=0;i<NUM_VERTEX_PARTICLE;i++) m_pVB[i].color = m_dwColor;

	m_vVelocity += m_vAccel * CN3Base::s_fSecPerFrm;
	m_fRot += CN3Base::s_fSecPerFrm*m_pRefParent->m_fPtRotVelocity;

	m_fDropVel += m_pRefParent->m_fPtGravity * CN3Base::s_fSecPerFrm;
	m_fDropY += m_fDropVel * CN3Base::s_fSecPerFrm;

	m_fCurrLife += CN3Base::s_fSecPerFrm;

	m_iTexIdx = (int)(m_fCurrLife * m_pRefParent->m_fTexFPS) % m_iNumTex;

	return true;
}