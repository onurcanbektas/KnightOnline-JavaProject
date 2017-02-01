// N3GERain.cpp: implementation of the CN3GERain class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3GERain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3GERain::CN3GERain()
{
	m_dwEffectType = GETYPE_RAIN;
	CN3GERain::Release();
}

CN3GERain::~CN3GERain()
{
	CN3GERain::Release();
}

void CN3GERain::Release()
{
	m_fWidth = m_fHeight = m_fRainLength = 0.0f;
	m_vVelocity.Set(0,0,0);
	CN3GlobalEffect::Release();
}

void CN3GERain::Tick()
{
	if (m_bActive == FALSE || m_iVC <= 0 || m_pVB == NULL) return;

	CN3GlobalEffect::Tick();

	int iCount = m_iVC/2;
	int iActiveCount = iCount;
	if(m_iFadeMode > 0) // ���� �������� �Ѵ�..
	{
		if(m_fFadeTime > 0 && m_fFadeTimeCur < m_fFadeTime) // Fade�ð����� �ǳʶٰ� ���� �� ����..
		{
			iActiveCount = iCount * m_fFadeTimeCur / m_fFadeTime;
			if(iActiveCount > iCount) iActiveCount = iCount;
			else if(iActiveCount < 0) iActiveCount = 0;
		}
	}
	else if(m_iFadeMode < 0)
	{
		if(m_fFadeTime > 0 && m_fFadeTimeCur < m_fFadeTime) // Fade�ð����� �ǳʶٰ� ���� �� ����..
		{
			iActiveCount = iCount * (1.0f - m_fFadeTimeCur / m_fFadeTime);
			if(iActiveCount > iCount) iActiveCount = iCount;
			else if(iActiveCount < 0) iActiveCount = 0;
		}
	}
	if(iActiveCount <= 0) return;

	int i;

	__VertexXyzColor* pVertices;
	HRESULT hr = m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_NOSYSLOCK);

	__Vector3 vN = m_vVelocity;	vN.Normalize();
	__Vector3 vAdd = m_vVelocity*s_fSecPerFrm;
	__Vector3 vAddLength = vN*m_fRainLength;

	const float fHalfWidth = m_fWidth/2.0f;
	const float fHalfHeight = m_fHeight/2.0f;
	const float fCurY = s_CameraData.vEye.y;

	for (i=0; i<iActiveCount; ++i)
	{
		// tail ��ġ �����ϱ�
		__VertexXyzColor* pVTail = pVertices+i*2+0;
		__VertexXyzColor* pVHead = pVertices+i*2+1;
		pVTail->x += vAdd.x;	pVTail->y += vAdd.y;	pVTail->z += vAdd.z;

		float fDiff = pVTail->y - (fCurY-fHalfHeight);
		if (fDiff < 0)	// ���� ������ ����� ���
		{
			pVTail->y -= (((int)(fDiff/m_fHeight)-1)*m_fHeight);
			pVTail->x = m_fWidth*(rand()%10000-5000)/10000.f;
			pVTail->z = m_fWidth*(rand()%10000-5000)/10000.f;
		}
		else
		{
			fDiff = pVTail->y - (fCurY+fHalfHeight);
			if (fDiff > 0)	// ���� ������ �ݴ�� ��������
				pVTail->y -= ((int)(fDiff/m_fHeight)+1)*m_fHeight;
			// x �ʺ� ������ ����� ���
			fDiff = pVTail->x - fHalfWidth;
			if (fDiff > 0) pVTail->x -= ((int)(fDiff/m_fWidth)+1)*m_fWidth;
			fDiff = pVTail->x + fHalfWidth;
			if (fDiff < 0) pVTail->x -= ((int)(fDiff/m_fWidth)-1)*m_fWidth;
			// z �ʺ� ������ ����� ���
			fDiff = pVTail->z - fHalfWidth;
			if (fDiff >  0) pVTail->z -= ((int)(fDiff/m_fWidth)+1)*m_fWidth;
			fDiff = pVTail->z + fHalfWidth;
			if (fDiff < 0) pVTail->z -= ((int)(fDiff/m_fWidth)-1)*m_fWidth;
		}
		// Head�� ��ġ�� Tail�� ��ġ�κ��� ���� �Ÿ��� ������ ���� ��ġ��Ų��.
		pVHead->x = pVTail->x + vAddLength.x;
		pVHead->y = pVTail->y + vAddLength.y;
		pVHead->z = pVTail->z + vAddLength.z;
	}
	m_pVB->Unlock();
}

