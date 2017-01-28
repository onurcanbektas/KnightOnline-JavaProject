// N3Pond.cpp: implementation of the CN3Pond class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3Pond.h"
#include "N3Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#define ATISQRT	4.94974747f

// ������.. ���� ����Ʈ�� �Ҵ�..
CN3Pond::CN3Pond()
{
	m_iPondMeshNum = 0;
	m_pCPondMesh=NULL;
	m_pfMaxVtx=NULL;

	memset(m_pTexPond, 0, sizeof(m_pTexPond));
}

CN3Pond::~CN3Pond()
{
	Release();
}

void CN3Pond::Release()
{
	if(m_iPondMeshNum>0)
	{
		if(m_pCPondMesh!=NULL)
		{
			delete []m_pCPondMesh;
			m_pCPondMesh=NULL;
		}
		m_iPondMeshNum=0;
	}

	if(m_iMaxVtxNum>0)
	{
		if(m_pfMaxVtx)
		{
			delete [] m_pfMaxVtx;
			m_pfMaxVtx = NULL;
		}
	
		m_iMaxVtxNum=0;
	}

	for(int i = 0; i < MAX_POND_TEX; i++)
		s_MngTex.Delete(&(m_pTexPond[i]));

	m_fTexIndex = 0.0f;
}

bool CN3Pond::Load(HANDLE hFile)
{
	Release();

	DWORD dwNum;

	ReadFile(hFile, &m_iPondMeshNum, sizeof(int), &dwNum, NULL);	///
//	if(m_iPondMeshNum<=0) {m_iPondMeshNum=0;return 1;}
	if(true) {m_iPondMeshNum=0;return 1;}

	m_pCPondMesh = new CPongMesh [m_iPondMeshNum];	///

	CPongMesh* ptmpPondMesh;
	for (int i=0;i<m_iPondMeshNum;i++)
	{
		ptmpPondMesh = &m_pCPondMesh[i];

		int iVC;
		ReadFile(hFile, &iVC, sizeof(iVC), &dwNum, NULL);				// �� ����
		ptmpPondMesh->m_iVC = iVC;	///
		ptmpPondMesh->m_bTick2Rand = FALSE;		///
		if(iVC<=0) 
		{
			ptmpPondMesh->m_pVertices=NULL;
			continue;
		}

		int iWidthVertex;
		ReadFile(hFile, &iWidthVertex, sizeof(iWidthVertex), &dwNum, NULL);				// �� ���δ� �� ����
		ptmpPondMesh->m_iWidthVtx = iWidthVertex;		///
		ptmpPondMesh->m_iHeightVtx = iVC/iWidthVertex;	///

		int iTexNameLength = 0;
		ReadFile(hFile, &iTexNameLength,sizeof(int),&dwNum,NULL);
		if(iTexNameLength>0)
		{
			char szTextueFName[_MAX_PATH],szTextue[50];
			ReadFile(hFile, szTextue, iTexNameLength, &dwNum, NULL);			// texture name
			szTextue[iTexNameLength] = NULL;
			sprintf(szTextueFName,"misc\\river\\%s",szTextue);

			ptmpPondMesh->m_pTexWave = s_MngTex.Get(szTextueFName);
			__ASSERT(ptmpPondMesh->m_pTexWave, "CN3Pond::texture load failed");
		}		


		// XyxT2 -> XyzColorT2 Converting.
		ptmpPondMesh->m_pVertices = new __VertexPond[iVC];	///
		ReadFile(hFile,ptmpPondMesh->m_pVertices,iVC*sizeof(__VertexPond),&dwNum,NULL);
		ptmpPondMesh->m_pVertices[0].y += 0.2f;				//	��ġ�� ������ ������ ũ�� �䵿ģ��
		ptmpPondMesh->m_pVertices[iWidthVertex].y += 0.2f;	//	��ġ�� ������ ������ ũ�� �䵿ģ��
		ptmpPondMesh->m_pfMaxHeight = ptmpPondMesh->m_pVertices[0].y += 0.3f;		//	������ �ִ�ġ

		ptmpPondMesh->m_pfVelocityArray = new float[iVC];	///
		memset(ptmpPondMesh->m_pfVelocityArray,0,sizeof(float)*iVC);

		
		int iIC;
		ReadFile(hFile, &iIC, sizeof(iIC), &dwNum, NULL);				// IndexBuffer Count.
		ptmpPondMesh->m_iIC = iIC;		///
		ptmpPondMesh->m_wpIndex = new WORD [iVC*6];		///

		int j,k;
		int iWidth = iWidthVertex,iHeight = iVC/iWidthVertex;
		int x=0,y=iWidth;
		WORD* indexPtr = ptmpPondMesh->m_wpIndex;	//	�ﰢ���� �θ� ��ġ ����
		iWidth--;

		__VertexPond* ptVtx = ptmpPondMesh->m_pVertices;
		float StX,EnX,StZ,EnZ;
		StX = ptVtx[0].x,EnX = ptVtx[iWidth].x;
		StZ = ptVtx[0].z,EnZ = ptVtx[iHeight].z;
		for (j=0; j<iHeight; j++)
		{
			for (k=0; k<iWidth; k++)
			{
				//	�ﰢ���� �θ� ��ġ ����
				indexPtr[0] = x;
				indexPtr[1] = x+1;
				indexPtr[2] = y;
				indexPtr[3] = y;
				indexPtr[4] = x+1;
				indexPtr[5] = y+1;

				indexPtr += 6;
				x++;
				y++;

				//	������ �ּ��ִ� ��ġ ����
				if(StX>ptVtx->x) StX = ptVtx->x;
				if(EnX<ptVtx->x) EnX = ptVtx->x;
				if(StZ>ptVtx->z) StZ = ptVtx->z;
				if(EnZ<ptVtx->z) EnZ = ptVtx->z;
				ptVtx++;
			}
			x++;
			y++;
		}


		float fmin,fmax,fmaxcal,fmincal;
		if(ptmpPondMesh->m_pfMaxHeight>0.0f)
		{
			fmax = ptmpPondMesh->m_pfMaxHeight*0.04f;
			fmin = -fmax;
			fmaxcal = fmax*ATISQRT;
			fmincal = -fmaxcal;
		}
		else if(ptmpPondMesh->m_pfMaxHeight<0.0f)
		{
			fmin = ptmpPondMesh->m_pfMaxHeight*0.04f;
			fmax = -fmin;
			fmincal = fmin*ATISQRT;
			fmaxcal = -fmincal;
		}
		else
		{
			fmax = 0.04f;
			fmin = -fmax;
			fmaxcal = fmax*ATISQRT;
			fmincal = -fmaxcal;
		}

		ptmpPondMesh->m_fmin = fmin;
		ptmpPondMesh->m_fmax = fmax;
		ptmpPondMesh->m_fmaxcal = fmaxcal;
		ptmpPondMesh->m_fmincal = fmincal;

		ptmpPondMesh->m_vCenterPo.Set(((EnX-StX)/2.0f) + StX
									 ,ptmpPondMesh->m_pVertices[1].y
									 ,((EnZ-StZ)/2.0f) + StZ);
		if(EnX-StX > EnZ-StZ)
			ptmpPondMesh->m_fRadius = EnX-StX;
		else ptmpPondMesh->m_fRadius = EnZ-StZ;


		ptmpPondMesh->m_bTick2Rand = TRUE;		///

		if(m_iMaxVtxNum<iVC) m_iMaxVtxNum=iVC;	//	����ū ������ ����
	}	

	m_pfMaxVtx = new float [m_iMaxVtxNum];
	m_iMaxVtxNum *= sizeof(float);

#ifdef _DEBUG
	for(int x=0;x<2000;x++) UpdateWaterPositions();
#endif

	if(m_iPondMeshNum<=0) return false;

	char szFileName[30];
	for (int i=0;i<MAX_POND_TEX;i++)
	{
		sprintf(szFileName, "misc\\river\\caust%02d.dxt", i);
		m_pTexPond[i] = CN3Base::s_MngTex.Get(szFileName);
		__ASSERT(m_pTexPond[i], "CN3Pond::texture load failed");
	}

	return true;
}

