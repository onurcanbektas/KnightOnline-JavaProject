// N3AnimControl.cpp: implementation of the CN3AnimControl class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#include "N3AnimControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3AnimControl::CN3AnimControl()
{
	m_dwType |= OBJ_ANIM_CONTROL;
}

CN3AnimControl::~CN3AnimControl()
{
}

void CN3AnimControl::Release()
{
	m_Datas.clear(); // animation Data List

	CN3BaseFileAccess::Release();
}

bool CN3AnimControl::Load(HANDLE hFile)
{
	if(m_Datas.size() > 0) this->Release();

	DWORD dwRWC = 0;
	int nCount = 0;
	ReadFile(hFile, &nCount, 4, &dwRWC, NULL);

	m_Datas.clear(); // animation Data List
	for(int i = 0; i < nCount; i++)
	{
		__AnimData Data;
		Data.Load(hFile);
		m_Datas.push_back(Data);
	}

	return true;
}

#ifdef _N3TOOL
bool CN3AnimControl::Save(HANDLE hFile)
{
	DWORD dwRWC = 0;

	int nL = 0;
	int iSize = m_Datas.size();
	WriteFile(hFile, &iSize, 4, &dwRWC, NULL);

	for(int i = 0; i < iSize; i++)
	{
		m_Datas[i].Save(hFile);
	}

	return true;
}
#endif // endof #ifdef _N3TOOL

#ifdef _N3TOOL
__AnimData* CN3AnimControl::Add()
{
	__AnimData Data;
	Data.szName = "No Name";
	m_Datas.push_back(Data);

	return &(m_Datas[m_Datas.size() - 1]);
}
#endif // endof #ifdef _N3TOOL

#ifdef _N3TOOL
__AnimData* CN3AnimControl::Insert(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_Datas.size()) return NULL;
	
	it_Ani it = m_Datas.begin();
	for(int i = 0; i < nIndex; i++, it++);

	__AnimData Data;
	Data.szName = "No Name";
	it = m_Datas.insert(it, Data);

	return &(*it);
}
#endif // endof #ifdef _N3TOOL

#ifdef _N3TOOL
void CN3AnimControl::Swap(int nAni1, int nAni2)
{
	if(nAni1 == nAni2) return;
	if(nAni1 < 0 || nAni1 >= m_Datas.size()) return;
	if(nAni2 < 0 || nAni2 >= m_Datas.size()) return;

	__AnimData Tmp = m_Datas[nAni2];
	m_Datas[nAni2] = m_Datas[nAni1];
	m_Datas[nAni1] = Tmp;
}
#endif // endof #ifdef _N3TOOL

#ifdef _N3TOOL
void CN3AnimControl::Delete(int nIndex)
{
	if(nIndex < 0 || nIndex >= m_Datas.size()) return;

	it_Ani it = m_Datas.begin();
	for(int i = 0; i < nIndex; i++, it++);

	m_Datas.erase(it);
}
#endif // endof #ifdef _N3TOOL
