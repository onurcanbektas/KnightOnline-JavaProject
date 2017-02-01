/*
*/

#include "N3Eng.h"
#include "N3Light.h"
#include "LogWriter.h"

//-----------------------------------------------------------------------------
CN3Eng::CN3Eng(void)
{
	m_lpDD          = NULL;
	m_lpD3D         = NULL;
	s_lpD3DDev      = NULL;
	m_nModeActive   = -1;
	m_nAdapterCount =  1;

	memset(&m_DeviceInfo, 0x00, sizeof(__D3DDEV_INFO));

	delete [] m_DeviceInfo.pModes;
	memset(&m_DeviceInfo, 0x00, sizeof(m_DeviceInfo));

#ifdef _N3GAME
	CLogWriter::Open("Log.txt");
#endif

	m_lpD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(m_lpD3D == NULL)
	{
#ifdef _N3GAME
		CLogWriter::Write("Direct3D9 is not installed or lower version");
#endif
		Release();
		exit(-1);
	}

	if(s_szPath.empty())
		PathSet(SDL_GetBasePath());
}

//-----------------------------------------------------------------------------
CN3Eng::~CN3Eng(void)
{
	CN3Base::ReleaseResrc();
	delete [] m_DeviceInfo.pModes;

	if(s_lpD3DDev)
	{
		int nRefCount = s_lpD3DDev->Release();

		if (nRefCount == 0) {
			s_lpD3DDev = NULL;
		} else {
#ifdef _N3GAME
			CLogWriter::Write("CNEng::~CN3Eng - Device reference count is bigger than 0");
#endif
		}
	}

	if(m_lpD3D) if(m_lpD3D->Release() == 0) m_lpD3D = NULL;
	if(m_lpDD) m_lpDD->Release(); m_lpDD = NULL;

#ifdef _N3GAME
	CLogWriter::Close();
#endif
}

//-----------------------------------------------------------------------------
void CN3Eng::Release(void)
{
	m_nModeActive   = -1;
	m_nAdapterCount =  1;

	delete [] m_DeviceInfo.pModes;
	memset(&m_DeviceInfo, 0, sizeof(m_DeviceInfo));

	if(s_lpD3DDev)
	{
		int nRefCount = s_lpD3DDev->Release();

		if (nRefCount == 0) {
			s_lpD3DDev = NULL;
		} else {
#ifdef _N3GAME
			CLogWriter::Write("CNEng::Release Device reference count is bigger than 0");
#endif
		}
	}

	if(m_lpDD) m_lpDD->Release(); m_lpDD = NULL;
}

//-----------------------------------------------------------------------------
void CN3Eng::SetViewPort(RECT& rc)
{
	if (s_lpD3DDev == NULL) return;

	D3DVIEWPORT9 vp;
	vp.X      = rc.left;
	vp.Y      = rc.top;
	vp.Width  = rc.right  - rc.left;
	vp.Height = rc.bottom - rc.top;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;

	s_lpD3DDev->SetViewport(&vp);
	memcpy(&s_CameraData.vp, &vp, sizeof(D3DVIEWPORT9));
}

//-----------------------------------------------------------------------------
void CN3Eng::SetDefaultEnvironment(void)
{
	__Matrix44 matWorld; matWorld.Identity();

	s_lpD3DDev->SetTransform(D3DTS_WORLD, &matWorld);
	s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	s_lpD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	s_lpD3DDev->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	s_lpD3DDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	s_lpD3DDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	s_lpD3DDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	s_lpD3DDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	s_lpD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	float fMipMapLODBias = -1.0f;

	for (int i = 0; i < 8; ++i) {
		s_lpD3DDev->SetTexture(i, NULL);
		s_lpD3DDev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		s_lpD3DDev->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		s_lpD3DDev->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		s_lpD3DDev->SetSamplerState(i, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD)(&fMipMapLODBias)));
	}

	D3DCLIPSTATUS9 cs;
	cs.ClipUnion = cs.ClipIntersection = D3DCS_ALL;

	s_lpD3DDev->SetClipStatus(&cs);
}