void CN3Pond::Tick()
{
	if(m_iPondMeshNum==0) return;

	float frame;

	m_fTexIndex += s_fSecPerFrm*15.0f;
	if (m_fTexIndex >= 32.0f)
	{
		m_fTexIndex -= 32.0f;
	}

	// �������� �Ӱ谪���� ������ ������..
	if ( CN3Base::s_fFrmPerSec < 0.1f ) return;
	
	// Desire Frame Rate���� Frame�� �� ������ ���..
	if ( 30.0f <= CN3Base::s_fFrmPerSec )
	{
		static float ftemp = 0.0f;
		frame = (30.0f / CN3Base::s_fFrmPerSec)*1.2f;
		ftemp += frame;
		if ( ftemp > 1.0f )
		{
			UpdateWaterPositions();
			ftemp -= 1.0f;
		}
	}
	// Desire Frame���� Frame�� �� �ȳ����� ���..
	else 
	{
		static float ftemp = 0.0f;
		frame = (30.0f / CN3Base::s_fFrmPerSec)*1.2f;
		ftemp   += frame;
		int i    = (int)ftemp;
		float j  = ftemp - (float)i;

		for ( int k = 0; k < i; k++ )
		{
			UpdateWaterPositions();
		}
//		TRACE("Frame not Accecl \n");
		ftemp = j;
	}
}

