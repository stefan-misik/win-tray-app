#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

#include "common.h"
#include "resource.h"
#include "settingsDialog.h"



static TCHAR g_lptstrRCTClassName[] = TEXT("remote-control-translator-tray-icon-wnd");


HINSTANCE g_hInstance;
HWND g_hwndMain;




/******************************************************************************/
static VOID AddTrayIcon(
	HWND hwnd,
	UINT uID,
	UINT uCallbackMsg
	)
{
	NOTIFYICONDATA  nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uID = uID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = uCallbackMsg;
	nid.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_TRAY));
	
	GetWindowText(hwnd, nid.szTip, sizeof(nid.szTip) / sizeof(TCHAR));
	
	Shell_NotifyIcon(NIM_ADD, &nid);
}

/******************************************************************************/
static VOID RemoveTrayIcon(
	HWND hWnd,
	UINT uID
	)
{
	NOTIFYICONDATA  nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uID = uID;

	Shell_NotifyIcon(NIM_DELETE, &nid);
}

/******************************************************************************/
static BOOL ShowPopupMenu(
	HWND hwndParent
	)
{
	BOOL bRet = TRUE;
	HMENU hMainMenu;
	POINT pt;

	/* Load Menu */
	hMainMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MMENU));

	/* Get cursor pos */
	GetCursorPos(&pt);

	/* Set focus */
	SetFocus(hwndParent);
	
	/* Show popum menu */
	if (0 == TrackPopupMenuEx(
		GetSubMenu(hMainMenu, 0),
		0,
		pt.x, pt.y,		
		hwndParent,
		NULL
		))
	{
		bRet = FALSE;
	}

	/* Destroey Menu */
	DestroyMenu(hMainMenu);

	return bRet;
}


/******************************************************************************/
static LRESULT CALLBACK TrayIconMessageWndProc(
	HWND   hwnd,
	UINT   uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch (uMsg)
	{
	case WM_CREATE:
		AddTrayIcon(hwnd, 0, WM_USER);
		return 0;

	case WM_DESTROY:
		RemoveTrayIcon(hwnd, 0);

		/* End process */
		PostQuitMessage(0);
		return 0;

	case WM_USER:
		switch (lParam)
		{
		case WM_RBUTTONUP:		
			/* Create popup menu */
			if (FALSE == ShowPopupMenu(hwnd))
			{
				DestroyWindow(hwnd);
			}
			break;
		case WM_LBUTTONUP:
			
			break;
		}
		return 0;

	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case 0:
			switch (LOWORD(wParam))
			{
			case ID_EXIT:
				DestroyWindow(hwnd);
				return 0;

            case ID_SETTINGS:
                /* Show window */
                SettingsShow(NULL);
                return 0;
			}
		}
	}
		break;
	}	

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


/******************************************************************************/
INT WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine, 
	INT nCmdShow
)
{
	MSG msg;
	ATOM aClassAtom;

	/* Store instance handle */
	g_hInstance = hInstance;

	
	/* Initialize common controls */
	{
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_STANDARD_CLASSES;
		InitCommonControlsEx(&icex);
	}

	/* Registe class */
	{
		WNDCLASSEX wc;
		memset(&wc, 0, sizeof(WNDCLASSEX));

		wc.cbSize			= sizeof(WNDCLASSEX);
		wc.style			= CS_PARENTDC;
		wc.lpfnWndProc		= TrayIconMessageWndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= hInstance;
		wc.hIcon			= NULL;
		wc.hCursor			= NULL;
		wc.hbrBackground	= NULL;
		wc.lpszMenuName		= NULL;
		wc.lpszClassName	= g_lptstrRCTClassName;
		wc.hIconSm			= NULL;

		aClassAtom = RegisterClassEx(&wc);		
	}

	/* Create main Window */
	g_hwndMain = NULL;
	if (0 != aClassAtom)
	{
		g_hwndMain = CreateWindowEx(
			0,
			(LPCTSTR)aClassAtom,
			TEXT("Remote Control Translator"),
			0,
			CW_USEDEFAULT, 0,
			CW_USEDEFAULT, 0,
			HWND_MESSAGE,
			NULL,
			hInstance,
			NULL);
	}

	if (NULL == g_hwndMain)
	{
		MessageBox(NULL, TEXT("Application Initialization Failed!"),
			TEXT("Error!"), MB_OK | MB_ICONHAND);
		return -1;
	}

			
	/* Enter the message loop */
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{		
		TranslateMessage(&msg);
		DispatchMessage(&msg);		
	}

	
	ExitProcess((UINT)(msg.wParam));

	return (INT)(msg.wParam);
}

