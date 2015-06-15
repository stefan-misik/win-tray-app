#ifndef COMMON_H
#define	COMMON_H

#undef _WIN32_WINNT
#undef _WIN32_IE
#define _WIN32_WINNT 0x0600
#define _WIN32_IE 0x0900
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <commctrl.h>

/**
* \brief Global instance handle
*/
extern HINSTANCE g_hInstance;

/**
* \brief Global handle to main window
*/
extern HWND g_hwndMain;


#endif	/* COMMON_H */
