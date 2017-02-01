// BirdMng.h: interface for the CBirdMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIRDMNG_H__5307F307_1E9C_469B_BD46_52A408740383__INCLUDED_)
#define AFX_BIRDMNG_H__5307F307_1E9C_469B_BD46_52A408740383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"


typedef struct __TABLE_BIRD
{
	DWORD dwID; // zone ID
	char* pszTerrainFN; // Terrain FileName - Terrain, Tile Map, ColorMap....
	char* pszColorMapFN;
	char* pszObjectPostDataFN; // Object ��ġ ���� - �̾ȿ� �浹 üũ �����͵� ��� �ִ�.
} TABLE_BIRD;


class CBird;

class CBirdMng : public CN3Base  
{
public:
	CBirdMng();
	virtual ~CBirdMng();

// Attributes
protected:
	CBird*			m_pBird;
	int				m_iBirdCount;

// Operations
public:
	virtual void	Release();
	void			Tick();
	void			Render();
	void			LoadFromFile(const std::string& szFN);
protected:

};

#endif // !defined(AFX_BIRDMNG_H__5307F307_1E9C_469B_BD46_52A408740383__INCLUDED_)