//-----------------------------------------------------------------------------
/*!
Used to set the view matrix for DirectX
*/
void CN3Eng::LookAt(__Vector3 &vEye, __Vector3 &vAt, __Vector3 &vUp)
{
	__Matrix44 matView;
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	s_lpD3DDev->SetTransform(D3DTS_VIEW, &matView);
}

//-----------------------------------------------------------------------------
bool CN3Eng::Reset(bool bWindowed, Uint32 dwWidth, Uint32 dwHeight, Uint32 dwBPP)
{
	if (s_lpD3DDev == NULL) return false;
	if (dwWidth <= 0 || dwHeight <= 0) return false;

	if (dwWidth == s_DevParam.BackBufferWidth && dwHeight == s_DevParam.BackBufferHeight)
	{
		if (0 == dwBPP) return false;
		if (16 == dwBPP && D3DFMT_R5G6B5 == s_DevParam.BackBufferFormat) return false;
		if (24 == dwBPP && D3DFMT_R8G8B8 == s_DevParam.BackBufferFormat) return false;
		if (32 == dwBPP && D3DFMT_X8R8G8B8 == s_DevParam.BackBufferFormat) return false;
	}

	D3DPRESENT_PARAMETERS DevParamBackUp;
	memcpy(&DevParamBackUp, &s_DevParam, sizeof(D3DPRESENT_PARAMETERS));

	D3DFORMAT BBFormat = D3DFMT_UNKNOWN;

	if (bWindowed)
	{
		D3DDISPLAYMODE dm;
		m_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);

		BBFormat = dm.Format;

	} else
	{
		if (16 == dwBPP) BBFormat = D3DFMT_R5G6B5;
		else if (24 == dwBPP) BBFormat = D3DFMT_R8G8B8;
		else if (32 == dwBPP) BBFormat = D3DFMT_X8R8G8B8;
	}

	s_DevParam.Windowed         = bWindowed;
	s_DevParam.BackBufferWidth  = dwWidth;
	s_DevParam.BackBufferHeight = dwHeight;
	s_DevParam.BackBufferFormat = BBFormat;

	int nMC = m_DeviceInfo.nModeCount;
	for (int i = 0; i < nMC; i++)
	{
		if (m_DeviceInfo.pModes[i].Format == s_DevParam.BackBufferFormat)
		{
			FindDepthStencilFormat(
				0, m_DeviceInfo.DevType, m_DeviceInfo.pModes[i].Format,
				&s_DevParam.AutoDepthStencilFormat
			);

			m_nModeActive = i;

			break;
		}
	}

	if (D3D_OK != s_lpD3DDev->Reset(&s_DevParam))
	{
#ifdef _N3GAME
		CLogWriter::Write("CNEng::Reset - Insufficient video memory");
#endif
		memcpy(&s_DevParam, &DevParamBackUp, sizeof(D3DPRESENT_PARAMETERS));

		if (D3D_OK != s_lpD3DDev->Reset(&s_DevParam))
		{
#ifdef _N3GAME
			CLogWriter::Write("CNEng::Reset - Insufficient video memory");
#endif
		}

		return false;
	}

	RECT rcView = {
		0, 0, dwWidth, dwHeight
	};

	SetViewPort(rcView);
	SetDefaultEnvironment();

	return true;
}

//-----------------------------------------------------------------------------
/*!
Set the projection matrix for DirectX
*/
void CN3Eng::SetProjection(float fNear, float fFar, float fLens, float fAspect)
{
	__Matrix44 matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, fLens, fAspect, fNear, fFar);
	s_lpD3DDev->SetTransform(D3DTS_PROJECTION, &matProjection);
}

//-----------------------------------------------------------------------------















