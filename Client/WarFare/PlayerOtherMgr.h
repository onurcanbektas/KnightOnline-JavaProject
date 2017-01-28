// PlayerOtherMgr.h: interface for the CPlayerOtherMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PlayerOtherMgr_H__B32C59B8_6C08_494E_B9DE_338B3CD026C5__INCLUDED_)
#define AFX_PlayerOtherMgr_H__B32C59B8_6C08_494E_B9DE_338B3CD026C5__INCLUDED_

#include <map>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlayerOther.h"
#include "GameBase.h"
#include "GameProcedure.h"

//typedef std::list<CPlayerOther*>::iterator it_UPC;
//typedef std::list<CPlayerNPC*>::iterator it_NPC;
//typedef std::list<CPlayerBase*>::iterator it_BPC;

typedef std::map<int, CPlayerOther*>::iterator it_UPC;
typedef std::map<int, CPlayerNPC*>::iterator it_NPC;
typedef std::map<int, CPlayerBase*>::iterator it_BPC;

typedef std::map<int, CPlayerOther*>::value_type val_UPC;
typedef std::map<int, CPlayerNPC*>::value_type val_NPC;
typedef std::map<int, CPlayerBase*>::value_type val_BPC;

class CPlayerOtherMgr : public CGameBase
{
public:
//	std::list<CPlayerNPC*>		m_NPCs;		// NPC
//	std::list<CPlayerOther*>	m_UPCs;		// User Player Character
//	std::list<CPlayerNPC*>		m_Corpses;	// ������.. �״� ���ϸ��̼� �� �ð��� ������ �������� �Ѵ�..
	std::map<int, CPlayerNPC*>		m_NPCs;		// NPC
	std::map<int, CPlayerOther*>	m_UPCs;		// User Player Character
	std::map<int, CPlayerNPC*>		m_Corpses;	// ������.. �״� ���ϸ��̼� �� �ð��� ������ �������� �Ѵ�..
	int								m_iChrCountToRender; // �������Ǵ� ĳ�� ī��Ʈ

public:
	bool				IsValidCharacter(CPlayerBase* pCharacter);
	void				Tick(const __Vector3& vPosPlayer);
	void				Render(float fSunAngle);

	void				NPCAdd(CPlayerNPC* pNPC);
	bool				NPCDelete(int iID);								// ���� ID �� ��ġ�ϴ� NPC�� ����Ʈ���� ����.. �� ���ҽ� ���� 
	void				UPCAdd(CPlayerOther* pPlayer);
	bool				UPCDelete(int iID);								// ���� ID �� ��ġ�ϴ� NPC�� ����Ʈ���� ����.. �� ���ҽ� ���� 

//	CPlayerOther*		UPCGetByName(const char* szID);					// User Player Character �� NPC �� �����ؼ� �����͸� �����´�.
	CPlayerOther*		UPCGetByID(int iID, bool bFromAliveOnly);		// User Player Character �� NPC �� �����ؼ� �����͸� �����´�.
//	CPlayerNPC*			NPCGetByName(const char* szID);					// User Player Character �� NPC �� �����ؼ� �����͸� �����´�.
	CPlayerNPC*			NPCGetByID(int iID, bool bFromAliveOnly);		// User Player Character �� NPC �� �����ؼ� �����͸� �����´�.
	CPlayerNPC*			NPCGetByPos(const __Vector3& vPos);
	CPlayerNPC*			CharacterGetByID(int iID, bool bFromAliveOnly); // User, NPC �� ������ �����´�..
	CPlayerNPC*			CharacterGetByNearstEnemy(e_Nation eNation, const __Vector3& vPosPlayer); // ���� ����� �� ��������..
	bool				CharacterDelete(int iID); // User, NPC �� ������ �����..

	CPlayerBase*		CorpseGetByID(int iID);							// ��ü�鿡�� Player Character �� NPC �� �����ؼ� �����͸� �����´�.
	void				CorpseRemove(CPlayerNPC* pCorpse, bool bRemoveImmediately = false);
	void				CorpseAdd(CPlayerNPC* pNPC);
	void				CorpseAdd(int iID);
	CPlayerNPC*			CorpseGetNearstNPC(bool bMustHaveItem, e_Nation eNation, const __Vector3& vPosPlayer); // ���� ����� �� ��ü ��������..
	void				MoveToCorpsesForcely(CPlayerNPC* pNPC, bool bErase);			// ���̵� ��ġ�ų� �ϸ� ������ ��ü�� �����..