void CN3GERain::Render(__Vector3& vPos)
{
	if (m_bActive == FALSE || m_iVC <= 0 || m_pVB == NULL) return;
	
	CN3GlobalEffect::Render(vPos);
	
	int iCount = m_iVC / 2;
	
	int iActiveCount = iCount;
	if(m_iFadeMode > 0) // ���� �������� �Ѵ�..
	{
		if(m_fFadeTime > 0 && m_fFadeTimeCur < m_fFadeTime) // Fade�ð����� �ǳʶٰ� ���� �� ����..
		{
			iActiveCount = iCount * m_fFadeTimeCur / m_fFadeTime;
			if(iActiveCount > iCount) iActiveCount = iCount;
			else if(iActiveCount < 0) iActiveCount = 0;
		}
	}
	else if(m_iFadeMode < 0)
	{
		if(m_fFadeTime > 0 && m_fFadeTimeCur < m_fFadeTime) // Fade�ð����� �ǳʶٰ� ���� �� ����..
		{
			iActiveCount = iCount * (1.0f - m_fFadeTimeCur / m_fFadeTime);
			if(iActiveCount > iCount) iActiveCount = iCount;
			else if(iActiveCount < 0) iActiveCount = 0;
		}
	}
	if(iActiveCount <= 0) return;

	// ���� render state ����
	DWORD dwColorVertex, dwLighting, dwAlphaBlend, dwSrcAlpha, dwDestAlpha;
	s_lpD3DDev->GetRenderState( D3DRS_COLORVERTEX , &dwColorVertex );
	s_lpD3DDev->GetRenderState( D3DRS_LIGHTING, &dwLighting );
	s_lpD3DDev->GetRenderState( D3DRS_ALPHABLENDENABLE, &dwAlphaBlend );
	s_lpD3DDev->GetRenderState( D3DRS_SRCBLEND, &dwSrcAlpha );
	s_lpD3DDev->GetRenderState( D3DRS_DESTBLEND, &dwDestAlpha );

	// set render state
	s_lpD3DDev->SetRenderState( D3DRS_COLORVERTEX , TRUE );
	s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, FALSE );
	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA);
	s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA);

	// set transform
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &m_Matrix);

	// set texture 
	s_lpD3DDev->SetTexture(0, NULL);

	// render
	s_lpD3DDev->SetFVF(FVF_XYZCOLOR);
	s_lpD3DDev->SetStreamSource(0, m_pVB, 0, sizeof(__VertexXyzColor)); // ���ؽ� ���� ����
	s_lpD3DDev->DrawPrimitive(D3DPT_LINELIST, 0, iActiveCount);

	// restore
	s_lpD3DDev->SetRenderState( D3DRS_COLORVERTEX , dwColorVertex );
	s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, dwLighting );
	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlend);
	s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND,   dwSrcAlpha);
	s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND,  dwDestAlpha);
}

void CN3GERain::Create(float fDensity, 
					   float fWidth,
					   float fHeight, 
					   float fRainLength, 
					   const __Vector3& vVelocity,
					   float fTimeToFade)
// fDensity : 1 (����������) �� ������� ����
// fWidth : ����� ���� X,Z ����
// fHeight : ����� ������ ����
// fRainLength : ���ٱ��� ����
// vVelocity : ���ٱ��� �ӵ�
{
	if(NULL == s_lpD3DDev) return;

	Release();

	CN3GlobalEffect::FadeSet(fTimeToFade, true);

	m_fFadeTime = fTimeToFade;
	m_fFadeTimeCur = 0;

	float fVolume = fWidth*fWidth*fHeight;
	__ASSERT(fVolume > 0, "Rain volume is less than 0");
	if(fVolume <= 0) return;

	m_fWidth = fWidth;	m_fHeight = fHeight;	m_fRainLength = fRainLength;
	m_vVelocity = vVelocity;
	int iRainCount = (int)(fVolume*fDensity);

	// m_pVB, m_pIB �����
	m_iVC = iRainCount*2;
	HRESULT hr = s_lpD3DDev->CreateVertexBuffer(m_iVC*sizeof(__VertexXyzColor), 0, FVF_XYZCOLOR, D3DPOOL_MANAGED, &m_pVB, NULL);

	if (FAILED(hr)) return;
	__VertexXyzColor* pVertices;
	hr = m_pVB->Lock(0, iRainCount*2*sizeof(__VertexXyzColor), (void**)&pVertices, D3DLOCK_NOSYSLOCK);
	if (FAILED(hr)) return;

	const DWORD dwColorA = 0x00bfbfbf,	// ����
				dwColorB = 0x80bfbfbf;	// �Ӹ�
	int i;
	__Vector3 vN = vVelocity; vN.Normalize();
	__Vector3 vAdd = vN*fRainLength;
	for (i=0; i<iRainCount; ++i)
	{
		pVertices[i*2+0].Set(	fWidth*(rand()%10000-5000)/10000.f,
								fHeight*(rand()%10000-5000)/10000.f,
								fWidth*(rand()%10000-5000)/10000.f,
								dwColorA);
		pVertices[i*2+1].Set(	pVertices[i*2+0].x + vAdd.x, pVertices[i*2+0].y + vAdd.y, pVertices[i*2+0].z + vAdd.z,
								dwColorB);
	}
	m_pVB->Unlock();
}

