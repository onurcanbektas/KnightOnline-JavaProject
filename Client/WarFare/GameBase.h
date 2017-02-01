// N3GameBase.h: interface for the CN3GameBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3GAMEBASE_H__5F0720A7_AFF6_4AB5_B469_701AA2935158__INCLUDED_)
#define AFX_N3GAMEBASE_H__5F0720A7_AFF6_4AB5_B469_701AA2935158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"
#include "N3TableBase.h"
#include "GameDef.h"

class CGameBase : public CN3Base
{
#define ACT_WORLD s_pWorldMgr->GetActiveWorld()

public:
	static bool s_bRunning;

	static CN3TableBase<struct __TABLE_ZONE>*			s_pTbl_Zones;			// Zone ������ ���� ���̺�..
	static CN3TableBase<struct __TABLE_UI_RESRC>*		s_pTbl_UI;				// UI FileName Table
	static CN3TableBase<struct __TABLE_ITEM_BASIC>*		s_pTbl_Items_Basic;		// �� ������(�� �ڽŰ� ���� �ٸ� ���) Ŭ������ ���� ������ ���ҽ� ���̺�
	static CN3TableBase<struct __TABLE_ITEM_EXT>*		s_pTbl_Items_Exts[MAX_ITEM_EXTENSION];		// �� ������(�� �ڽŰ� ���� �ٸ� ���) Ŭ������ ���� ������ ���ҽ� ���̺�
	static CN3TableBase<struct __TABLE_PLAYER_LOOKS>*	s_pTbl_UPC_Looks;		// �� ������(�� �ڽŰ� ���� �ٸ� ���) Ŭ������ �⺻ ��Ų
	static CN3TableBase<struct __TABLE_PLAYER_LOOKS>*	s_pTbl_NPC_Looks;		// NPC, Mob �⺻ ��� - 6���� ĳ���� ��Ʈ(), 2���� �÷���
	static CN3TableBase<struct __TABLE_UPC_SKILL>*		s_pTbl_Skill;			// Skill ������ ���� Table
	static CN3TableBase<struct __TABLE_EXCHANGE_QUEST>*	s_pTbl_Exchange_Quest;	// ��ȯ ����Ʈ�� ���� ���̺�..
	static CN3TableBase<struct __TABLE_FX>*				s_pTbl_FXSource;		// FX�ҽ� ���� ���̺�..
	static CN3TableBase<struct __TABLE_QUEST_MENU>*		s_pTbl_QuestMenu;		// ����Ʈ ���� �޴�
	static CN3TableBase<struct __TABLE_QUEST_TALK>*		s_pTbl_QuestTalk;		// ����Ʈ ����

	static class CN3WorldManager*	s_pWorldMgr;								// ���� �Ŵ��� Ŭ����..
	static class CPlayerOtherMgr*		s_pOPMgr;									// Other Player Manager - �ٸ� ���� ���� Ŭ����..
	static class CPlayerMySelf*			s_pPlayer;			// ���� Ŭ����..
	
protected:
	static void StaticMemberInit(); // ���̺� �� �⺻ ����, ������Ʈ, ĳ���͵� �ʱ�ȭ..
	static void StaticMemberRelease(); // ���̺� �� �⺻ ����, ������Ʈ, ĳ���͵� �ʱ�ȭ..

public:
	static bool	GetTextByAttrib(e_ItemAttrib eAttrib, std::string& szAttrib);
	static bool GetTextByClass(e_Class eClass, std::string& szText);
	static bool GetTextByItemClass(e_ItemClass eItemClass, std::string& szText);
	static bool GetTextByKnightsDuty(e_KnightsDuty eDuty, std::string& szText);
	static bool GetTextByNation(const NationEnum & eNation, std::string& szText);
	static bool GetTextByRace(e_Race eRace, std::string& szText);
	static D3DCOLOR				GetIDColorByLevelDifference(int iLevelDiff); // ���� ���̿� ���� ID �� �����ش�.
	static e_Class_Represent	GetRepresentClass(e_Class eClass); // ���������� �־��ָ� ��ǥ�Ǵ� ������ �����ش�.
	static e_ItemType MakeResrcFileNameForUPC(	__TABLE_ITEM_BASIC* pItem,		// ������ ������...
												std::string* szResrcFN,			// Resource FileName
												std::string* szIconFN,			// Icon FileName
												e_PartPosition& ePartPosition,	// Part �ϰ�� Index
												e_PlugPosition& ePlugPosition,  // Plug �ϰ�� Index
												e_Race eRace = RACE_UNKNOWN);

	class CPlayerBase*	CharacterGetByID(int iID, bool bFromAlive);
	bool				IsValidCharacter(CPlayerBase* pCharacter);
	
	CGameBase();
	virtual ~CGameBase();
};

#endif // !defined(AFX_N3GAMEBASE_H__5F0720A7_AFF6_4AB5_B469_701AA2935158__INCLUDED_)
