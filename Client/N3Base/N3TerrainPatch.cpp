// N3TerrainPatch.cpp: implementation of the CN3TerrainPatch class.
// 2001.10.22.
//
//////////////////////////////////////////////////////////////////////

//#include "StdAfx.h"
#include "..\WarFare\N3Terrain.h"
#include "..\WarFare\N3TerrainPatch.h"
#include "N3Texture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif


//
//	������
//
CN3TerrainPatch::CN3TerrainPatch()
{
	m_bIsRender = FALSE;
	m_iLevel = 0;
	m_CellSize = 0;
	m_NumCell = 0;
	m_ti_LBPoint.x = m_ti_LBPoint.y = -1;
	m_pRefTerrain = NULL;
	for(int i=0;i<4;i++) m_IsBlunt[i] = true;

	m_pRefColorTex = NULL;
	
	m_VBSize[2] = 10;
	m_VBSize[1] = 56;
	m_VBSize[0] = 256;

	m_pVB = NULL;

	m_FanInfoList.clear();

	m_pTileTexIndx[0] = NULL;
	m_pTileTexIndx[1] = NULL;
	m_pIsTileFull = NULL;

	//lightmap...
	m_pLightMapVB = NULL;
	m_pRefLightMapTex = NULL;
	m_NumLightMapTex = 0;
}


//
//	�Ҹ���
//
CN3TerrainPatch::~CN3TerrainPatch()
{
	Release();
}


//
//
//
void CN3TerrainPatch::Release()
{
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if(m_pLightMapVB)
	{
		m_pLightMapVB->Release();
		m_pLightMapVB = NULL;
	}
	
	m_FanInfoList.clear();

	for(int i=0;i<2;i++)
	{
		delete[] m_pTileTexIndx[i];
		m_pTileTexIndx[i] = NULL;
	}
	delete[] m_pIsTileFull;
	m_pIsTileFull = NULL;

	if(m_pRefLightMapTex)
	{
		delete[] m_pRefLightMapTex;	
		m_pRefLightMapTex = NULL;
	}
}


//
//
//
void CN3TerrainPatch::Init()
{
	Release();

	for(int i=0;i<4;i++) m_IsBlunt[i] = true;
	m_bIsRender = FALSE;
	m_iLevel = 0;
	m_CellSize = 0;
	m_NumCell = 0;

	m_NumLightMapTex = 0;
	m_pLightMapVB = NULL;
	m_pRefLightMapTex = NULL;

	m_ti_LBPoint.x = m_ti_LBPoint.y = -1;
	m_pRefTerrain = NULL;

	m_pVB = NULL;
	m_pRefColorTex = NULL;

	m_pTileTexIndx[0] = NULL;
	m_pTileTexIndx[1] = NULL;
	m_pIsTileFull = NULL;
	
	m_FanInfoList.clear();
}


//
//
//
void CN3TerrainPatch::Init(CN3Terrain* pTerrain)
{
	Release();

	for(int i=0;i<4;i++) m_IsBlunt[i] = true;
	m_bIsRender = FALSE;
	m_iLevel = 0;
	m_CellSize = 0;
	m_NumCell = 0;

	m_NumLightMapTex = 0;
	m_pLightMapVB = NULL;
	m_pRefLightMapTex = NULL;

	m_ti_LBPoint.x = m_ti_LBPoint.y = -1;
	m_pRefTerrain = pTerrain;

	m_pVB = NULL;
	m_pRefColorTex = NULL;

	for(int i=0;i<2;i++) 
	{
		m_pTileTexIndx[i] = new int [PATCH_TILE_SIZE*PATCH_TILE_SIZE];
		memset(m_pTileTexIndx[i], -1, sizeof(int)*PATCH_TILE_SIZE*PATCH_TILE_SIZE);
	}
	m_pIsTileFull = new bool [PATCH_TILE_SIZE*PATCH_TILE_SIZE];

	m_FanInfoList.clear();
}


