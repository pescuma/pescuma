/* 
Copyright (C) 2005-2010 Ricardo Pescuma Domenecci

This is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this file; see the file license.txt.  If
not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  
*/


#include "commons.h"



// Prototypes /////////////////////////////////////////////////////////////////////////////////////

#define WMU_ACTION	(WM_USER + 1)


LRESULT CALLBACK PopupWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hPopupWindow = NULL;


static LRESULT CALLBACK PopupDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK DumbPopupDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



// Functions //////////////////////////////////////////////////////////////////////////////////////


// Initializations needed by popups
void InitPopups()
{
	// window needed for popup commands
	hPopupWindow = CreateWindowEx(WS_EX_TOOLWINDOW, _T("static"), _T(MODULE_NAME) _T("_PopupWindow"), 
		0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP,
		NULL, hInst, NULL);
	SetWindowLong(hPopupWindow, GWL_WNDPROC, (LONG)(WNDPROC)PopupWndProc);
}


// Deinitializations needed by popups
void DeInitPopups()
{
}


void ShowErrPopup(const TCHAR *description, const TCHAR *title)
{
	ShowPopupEx(NULL, title == NULL ? _T(MODULE_NAME) _T(" Error") : title, description,
			  NULL, POPUP_TYPE_ERROR, NULL);
}


void ShowInfoPopup(const TCHAR *description, const TCHAR *title)
{
	ShowPopupEx(NULL, title == NULL ? _T(MODULE_NAME) _T(" Information") : title, description,
		NULL, POPUP_TYPE_INFO, NULL);
}


void ShowTestPopup(const TCHAR *title, const TCHAR *description, const Options *op)
{
	ShowPopupEx(NULL, title, description, NULL, POPUP_TYPE_TEST, op);
}


void ShowPopup(HANDLE hContact, const TCHAR *title, const TCHAR *description)
{
	ShowPopupEx(hContact, title, description, hContact, POPUP_TYPE_NORMAL, NULL);
}