//-----------------------------------------------------------------------------
bool CN3Eng::Init(BOOL bWindowed, SDL_Window* pWindow, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, BOOL bUseHW)
{
	/*
	SDL_Renderer* s_pSDLRenderer;

	s_pSDLRenderer = SDL_CreateRenderer(
		pWindow, -1,
		SDL_RENDERER_ACCELERATED|
		SDL_RENDERER_PRESENTVSYNC
	);

	if(s_pSDLRenderer == NULL) {
		fprintf(stderr, "ER: %s\n", SDL_GetError());
		exit(-1);
	}

	s_lpD3DDev = SDL_RenderGetD3D9Device(s_pSDLRenderer);
	s_lpD3DDev->GetDirect3D(&m_lpD3D);
	*/

	//

	memset(&s_ResrcInfo, 0, sizeof(__ResrcInfo)); // Rendering Information �ʱ�ȭ..

	s_hWndBase = pWindow;

	// FIX (srmeier): I really have no idea what the second arguement here should be
	int nAMC = m_lpD3D->GetAdapterModeCount(0, D3DFMT_X8R8G8B8); // ���÷��� ��� ī��Ʈ
	if(nAMC <= 0)
	{
		//MessageBox(hWnd, "Can't create D3D - Invalid display mode property.", "initialization", MB_OK);
//		{ for(int iii = 0; iii < 2; iii++) Beep(2000, 200); Sleep(300); } // ������ ��~
#ifdef _N3GAME
		CLogWriter::Write("Can't create D3D - Invalid display mode property.");
#endif
		this->Release();
		return false;
	}

	m_DeviceInfo.nAdapter = 0;
	m_DeviceInfo.DevType = D3DDEVTYPE_HAL;
	m_DeviceInfo.nDevice = 0;
	m_DeviceInfo.nModeCount = nAMC;
	delete [] m_DeviceInfo.pModes;
	m_DeviceInfo.pModes = new D3DDISPLAYMODE[nAMC];
	for(int i = 0; i < nAMC; i++)
	{
		// FIX (srmeier): I really have no idea what the second arguement here should be
		m_lpD3D->EnumAdapterModes(0, D3DFMT_X8R8G8B8, i, &m_DeviceInfo.pModes[i]); // ���÷��� ��� ��������..
	}

	D3DDEVTYPE DevType = D3DDEVTYPE_REF;
	if(TRUE == bUseHW) DevType = D3DDEVTYPE_HAL;

	memset(&s_DevParam, 0, sizeof(s_DevParam));
	s_DevParam.Windowed = bWindowed;
	s_DevParam.EnableAutoDepthStencil = TRUE;
	s_DevParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	s_DevParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	s_DevParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	D3DFORMAT BBFormat = D3DFMT_UNKNOWN;
	if(TRUE == bWindowed) // ������ ����� ���
	{
		D3DDISPLAYMODE dm;
		m_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
		s_DevParam.BackBufferCount = 1;
		if(dwWidth <= 0) dwWidth = dm.Width;
		if(dwHeight <= 0) dwHeight = dm.Height;
		BBFormat = dm.Format;
		s_DevParam.hDeviceWindow = ::GetActiveWindow();
	}
	else
	{
		s_DevParam.BackBufferCount = 1;
		s_DevParam.AutoDepthStencilFormat = D3DFMT_D16; // �ڵ� �����̸� ���õȴ�.
		if(16 == dwBPP) BBFormat = D3DFMT_R5G6B5;
		else if(24 == dwBPP) BBFormat = D3DFMT_R8G8B8;
		else if(32 == dwBPP) BBFormat = D3DFMT_X8R8G8B8;
		s_DevParam.hDeviceWindow = ::GetActiveWindow();
	}

	s_DevParam.BackBufferWidth = dwWidth;
	s_DevParam.BackBufferHeight = dwHeight;
	s_DevParam.BackBufferFormat = BBFormat;
	s_DevParam.MultiSampleType = D3DMULTISAMPLE_NONE; // Swap Effect �� Discard ���°� �ƴϸ� �ݵ�� �̷� ���̾�� �Ѵ�.
	s_DevParam.Flags = 0;
//#ifdef _N3TOOL
	s_DevParam.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
//#endif // end of _N3TOOL

	int nMC = m_DeviceInfo.nModeCount;
	for(int i = 0; i < nMC; i++)
	{
//		if(	m_DeviceInfo.pModes[i].Width == dwWidth && 
//			m_DeviceInfo.pModes[i].Height == dwHeight && 
		if(	m_DeviceInfo.pModes[i].Format == BBFormat) // ��尡 ��ġ�ϸ�
		{
			this->FindDepthStencilFormat(0, m_DeviceInfo.DevType, m_DeviceInfo.pModes[i].Format, &s_DevParam.AutoDepthStencilFormat); // ���̿� ���ٽ� ���۸� ã�´�.
			m_nModeActive = i;
			break;
		}
	}

	
	HRESULT rval = m_lpD3D->CreateDevice(0, DevType, GetActiveWindow(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &s_DevParam, &s_lpD3DDev);
	if(rval != D3D_OK)
	{
		rval = m_lpD3D->CreateDevice(0, DevType, GetActiveWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &s_DevParam, &s_lpD3DDev);
		if(rval != D3D_OK)
		{
			char szDebug[256];
			//D3DXGetErrorString(rval, szDebug, 256);
			MessageBox(GetActiveWindow(), "Can't create D3D Device - please, check DirectX or display card driver", "initialization", MB_OK);
#ifdef _N3GAME
			CLogWriter::Write("Can't create D3D Device - please, check DirectX or display card driver");
			CLogWriter::Write(szDebug);
#endif
//			{ for(int iii = 0; iii < 3; iii++) Beep(2000, 200); Sleep(300); } // ������ ��~

			this->Release();
			return false;
		}
#ifdef _N3GAME
		CLogWriter::Write("CNEng::Init - Not supported HardWare TnL");
#endif
	}


	// Device ���� �׸���??
	// DXT ���� ����..
	s_dwTextureCaps = 0;
	s_DevCaps.DeviceType = DevType;

	s_lpD3DDev->GetDeviceCaps(&s_DevCaps);
	if(s_DevCaps.MaxTextureWidth < 256 || s_DevCaps.MaxTextureHeight < 256) // �ؽ�ó ���� ũ�Ⱑ 256 ���ϸ�.. �ƿ� ����..
	{
		MessageBox(::GetActiveWindow(), "Can't support this graphic card : Texture size is too small", "Initialization error", MB_OK);
#ifdef _N3GAME
		CLogWriter::Write("Can't support this graphic card : Texture size is too small");
#endif
//		{ for(int iii = 0; iii < 4; iii++) Beep(2000, 200); Sleep(300); } // ������ ��~

		this->Release();
		return false;
	}

	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT1)) s_dwTextureCaps |= TEX_CAPS_DXT1;
	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT2)) s_dwTextureCaps |= TEX_CAPS_DXT2;
	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT3)) s_dwTextureCaps |= TEX_CAPS_DXT3;
	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT4)) s_dwTextureCaps |= TEX_CAPS_DXT4;
	if(D3D_OK == m_lpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, DevType, BBFormat, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT5)) s_dwTextureCaps |= TEX_CAPS_DXT5;
	if(s_DevCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) s_dwTextureCaps |= TEX_CAPS_SQUAREONLY;
	if(s_DevCaps.TextureCaps & D3DPTEXTURECAPS_MIPMAP) s_dwTextureCaps |= TEX_CAPS_MIPMAP;
	if(s_DevCaps.TextureCaps & D3DPTEXTURECAPS_POW2) s_dwTextureCaps |= TEX_CAPS_POW2;

	// �⺻ ����Ʈ ���� ����..
	for(int i = 0; i < 8; i++)
	{
		CN3Light::__Light Lgt;
		_D3DCOLORVALUE LgtColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		Lgt.InitPoint(i, __Vector3(0,0,0), LgtColor);
		s_lpD3DDev->SetLight(i, &Lgt);
	}

	// �⺻ ��� �������� ����.
	this->LookAt(__Vector3(5,5,-10), __Vector3(0,0,0), __Vector3(0,1,0));
	this->SetProjection(0.1f, 256.0f, D3DXToRadian(45.0f), (float)dwHeight/dwWidth);
	
	RECT rcView = { 0, 0, dwWidth, dwHeight };
	this->SetViewPort(rcView);
	this->SetDefaultEnvironment(); // �⺻ ���·� ����..

	return true;
}