//
//	Set Level....
//
void CN3TerrainPatch::SetLevel(int level)
{
	if(level==m_iLevel) return;

	m_iLevel = level;

	m_CellSize = 1 << level;
	m_NumCell = PATCH_TILE_SIZE / m_CellSize;
	m_FanInfoList.clear();

	if(m_pVB) { m_pVB->Release(); m_pVB = NULL; }
	if(m_pLightMapVB) { m_pLightMapVB->Release(); m_pLightMapVB = NULL; }
	m_NumLightMapTex = 0;
	delete [] m_pRefLightMapTex; m_pRefLightMapTex = NULL;

	HRESULT hr;
	if(level==1)
	{
		hr = CN3Base::s_lpD3DDev->CreateVertexBuffer( m_VBSize[level-1]*sizeof(__VertexT2), 0, FVF_VNT2, D3DPOOL_MANAGED, &m_pVB, NULL );
		hr = CN3Base::s_lpD3DDev->CreateVertexBuffer( m_VBSize[level-1]*sizeof(__VertexT1), 0, FVF_VNT1, D3DPOOL_MANAGED, &m_pLightMapVB, NULL );

		m_NumLightMapTex = 0;
		m_pRefLightMapTex = new CN3Texture* [PATCH_TILE_SIZE*PATCH_TILE_SIZE];
	}
	else
	{
		hr = CN3Base::s_lpD3DDev->CreateVertexBuffer( m_VBSize[level-1]*sizeof(__VertexT1), 0, FVF_VNT1, D3DPOOL_MANAGED, &m_pVB, NULL );
	}
}


