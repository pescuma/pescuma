/* 
Copyright (C) 2008 Ricardo Pescuma Domenecci

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


#define OEMRESOURCE 
#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>
#include <richedit.h>
#include <tom.h>
#include <richole.h>
#include "flash9e.tlh"


// Disable "...truncated to '255' characters in the debug information" warnings
#pragma warning(disable: 4786)

#include <map>
#include <string>
using namespace std;


// Miranda headers
#define MIRANDA_VER 0x0700
#include <win2k.h>
#include <newpluginapi.h>
#include <m_system.h>
#include <m_system_cpp.h>
#include <m_protocols.h>
#include <m_protosvc.h>
#include <m_clist.h>
#include <m_contacts.h>
#include <m_langpack.h>
#include <m_database.h>
#include <m_options.h>
#include <m_utils.h>
#include <m_updater.h>
#include <m_metacontacts.h>
#include <m_popup.h>
#include <m_history.h>
#include <m_message.h>
#include <m_folders.h>
#include <m_icolib.h>
#include <m_avatars.h>
#include <m_imgsrvc.h>
#include <m_smileyadd.h>
#include <m_anismiley.h>
#include <anismiley.tlh>
#include <m_netlib.h>
#include <m_fontservice.h>

#include "../utils/mir_memory.h"
#include "../utils/mir_options.h"
#include "../utils/mir_icons.h"
#include "../utils/mir_buffer.h"
#include "../utils/ContactAsyncQueue.h"

#include "resource.h"
#include "m_emoticons.h"
#include "options.h"
#include "selwin.h"
#include "OleImage.h"


#define MODULE_NAME		"Emoticons"


// Global Variables
extern HINSTANCE hInst;
extern PLUGINLINK *pluginLink;
extern FI_INTERFACE *fei;
extern HANDLE hChangedEvent;

#define MAX_REGS(_A_) ( sizeof(_A_) / sizeof(_A_[0]) )
#define MIR_FREE(_X_) if (_X_ != NULL) { mir_free(_X_); _X_ = NULL; }
#define RELEASE(_X_) if (_X_ != NULL) { _X_->Release(); _X_ = NULL; }


struct EmoticonPack;
struct Module;

struct EmoticonImage
{
	EmoticonPack *pack;
	char *name;
	char *relPath;
	char *module;
	char *url;

	// For selection window
	HBITMAP img;
	BOOL transparent;
	int selectionFrame;

	EmoticonImage() : name(0), relPath(0), img(0), module(0), url(0), selectionFrame(0) {}
	~EmoticonImage();

	void Download();
	void Load();
	void Load(int &max_height, int &max_width);
	void Release();
	BOOL isAvaiable();
	BOOL isAvaiableFor(char *nodule);
};


struct Emoticon
{
	char *name;
	TCHAR *description;
	char *group;
	LIST<TCHAR> texts;
	EmoticonImage *img;
	char *service[6];

	// For selection window
	HWND tt;

	Emoticon() : name(0), description(0), group(""), texts(20), img(0), tt(0) {
		service[0] = NULL;
		service[1] = NULL;
		service[2] = NULL;
		service[3] = NULL;
		service[4] = NULL;
		service[5] = NULL;
	}
	~Emoticon();

	BOOL IgnoreFor(Module *m);

};


struct CustomEmoticon
{
	TCHAR *text;
	char *path;
	DWORD firstReceived;

	CustomEmoticon() : text(0), path(0), firstReceived(0) {}
};


struct Module
{
	char *name;
	TCHAR *path;
	LIST<Emoticon> emoticons;

	struct {
		char *proto_name;
		char *db_key;
		char *db_val;
	} derived;

	Module() : name(0), path(0), emoticons(20)  { derived.proto_name=0; derived.db_key=0; derived.db_val=0; }
	~Module();
};


struct EmoticonPack
{
	char *name;
	TCHAR *description;
	char *path;
	TCHAR *creator;
	TCHAR *updater_URL;
	LIST<EmoticonImage> images;

	EmoticonPack() : name(0), path(0), creator(0), updater_URL(0), images(20) {}
	~EmoticonPack();
};


struct RichEditCtrl
{
	HWND hwnd;
	IRichEditOle *ole;
	ITextDocument *textDocument;
	WNDPROC old_edit_proc;
	BOOL sending;

	BOOL isLoaded()
	{
		return ole != NULL;
	}
};

struct Contact
{
	HANDLE hContact;
	LIST<CustomEmoticon> emoticons;
	int lastId;

	Contact(HANDLE aHContact) : hContact(aHContact), emoticons(5), lastId(-1) {}
};

struct Dialog 
{
	HANDLE hOriginalContact;
	Contact *contact;
	Module *module;

	HWND hwnd_owner;
	WNDPROC owner_old_edit_proc;

	RichEditCtrl input;
	RichEditCtrl log;
};

extern LIST<Module> modules;
extern LIST<EmoticonPack> packs;

extern TCHAR protocolsFolder[1024];
extern TCHAR emoticonPacksFolder[1024];

// SRMM messages
#define DM_REMAKELOG         (WM_USER + 11)
#define DM_APPENDTOLOG       (WM_USER + 17)


HANDLE GetRealContact(HANDLE hContact);
Module * GetContactModule(HANDLE hContact, const char *proto = NULL);

void FillModuleImages(EmoticonPack *pack);
int CallEmoticonService(char *proto, HANDLE hContact, char *service, char *wparam, char *lparam);
BOOL EmoticonServiceExists(char *proto, char *service);



#include "EmoticonsSelectionLayout.h"
#include "SingleListEmoticons.h"
#include "GroupListEmoticons.h"


// See if a protocol service exists
static __inline int ProtoServiceExists(const char *szModule,const char *szService)
{
	char str[MAXMODULELABELLENGTH];
	strcpy(str,szModule);
	strcat(str,szService);
	return ServiceExists(str);
}


#endif // __COMMONS_H__