/*
LRESULT WINAPI CN3Eng::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
//          PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:
//          Render();
//          ValidateRect( hWnd, NULL );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}
*/

BOOL CN3Eng::FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat)
{
	int nDSC = 6;
	D3DFORMAT DepthFmts[] = { D3DFMT_D32, D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D24X8, D3DFMT_D16, D3DFMT_D15S1};

	HRESULT rval = 0;
	for(int i = 0; i < nDSC; i++)
	{
		rval = m_lpD3D->CheckDeviceFormat(iAdapter, DeviceType, TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, DepthFmts[i]);
		if(D3D_OK == rval)
		{
			rval = m_lpD3D->CheckDepthStencilMatch(iAdapter, DeviceType, TargetFormat, TargetFormat, DepthFmts[i]);
			if(D3D_OK == rval)
			{
				*pDepthStencilFormat = DepthFmts[i];
				return TRUE;
			}
		}
	}

    return FALSE;
}
void CN3Eng::Present(SDL_Window* pWindow, RECT* pRC)
{
//	HRESULT rval = s_lpD3DDev->TestCooperativeLevel();
//	if(D3D_OK != rval)
//	{
//#if _DEBUG
//		char szDebug[256];
//		D3DXGetErrorString(rval, szDebug, 256);
//#endif
//		return;
//	}

	/*
	RECT rc;
	if(s_DevParam.Windowed) // ������ ����...
	{
		GetClientRect(s_hWndBase, &rc);
		pRC = &rc;
	}
	*/




	//SDL_Renderer* s_pRenderer = SDL_GetRenderer(pWindow);
	//SDL_RenderPresent(s_pRenderer);



	
	HRESULT rval = s_lpD3DDev->Present(pRC, pRC, GetActiveWindow(), NULL);
	if(D3D_OK == rval)
	{
		//s_hWndPresent = hWnd; // Present window handle �� ������ ���´�.
	}
	else if(D3DERR_DEVICELOST == rval || D3DERR_DEVICENOTRESET == rval)
	{
		rval = s_lpD3DDev->Reset(&s_DevParam);
		if(D3D_OK != rval)
		{
#ifdef _N3GAME
//			char szErr[256];
//			D3DXGetErrorString(rval, szErr, 256);
//			CLogWriter::Write("CNEng::Present - device present failed (%s)", szErr);
//			Beep(2000, 50);
#endif
		}
		else
		{
			rval = s_lpD3DDev->Present(pRC, pRC, GetActiveWindow(), NULL);
		}
		return;
	}
	else
	{
#ifdef _N3GAME
//		char szErr[256];
//		D3DXGetErrorString(rval, szErr, 256);
//		CLogWriter::Write("CNEng::Present - device present failed (%s)", szErr);
//		Beep(2000, 50);
#endif
	}
	

	////////////////////////////////////////////////////////////////////////////////
	// ������ �� ����...
//	float fTime = CN3Base::TimerProcess(TIMER_GETABSOLUTETIME);
//	static float fTimePrev = fTime - 0.03333f;
//	static DWORD dwFrm = 0;
//	dwFrm++;
//	if(fTime - fTimePrev > 1.0f) // 1 �� �̻� ������ ������ �����Ѵ�.. �׷��� ������ ���� ���� �� ��ġ�� ���´�..
//	{
//		s_fFrmPerSec = (float)dwFrm / (fTime - fTimePrev);
//		dwFrm = 0;
//		fTimePrev = fTime;
//	}

	s_fSecPerFrm = CN3Base::TimerProcess(TIMER_GETELAPSEDTIME);
	if(s_fSecPerFrm <= 0.001f || s_fSecPerFrm >= 1.0f) s_fSecPerFrm = 0.033333f; // �ʹ� �ȳ����� �⺻ ���� 30 ���������� �����..
	s_fFrmPerSec = 1.0f / s_fSecPerFrm; // �ʴ� ������ �� ����..

//	fTimePrev = fTime;
	// ������ �� ����...
	////////////////////////////////////////////////////////////////////////////////
}

