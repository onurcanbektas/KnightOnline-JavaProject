// N3Cloud.cpp: implementation of the CN3Cloud class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxBase.h"
#include "N3Cloud.h"
#include "N3Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Cloud::CN3Cloud()
{
	for(int i = 0; i < NUM_CLOUD; i++) 
	{
		m_pTextures[i] = NULL;
		m_szTextures[i] = "";
	}

	m_Color1.ChangeColor(0xffffffff);
	m_Color2.ChangeColor(0xffffffff);
	m_Alpha.ChangeColor(0xffffffff);
	m_eCloud1 = CLOUD_WISPS;
	m_eCloud2 = CLOUD_PUFFS;
	m_eCloud3 = CLOUD_NONE;

	m_fCloudTexTime = 0.0f;
	m_eBackupCloud = CLOUD_NONE;
	m_fBackupTime = 0.0f;
}

CN3Cloud::~CN3Cloud()
{
	int i;
	for (i=0; i<NUM_CLOUD; ++i)
	{
		s_MngTex.Delete(&m_pTextures[i]);
	}
}

void CN3Cloud::Release()
{
	CN3Base::Release();
	int i;
	for (i=0; i<NUM_CLOUD; ++i)
	{
		if (m_pTextures[i])
		{
			s_MngTex.Delete(&m_pTextures[i]);
			m_szTextures[i] = "";
		}
	}
	m_Color1.ChangeColor(0xffffffff);
	m_Color2.ChangeColor(0xffffffff);
	m_Alpha.ChangeColor(0xffffffff);
	m_eCloud1 = CLOUD_WISPS;
	m_eCloud2 = CLOUD_PUFFS;
	m_eCloud3 = CLOUD_NONE;

	m_fCloudTexTime = 0.0f;
	m_eBackupCloud = CLOUD_NONE;
	m_fBackupTime = 0.0f;
}
void CN3Cloud::Tick()
{
	int i;
	static float fCloudLayer = 0.0f;
	fCloudLayer += s_fSecPerFrm;

	// ������ �����̱�
	if (fCloudLayer>=0.05)
	{
//		float du = 0.001*fCloudLayer;
//		float dv = 0.003*fCloudLayer;
//		float du2 = 0.003*fCloudLayer;
//		float dv2 = 0.005*fCloudLayer;
		float du = 0.005*fCloudLayer;
		float dv = 0.015*fCloudLayer;
		float du2 = 0.015*fCloudLayer;
		float dv2 = 0.025*fCloudLayer;

		for (i=0; i<NUM_CLOUD_VERTEX; ++i)
		{
			m_pVertices[i].tu += du;
			m_pVertices[i].tv += dv;
			m_pVertices[i].tu2 += du2;
			m_pVertices[i].tv2 += dv2;
		}

		fCloudLayer = 0.0f;

		if (m_pVertices[0].tu > 10.0f)
		{
			for (i=0; i<NUM_CLOUD_VERTEX; ++i)	m_pVertices[i].tu -= 10.0f;
		}
		if (m_pVertices[0].tv > 10.0f)
		{
			for (i=0; i<NUM_CLOUD_VERTEX; ++i)	m_pVertices[i].tv -= 10.0f;
		}
	}

	// �� ��ȭ�ϱ�
	m_Color1.Tick();
	m_Color2.Tick();

	// ���� ��ü
	if (CLOUD_NONE != m_eCloud3)
	{
		m_Alpha.Tick();
		m_fCloudTexTime -= s_fSecPerFrm;
		if (m_fCloudTexTime < 0.0f)	// ���� ��ü�� ��������
		{
			m_fCloudTexTime = 0.0f;
			m_eCloud2 = m_eCloud3;
			m_eCloud3 = CLOUD_NONE;
			m_Alpha.ChangeColor(0xffffffff);

			if (CLOUD_NONE != m_eBackupCloud)	// ���� �ٲ�� �� ���� ������
			{
				e_CLOUDTEX eTmp = m_eCloud1;
				m_eCloud1 = m_eCloud2;
				m_eCloud2 = eTmp;

				m_eCloud3 = m_eBackupCloud;
				m_fCloudTexTime = m_fBackupTime;
				m_eBackupCloud = CLOUD_NONE;
				m_fBackupTime = 0.0f;
				m_Alpha.ChangeColor(0x00ffffff, m_fCloudTexTime);

				// uv ��ǥ�� �ٲٱ�
				for (i=0; i<NUM_CLOUD_VERTEX; ++i)
				{
					float fTempUV = m_pVertices[i].tu;
					m_pVertices[i].tu = m_pVertices[i].tu2;
					m_pVertices[i].tu2 = fTempUV;
					fTempUV = m_pVertices[i].tv;
					m_pVertices[i].tv = m_pVertices[i].tv2;
					m_pVertices[i].tv2 = fTempUV;
				}
			}
		}
	}
}