//
//
//
void CN3TerrainPatch::Tick()
{
	if(m_ti_LBPoint.x < 0 || m_ti_LBPoint.y < 0 ||
		m_ti_LBPoint.x >= (m_pRefTerrain->m_ti_MapSize-1) || m_ti_LBPoint.y >= (m_pRefTerrain->m_ti_MapSize-1)
	)
	{
		m_bIsRender = FALSE;		
	}
	if(m_bIsRender==FALSE) return;

	int HalfCell = m_CellSize >> 1;
	
	int VertexIdx = 0;
	int FaceCount = 0;
	int VertexCount = 0;
	
	int ix, iz;
	int tx, tz;
	int cx, cz;	//cell center point...
		
	FANINFO FaceInfo;
	
	if(m_iLevel==1)
	{
		m_FanInfoList.clear();
		m_NumLightMapTex = 0;
		__VertexT1* pLightMapVertices = NULL;

		__VertexT2* pVertices;
		m_pVB->Lock( 0, 0, (void**)&pVertices, 0 );

		int dir1, dir2;
		int TileCount = 0;
		float u1[4], u2[4], v1[4], v2[4];

		///////////////////////////////////
		//unitUV
		//				
		for(ix=0; ix<PATCH_TILE_SIZE;ix++)
		{
			for(iz=0; iz<PATCH_TILE_SIZE;iz++)
			{
				tx = ix + m_ti_LBPoint.x;
				tz = iz + m_ti_LBPoint.y;

				MAPDATA MapData = m_pRefTerrain->GetMapData(tx,tz);

				dir1 = MapData.Tex1Dir;
				dir2 = MapData.Tex2Dir;

				m_pTileTexIndx[0][TileCount] = MapData.Tex1Idx;
				m_pTileTexIndx[1][TileCount] = MapData.Tex2Idx;
				m_pIsTileFull[TileCount] = MapData.bIsTileFull;

				if( m_pTileTexIndx[0][TileCount]<0 || m_pTileTexIndx[0][TileCount]>m_pRefTerrain->m_NumTileTex || m_pIsTileFull[TileCount]==false)	// Ÿ���� ���� ���..�÷����� ���� ��...
				{
					u1[0] = u1[1] = UVConvert((float)(tx%UNITUV) / (float)UNITUV);
					u1[2] = u1[3] = UVConvert(u1[0] + (1.0f/(float)UNITUV));

					v1[0] = v1[3] = UVConvert((float)(UNITUV - (tz%UNITUV)) / (float)UNITUV);
					v1[1] = v1[2] = UVConvert(v1[0] - (1.0f/(float)UNITUV));

					//u1[0] =	u1[1] = (float)(tx%UNITUV) / (float)UNITUV;
					//u1[2] = u1[3] = u1[0] + (1.0f/(float)UNITUV);

					//v1[0] = v1[3] = (float)(UNITUV - (tz%UNITUV)) / (float)UNITUV;
					//v1[1] = v1[2] = v1[0] - (1.0f/(float)UNITUV);

					u2[0] =	u2[1] = 0.0f;
					u2[2] = u2[3] = 1.0f;

					v2[0] = v2[3] = 0.0f;
					v2[1] = v2[2] = 1.0f;
				}
				else	// Ÿ���� ��� ���...
				{
					u1[0] = m_pRefTerrain->m_fTileDirU[dir1][2];
					u1[1] = m_pRefTerrain->m_fTileDirU[dir1][0];
					u1[2] = m_pRefTerrain->m_fTileDirU[dir1][1];
					u1[3] = m_pRefTerrain->m_fTileDirU[dir1][3];

					v1[0] = m_pRefTerrain->m_fTileDirV[dir1][2];
					v1[1] = m_pRefTerrain->m_fTileDirV[dir1][0];
					v1[2] = m_pRefTerrain->m_fTileDirV[dir1][1];
					v1[3] = m_pRefTerrain->m_fTileDirV[dir1][3];

					u2[0] = m_pRefTerrain->m_fTileDirU[dir2][2];
					u2[1] = m_pRefTerrain->m_fTileDirU[dir2][0];
					u2[2] = m_pRefTerrain->m_fTileDirU[dir2][1];
					u2[3] = m_pRefTerrain->m_fTileDirU[dir2][3];

					v2[0] = m_pRefTerrain->m_fTileDirV[dir2][2];
					v2[1] = m_pRefTerrain->m_fTileDirV[dir2][0];
					v2[2] = m_pRefTerrain->m_fTileDirV[dir2][1];
					v2[3] = m_pRefTerrain->m_fTileDirV[dir2][3];
				}

				if((ix+iz)%2==0)
				{
					pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx,tz).fHeight,
												(float)(tz * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												u1[0], v1[0], u2[0], v2[0]);
					pVertices[VertexIdx+1].Set( (float)(tx * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx,tz+1).fHeight,
												(float)((tz+1) * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												u1[1], v1[1], u2[1], v2[1]);
					pVertices[VertexIdx+2].Set( (float)((tx+1) * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx+1,tz+1).fHeight,
												(float)((tz+1) * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												u1[2], v1[2], u2[2], v2[2]);
					pVertices[VertexIdx+3].Set( (float)((tx+1) * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx+1,tz).fHeight,
												(float)(tz * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												u1[3], v1[3], u2[3], v2[3]);
					VertexIdx+=4;
					TileCount++;

					//set lightmap vb...
					if(m_pRefTerrain->GetLightMap(tx, tz))
					{
						m_pRefLightMapTex[m_NumLightMapTex] = m_pRefTerrain->GetLightMap(tx, tz);
						if(!pLightMapVertices) m_pLightMapVB->Lock( 0, 0, (void**)&pLightMapVertices, 0 );
						int VBIndx = m_NumLightMapTex * 4;
						float suv = 1.0f/(float)LIGHTMAP_TEX_SIZE;
						float euv = (float)(LIGHTMAP_TEX_SIZE - 1.0f)/(float)LIGHTMAP_TEX_SIZE;

						pLightMapVertices[VBIndx].Set((float)(tx * TILE_SIZE), m_pRefTerrain->GetMapData(tx,tz).fHeight,
														(float)(tz * TILE_SIZE), 0.0f, 1.0f, 0.0f,
														suv, euv);
						pLightMapVertices[VBIndx+1].Set((float)(tx * TILE_SIZE), m_pRefTerrain->GetMapData(tx,tz+1).fHeight,
														(float)((tz+1) * TILE_SIZE), 0.0f, 1.0f, 0.0f,
														suv, suv);
						pLightMapVertices[VBIndx+2].Set((float)((tx+1) * TILE_SIZE), m_pRefTerrain->GetMapData(tx+1,tz+1).fHeight,
														(float)((tz+1) * TILE_SIZE), 0.0f, 1.0f, 0.0f,
														euv, suv);
						pLightMapVertices[VBIndx+3].Set((float)((tx+1) * TILE_SIZE), m_pRefTerrain->GetMapData(tx+1,tz).fHeight,
														(float)(tz * TILE_SIZE), 0.0f, 1.0f, 0.0f,
														euv, euv);
						
						m_NumLightMapTex++;			
					}
				}
				else
				{
					pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx,tz+1).fHeight,
												(float)((tz+1) * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												u1[1], v1[1], u2[1], v2[1]);
					pVertices[VertexIdx+1].Set( (float)((tx+1) * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx+1,tz+1).fHeight,
												(float)((tz+1) * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												u1[2], v1[2], u2[2], v2[2]);
					pVertices[VertexIdx+2].Set( (float)((tx+1) * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx+1,tz).fHeight,
												(float)(tz * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												u1[3], v1[3], u2[3], v2[3]);
					pVertices[VertexIdx+3].Set( (float)(tx * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx,tz).fHeight,
												(float)(tz * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												u1[0], v1[0], u2[0], v2[0]);
					VertexIdx+=4;
					TileCount++;

					//set lightmap vb...
					if(m_pRefTerrain->GetLightMap(tx, tz))
					{
						m_pRefLightMapTex[m_NumLightMapTex] = m_pRefTerrain->GetLightMap(tx, tz);
						if(!pLightMapVertices) m_pLightMapVB->Lock( 0, 0, (void**)&pLightMapVertices, 0 );
						int VBIndx = m_NumLightMapTex * 4;
						float suv = 1.0f/(float)LIGHTMAP_TEX_SIZE;
						float euv = (float)(LIGHTMAP_TEX_SIZE - 1.0f)/(float)LIGHTMAP_TEX_SIZE;

						pLightMapVertices[VBIndx].Set((float)(tx * TILE_SIZE), m_pRefTerrain->GetMapData(tx,tz+1).fHeight,
														(float)((tz+1) * TILE_SIZE), 0.0f, 1.0f, 0.0f,
														suv, suv);
						pLightMapVertices[VBIndx+1].Set((float)((tx+1) * TILE_SIZE), m_pRefTerrain->GetMapData(tx+1,tz+1).fHeight,
														(float)((tz+1) * TILE_SIZE), 0.0f, 1.0f, 0.0f,
														euv, suv);
						pLightMapVertices[VBIndx+2].Set((float)((tx+1) * TILE_SIZE), m_pRefTerrain->GetMapData(tx+1,tz).fHeight,
														(float)(tz * TILE_SIZE), 0.0f, 1.0f, 0.0f,
														euv, euv);
						pLightMapVertices[VBIndx+3].Set((float)(tx * TILE_SIZE), m_pRefTerrain->GetMapData(tx,tz).fHeight,
														(float)(tz * TILE_SIZE), 0.0f, 1.0f, 0.0f,
														suv, euv);						
						m_NumLightMapTex++;			
					}
				}
			}
		}

		if(pLightMapVertices) m_pLightMapVB->Unlock();

		m_pVB->Unlock();
		return;
	}

	float tu,tv;	//center u,v...
	if(m_iLevel==2)
	{
		m_FanInfoList.clear();
		__VertexT1* pVertices;
		m_pVB->Lock( 0, 0, (void**)&pVertices, 0 );

		float HalfUV = (float)HalfCell/(float)UNITUV;

		for(ix=0; ix<m_NumCell; ix++)
		{
			for(iz=0; iz<m_NumCell; iz++)
			{
				VertexCount = VertexIdx;
				FaceCount = 0;

				cx = ix*m_CellSize + HalfCell + m_ti_LBPoint.x;
				cz = iz*m_CellSize + HalfCell + m_ti_LBPoint.y;

				//cell�� ��� ��...
				tx = cx;
				tz = cz;

				tu = (float)(tx%UNITUV) / (float)UNITUV;
				tv = (float)(UNITUV - (tz%UNITUV)) / (float)UNITUV;
				
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
										m_pRefTerrain->GetMapData(tx,tz).fHeight,
										(float)(tz * TILE_SIZE),
										0.0f, 1.0f, 0.0f,
										UVConvert(tu),
										UVConvert(tv));
										//tu, tv);
				VertexIdx++;
				
				//LB Point..
				tx = cx - HalfCell;
				tz = cz - HalfCell;
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu - HalfUV),
											UVConvert(tv + HalfUV));
											//tu - HalfUV, tv + HalfUV);
				VertexIdx++;
				
				// LM Point...
				if( (ix==0) && (m_IsBlunt[0]==false) )
				{
					for(int i=0;i<3;i++)
					{
						tz++;
						pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
													m_pRefTerrain->GetMapData(tx,tz).fHeight,
													(float)(tz * TILE_SIZE),
													0.0f, 1.0f, 0.0f,
													UVConvert(tu - HalfUV),
													UVConvert((float)(UNITUV - (tz%UNITUV)) / (float)UNITUV));
													//(tu - HalfUV), ((float)(UNITUV - (tz%UNITUV)) / (float)UNITUV));
						VertexIdx++;
						FaceCount++;
					}
				}

				//LT.......
				tz = cz + HalfCell;
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu - HalfUV),
											UVConvert(tv - HalfUV));
											//(tu - HalfUV), (tv - HalfUV));
				VertexIdx++;
				FaceCount++;
				
				// MT...
				if( ((iz+1)==m_NumCell) && (m_IsBlunt[1]==false) )
				{
					for(int i=0;i<3;i++)
					{
						tx++;
						pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
													m_pRefTerrain->GetMapData(tx,tz).fHeight,
													(float)(tz * TILE_SIZE),
													0.0f, 1.0f, 0.0f,
													UVConvert((float)(tx%UNITUV) / (float)UNITUV),
													UVConvert(tv - HalfUV));
													//((float)(tx%UNITUV) / (float)UNITUV), (tv - HalfUV));
						VertexIdx++;
						FaceCount++;
					}
				}

				//RT.......
				tx = cx + HalfCell;
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu + HalfUV),
											UVConvert(tv - HalfUV));
											//(tu + HalfUV), (tv - HalfUV));
				VertexIdx++;
				FaceCount++;

				// RM...
				if( ((ix+1)==m_NumCell) && (m_IsBlunt[2]==false) )
				{
					for(int i=0;i<3;i++)
					{
						//tx = cx + HalfCell;
						tz--;
						pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
													m_pRefTerrain->GetMapData(tx,tz).fHeight,
													(float)(tz * TILE_SIZE),
													0.0f, 1.0f, 0.0f,
													UVConvert(tu + HalfUV),
													UVConvert((float)(UNITUV - (tz%UNITUV)) / (float)UNITUV));
													//(tu + HalfUV), ((float)(UNITUV - (tz%UNITUV)) / (float)UNITUV));
						VertexIdx++;
						FaceCount++;
					}
				}

				//RB......
				//tx = cx + HalfCell;
				tz = cz - HalfCell;
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu + HalfUV),
											UVConvert(tv + HalfUV));
											//(tu + HalfUV), (tv + HalfUV));
				VertexIdx++;
				FaceCount++;

				// MB...
				if( (iz==0) && (m_IsBlunt[3]==false) )
				{
					for(int i=0;i<3;i++)
					{
						tx--;							
						pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
													m_pRefTerrain->GetMapData(tx,tz).fHeight,
													(float)(tz * TILE_SIZE),
													0.0f, 1.0f, 0.0f,
													UVConvert((float)(tx%UNITUV) / (float)UNITUV),
													UVConvert(tv + HalfUV));
													//((float)(tx%UNITUV) / (float)UNITUV), (tv + HalfUV));
						VertexIdx++;
						FaceCount++;
					}
				}

				//LB Point..
				tx = cx - HalfCell;
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu - HalfUV),
											UVConvert(tv + HalfUV));
											//(tu - HalfUV), (tv + HalfUV));
				VertexIdx++;
				FaceCount++;
				FaceInfo.NumFace = FaceCount;
				FaceInfo.NumVertex = VertexIdx - VertexCount;
				m_FanInfoList.push_back(FaceInfo);
			}
		}
	
		m_pVB->Unlock();
		return;
	}

	if(m_iLevel>2)	// level3 �̻�..	
	{
		m_FanInfoList.clear();
		__VertexT1* pVertices;
		HRESULT hr = m_pVB->Lock( 0, 0, (void**)&pVertices, 0 );

		float HalfUV = (float)HalfCell/(float)UNITUV;

		for(ix=0; ix<m_NumCell; ix++)
		{
			for(iz=0; iz<m_NumCell; iz++)
			{
				VertexCount = VertexIdx;
				FaceCount = 0;

				cx = ix*m_CellSize + HalfCell + m_ti_LBPoint.x;
				cz = iz*m_CellSize + HalfCell + m_ti_LBPoint.y;

				//cell�� ��� ��...
				tx = cx;
				tz = cz;

				tu = (float)(tx%UNITUV) / (float)UNITUV;
				tv = (float)(UNITUV - (tz%UNITUV)) / (float)UNITUV;

				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
										m_pRefTerrain->GetMapData(tx,tz).fHeight,
										(float)(tz * TILE_SIZE),
										0.0f, 1.0f, 0.0f,
										UVConvert(tu),
										UVConvert(tv));
										//(tu), (tv));
				VertexIdx++;
				
				//LB Point..
				tx = cx - HalfCell;
				tz = cz - HalfCell;

				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu - HalfUV),
											UVConvert(tv + HalfUV));
											//(tu - HalfUV),
											//(tv + HalfUV));
				VertexIdx++;
				
				// LM Point...
				if( (ix==0) && (m_IsBlunt[0]==false) )
				{
					tz += HalfCell;
					pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx,tz).fHeight,
												(float)(tz * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												UVConvert(tu - HalfUV),
												UVConvert(tv));
												//(tu - HalfUV),
												//(tv));
					VertexIdx++;
					FaceCount++;
				}

				//LT.......
				//tx = cx - HalfCell;
				tz = cz + HalfCell;
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu - HalfUV),
											UVConvert(tv - HalfUV));
											//(tu - HalfUV), (tv - HalfUV));
				VertexIdx++;
				FaceCount++;
				
				// MT...
				if( ((iz+1)==m_NumCell) && (m_IsBlunt[1]==false) )
				{
					tx += HalfCell;
					pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx,tz).fHeight,
												(float)(tz * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												UVConvert(tu),
												UVConvert(tv - HalfUV));
												//(tu), (tv - HalfUV));
					VertexIdx++;
					FaceCount++;
				}

				//RT.......
				tx = cx + HalfCell;		
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu + HalfUV),
											UVConvert(tv - HalfUV));
											//(tu + HalfUV), (tv - HalfUV));
				VertexIdx++;
				FaceCount++;

				// RM...
				if( ((ix+1)==m_NumCell) && (m_IsBlunt[2]==false) )
				{
					tz -= HalfCell;
					pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx,tz).fHeight,
												(float)(tz * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												UVConvert(tu + HalfUV),
												UVConvert(tv));
												//(tu + HalfUV), (tv));
					VertexIdx++;
					FaceCount++;
				}

				//RB......
				//tx = cx + HalfCell;
				tz = cz - HalfCell;
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu + HalfUV),
											UVConvert(tv + HalfUV));
											//(tu + HalfUV), (tv + HalfUV));
				VertexIdx++;
				FaceCount++;

				// MB...
				if( (iz==0) && (m_IsBlunt[3]==false) )
				{
					tx-=HalfCell;
					pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
												m_pRefTerrain->GetMapData(tx,tz).fHeight,
												(float)(tz * TILE_SIZE),
												0.0f, 1.0f, 0.0f,
												UVConvert(tu),
												UVConvert(tv + HalfUV));
												//(tu), (tv + HalfUV));
					VertexIdx++;
					FaceCount++;
				}

				//LB Point..
				tx = cx - HalfCell;
				pVertices[VertexIdx].Set( (float)(tx * TILE_SIZE),
											m_pRefTerrain->GetMapData(tx,tz).fHeight,
											(float)(tz * TILE_SIZE),
											0.0f, 1.0f, 0.0f,
											UVConvert(tu - HalfUV),
											UVConvert(tv + HalfUV));
											//(tu - HalfUV), (tv + HalfUV));
				VertexIdx++;
				FaceCount++;
				FaceInfo.NumFace = FaceCount;
				FaceInfo.NumVertex = VertexIdx - VertexCount;
				m_FanInfoList.push_back(FaceInfo);
			}
		}
	
		m_pVB->Unlock();
	}
}


