// N3FXPartBillBoard.cpp: implementation of the CN3FXPartBillBoard class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "..\WarFare\GameProcedure.h"
#include "..\WarFare\N3WorldManager.h"
#include "..\WarFare\N3FXPartBillBoardGame.h"
//#include "StdAfxBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//
//
//

float CN3FXPartBillBoardGame::GetGroundHeight(float x, float z)
{
	float fHeight = 0.1f;
	fHeight = CGameBase::ACT_WORLD->GetHeightWithTerrain(x, z) + 0.1f;
	return fHeight;
}