void CN3Cloud::Render()
{
	__Matrix44 matWorld; matWorld.Identity();
	s_lpD3DDev->SetTransform( D3DTS_WORLD, &matWorld );

	static WORD CloudIndex[30] = {0,1,4,1,2,5,2,3,6,3,0,7,5,4,1,6,5,2,7,6,3,4,7,0,4,5,7,5,6,7};
	//static WORD CloudIndex[54] = {0,1,4,1,2,5,2,3,6,3,0,7,5,4,1,6,5,2,7,6,3,4,7,0,
	///							4,5,8,5,6,9,6,7,10,7,4,11,9,8,5,10,9,6,11,10,7,8,11,4,
	//							8,9,11,9,10,11};

	// backup state
	DWORD dwAlphaOp, dwAlphaArg1, dwAlphaArg2;
	s_lpD3DDev->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwAlphaOp );
	s_lpD3DDev->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwAlphaArg1);
	s_lpD3DDev->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwAlphaArg2);
	// set state
	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	if (D3DTOP_MODULATE != dwAlphaOp) s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	if (D3DTA_TEXTURE != dwAlphaArg1) s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	if (D3DTA_DIFFUSE != dwAlphaArg2) s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	s_lpD3DDev->SetFVF(FVF_XYZCOLORT2);

	D3DCOLOR color1 = m_Color1.GetCurColor();
	D3DCOLOR color2 = m_Color2.GetCurColor();
	__ASSERT(CLOUD_NONE != m_eCloud1 && CLOUD_NONE != m_eCloud2, "no cloud texture type");
	// render cloud 1
	int i;
	for (i=0; i<4; ++i) m_pVertices[i].color = color1&0x00ffffff;
	for (i=4; i<NUM_CLOUD_VERTEX; ++i) m_pVertices[i].color = color1;
	s_lpD3DDev->SetTexture(0, GetTex(m_eCloud1));
	s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 10,
						CloudIndex, D3DFMT_INDEX16, m_pVertices, sizeof(__VertexXyzColorT2));

	s_lpD3DDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX , 1);
	if (CLOUD_NONE != m_eCloud3)
	{
		D3DCOLOR Alpha = m_Alpha.GetCurColor();
		if (Alpha<color2) color2 = (Alpha&0xff000000) | (color2&0x00ffffff);	// ���� �� ��ȭ�� alpha���� ���� ��üalpha������ ū ��� ���� ��ü alpha������ ��ü
		// render cloud 2
		for (i=0; i<4; ++i) m_pVertices[i].color = color2&0x00ffffff;
		for (i=4; i<NUM_CLOUD_VERTEX; ++i) m_pVertices[i].color = color2;
		s_lpD3DDev->SetTexture(0, GetTex(m_eCloud2));
		s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 10,
							CloudIndex, D3DFMT_INDEX16, m_pVertices, sizeof(__VertexXyzColorT2));
		// render cloud 3
		D3DCOLOR color3 = ((0xff-(color2>>24))<<24) | (color2&0x00ffffff);	// color2�� alpha���� 0xff���� �� ������ �ٲ�
		for (i=4; i<NUM_CLOUD_VERTEX; ++i) m_pVertices[i].color = color3;			
		s_lpD3DDev->SetTexture(0, GetTex(m_eCloud3));
		s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 10,
							CloudIndex, D3DFMT_INDEX16, m_pVertices, sizeof(__VertexXyzColorT2));
	}
	else
	{
		// render cloud 2
		for (i=0; i<4; ++i) m_pVertices[i].color = color2&0x00ffffff;
		for (i=4; i<NUM_CLOUD_VERTEX; ++i) m_pVertices[i].color = color2;
		s_lpD3DDev->SetTexture(0, GetTex(m_eCloud2));
		s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 10,
							CloudIndex, D3DFMT_INDEX16, m_pVertices, sizeof(__VertexXyzColorT2));
	}
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX , 0);

	// restore state
	if (D3DTOP_MODULATE != dwAlphaOp) s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	if (D3DTA_TEXTURE != dwAlphaArg1) s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	if (D3DTA_DIFFUSE != dwAlphaArg2) s_lpD3DDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

}

