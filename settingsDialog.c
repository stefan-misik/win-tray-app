#include "settingsDialog.h"
#include "resource.h"

/******************************************************************************/
static INT_PTR CALLBACK DialogProc(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch (uMsg)
	{

	case WM_INITDIALOG:
	{		
		HICON hIcon;

		/* Load and set the icon */
		hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_TRAY));
		if (NULL != hIcon)
		{
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

			DestroyIcon(hIcon);
		}
		
		return TRUE;
	}

	case WM_COMMAND:
	{
		switch (HIWORD(wParam))
		{
		case 0:
			switch (LOWORD(wParam))
			{
			case IDC_EXIT:
				DestroyWindow(hwndDlg);
				DestroyWindow(g_hwndMain);
				return TRUE;

			case IDOK:
				DestroyWindow(hwndDlg);
				return TRUE;
				
			case IDCANCEL:
				DestroyWindow(hwndDlg);
				return TRUE;
			}
		}
	}
		break;

	case WM_CLOSE:		
		DestroyWindow(hwndDlg);
		return TRUE;

	case WM_DESTROY:
		return TRUE;

	default:
		return FALSE;
		break;
	}
	return FALSE;
}

/******************************************************************************/
INT_PTR SettingsShow(
	LPSETTINGSDATA lpData
	)
{
	/* TODO: Disable window from creating multiple copies */

	return DialogBox(
		g_hInstance,
		MAKEINTRESOURCE(IDD_SETTINGS_WND),
		NULL,
		DialogProc
		);
}