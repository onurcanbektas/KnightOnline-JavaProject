// N3Light.cpp: implementation of the CN3Light class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3Light.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Light::CN3Light()
{
	m_dwType |= OBJ_LIGHT;
	memset(&m_Data, 0, sizeof(m_Data));
}

CN3Light::~CN3Light()
{
}

void CN3Light::Release()
{
	memset(&m_Data, 0, sizeof(m_Data));
	CN3Transform::Release();
}

bool CN3Light::Load(HANDLE hFile)
{
	CN3Transform::Load(hFile);

	DWORD dwRWC;
	ReadFile(hFile, &m_Data, sizeof(m_Data), &dwRWC, NULL); // ����Ʈ ����.

	__ASSERT(m_Data.nNumber >= 0 && m_Data.nNumber < 8, "Light Loading Warning - Light ��ȣ�� ������ ������ϴ�.");
	
	return true;
}

#ifdef _N3TOOL
bool CN3Light::Save(HANDLE hFile)
{
	CN3Transform::Save(hFile);

	DWORD dwRWC;
	WriteFile(hFile, &m_Data, sizeof(m_Data), &dwRWC, NULL); // ����Ʈ ����.

	return true;
}
#endif // end of _N3TOOL

void CN3Light::Tick(float fFrm)
{
	CN3Transform::Tick(fFrm);

	m_Data.Position = m_vPos;
}

void CN3Light::Apply()
{
	__ASSERT(m_Data.nNumber >= 0 && m_Data.nNumber < 8, "Invalid Light Number");
	s_lpD3DDev->LightEnable(m_Data.nNumber, m_Data.bOn);
	if(m_Data.bOn)
	{
		if(D3DLIGHT_POINT == m_Data.Type || D3DLIGHT_DIRECTIONAL == m_Data.Type || D3DLIGHT_SPOT == m_Data.Type)
			s_lpD3DDev->SetLight(m_Data.nNumber, &m_Data);
	}
}
