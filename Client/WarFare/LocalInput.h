#ifndef _LocalInput_H_
#define _LocalInput_H_

#include <DInput.h>

#include "My_3DStruct.h"

const int DK_NONE = 0;
const int DK_RELEASE = 1;
const int DK_PRESS = 2;
const int DK_REPEAT = 4;
const int NUMDIKEYS = SDL_NUM_SCANCODES;//282;

// ���콺 �÷��� - �Ѱ� �̻��� �÷��װ� OR �������� ���յǾ� �ִ�..
const int MOUSE_LBCLICK		= 0x1;
const int MOUSE_LBCLICKED	= 0x2;
const int MOUSE_LBDOWN		= 0x4;
const int MOUSE_MBCLICK		= 0x8;
const int MOUSE_MBCLICKED	= 0x10;
const int MOUSE_MBDOWN		= 0x20;
const int MOUSE_RBCLICK		= 0x40;
const int MOUSE_RBCLICKED	= 0x80;
const int MOUSE_RBDOWN		= 0x100;
const int MOUSE_LBDBLCLK	= 0x200;
const int MOUSE_MBDBLCLK	= 0x400;
const int MOUSE_RBDBLCLK	= 0x800;

//////////////////////////////////////////////////////////////////////////////////
// CLocalInput is a class wrapper for DirectInput and contains functions to receive 
// data from the mouse, keyboard
//////////////////////////////////////////////////////////////////////////////////
class CLocalInput
{
private:
	//void AcquireMouse();
	//void UnacquireMouse();
	//void AcquireKeyboard();
	//void UnacquireKeyboard();

protected:
	//LPDIRECTINPUT8			m_lpDI;
	//LPDIRECTINPUTDEVICE8	m_lpDIDKeyboard;
//	LPDIRECTINPUTDEVICE8	m_lpDIDMouse;

	//SDL_Window* m_hWnd;

//	BOOL m_bMouse;
//	BOOL m_bKeyboard;

	int m_nMouseFlag, m_nMouseFlagOld; // ���콺 ��ư ���� �÷���
	DWORD m_dwTickLBDown; // ���콺 ���� ��ư ���� Ŭ�� ������
	DWORD m_dwTickRBDown; // ���콺 ������ ��ư ���� Ŭ�� ������

	POINT	m_ptCurMouse; // ���� ���콺 ������
	POINT	m_ptOldMouse; // ���� ���콺 ������

	RECT	m_rcLBDrag; // �巡�� ����
	RECT	m_rcMBDrag; // �巡�� ����
	RECT	m_rcRBDrag; // �巡�� ����

	RECT m_rcMLimit; // ���콺 ������ ���� ����
	BYTE m_byCurKeys[NUMDIKEYS]; // ���� Ű ����
	BYTE m_byOldKeys[NUMDIKEYS]; // ���� Ű ����
	BOOL m_bKeyPresses[NUMDIKEYS]; // Ű�� ���� ��������
	BOOL m_bKeyPresseds[NUMDIKEYS]; // Ű�� ������ ���� ��������
	BOOL m_bNoKeyDown; // �ƹ� Ű�Էµ� ������

	DWORD m_dwTickKeyPress[NUMDIKEYS];
	
public:
	void KeyboardClearInput(int iIndex = -1) // Ű���� �Է��� ��ȿȭ ��Ų��.. �⺻���� ���� ��ȿȭ�̴�..
	{
		if(-1 == iIndex)
		{
			memset(m_byOldKeys, 0, sizeof(m_byOldKeys));
			memset(m_byCurKeys, 0, sizeof(m_byCurKeys));
			memset(m_bKeyPresses, 0, sizeof(m_bKeyPresses));
			memset(m_bKeyPresseds, 0, sizeof(m_bKeyPresseds));
		}
		else if(iIndex >= 0 && iIndex < NUMDIKEYS) // Ư���� Ű�� ��ȿȭ..
		{
			m_byCurKeys[iIndex] = m_byOldKeys[iIndex] = m_bKeyPresses[iIndex] = m_bKeyPresseds[iIndex] = 0;
		}
	}
	BOOL IsNoKeyDown() { return m_bNoKeyDown; }
	BOOL IsKeyDown(int iIndex) { if(iIndex < 0 || iIndex >= NUMDIKEYS) return FALSE; return m_byCurKeys[iIndex]; } // Ű���尡 �����ִ���... "DInput.h" �� ���� �Ǿ� �ִ� DIK_???? ��ĵ�ڵ带 ����..
	BOOL IsKeyPress(int iIndex) { if(iIndex < 0 || iIndex >= NUMDIKEYS) return FALSE; return m_bKeyPresses[iIndex]; } // Ű���带 ������ ����... "DInput.h" �� ���� �Ǿ� �ִ� DIK_???? ��ĵ�ڵ带 ����..
	BOOL IsKeyPressed(int iIndex) { if(iIndex < 0 || iIndex >= NUMDIKEYS) return FALSE; return m_bKeyPresseds[iIndex]; } // Ű���带 �������� ���� ����... "DInput.h" �� ���� �Ǿ� �ִ� DIK_???? ��ĵ�ڵ带 ����..
	
	BOOL Init(SDL_Window* pWindow, BOOL bActivateKeyboard= TRUE, BOOL bActivateMouse = TRUE, BOOL ExclusiveMouseAccess = TRUE );

	void Tick(void);
	void KeyboardFlushData();
	//void RunMouseControlPanel(HWND hWnd);
	void MouseSetLimits(int x1, int y1, int x2, int y2);
	//void SetActiveDevices(BOOL bKeyboard, BOOL bMouse);
	void MouseSetPos(int x, int y);

	BOOL KeyboardGetKeyState(int nDIKey); // �ֱ� ������ Ű �˻�..

	const POINT MouseGetPos() { return m_ptCurMouse; }
	const POINT MouseGetPosOld() { return m_ptOldMouse; }
	
	RECT MouseGetLBDragRect() { return m_rcLBDrag; }
	RECT MouseGetMBDragRect() { return m_rcMBDrag; }
	RECT MouseGetRBDragRect() { return m_rcRBDrag; }

	int MouseGetFlag() { return m_nMouseFlag; } // Mouse Flag �� or �������� ���յǾ� �ִ�.
	int MouseGetFlagOld() { return m_nMouseFlagOld; }
	void MouseRemoveFlag(int nFlag = -1) { if(-1 == nFlag) m_nMouseFlag = m_nMouseFlagOld = 0; else m_nMouseFlag &= (~nFlag); } // Ư���� Mouse Flag ����

	CLocalInput(void);
	~CLocalInput(void);
};

#endif // end of _LocalInput_H_
