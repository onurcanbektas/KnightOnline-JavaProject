// GameProcOption.cpp: implementation of the CGameProcOption class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "GameProcOption.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcOption::CGameProcOption()
{
}

CGameProcOption::~CGameProcOption()
{
}

void CGameProcOption::Init()
{
	CGameProcedure::Init();
}

void CGameProcOption::Release()
{
	CGameProcedure::Release();
}

void CGameProcOption::Render()
{
	CGameProcedure::Render(); // UI �� �׹��� �⺻���� �͵� ������..
}

void CGameProcOption::Tick()
{
	CGameProcedure::Tick();	// Ű, ���콺 �Է� ���..
}