//
//
//
void CN3TerrainPatch::Render()
{
	if(m_bIsRender==FALSE) return;

	HRESULT hr;
	
	if(m_iLevel>1)
	{
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);	
		hr = CN3Base::s_lpD3DDev->SetTexture( 1, NULL);
		
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE);	
		hr = CN3Base::s_lpD3DDev->SetTexture( 2, NULL);

		CN3Base::s_lpD3DDev->SetStreamSource( 0, m_pVB, 0, sizeof(__VertexT1) );
		CN3Base::s_lpD3DDev->SetFVF(FVF_VNT1);

		if(m_pRefColorTex->Get()) hr = CN3Base::s_lpD3DDev->SetTexture( 0, m_pRefColorTex->Get() );
		else hr = CN3Base::s_lpD3DDev->SetTexture( 0, NULL );

		FIIt it;
		int vc = 0;
		int fc;
		for(it = m_FanInfoList.begin(); it != m_FanInfoList.end(); it++)
		{
			fc = it->NumFace;
			hr = CN3Base::s_lpD3DDev->DrawPrimitive( D3DPT_TRIANGLEFAN, vc, fc);
			vc += it->NumVertex;
#if _DEBUG
			CN3Base::s_RenderInfo.nTerrain_Polygon += it->NumVertex; // Rendering Information ����..
#endif			
		}		
		return;
	}	
	
	if(m_iLevel==1)
	{
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	
		CN3Base::s_lpD3DDev->SetStreamSource( 0, m_pVB, 0, sizeof(__VertexT2) );
		CN3Base::s_lpD3DDev->SetFVF(FVF_VNT2);
			
		int TotalTile = PATCH_TILE_SIZE*PATCH_TILE_SIZE;
		for(int i=0; i<TotalTile; i++)
		{
			hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			hr = CN3Base::s_lpD3DDev->SetTexture( 2, NULL );

			if( (m_pTileTexIndx[0][i]<0) || m_pTileTexIndx[0][i] >= m_pRefTerrain->m_NumTileTex || (m_pIsTileFull[i]==false) )	// 0: �÷���, 1:���� or 0:�κ�Ÿ�� 1:NONE...
			{
				hr = CN3Base::s_lpD3DDev->SetTexture( 0, m_pRefColorTex->Get());
				hr = CN3Base::s_lpD3DDev->SetTexture( 1, m_pRefTerrain->m_pBaseTex.Get());

				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE);
				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT);

				hr = CN3Base::s_lpD3DDev->SetTexture( 2, NULL);
				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_MODULATE);
				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLORARG1, D3DTA_CURRENT);
				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);			
			}
			else if(m_pTileTexIndx[0][i] >= 0 && m_pTileTexIndx[0][i] < m_pRefTerrain->m_NumTileTex)
			{
				if(m_pTileTexIndx[1][i]>=0 && m_pTileTexIndx[1][i] < m_pRefTerrain->m_NumTileTex)	//0: tile, 1: tile..
				{
					hr = CN3Base::s_lpD3DDev->SetTexture( 0, m_pRefTerrain->m_pTileTex[m_pTileTexIndx[0][i]].Get());
					hr = CN3Base::s_lpD3DDev->SetTexture( 1, m_pRefTerrain->m_pTileTex[m_pTileTexIndx[1][i]].Get());
					hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_ADD);
					hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT);

					if(m_pRefTerrain->m_bAvailableTile)
					{
						hr = CN3Base::s_lpD3DDev->SetTexture( 2, NULL);
						hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_MODULATE);
						hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLORARG1, D3DTA_CURRENT);
						hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
					}
				}
				else	//0: tile, 1:NONE...
				{
					hr = CN3Base::s_lpD3DDev->SetTexture( 0, m_pRefTerrain->m_pTileTex[m_pTileTexIndx[0][i]].Get());
					hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

					hr = CN3Base::s_lpD3DDev->SetTexture( 1, NULL);
					hr = CN3Base::s_lpD3DDev->SetTexture( 2, NULL);
					hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
					hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE);
				}
			}

			hr = CN3Base::s_lpD3DDev->DrawPrimitive( D3DPT_TRIANGLEFAN, (i<<2), 2);
			
			if( (!m_pRefTerrain->m_bAvailableTile) &&
				m_pTileTexIndx[0][i] >= 0 && m_pTileTexIndx[0][i] < m_pRefTerrain->m_NumTileTex &&
				m_pTileTexIndx[1][i] >= 0 && m_pTileTexIndx[1][i] < m_pRefTerrain->m_NumTileTex )
			{
				DWORD dwAlphaEnable, dwSrcBlend, dwDestBlend;
				hr = s_lpD3DDev->GetRenderState( D3DRS_ALPHABLENDENABLE, &dwAlphaEnable);
				hr = s_lpD3DDev->GetRenderState( D3DRS_SRCBLEND, &dwSrcBlend);
				hr = s_lpD3DDev->GetRenderState( D3DRS_DESTBLEND, &dwDestBlend);

				hr = CN3Base::s_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
				hr = CN3Base::s_lpD3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO);
				hr = CN3Base::s_lpD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);

				hr = CN3Base::s_lpD3DDev->SetTexture( 0, NULL);
				hr = CN3Base::s_lpD3DDev->SetTexture( 1, NULL);
				hr = CN3Base::s_lpD3DDev->SetTexture( 2, NULL);
				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
				hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE);

				hr = CN3Base::s_lpD3DDev->DrawPrimitive( D3DPT_TRIANGLEFAN, (i<<2), 2);

				hr = s_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, dwAlphaEnable);
				hr = s_lpD3DDev->SetRenderState( D3DRS_SRCBLEND, dwSrcBlend);
				hr = s_lpD3DDev->SetRenderState( D3DRS_DESTBLEND, dwDestBlend);
			}
		}

