/* 
Copyright (C) 2006 Ricardo Pescuma Domenecci
Based on work (C) Heiko Schillinger

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


#ifndef __COMMONS_H__
# define __COMMONS_H__


#define WINVER 0x0500
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <tchar.h>


// Miranda headers
#define MIRANDA_VER 0x0A00
#include <newpluginapi.h>
#include <m_system.h>
#include <m_system_cpp.h>
#include <m_protocols.h>
#include <m_protosvc.h>
#include <m_clist.h>
#include <m_ignore.h>
#include <m_contacts.h>
#include <m_message.h>
#include <m_userinfo.h>
#include <m_skin.h>
#include <m_langpack.h>
#include <m_database.h>
#include <m_options.h>
#include <m_utils.h>
#include <m_button.h>
#include <m_file.h>
#include <m_url.h>
#include <m_history.h>
#include <m_updater.h>
#include <m_metacontacts.h>
#include <m_MagneticWindows.h>
#include <m_hotkeysservice.h>
#include <m_hotkeysplus.h>
#include <m_popup.h>
#include <m_voice.h>
#include <m_voiceservice.h>
#include <m_icolib.h>
#include <m_hotkeys.h>

#include "../utils/mir_memory.h"
#include "../utils/mir_options.h"
#include "../utils/utf8_helpers.h"

#include "resource.h"
#include "m_quickcontacts.h"
#include "options.h"


#define MODULE_NAME		"QuickContacts"


// Global Variables
extern HINSTANCE hInst;
extern char *metacontacts_proto;


#define MAX_REGS(_A_) ( sizeof(_A_) / sizeof(_A_[0]) )





// Copied from "../modernb/clc.h" ///////////////////////////////////////////////////////////////////

//add a new hotkey so it has a default and can be changed in the options dialog
//wParam=0
//lParam=(LPARAM)(SKINHOTKEYDESC*)ssd;
//returns 0 on success, nonzero otherwise

typedef struct {
	int cbSize;
	const char *pszName;		   //name to refer to sound when playing and in db
	const char *pszDescription;	   //description for options dialog
    const char *pszSection;        //section name used to group sounds (NULL is acceptable)
	const char *pszService;        //Service to call when HotKey Pressed

	int DefHotKey; //default hot key for action
} SKINHOTKEYDESCEX;

#define MS_SKIN_ADDHOTKEY      "Skin/HotKeys/AddNew"
#define MS_SKIN_PLAYHOTKEY		"Skin/HotKeys/Run"



#endif // __COMMONS_H__
