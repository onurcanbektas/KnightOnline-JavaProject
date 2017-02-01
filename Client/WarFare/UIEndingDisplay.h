// UIEndingDisplay.h: interface for the CUIEndingDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIENDINGDISPLAY_H__6AF00D52_3B84_4E80_A27F_279D266EB238__INCLUDED_)
#define AFX_UIENDINGDISPLAY_H__6AF00D52_3B84_4E80_A27F_279D266EB238__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"

class CUIEndingDisplay      : public CN3UIBase  
{
public:
	void	Render();
	void	Release();

	CUIEndingDisplay();
	virtual ~CUIEndingDisplay();
};

#endif // !defined(AFX_UIENDINGDISPLAY_H__6AF00D52_3B84_4E80_A27F_279D266EB238__INCLUDED_)