LPDIRECT3DTEXTURE9	CN3Cloud::GetTex(e_CLOUDTEX tex)
{
	if (NULL == m_pTextures[tex])
	{
		m_pTextures[tex] = s_MngTex.Get(m_szTextures[tex]);
		if (NULL == m_pTextures[tex]) return NULL;
	}

	return m_pTextures[tex]->Get();
}

void	CN3Cloud::Init(	const std::string* pszFNs)
{
	Release();

	for(int i = 0; i < NUM_CLOUD; i++) m_szTextures[i] = pszFNs[i];

//	3���϶�
/*	const float fSqrt3 = 1.0f/sqrtf(3.0f);
	const float fOffset = 3.0f;
	const float fSmallLength = 5.0f;
	const float fMediumLength = fSmallLength + fOffset;
	const float fBigLenth = fMediumLength + fOffset*fSqrt3;
	const float fBigHeight = 0.0f;
	const float fMediumHeight = fBigHeight + fOffset;
	const float fSmallHeight = fMediumHeight + fOffset*fSqrt3;

	D3DCOLOR BigColor = 0x00ffffff;
	D3DCOLOR MediumColor = 0xffffffff;
	D3DCOLOR SmallColor = 0xffffffff;

	float fTmp1, fTmp2;
	fTmp1 = fBigLenth-fMediumLength;
	fTmp2 = fMediumHeight - fBigHeight;
	const float fBig = sqrtf(fTmp1*fTmp1 + fTmp2*fTmp2);	// ��ū �簢���� �߰��簢������ �Ÿ�
	fTmp1 = fMediumLength - fSmallLength;
	fTmp2 = fSmallHeight - fMediumHeight;
	const float fMedium = sqrtf(fTmp1*fTmp1 + fTmp2*fTmp2);	// �߰� �簢���� �����簢������ �Ÿ�
	const float fSmall = fSmallLength;						// ���� �簢���� 0���� ������ �Ÿ�
	const float fTotal = fBig + fMedium + fSmall;

	float fTexUVLeft = 0.0f, fTexUVTop = 0.0f, fTexUVRight = 3.0f, fTexUVBottom = 3.0f;	// �ؽ��� �������� ǥ���� ���� ��ǥ(�ؽ����� ��ü�� �ɼ��� �ְ� �Ϻκ��� �� ���� �ֱ� ������)
//	const float fTexOffsetU_M = (fBig/fTotal)*(fTexUVRight-fTexUVLeft)/2;
//	const float fTexOffsetV_M = (fBig/fTotal)*(fTexUVBottom-fTexUVTop)/2;
//	const float fTexOffsetU_S = ((fBig+fMedium)/fTotal)*(fTexUVRight-fTexUVLeft)/2;
//	const float fTexOffsetV_S = ((fBig+fMedium)/fTotal)*(fTexUVBottom-fTexUVTop)/2;

	const float fTexOffsetU_M = (1.0f - fMediumLength/fBigLenth)*(fTexUVRight-fTexUVLeft)/2;
	const float fTexOffsetV_M = (1.0f - fMediumLength/fBigLenth)*(fTexUVBottom-fTexUVTop)/2;
	const float fTexOffsetU_S = (1.0f - fSmallLength/fBigLenth)*(fTexUVRight-fTexUVLeft)/2;
	const float fTexOffsetV_S = (1.0f - fSmallLength/fBigLenth)*(fTexUVBottom-fTexUVTop)/2;

	// big
	m_pVertices[0].Set(-fBigLenth, fBigHeight,-fBigLenth, BigColor, fTexUVLeft,	fTexUVTop,		fTexUVLeft,		fTexUVTop);
	m_pVertices[1].Set( fBigLenth, fBigHeight,-fBigLenth, BigColor, fTexUVRight,	fTexUVTop,		fTexUVRight,	fTexUVTop);
	m_pVertices[2].Set( fBigLenth, fBigHeight, fBigLenth, BigColor, fTexUVRight,	fTexUVBottom,	fTexUVRight,	fTexUVBottom);
	m_pVertices[3].Set(-fBigLenth, fBigHeight, fBigLenth, BigColor, fTexUVLeft,	fTexUVBottom,	fTexUVLeft,		fTexUVBottom);
	// medium
	m_pVertices[4].Set(-fMediumLength, fMediumHeight,-fMediumLength, MediumColor, fTexUVLeft+fTexOffsetU_M,	fTexUVTop+fTexOffsetV_M,		fTexUVLeft+fTexOffsetU_M,	fTexUVTop+fTexOffsetV_M);
	m_pVertices[5].Set( fMediumLength, fMediumHeight,-fMediumLength, MediumColor, fTexUVRight-fTexOffsetU_M, fTexUVTop+fTexOffsetV_M,		fTexUVRight-fTexOffsetU_M, fTexUVTop+fTexOffsetV_M);
	m_pVertices[6].Set( fMediumLength, fMediumHeight, fMediumLength, MediumColor, fTexUVRight-fTexOffsetU_M, fTexUVBottom-fTexOffsetV_M,	fTexUVRight-fTexOffsetU_M, fTexUVBottom-fTexOffsetV_M);
	m_pVertices[7].Set(-fMediumLength, fMediumHeight, fMediumLength, MediumColor, fTexUVLeft+fTexOffsetU_M,	fTexUVBottom-fTexOffsetV_M,	fTexUVLeft+fTexOffsetU_M,	fTexUVBottom-fTexOffsetV_M);

	// small
	m_pVertices[8].Set( -fSmallLength, fSmallHeight,-fSmallLength, SmallColor, fTexUVLeft+fTexOffsetU_S,	fTexUVTop+fTexOffsetV_S,		fTexUVLeft+fTexOffsetU_S,	fTexUVTop+fTexOffsetV_S);
	m_pVertices[9].Set(  fSmallLength, fSmallHeight,-fSmallLength, SmallColor, fTexUVRight-fTexOffsetU_S, fTexUVTop+fTexOffsetV_S,		fTexUVRight-fTexOffsetU_S, fTexUVTop+fTexOffsetV_S);
	m_pVertices[10].Set( fSmallLength, fSmallHeight, fSmallLength, SmallColor, fTexUVRight-fTexOffsetU_S, fTexUVBottom-fTexOffsetV_S,	fTexUVRight-fTexOffsetU_S, fTexUVBottom-fTexOffsetV_S);
	m_pVertices[11].Set(-fSmallLength, fSmallHeight, fSmallLength, SmallColor, fTexUVLeft+fTexOffsetU_S,	fTexUVBottom-fTexOffsetV_S,	fTexUVLeft+fTexOffsetU_S,	fTexUVBottom-fTexOffsetV_S);
*/ 
// 2���϶�
	const float fSqrt3 = 1.0f/sqrtf(3.0f);
	float fBigLenth = 16.0f;				// ���� ���θ�ü�� �Ʒ� ū �簢�� ����
	const float fSmallLength = 8.0f;		// ���� ���θ�ü�� �� ���� �簢�� ����
	const float fBigHeight = 5.0f;			// ���� �簢���θ�ü�� �Ʒ� ����(���� 0.0f ����)
	const float fSmallHeight = fBigHeight + (fBigLenth-fSmallLength)*fSqrt3;	// ���� ���θ�ü�� �� ����(���� 0.0f ����)

	fBigLenth = 24.0f;	//���� �ڵ�� ����ȭ ����, �̰��� ū ������� �ٽ� �ø�

	D3DCOLOR BigColor = 0x00ffffff;
	D3DCOLOR SmallColor = 0xffffffff;

	float fTexUVLeft = 0.0f, fTexUVTop = 0.0f, fTexUVRight = 4.0f, fTexUVBottom = 4.0f;	// �ؽ��� �������� ǥ���� ���� ��ǥ(�ؽ����� ��ü�� �ɼ��� �ְ� �Ϻκ��� �� ���� �ֱ� ������)
	float fTmp1 = fBigLenth - fSmallLength;
	float fTmp2 = fSmallHeight - fBigHeight;
//	const float fBig = sqrtf(fTmp1*fTmp1 + fTmp2*fTmp2);
//	const float fSmall = fSmallLength;
//	const float fTotal = fBig + fSmall;
//	const float fTexOffsetU = (fSmall/fTotal)*(fTexUVRight-fTexUVLeft)/2;
//	const float fTexOffsetV = (fSmall/fTotal)*(fTexUVBottom-fTexUVTop)/2;
	const float fTexOffsetU = (1.0f - fSmallLength/fBigLenth)*(fTexUVRight-fTexUVLeft)/2;
	const float fTexOffsetV = (1.0f - fSmallLength/fBigLenth)*(fTexUVBottom-fTexUVTop)/2;
	// big
	m_pVertices[0].Set(-fBigLenth, fBigHeight,-fBigLenth, BigColor, fTexUVLeft,		fTexUVTop,		fTexUVLeft,		fTexUVTop);
	m_pVertices[1].Set( fBigLenth, fBigHeight,-fBigLenth, BigColor, fTexUVRight,	fTexUVTop,		fTexUVRight,	fTexUVTop);
	m_pVertices[2].Set( fBigLenth, fBigHeight, fBigLenth, BigColor, fTexUVRight,	fTexUVBottom,	fTexUVRight,	fTexUVBottom);
	m_pVertices[3].Set(-fBigLenth, fBigHeight, fBigLenth, BigColor, fTexUVLeft,		fTexUVBottom,	fTexUVLeft,		fTexUVBottom);
	// small
	m_pVertices[4].Set(-fSmallLength, fSmallHeight,-fSmallLength, SmallColor, fTexUVLeft+fTexOffsetU,	fTexUVTop+fTexOffsetV,		fTexUVLeft+fTexOffsetU,	fTexUVTop+fTexOffsetV);
	m_pVertices[5].Set( fSmallLength, fSmallHeight,-fSmallLength, SmallColor, fTexUVRight-fTexOffsetU, fTexUVTop+fTexOffsetV,		fTexUVRight-fTexOffsetU, fTexUVTop+fTexOffsetV);
	m_pVertices[6].Set( fSmallLength, fSmallHeight, fSmallLength, SmallColor, fTexUVRight-fTexOffsetU, fTexUVBottom-fTexOffsetV,	fTexUVRight-fTexOffsetU, fTexUVBottom-fTexOffsetV);
	m_pVertices[7].Set(-fSmallLength, fSmallHeight, fSmallLength, SmallColor, fTexUVLeft+fTexOffsetU,	fTexUVBottom-fTexOffsetV,	fTexUVLeft+fTexOffsetU,	fTexUVBottom-fTexOffsetV);
}


