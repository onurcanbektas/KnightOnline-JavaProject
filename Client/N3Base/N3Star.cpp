// N3Star.cpp: implementation of the CN3Star class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxBase.h"
#include "N3Star.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Star::CN3Star()
{
	m_iCurNumStar = 0;
	m_iNextNumStar = 0;
	m_fAddTime = 0;
}

CN3Star::~CN3Star()
{
}

void CN3Star::Release()
{
	CN3Base::Release();
	m_iCurNumStar = 0;
	m_iNextNumStar = 0;
	m_fAddTime = 0;
}

void CN3Star::Tick()
{
	// ���� ���� �þ�� �پ��°� �����ϱ�
	static float fSec = 0.0f;
	if (0.0f == m_fAddTime) return;

	fSec += s_fSecPerFrm;
	if (fSec > m_fAddTime)
	{
		int iCount = (int)(fSec/m_fAddTime);
		fSec -= m_fAddTime*iCount;
		if (m_bIncrease)
		{
			m_iCurNumStar += iCount;
			if (m_iCurNumStar>=m_iNextNumStar)
			{
				m_iCurNumStar = m_iNextNumStar;
				m_fAddTime = 0.0f;
				fSec = 0.0f;
			}
		}
		else
		{
			m_iCurNumStar -= iCount;
			if (m_iCurNumStar<=m_iNextNumStar)
			{
				m_iCurNumStar = m_iNextNumStar;
				m_fAddTime = 0.0f;
				fSec = 0.0f;
			}
		}
	}
}

void CN3Star::Render()
{
	if (0 == m_iCurNumStar) return;
	__Matrix44 matWorld; matWorld.Identity();
	s_lpD3DDev->SetTransform( D3DTS_WORLD, &matWorld );

	s_lpD3DDev->SetTexture(0, NULL);
	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );

	s_lpD3DDev->SetFVF(FVF_XYZCOLOR);
	s_lpD3DDev->DrawPrimitiveUP(D3DPT_POINTLIST, m_iCurNumStar, m_Stars, sizeof(m_Stars[0]));
}

void CN3Star::Init()
{
	Release();

	int i;
	BYTE alpha = 0xff;
	BYTE alpha_min = 0x80;
	BYTE alpha_max = 0xff;
	float fInc = ((float)(alpha_max - alpha_min))/MAX_STAR;
	for (i=0; i<MAX_STAR; ++i)
	{
		float fX = ((float)(rand()%10000))/1000.f - 5.0f;
		float fY = ((float)(rand()%10000))/1000.f - 2.0f;
		float fZ = ((float)(rand()%10000))/1000.f - 5.0f;

		while ( (fX*fX + fY*fY + fZ*fZ) < 2.0f*2.0f )
		{
			fX = ((float)(rand()%10000))/1000.f - 5.0f;
			fY = ((float)(rand()%10000))/1000.f - 2.0f;
			fZ = ((float)(rand()%10000))/1000.f - 5.0f;
		}

		alpha = alpha_max - (BYTE)(fInc*i);
		m_Stars[i].Set(fX, fY, fZ, (alpha<<24)|0x00ffffff);
	}
	m_iCurNumStar = 0;
}

void CN3Star::SetStar(int iNum, float fSec)
{
	if (iNum>MAX_STAR) iNum = MAX_STAR;
	else if (iNum<0) iNum = 0;
	m_iNextNumStar = iNum;
	if (0.0f>=fSec)
	{
		m_iCurNumStar = iNum;
		m_fAddTime = 0;
		return;
	}

	int iDiff = m_iNextNumStar - m_iCurNumStar;
	if (0 == iDiff) return;
	else if (iDiff>0)
	{
		m_bIncrease = TRUE;
		m_fAddTime = fSec/iDiff;
	}
	else
	{
		m_bIncrease = FALSE;
		m_fAddTime = fSec/(-iDiff);
	}
}