#ifdef _N3TOOL
void CN3Eng::Present(HWND hWnd, RECT* pRC) {
	RECT rc;
	if(s_DevParam.Windowed)
	{
		GetClientRect(hWnd, &rc);
		pRC = &rc;
	}

	HRESULT rval = s_lpD3DDev->Present(pRC, pRC, hWnd, NULL);
	if (D3D_OK == rval)
	{
		s_hWndPresent = hWnd; // Present window handle �� ������ ���´�.
	}
	else if (D3DERR_DEVICELOST == rval || D3DERR_DEVICENOTRESET == rval)
	{
		rval = s_lpD3DDev->Reset(&s_DevParam);
		if (D3D_OK != rval)
		{
		}
		else
		{
			rval = s_lpD3DDev->Present(pRC, pRC, hWnd, NULL);
		}
		return;
	}
	else
	{
	}

	s_fSecPerFrm = CN3Base::TimerProcess(TIMER_GETELAPSEDTIME);
	if (s_fSecPerFrm <= 0.001f || s_fSecPerFrm >= 1.0f) s_fSecPerFrm = 0.033333f; // �ʹ� �ȳ����� �⺻ ���� 30 ���������� �����..
	s_fFrmPerSec = 1.0f / s_fSecPerFrm; // �ʴ� ������ �� ����..
}
#endif