void CN3Cloud::SetCloud(e_CLOUDTEX eCloud1, e_CLOUDTEX eCloud2, float fSec)
{
	if (CLOUD_NONE == eCloud1 || CLOUD_NONE == eCloud2) return;

	// ���� �ʱ�ȭ
	m_Alpha.ChangeColor(0xffffffff);
	m_fCloudTexTime = 0.0f;
	m_eCloud3 = CLOUD_NONE;
	m_eBackupCloud = CLOUD_NONE;
	m_fBackupTime = 0.0f;

	// �ٲ� ���� �Ǵ�
	int iSameCount = 0;
	if (eCloud1 == m_eCloud1) ++iSameCount;
	else if (eCloud1 == m_eCloud2) ++iSameCount;
	if (eCloud2 == m_eCloud1) ++iSameCount;
	else if (eCloud2 == m_eCloud2) ++iSameCount;

	if (2 == iSameCount)		// ���� �����̹Ƿ� �ٲ� �ʿ䰡 ����
	{
		return;
	}

	if (1 == iSameCount)		// �ϳ��� ��ü�ϸ� �ȴ�.
	{
		if (m_eCloud1 == eCloud1)
		{
			m_eCloud3 = eCloud2;
		}
		else if (m_eCloud1 == eCloud2)
		{
			m_eCloud3 = eCloud1;
		}
		else if (m_eCloud2 == eCloud1)
		{
			m_eCloud2 = m_eCloud1;
			m_eCloud1 = eCloud1;
			m_eCloud3 = eCloud2;
		}
		else
		{
			m_eCloud2 = m_eCloud1;
			m_eCloud1 = eCloud2;
			m_eCloud3 = eCloud1;
		}
		if (0.0f == fSec)
		{
			m_eCloud2 = m_eCloud3;
			m_eCloud3 = CLOUD_NONE;
		}
		else
		{
			m_Alpha.ChangeColor(0x00ffffff, fSec);
			m_fCloudTexTime = fSec;
		}
		return;
	}

	// �ΰ� ��� ��ü
	if (0.0f == fSec)
	{
		m_eCloud1 = eCloud1;
		m_eCloud2 = eCloud2;
		m_eCloud3 = CLOUD_NONE;
		return;
	}
	m_eCloud3 = eCloud1;
	m_eBackupCloud = eCloud2;
	m_fCloudTexTime = fSec*0.5f;
	m_fBackupTime = m_fCloudTexTime;
	m_Alpha.ChangeColor(0x00ffffff, m_fCloudTexTime);
}