#if _DEBUG
		CN3Base::s_RenderInfo.nTerrain_Tile_Polygon += TotalTile * 2; // Rendering Information ����..
#endif

		// Render Light Map...
		s_lpD3DDev->SetStreamSource( 0, m_pLightMapVB, 0, sizeof(__VertexT1) );
		s_lpD3DDev->SetFVF(FVF_VNT1);

		DWORD dwAlphaEnable, dwSrcBlend, dwDestBlend;

		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 2, D3DTSS_COLOROP, D3DTOP_DISABLE);
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		hr = CN3Base::s_lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		
		hr = s_lpD3DDev->GetRenderState( D3DRS_ALPHABLENDENABLE, &dwAlphaEnable);
		hr = s_lpD3DDev->GetRenderState( D3DRS_SRCBLEND, &dwSrcBlend);
		hr = s_lpD3DDev->GetRenderState( D3DRS_DESTBLEND, &dwDestBlend);

		hr = s_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		hr = s_lpD3DDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		hr = s_lpD3DDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		for(int i=0;i<m_NumLightMapTex;i++)
		{
			s_lpD3DDev->SetTexture( 0, m_pRefLightMapTex[i]->Get());
			hr = s_lpD3DDev->DrawPrimitive( D3DPT_TRIANGLEFAN, (i<<2), 2);
		}
		hr = s_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE, dwAlphaEnable);
		hr = s_lpD3DDev->SetRenderState( D3DRS_SRCBLEND, dwSrcBlend);
		hr = s_lpD3DDev->SetRenderState( D3DRS_DESTBLEND, dwDestBlend);
	}
}


//
//	���ȼ��� ��ҽ�Ų Ŀ���� ���� uv�ٲ��ִ� �Լ�..
//	�ʿ���� �ƴ�..-.- �� �ؽ��� �������� ������Ʈ���� mirror���� �ȴ�..��.��
//
inline float CN3TerrainPatch::UVConvert(float uv)
{
	//return ( (uv*((float)COLORMAPTEX_SIZE - 2.0f) + 1.0f) / (float)COLORMAPTEX_SIZE);
	return uv;
}