void CN3Eng::Clear(D3DCOLOR crFill, RECT* pRC)
{
	RECT rc;
	if(NULL == pRC && s_DevParam.Windowed) // ������ ����...
	{
		GetClientRect(::GetActiveWindow(), &rc);

		//int x, y, w, h;
		//SDL_GetWindowPosition(s_hWndBase, &x, &y);
		//SDL_GetWindowSize(s_hWndBase, &w, &h);
		//rc.left = x; rc.right = (x+w); rc.top = y; rc.bottom = (y+h);

		pRC = &rc;
	}

	if(pRC)
	{
		_D3DRECT rc3D = { pRC->left, pRC->top, pRC->right, pRC->bottom };
		s_lpD3DDev->Clear(1, &rc3D, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, crFill, 1.0f, 0);
	}
	else
	{
		s_lpD3DDev->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, crFill, 1.0f, 0);
	}

#ifdef _DEBUG
	memset(&s_RenderInfo, 0, sizeof(__RenderInfo));
#endif
}

void CN3Eng::ClearAuto(RECT* pRC)
{
	DWORD dwFillColor = D3DCOLOR_ARGB(255,192,192,192); // �⺻��
	DWORD dwUseFog = FALSE;
	s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwUseFog); // �Ȱ��� ���� �������� �Ȱ����� ����ش�..
	if(dwUseFog != 0) s_lpD3DDev->GetRenderState(D3DRS_FOGCOLOR, &dwFillColor);
	else
	{
		CN3Light::__Light Lgt;

		BOOL bEnable;
		s_lpD3DDev->GetLightEnable(0, &bEnable);
		if(bEnable)
		{
			s_lpD3DDev->GetLight(0, &Lgt);
			dwFillColor = D3DCOLOR_ARGB((BYTE)(Lgt.Diffuse.a * 255.0f), (BYTE)(Lgt.Diffuse.r * 255.0f), (BYTE)(Lgt.Diffuse.g * 255.0f), (BYTE)(Lgt.Diffuse.b * 255.0f));
		}
	}

	CN3Eng::Clear(dwFillColor, pRC);
}

void CN3Eng::ClearZBuffer(const RECT* pRC)
{
	RECT rc;
	if(NULL == pRC && s_DevParam.Windowed) // ������ ����...
	{
		GetClientRect(::GetActiveWindow(), &rc);

		//int x, y, w, h;
		//SDL_GetWindowPosition(s_hWndBase, &x, &y);
		//SDL_GetWindowSize(s_hWndBase, &w, &h);
		//rc.left = x; rc.right = (x+w); rc.top = y; rc.bottom = (y+h);

		pRC = &rc;
	}

	if(pRC) 
	{
		D3DRECT rc3D = { pRC->left, pRC->top, pRC->right, pRC->bottom };
		s_lpD3DDev->Clear(1, &rc3D, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	}
	else
	{
		s_lpD3DDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	}
}