// Show an popup
void ShowPopupEx(HANDLE hContact, const TCHAR *title, const TCHAR *description, 
			   void *plugin_data, int type, const Options *op)
{
#ifdef UNICODE
	if(ServiceExists(MS_POPUP_ADDPOPUPW)) 
	{
		// Make popup
		POPUPDATAW ppd;

		ZeroMemory(&ppd, sizeof(ppd)); 
		ppd.lchContact = hContact; 
		ppd.lchIcon = (HICON) LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(174),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

		if (title != NULL)
			lstrcpyn(ppd.lpwzContactName, title, MAX_REGS(ppd.lpwzContactName));
		else if (hContact != NULL)
			lstrcpyn(ppd.lpwzContactName, (TCHAR *) CallService(MS_CLIST_GETCONTACTDISPLAYNAME, (WPARAM)hContact, GCDNF_TCHAR), 
					MAX_REGS(ppd.lpwzContactName));

		if (description != NULL)
			lstrcpyn(ppd.lpwzText, description, MAX_REGS(ppd.lpwzText));

		if (type == POPUP_TYPE_NORMAL || type == POPUP_TYPE_TEST)
		{
/*			if (op->popup_use_default_colors)
			{
				ppd.colorBack = 0;
				ppd.colorText = 0;
			}
			else if (op->popup_use_win_colors)
			{
				ppd.colorBack = GetSysColor(COLOR_BTNFACE);
				ppd.colorText = GetSysColor(COLOR_WINDOWTEXT);
			}
			else
			{
				ppd.colorBack = op->popup_bkg_color;
				ppd.colorText = op->popup_text_color;
			}
*/		}
		else if (type == POPUP_TYPE_ERROR)
		{
			ppd.colorBack = RGB(200,0,0);
			ppd.colorText = RGB(255,255,255);
		}
		else // if (type == POPUP_TYPE_INFO)
		{
			ppd.colorBack = RGB(255,255,128);
			ppd.colorText = RGB(0,0,0);
		}

		if (type == POPUP_TYPE_NORMAL)
		{
			ppd.PluginWindowProc = PopupDlgProc;
			ppd.PluginData = plugin_data;
		}
		else // if (type == POPUP_TYPE_TEST || type == POPUP_TYPE_ERROR)
		{
			ppd.PluginWindowProc = DumbPopupDlgProc;
		}
		
		if (type == POPUP_TYPE_NORMAL || type == POPUP_TYPE_TEST)
		{
/*			switch (op->popup_delay_type) 
			{
				case POPUP_DELAY_CUSTOM:
					ppd.iSeconds = opts.popup_timeout;
					break;

				case POPUP_DELAY_PERMANENT:
					ppd.iSeconds = -1;
					break;

				//case POPUP_DELAY_DEFAULT:
				default:
					ppd.iSeconds = 0;
					break;
			}
*/		}
		else // if (type == POPUP_TYPE_ERROR || type == POPUP_TYPE_INFO)
		{
			ppd.iSeconds = 0;
		}

		// Now that every field has been filled, we want to see the popup.
		CallService(MS_POPUP_ADDPOPUPW, (WPARAM)&ppd,0);
	}
	else
#endif
	if(ServiceExists(MS_POPUP_ADDPOPUPEX)) 
	{
		// Make popup
		POPUPDATAEX ppd;

		ZeroMemory(&ppd, sizeof(ppd)); 
		ppd.lchContact = hContact; 
		ppd.lchIcon = (HICON) LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(174),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

		if (title != NULL)
			lstrcpynA(ppd.lpzContactName, TcharToChar(title), MAX_REGS(ppd.lpzContactName));
		else
			lstrcpynA(ppd.lpzContactName, (char *) CallService(MS_CLIST_GETCONTACTDISPLAYNAME, (WPARAM)hContact, 0), 
					MAX_REGS(ppd.lpzContactName));

		if (description != NULL)
			lstrcpynA(ppd.lpzText, TcharToChar(description), MAX_REGS(ppd.lpzText));

		if (type == POPUP_TYPE_NORMAL || type == POPUP_TYPE_TEST)
		{
/*			if (op->popup_use_default_colors)
			{
				ppd.colorBack = 0;
				ppd.colorText = 0;
			}
			else if (op->popup_use_win_colors)
			{
				ppd.colorBack = GetSysColor(COLOR_BTNFACE);
				ppd.colorText = GetSysColor(COLOR_WINDOWTEXT);
			}
			else
			{
				ppd.colorBack = op->popup_bkg_color;
				ppd.colorText = op->popup_text_color;
			}
*/
		}
		else if (type == POPUP_TYPE_ERROR)
		{
			ppd.colorBack = RGB(200,0,0);
			ppd.colorText = RGB(255,255,255);
		}
		else // if (type == POPUP_TYPE_INFO)
		{
			ppd.colorBack = RGB(255,255,128);
			ppd.colorText = RGB(0,0,0);
		}

		if (type == POPUP_TYPE_NORMAL)
		{
			ppd.PluginWindowProc = PopupDlgProc;
			ppd.PluginData = plugin_data;
		}
		else // if (type == POPUP_TYPE_TEST || type == POPUP_TYPE_ERROR)
		{
			ppd.PluginWindowProc = DumbPopupDlgProc;
		}
		
		if (type == POPUP_TYPE_NORMAL || type == POPUP_TYPE_TEST)
		{
/*			switch (op->popup_delay_type) 
			{
				case POPUP_DELAY_CUSTOM:
					ppd.iSeconds = opts.popup_timeout;
					break;

				case POPUP_DELAY_PERMANENT:
					ppd.iSeconds = -1;
					break;

				//case POPUP_DELAY_DEFAULT:
				default:
					ppd.iSeconds = 0;
					break;
			}
*/		}
		else // if (type == POPUP_TYPE_ERROR || type == POPUP_TYPE_INFO)
		{
			ppd.iSeconds = 0;
		}

		// Now that every field has been filled, we want to see the popup.
		CallService(MS_POPUP_ADDPOPUPEX, (WPARAM)&ppd,0);
	}
}


// Handle to the hidden windows to handle actions for popup clicks
// wParam has the number of MOTD in case of WMU_SHOW_MOTD_DETAILS
LRESULT CALLBACK PopupWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	if (uMsg == WMU_ACTION)
	{
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


// Handle to popup events
static LRESULT CALLBACK PopupDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
		case WM_COMMAND:
		{
			SendMessage(hPopupWindow, WMU_ACTION, (WPARAM)PUGetPluginData(hWnd), 0);
			PUDeletePopUp(hWnd);

			return TRUE;
		}

		case WM_CONTEXTMENU: 
		{
			SendMessage(hPopupWindow, WMU_ACTION, (WPARAM)PUGetPluginData(hWnd), 0);
			PUDeletePopUp(hWnd);

			return TRUE;
		}

		case UM_FREEPLUGINDATA: 
		{
			return TRUE;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// Handle to popup events
static LRESULT CALLBACK DumbPopupDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
		case WM_COMMAND:
		{
			PUDeletePopUp(hWnd);
			return TRUE;
		}

		case WM_CONTEXTMENU: 
		{
			PUDeletePopUp(hWnd);
			return TRUE;
		}

		case UM_FREEPLUGINDATA: 
		{
			return TRUE;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

