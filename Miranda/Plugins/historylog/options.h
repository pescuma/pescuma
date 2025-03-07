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


#ifndef __OPTIONS_H__
# define __OPTIONS_H__


#include "commons.h"


typedef struct 
{
	BOOL disk_log_enabled;
	TCHAR filename_pattern[1024];
	BYTE ident_multiline_msgs;

	BOOL chat_disk_log_enabled;
	TCHAR chat_filename_pattern[1024];
	BYTE chat_ident_multiline_msgs;

} Options;

extern Options opts;


// Initializations needed by options
void InitOptions();

// Deinitializations needed by options
void DeInitOptions();



#endif // __OPTIONS_H__
