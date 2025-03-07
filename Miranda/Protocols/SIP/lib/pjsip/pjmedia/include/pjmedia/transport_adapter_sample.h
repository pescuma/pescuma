/* $Id: transport_adapter_sample.h 2394 2008-12-23 17:27:53Z bennylp $ */
/* 
 * Copyright (C) 2008-2009 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
#ifndef __PJMEDIA_TRANSPORT_ADAPTER_SAMPLE_H__
#define __PJMEDIA_TRANSPORT_ADAPTER_SAMPLE_H__


/**
 * @file transport_adapter_sample.h
 * @brief Sample Media Transport Adapter
 */

#include <pjmedia/transport.h>


/**
 * @defgroup PJMEDIA_TRANSPORT_ADAPTER_SAMPLE Sample Transport Adapter
 * @ingroup PJMEDIA_TRANSPORT
 * @brief Example on how to create transport adapter.
 * @{
 *
 * This describes a sample implementation of transport adapter, similar to
 * the way the SRTP transport adapter works.
 */

PJ_BEGIN_DECL


/**
 * Create the transport adapter, specifying the underlying transport to be
 * used to send and receive RTP/RTCP packets.
 *
 * @param endpt		The media endpoint.
 * @param name		Optional name to identify this media transport
 *			for logging purposes.
 * @param transport	The underlying media transport to send and receive
 *			RTP/RTCP packets.
 * @param p_tp		Pointer to receive the media transport instance.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjmedia_tp_adapter_create( pjmedia_endpt *endpt,
					        const char *name,
					        pjmedia_transport *transport,
						pjmedia_transport **p_tp);

PJ_END_DECL


/**
 * @}
 */


#endif	/* __PJMEDIA_TRANSPORT_ADAPTER_SAMPLE_H__ */


