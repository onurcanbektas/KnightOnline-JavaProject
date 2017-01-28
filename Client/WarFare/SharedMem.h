// SharedMem.h: interface for the CSharedMemQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREDMEM_H__067280DE_BF70_478A_A48F_99A5A0388BBD__INCLUDED_)
#define AFX_SHAREDMEM_H__067280DE_BF70_478A_A48F_99A5A0388BBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WM_RECEIVEDATA WM_USER+21

#include "N3Base.h"

struct _SMQ_HEADER 
{
	BOOL	Connect;	
	WORD	Head;
	WORD	Tail;
	char	*Data;	
};

class CSharedMemQueue//:public CObject
{
private:

public:
	CSharedMemQueue();
	virtual ~CSharedMemQueue();

	void	DestroySMQ();
	int		GetData( char* pBuf );
	BOOL	PutData( char* pBuf, WORD);
	BOOL	IsPartner();

	BOOL	CreateSmq(BOOL Server);
	//���� ����Ÿ�� ���ۿ� �����Ѵ�.
	void	SetReadData();
	//���� ����Ÿ�� ���ۿ� �����´�.
	void	GetReadData(LPSTR data, int nLength);
	//�޼��縦 ���� ������ �÷��� �����Ѵ�.
	void	SetHwnd(HWND hwnd);
	void	CloseSmq();
	BOOL	IsData();

	HANDLE	m_hReadQueueThread;
	HANDLE	m_hrMMFile;
	HANDLE	m_hsMMFile;
	char*	m_lpMMFile;
	HWND	m_hwnd;			//�޼����� ������ ������ �÷���
	BYTE    abIn[1024+1];//����Ʈ���� ������ ����Ÿ
	BOOL	fConnected;
	_SMQ_HEADER		*Txd, *Rxd;
};

#endif // !defined(AFX_SHAREDMEM_H__067280DE_BF70_478A_A48F_99A5A0388BBD__INCLUDED_)
