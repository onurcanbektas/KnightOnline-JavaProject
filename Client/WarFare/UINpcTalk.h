// UINpcTalk.h: interface for the CUINpcTalk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UINPCTALK_H__F44BD681_A8A1_43E3_B305_7B1B90EACA5B__INCLUDED_)
#define AFX_UINPCTALK_H__F44BD681_A8A1_43E3_B305_7B1B90EACA5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"

#include "N3UIBase.h"
#include "N3UIButton.h"
#include "N3UIString.h"

//////////////////////////////////////////////////////////////////////


class CUINpcTalk     : public CN3UIBase  
{
	int					m_iIDTarget;

public:
	CUINpcTalk();
	virtual ~CUINpcTalk();

	void Release();

	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);

	void Open(int iIDTarget = -1);
	void Close();
};

#endif // !defined(AFX_UINPCTALK_H__F44BD681_A8A1_43E3_B305_7B1B90EACA5B__INCLUDED_)