	//.. Picking�� PlayerOther ���..
	CPlayerNPC*			Pick(int ixScreen, int iyScreen, int& iIDResult, __Vector3* pvPick = NULL);
	CPlayerNPC*			PickNPC(int ixScreen, int iyScreen, int& iIDResult, __Vector3* pvPick = NULL);
	CPlayerOther*		PickUPC(int ixScreen, int iyScreen, int& iIDResult, __Vector3* pvPick = NULL);
	CPlayerNPC*			PickPrecisely(int ixScreen, int iyScreen, int& iIDResult, __Vector3* pvPick = NULL);
	CPlayerNPC*			PickNPCPrecisely(int ixScreen, int iyScreen, int& iIDResult, __Vector3* pvPick = NULL);
	CPlayerOther*		PickUPCPrecisely(int ixScreen, int iyScreen, int& iIDResult, __Vector3* pvPick = NULL);
	CPlayerNPC*			PickAllPrecisely(int ixScreen, int iyScreen, int &iIDResult, __Vector3* pvPick);
	CPlayerNPC*			PickCorpse(int ixScreen, int iyScreen, int& iIDResult); // ��ü�� Ŭ��..
	static int			SortByCameraDistance(const void* pArg1, const void* pArg2);

	void				ReleaseUPCs();
	void				ReleaseNPCs();
	void				ReleaseCorpses();
	void				Release();

	CPlayerOtherMgr();
	virtual ~CPlayerOtherMgr();
};

inline CPlayerOther* CPlayerOtherMgr::UPCGetByID(int iID, bool bFromAliveOnly)
{
	it_UPC it = m_UPCs.find(iID);
	if(it != m_UPCs.end())
	{
		CPlayerOther* pUPC = it->second;
		if(bFromAliveOnly)
		{
			if(PSA_DEATH != pUPC->m_eState) return pUPC;
			else return NULL;
		}
		else return pUPC;
	}
	else return NULL;
}

inline CPlayerNPC* CPlayerOtherMgr::NPCGetByID(int iID, bool bFromAliveOnly)
{
	it_NPC it = m_NPCs.find(iID);
	if(it != m_NPCs.end())
	{
		CPlayerNPC* pNPC = it->second;
		if(bFromAliveOnly)
		{
			if(PSA_DEATH != pNPC->m_eState) return pNPC;
			else return NULL;
		}
		else return pNPC;
	}
	else return NULL;
}

inline CPlayerBase* CPlayerOtherMgr::CorpseGetByID(int iID)
{
	it_NPC it = m_Corpses.find(iID);
	if(it != m_Corpses.end()) return it->second;
	else return NULL;
}

inline CPlayerNPC* CPlayerOtherMgr::CharacterGetByID(int iID, bool bFromAliveOnly)
{
	if(iID < 0) return 0;

	CPlayerNPC* pBPC = this->NPCGetByID(iID, bFromAliveOnly);		// ����  NPC �߿��� ã�ƺ���..
	if(NULL == pBPC) pBPC = this->UPCGetByID(iID, bFromAliveOnly);			// ���� User �鵵 ã�ƺ���..

	return pBPC;
}

inline void CPlayerOtherMgr::UPCAdd(CPlayerOther* pUPC)
{
	it_UPC it = m_UPCs.find(pUPC->IDNumber());
	if(it == m_UPCs.end()) // �ߺ��Ȱ� ������..
	{
		m_UPCs.insert(val_UPC(pUPC->IDNumber(), pUPC));
	}
	else // �ߺ��Ǿ�����..
	{
		T_Delete(it->second);
		it->second = pUPC;
	}
}

inline bool CPlayerOtherMgr::UPCDelete(int iID)
{
	it_UPC it = m_UPCs.find(iID);
	if(it == m_UPCs.end()) return false;

	T_Delete(it->second);
	m_UPCs.erase(it);
	return true;
}

inline void CPlayerOtherMgr::NPCAdd(CPlayerNPC* pNPC)
{
	it_NPC it = m_NPCs.find(pNPC->IDNumber());
	if(it == m_NPCs.end()) // �ߺ��Ȱ� ������..
	{
		m_NPCs.insert(val_NPC(pNPC->IDNumber(), pNPC));
	}
	else // �ߺ��Ǿ�����..
	{
		T_Delete(it->second);
		it->second = pNPC;
	}
}

inline bool CPlayerOtherMgr::NPCDelete(int iID)
{
	it_NPC it = m_NPCs.find(iID);
	if(it == m_NPCs.end()) return false;

	T_Delete(it->second);
	m_NPCs.erase(it);
	return true;
}

#endif // !defined(AFX_PlayerOtherMgr_H__B32C59B8_6C08_494E_B9DE_338B3CD026C5__INCLUDED_)
