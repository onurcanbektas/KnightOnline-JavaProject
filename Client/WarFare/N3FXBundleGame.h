// N3FXBundleGame.h: interface for the CN3FXBundleGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3FXBUNDLEGAME_H__1B6BEFC3_79A4_4FFE_A48B_475D1F07E26A__INCLUDED_)
#define AFX_N3FXBUNDLEGAME_H__1B6BEFC3_79A4_4FFE_A48B_475D1F07E26A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3FXBundle.h"

class CN3FXBundleGame : public CN3FXBundle
{
public:
	int					m_iID;			//�׿��� ���� �Ƶ�..
	int					m_iIdx;
	bool				m_bRegion;

	//������ ��� ����...���ΰ �̿��Դ�..
	float				m_fDistance;
	float				m_fHeight;
	
public:
	void	SetPreBundlePos(int iSourceID, int iJoint);
	void	Trigger(int iSourceID=0, int iTargetID = 0, int iTargetJoint = 0, int iSndID = -1);
	void	Trigger(int iSourceID, __Vector3 TargetPos, int iSndID = -1);
	bool	Tick();
	
	void	Duplicate(CN3FXBundleGame* pDestBundle);
	bool	Load(HANDLE hFile);

	CN3FXBundleGame();
	virtual ~CN3FXBundleGame();

};

#endif // !defined(AFX_N3FXBUNDLEGAME_H__1B6BEFC3_79A4_4FFE_A48B_475D1F07E26A__INCLUDED_)
