// N3FXPartBottomBoard.cpp: implementation of the CN3FXPartBottomBoard class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "..\WarFare\GameProcedure.h"
#include "..\WarFare\N3WorldManager.h"
#include "..\WarFare\N3FXPartBottomBoardGame.h"

//#include "StdAfxBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3FXPartBottomBoardGame::CN3FXPartBottomBoardGame()
{
}

CN3FXPartBottomBoardGame::~CN3FXPartBottomBoardGame()
{
	if(m_pVB)
	{
		delete[] m_pVB;
		m_pVB = NULL;
	}

	if(m_vUnit)
	{
		delete[] m_vUnit;
		m_vUnit = NULL;
	}
}


//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

float CN3FXPartBottomBoardGame::GetGroundHeight(float x, float z)
{
	float fHeight = 0.1f;
	fHeight = CGameBase::ACT_WORLD->GetHeightWithTerrain(x, z) + 0.1f;
	return fHeight;
}