void CN3Pond::Render()
{
	if(m_iPondMeshNum <= 0) return;

	int iTex = (int)m_fTexIndex;
	__ASSERT(iTex < MAX_POND_TEX, "Pond Texture index overflow..");
	if(iTex >= MAX_POND_TEX || NULL == m_pTexPond[iTex]) return;

	// Backup
	__Matrix44 matWorld, matOld;	matWorld.Identity();
	DWORD dwAlphaEnable, dwSrcBlend, dwDestBlend;
	s_lpD3DDev->GetTransform(D3DTS_WORLD, &matOld);
	s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaEnable);
	s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
	s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
	DWORD dwColor_0, dwColorArg1_0, dwColorArg2_0, dwMipFilter_0;
	s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwColor_0);
	s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLORARG1, &dwColorArg1_0);
	s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLORARG2, &dwColorArg2_0);
	s_lpD3DDev->GetSamplerState(0, D3DSAMP_MIPFILTER, &dwMipFilter_0);
	DWORD dwColor_1, dwColorArg1_1, dwColorArg2_1, dwMipFilter_1;
	s_lpD3DDev->GetTextureStageState(1, D3DTSS_COLOROP, &dwColor_1);
	s_lpD3DDev->GetTextureStageState(1, D3DTSS_COLORARG1, &dwColorArg1_1);
	s_lpD3DDev->GetTextureStageState(1, D3DTSS_COLORARG2, &dwColorArg2_1);
	s_lpD3DDev->GetSamplerState(1, D3DSAMP_MIPFILTER, &dwMipFilter_1);


	// Set
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &matWorld);


	// texture state ���� (alpha)
	s_lpD3DDev->SetTexture(0, m_pTexPond[iTex]->Get());
	s_lpD3DDev->SetTexture(2, NULL);

	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    s_lpD3DDev->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
    s_lpD3DDev->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);	
	s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT);	
    s_lpD3DDev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
    s_lpD3DDev->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

	s_lpD3DDev->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2);

	for(int i=0;i<m_iPondMeshNum;i++)
	{
		if(m_pCPondMesh[i].m_bTick2Rand)
		{
			if(m_pCPondMesh[i].m_pTexWave) s_lpD3DDev->SetTexture(1, m_pCPondMesh[i].m_pTexWave->Get());
			else s_lpD3DDev->SetTexture(1, NULL);

			s_lpD3DDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST
												, 0
												, m_pCPondMesh[i].m_iVC
												, m_pCPondMesh[i].m_iIC
												, m_pCPondMesh[i].m_wpIndex
												, D3DFMT_INDEX16
												, m_pCPondMesh[i].m_pVertices
												, sizeof(__VertexPond));
		}
	}


	// restore 
	s_lpD3DDev->SetTransform(D3DTS_WORLD, &matOld);
	s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaEnable);
	s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
	s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);

	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, dwColor_0);
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg1_0);
	s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG2, dwColorArg2_0);
	s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dwMipFilter_0);

	s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLOROP, dwColor_1);
	s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG1, dwColorArg1_1);
	s_lpD3DDev->SetTextureStageState(1, D3DTSS_COLORARG2, dwColorArg2_1);
	s_lpD3DDev->SetSamplerState(1, D3DSAMP_MIPFILTER, dwMipFilter_1);
}

