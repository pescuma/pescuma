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


#ifndef __POPUP_H__
# define __POPUP_H__

#include "commons.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef void * Options;


// Initializations needed by popups
void InitPopups();

// Deinitializations needed by popups
void DeInitPopups();


#define POPUP_TYPE_NORMAL	0
#define POPUP_TYPE_TEST		1
#define POPUP_TYPE_ERROR	2
#define POPUP_TYPE_INFO		3

void ShowPopup(HANDLE hContact, const TCHAR *title, const TCHAR *description);

void ShowTestPopup(const TCHAR *title, const TCHAR *description, const Options *op);

void ShowErrPopup(const TCHAR *description, const TCHAR *title = NULL);

void ShowInfoPopup(const TCHAR *description, const TCHAR *title = NULL);

void ShowPopupEx(HANDLE hContact, const TCHAR *title, const TCHAR *description, 
			   void *plugin_data, int type, const Options *op);


#ifdef __cplusplus
}
#endif


#endif // __POPUP_H__
