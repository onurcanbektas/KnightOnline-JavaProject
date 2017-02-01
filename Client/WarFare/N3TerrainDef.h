////////////////////////////////////////////////////////////////////////////////////////
//
//	N3TerrainDef.h
//	- �̰����� Terrain�� ���õ� �ڷ�������, �������...
//	- ���ӿ� ���� ���� (����Ʈ�� �ƴ�...^^)
//	- 2001. 10. 22.
//
//	By Donghoon..
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __N3TERRAINDEF_H__
#define __N3TERRAINDEF_H__

// Constant Definitions..
const int PATCH_TILE_SIZE = 8;		//	��ġ �ϳ��� Ÿ���� 8x8�� ����.
const int TILE_SIZE = 4;			//	Ÿ���ϳ��� ���� 4m
const int MAX_LOD_LEVEL = 10;		//	LOD������ ���� ���� �ܰ� ���� Detail...
const int MIN_LOD_LEVEL = 0;		//	LOD������ ���� ���� �ܰ� ���� Rough...
const int MAX_PATCH_LEVEL = 3;		//	2 ^ 3 = 8.
const int PATCH_PIXEL_SIZE = 32;	//	��ġ�ϳ��� ���� �ȼ���..
const int UNITUV = 32;				//	�÷��� �ؽ��� ���忡 ���� Ÿ�ϼ�..
const int LIGHTMAP_TEX_SIZE = 16;	//	Ÿ�� �ϳ��� ���� ����Ʈ�� �ؽ��� ũ��.
const int TILE_PIXEL_SIZE = 128;	//	Ÿ���ϳ��� ���� �ȼ� ������.
const int COLORMAPTEX_SIZE = 128;	//	�÷��� �ؽ����� �ȼ�ũ��..128x128
const int DISTANCE_TABLE_SIZE = 64; //	�������� �Ÿ� ���̺� ũ��(64 x 64)..
const int MAX_GRASS	= 8;
const int MAX_TERRAIN_SOUND_EFFECT = 4;

enum { DIR_LT = 0, DIR_CT = 1, DIR_RT = 2,
		DIR_LM = 3, DIR_CM = 4, DIR_RM = 5,
		DIR_LB = 6, DIR_CB = 7, DIR_RB = 8,
		DIR_WARP = 9 }; 

// Structure Definitions..
typedef struct __MapData			// �ʵ�����...
{
	float	fHeight;				//Ÿ���ؽ��İ� �������� �ƴ���..
	DWORD	bIsTileFull	: 1;		//������ ���̰�..
	DWORD	Tex1Dir		: 5;		//ù��° Ÿ�� �ؽ��� ��� ����.
	DWORD	Tex2Dir		: 5;		//�ι�° Ÿ�� �ؽ��� ��� ����.
	DWORD	Tex1Idx		: 10;		//ù��° Ÿ�� �ؽ����� �ε���.
	DWORD	Tex2Idx		: 10;		//�ι�° Ÿ�� �ؽ����� �ε���.
	
	__MapData()
	{
		bIsTileFull = true;
		fHeight = FLT_MIN;
		Tex1Idx = 1023;
		Tex1Dir = 0;
		Tex2Idx = 1023;
		Tex2Dir = 0;
	}
} MAPDATA, *LPMAPDATA;

typedef struct __FanInfo
{
	int		NumFace;
	int		NumVertex;
} FANINFO, *LPFANINFO;

typedef struct __TerrainTileTexInfo
{
	class CN3Texture* pTex;
	float u;
	float v;
	
	__TerrainTileTexInfo()
	{
		pTex = NULL;
		u = v = 0.0f;
	}
} TERRAINTILETEXINFO, *LPTERRAINTILETEXINFO;

typedef struct __TerrainBGMInfo
{
	std::string szBGM[MAX_TERRAIN_SOUND_EFFECT];
	float		fBGMRegenTime[MAX_TERRAIN_SOUND_EFFECT];
	std::string szBGE[MAX_TERRAIN_SOUND_EFFECT];
	float		fBGERegenTime[MAX_TERRAIN_SOUND_EFFECT];

	__TerrainBGMInfo()
	{
		for(int i=0;i<MAX_TERRAIN_SOUND_EFFECT;i++)
		{
			szBGM[i] = "";
			fBGMRegenTime[i] = 0.0f;
			szBGE[i] = "";
			fBGERegenTime[i] = 0.0f;
		}
	}
} TERRAINBGMINFO, *LPTERRAINBGMINFO;

#include <list>
typedef std::list<FANINFO> FanInfoList;
typedef FanInfoList::iterator FIIt;
//typedef CellInfoList::value_type DTValue;

#include <map>
typedef std::map<DWORD,class CN3Texture*> stlMap_N3Tex;
typedef stlMap_N3Tex::iterator stlMap_N3TexIt;
typedef stlMap_N3Tex::value_type stlMap_N3TexValue;

#endif	//end of #ifndef __LYTERRAINDEF_H__