void CN3Pond::UpdateWaterPositions()
{
	CPongMesh* pPondMesh;

	//	���� ����Ÿ
	int	x, y,n,m;
	float d;
	__VertexPond* pVtx,*ptmpVtx,*ptmpVtxSub,*ptmpVtxPlus;
	float* pForceArray,*ptmpForceArray,*ptmpFArrSub,*ptmpFArrPlus;

	//	��� ����
	float max,min,mincal,maxcal;

	for(int i=0;i<m_iPondMeshNum;i++)
	{
		pPondMesh = &m_pCPondMesh[i];

		//	�̹��� ������ ���� ��� �Ѿ
		if(CN3Base::s_CameraData.IsOutOfFrustum(pPondMesh->m_vCenterPo,pPondMesh->m_fRadius)==TRUE)
		{
			pPondMesh->m_bTick2Rand = FALSE;
			continue;
		}
		else pPondMesh->m_bTick2Rand = TRUE;

//		TRACE("Pond Is Chk  ---------- %d \n",i);
		//	���ʵ���Ÿ �ۼ�
		m = pPondMesh->m_iWidthVtx;
		n = pPondMesh->m_iHeightVtx;
		max = pPondMesh->m_fmax;
		min = pPondMesh->m_fmin;
		maxcal = pPondMesh->m_fmaxcal;
		mincal = pPondMesh->m_fmincal;

		memset(m_pfMaxVtx, 0, m_iMaxVtxNum);

		pVtx = pPondMesh->m_pVertices;
		pForceArray = m_pfMaxVtx;
		
		//	��� 
		for (x=1; x<n-1; x++)
		{
			ptmpFArrSub = pForceArray;
			pForceArray += m;
			ptmpForceArray = pForceArray;
			ptmpFArrPlus = ptmpForceArray+m;

			ptmpVtxSub = pVtx;
			pVtx += m;
			ptmpVtx = pVtx;
			ptmpVtxPlus = ptmpVtx+m;

			for (y=1; y<m-1; y++) 
			{
				 //   Kernel looks like this:
				 //
				 //    1/Root2 |    1    | 1/Root2 
				 //   ---------+---------+---------
				 //       1    |    0    |    1  
				 //   ---------+---------+---------
				 //    1/Root2 |    1    | 1/Root2 
				 
				ptmpForceArray++,ptmpFArrPlus++,ptmpFArrSub++;
				ptmpVtx++,ptmpVtxPlus++,ptmpVtxSub++;

				d = ptmpVtx->y - (ptmpVtx-1)->y;
				 if ( d < min ) d = min;	if ( d > max ) d = max;
				 *ptmpForceArray	-= d;
				 *(ptmpForceArray-1)+= d;

				 d = ptmpVtx->y - ptmpVtxSub->y;
				 if ( d < min ) d = min;	if ( d > max ) d = max;
				 *ptmpForceArray	-= d;
				 *ptmpFArrSub		+= d;

				 d = ptmpVtx->y - (ptmpVtx+1)->y;
				 if ( d < min ) d = min;	if ( d > max ) d = max;
				 *ptmpForceArray	-= d;
				 *(ptmpForceArray+1)+= d;

				 d = ptmpVtx->y - ptmpVtxPlus->y;
				 if ( d < min ) d = min;	if ( d > max ) d = max;
				 *ptmpForceArray	-= d;
				 *ptmpFArrPlus		+= d;

				 d = (ptmpVtx->y - (ptmpVtxPlus+1)->y)*ATISQRT;
				 if ( d < mincal ) d = mincal;	if ( d > maxcal ) d = maxcal;
				 *ptmpForceArray	-= d;
				 *(ptmpFArrPlus+1)	+= d;

				 d = (ptmpVtx->y - (ptmpVtxSub-1)->y)*ATISQRT;
				 if ( d < mincal ) d = mincal;	if ( d > maxcal ) d = maxcal;
				 *ptmpForceArray	-= d;
				 *(ptmpFArrSub-1)	+= d;

				 d = (ptmpVtx->y - (ptmpVtxPlus-1)->y)*ATISQRT;
				 if ( d < mincal ) d = mincal;	if ( d > maxcal ) d = maxcal;
				 *ptmpForceArray	-= d;
				 *(ptmpFArrPlus-1)	+= d;

				 d = (ptmpVtx->y - (ptmpVtxSub+1)->y)*ATISQRT;
				 if ( d < mincal ) d = mincal;	if ( d > maxcal ) d = maxcal;
				 *ptmpForceArray	-= d;
				 *(ptmpFArrSub+1)	+= d;
			}
		}

		ptmpForceArray = pPondMesh->m_pfVelocityArray;	//	�������̶� ������
		pForceArray = m_pfMaxVtx;
		pVtx = pPondMesh->m_pVertices;
		for (x=0; x<pPondMesh->m_iVC; x++)
		{
//			*ptmpForceArray += *pForceArray*0.02f;
			(*ptmpForceArray) += (*pForceArray)*0.001f;
			
			pVtx->y += (*ptmpForceArray);
			if(pVtx->y > pPondMesh->m_pfMaxHeight)
				pVtx->y = pPondMesh->m_pfMaxHeight;

			pForceArray++,pVtx++,ptmpForceArray++;
		}
	}
}
