/* $Id: pjsua.h 3021 2009-11-20 23:33:07Z bennylp $ */
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
#ifndef __PJSUA_H__
#define __PJSUA_H__

/**
 * @file pjsua.h
 * @brief PJSUA API.
 */


/* Include all PJSIP core headers. */
#include <pjsip.h>

/* Include all PJMEDIA headers. */
#include <pjmedia.h>

/* Include all PJMEDIA-CODEC headers. */
#include <pjmedia-codec.h>

/* Include all PJSIP-UA headers */
#include <pjsip_ua.h>

/* Include all PJSIP-SIMPLE headers */
#include <pjsip_simple.h>

/* Include all PJNATH headers */
#include <pjnath.h>

/* Include all PJLIB-UTIL headers. */
#include <pjlib-util.h>

/* Include all PJLIB headers. */
#include <pjlib.h>


PJ_BEGIN_DECL


/**
 * @defgroup PJSUA_LIB PJSUA API - High Level Softphone API
 * @brief Very high level API for constructing SIP UA applications.
 * @{
 *
 * @section pjsua_api_intro A SIP User Agent API for C/C++
 *
 * PJSUA API is very high level API for constructing SIP multimedia user agent
 * applications. It wraps together the signaling and media functionalities
 * into an easy to use call API, provides account management, buddy
 * management, presence, instant messaging, along with multimedia
 * features such as conferencing, file streaming, local playback,
 * voice recording, and so on.
 *
 * @subsection pjsua_for_c_cpp C/C++ Binding
 * Application must link with <b>pjsua-lib</b> to use this API. In addition,
 * this library depends on the following libraries:
 *  - <b>pjsip-ua</b>, 
 *  - <b>pjsip-simple</b>, 
 *  - <b>pjsip-core</b>, 
 *  - <b>pjmedia</b>,
 *  - <b>pjmedia-codec</b>, 
 *  - <b>pjlib-util</b>, and
 *  - <b>pjlib</b>, 
 *
 * so application must also link with these libraries as well. For more 
 * information, please refer to 
 * <A HREF="http://www.pjsip.org/using.htm">Getting Started with PJSIP</A>
 * page.
 *
 * @section pjsua_samples
 *
 * Few samples are provided:
 *
  - @ref page_pjsip_sample_simple_pjsuaua_c\n
    Very simple SIP User Agent with registration, call, and media, using
    PJSUA-API, all in under 200 lines of code.

  - @ref page_pjsip_samples_pjsua\n
    This is the reference implementation for PJSIP and PJMEDIA.
    PJSUA is a console based application, designed to be simple enough
    to be readble, but powerful enough to demonstrate all features
    available in PJSIP and PJMEDIA.\n

 * @section root_using_pjsua_lib Using PJSUA API
 *
 * Please refer to @ref PJSUA_LIB_BASE on how to create and initialize the API.
 * And then see the Modules on the bottom of this page for more information
 * about specific subject.
 */ 



/*****************************************************************************
 * BASE API
 */

/**
 * @defgroup PJSUA_LIB_BASE PJSUA-API Basic API
 * @ingroup PJSUA_LIB
 * @brief Basic application creation/initialization, logging configuration, etc.
 * @{
 *
 * The base PJSUA API controls PJSUA creation, initialization, and startup, and
 * also provides various auxiliary functions.
 *
 * @section using_pjsua_lib Using PJSUA Library
 *
 * @subsection creating_pjsua_lib Creating PJSUA
 *
 * Before anything else, application must create PJSUA by calling 
 * #pjsua_create().
 * This, among other things, will initialize PJLIB, which is crucial before 
 * any PJLIB functions can be called, PJLIB-UTIL, and create a SIP endpoint.
 *
 * After this function is called, application can create a memory pool (with
 * #pjsua_pool_create()) and read configurations from command line or file to
 * build the settings to initialize PJSUA below.
 *
 * @subsection init_pjsua_lib Initializing PJSUA
 *
 * After PJSUA is created, application can initialize PJSUA by calling
 * #pjsua_init(). This function takes several optional configuration settings 
 * in the argument, if application wants to set them.
 *
 * @subsubsection init_pjsua_lib_c_cpp PJSUA-LIB Initialization (in C)
 * Sample code to initialize PJSUA in C code:
 \code

 #include <pjsua-lib/pjsua.h>

 #define THIS_FILE  __FILE__

 static pj_status_t app_init(void)
 {
    pjsua_config	 ua_cfg;
    pjsua_logging_config log_cfg;
    pjsua_media_config   media_cfg;
    pj_status_t status;

    // Must create pjsua before anything else!
    status = pjsua_create();
    if (status != PJ_SUCCESS) {
	pjsua_perror(THIS_FILE, "Error initializing pjsua", status);
	return status;
    }

    // Initialize configs with default settings.
    pjsua_config_default(&ua_cfg);
    pjsua_logging_config_default(&log_cfg);
    pjsua_media_config_default(&media_cfg);

    // At the very least, application would want to override
    // the call callbacks in pjsua_config:
    ua_cfg.cb.on_incoming_call = ...
    ua_cfg.cb.on_call_state = ..
    ...

    // Customize other settings (or initialize them from application specific
    // configuration file):
    ...

    // Initialize pjsua
    status = pjsua_init(&ua_cfg, &log_cfg, &media_cfg);
    if (status != PJ_SUCCESS) {
          pjsua_perror(THIS_FILE, "Error initializing pjsua", status);
	  return status;
    }
    .
    ...
 }
 \endcode
 *
 *


 * @subsection other_init_pjsua_lib Other Initialization
 *
 * After PJSUA is initialized with #pjsua_init(), application will normally
 * need/want to perform the following tasks:
 *
 *  - create SIP transport with #pjsua_transport_create(). Application would
 *    to call #pjsua_transport_create() for each transport types that it
 *    wants to support (for example, UDP, TCP, and TLS). Please see
 *    @ref PJSUA_LIB_TRANSPORT section for more info.
 *  - create one or more SIP accounts with #pjsua_acc_add() or
 *    #pjsua_acc_add_local(). The SIP account is used for registering with
 *    the SIP server, if any. Please see @ref PJSUA_LIB_ACC for more info.
 *  - add one or more buddies with #pjsua_buddy_add(). Please see
 *    @ref PJSUA_LIB_BUDDY section for more info.
 *  - optionally configure the sound device, codec settings, and other
 *    media settings. Please see @ref PJSUA_LIB_MEDIA for more info.
 *
 *
 * @subsection starting_pjsua_lib Starting PJSUA
 *
 * After all initializations have been done, application must call
 * #pjsua_start() to start PJSUA. This function will check that all settings
 * have been properly configured, and apply default settings when they haven't,
 * or report error status when it is unable to recover from missing settings.
 *
 * Most settings can be changed during run-time. For example, application
 * may add, modify, or delete accounts, buddies, or change media settings
 * during run-time.
 *
 * @subsubsection starting_pjsua_lib_c C Example for Starting PJSUA
 * Sample code:
 \code
 static pj_status_t app_run(void)
 {
    pj_status_t status;

    // Start pjsua
    status = pjsua_start();
    if (status != PJ_SUCCESS) {
	pjsua_destroy();
	pjsua_perror(THIS_FILE, "Error starting pjsua", status);
	return status;
    }

    // Run application loop
    while (1) {
	char choice[10];
	
	printf("Select menu: ");
	fgets(choice, sizeof(choice), stdin);
	...
    }
 }
 \endcode

 */

/** Constant to identify invalid ID for all sorts of IDs. */
#define PJSUA_INVALID_ID	    (-1)

/** Call identification */
typedef int pjsua_call_id;

/** Account identification */
typedef int pjsua_acc_id;

/** Buddy identification */
typedef int pjsua_buddy_id;

/** File player identification */
typedef int pjsua_player_id;

/** File recorder identification */
typedef int pjsua_recorder_id;

/** Conference port identification */
typedef int pjsua_conf_port_id;

/** Opaque declaration for server side presence subscription */
typedef struct pjsua_srv_pres pjsua_srv_pres;

/** Forward declaration for pjsua_msg_data */
typedef struct pjsua_msg_data pjsua_msg_data;


/**
 * Maximum proxies in account.
 */
#ifndef PJSUA_ACC_MAX_PROXIES
#   define PJSUA_ACC_MAX_PROXIES    8
#endif

#if defined(PJMEDIA_HAS_SRTP) && (PJMEDIA_HAS_SRTP != 0)

/**
 * Default value of SRTP mode usage. Valid values are PJMEDIA_SRTP_DISABLED, 
 * PJMEDIA_SRTP_OPTIONAL, and PJMEDIA_SRTP_MANDATORY.
 */
#ifndef PJSUA_DEFAULT_USE_SRTP
    #define PJSUA_DEFAULT_USE_SRTP  PJMEDIA_SRTP_DISABLED
#endif

/**
 * Default value of secure signaling requirement for SRTP.
 * Valid values are:
 *	0: SRTP does not require secure signaling
 *	1: SRTP requires secure transport such as TLS
 *	2: SRTP requires secure end-to-end transport (SIPS)
 */
#ifndef PJSUA_DEFAULT_SRTP_SECURE_SIGNALING
    #define PJSUA_DEFAULT_SRTP_SECURE_SIGNALING 1
#endif

#endif

/**
 * Logging configuration, which can be (optionally) specified when calling
 * #pjsua_init(). Application must call #pjsua_logging_config_default() to
 * initialize this structure with the default values.
 */
typedef struct pjsua_logging_config
{
    /**
     * Log incoming and outgoing SIP message? Yes!
     */
    pj_bool_t	msg_logging;

    /**
     * Input verbosity level. Value 5 is reasonable.
     */
    unsigned	level;

    /**
     * Verbosity level for console. Value 4 is reasonable.
     */
    unsigned	console_level;

    /**
     * Log decoration.
     */
    unsigned	decor;

    /**
     * Optional log filename.
     */
    pj_str_t	log_filename;

    /**
     * Additional flags to be given to #pj_file_open() when opening
     * the log file. By default, the flag is PJ_O_WRONLY. Application
     * may set PJ_O_APPEND here so that logs are appended to existing
     * file instead of overwriting it.
     *
     * Default is 0.
     */
    unsigned	log_file_flags;

    /**
     * Optional callback function to be called to write log to 
     * application specific device. This function will be called for
     * log messages on input verbosity level.
     */
    void       (*cb)(int level, const char *data, int len);


} pjsua_logging_config;


/**
 * Use this function to initialize logging config.
 *
 * @param cfg	The logging config to be initialized.
 */
PJ_DECL(void) pjsua_logging_config_default(pjsua_logging_config *cfg);


/**
 * Use this function to duplicate logging config.
 *
 * @param pool	    Pool to use.
 * @param dst	    Destination config.
 * @param src	    Source config.
 */
PJ_DECL(void) pjsua_logging_config_dup(pj_pool_t *pool,
				       pjsua_logging_config *dst,
				       const pjsua_logging_config *src);


/**
 * Structure to be passed on MWI callback.
 */
typedef struct pjsua_mwi_info
{
    pjsip_evsub	    *evsub;	/**< Event subscription session, for
				     reference.				*/
    pjsip_rx_data   *rdata;	/**< The received NOTIFY request.	*/
} pjsua_mwi_info;


/**
 * This structure describes application callback to receive various event
 * notification from PJSUA-API. All of these callbacks are OPTIONAL, 
 * although definitely application would want to implement some of
 * the important callbacks (such as \a on_incoming_call).
 */
typedef struct pjsua_callback
{
    /**
     * Notify application when invite state has changed.
     * Application may then query the call info to get the
     * detail call states by calling  pjsua_call_get_info() function.
     *
     * @param call_id	The call index.
     * @param e		Event which causes the call state to change.
     */
    void (*on_call_state)(pjsua_call_id call_id, pjsip_event *e);

    /**
     * Notify application on incoming call.
     *
     * @param acc_id	The account which match the incoming call.
     * @param call_id	The call id that has just been created for
     *			the call.
     * @param rdata	The incoming INVITE request.
     */
    void (*on_incoming_call)(pjsua_acc_id acc_id, pjsua_call_id call_id,
			     pjsip_rx_data *rdata);

    /**
     * This is a general notification callback which is called whenever
     * a transaction within the call has changed state. Application can
     * implement this callback for example to monitor the state of 
     * outgoing requests, or to answer unhandled incoming requests 
     * (such as INFO) with a final response.
     *
     * @param call_id	Call identification.
     * @param tsx	The transaction which has changed state.
     * @param e		Transaction event that caused the state change.
     */
    void (*on_call_tsx_state)(pjsua_call_id call_id, 
			      pjsip_transaction *tsx,
			      pjsip_event *e);

    /**
     * Notify application when media state in the call has changed.
     * Normal application would need to implement this callback, e.g.
     * to connect the call's media to sound device. When ICE is used,
     * this callback will also be called to report ICE negotiation
     * failure.
     *
     * @param call_id	The call index.
     */
    void (*on_call_media_state)(pjsua_call_id call_id);

 
    /** 
     * Notify application when media session is created and before it is
     * registered to the conference bridge. Application may return different
     * media port if it has added media processing port to the stream. This
     * media port then will be added to the conference bridge instead.
     *
     * @param call_id	    Call identification.
     * @param sess	    Media session for the call.
     * @param stream_idx    Stream index in the media session.
     * @param p_port	    On input, it specifies the media port of the
     *			    stream. Application may modify this pointer to
     *			    point to different media port to be registered
     *			    to the conference bridge.
     */
    void (*on_stream_created)(pjsua_call_id call_id, 
			      pjmedia_session *sess,
                              unsigned stream_idx, 
			      pjmedia_port **p_port);

    /** 
     * Notify application when media session has been unregistered from the
     * conference bridge and about to be destroyed.
     *
     * @param call_id	    Call identification.
     * @param sess	    Media session for the call.
     * @param stream_idx    Stream index in the media session.
     */
    void (*on_stream_destroyed)(pjsua_call_id call_id,
                                pjmedia_session *sess, 
				unsigned stream_idx);

    /**
     * Notify application upon incoming DTMF digits.
     *
     * @param call_id	The call index.
     * @param digit	DTMF ASCII digit.
     */
    void (*on_dtmf_digit)(pjsua_call_id call_id, int digit);

    /**
     * Notify application on call being transfered (i.e. REFER is received).
     * Application can decide to accept/reject transfer request
     * by setting the code (default is 202). When this callback
     * is not defined, the default behavior is to accept the
     * transfer.
     *
     * @param call_id	The call index.
     * @param dst	The destination where the call will be 
     *			transfered to.
     * @param code	Status code to be returned for the call transfer
     *			request. On input, it contains status code 200.
     */
    void (*on_call_transfer_request)(pjsua_call_id call_id,
				     const pj_str_t *dst,
				     pjsip_status_code *code);

    /**
     * Notify application of the status of previously sent call
     * transfer request. Application can monitor the status of the
     * call transfer request, for example to decide whether to 
     * terminate existing call.
     *
     * @param call_id	    Call ID.
     * @param st_code	    Status progress of the transfer request.
     * @param st_text	    Status progress text.
     * @param final	    If non-zero, no further notification will
     *			    be reported. The st_code specified in
     *			    this callback is the final status.
     * @param p_cont	    Initially will be set to non-zero, application
     *			    can set this to FALSE if it no longer wants
     *			    to receie further notification (for example,
     *			    after it hangs up the call).
     */
    void (*on_call_transfer_status)(pjsua_call_id call_id,
				    int st_code,
				    const pj_str_t *st_text,
				    pj_bool_t final,
				    pj_bool_t *p_cont);

    /**
     * Notify application about incoming INVITE with Replaces header.
     * Application may reject the request by setting non-2xx code.
     *
     * @param call_id	    The call ID to be replaced.
     * @param rdata	    The incoming INVITE request to replace the call.
     * @param st_code	    Status code to be set by application. Application
     *			    should only return a final status (200-699).
     * @param st_text	    Optional status text to be set by application.
     */
    void (*on_call_replace_request)(pjsua_call_id call_id,
				    pjsip_rx_data *rdata,
				    int *st_code,
				    pj_str_t *st_text);

    /**
     * Notify application that an existing call has been replaced with
     * a new call. This happens when PJSUA-API receives incoming INVITE
     * request with Replaces header.
     *
     * After this callback is called, normally PJSUA-API will disconnect
     * \a old_call_id and establish \a new_call_id.
     *
     * @param old_call_id   Existing call which to be replaced with the
     *			    new call.
     * @param new_call_id   The new call.
     * @param rdata	    The incoming INVITE with Replaces request.
     */
    void (*on_call_replaced)(pjsua_call_id old_call_id,
			     pjsua_call_id new_call_id);


    /**
     * Notify application when registration status has changed.
     * Application may then query the account info to get the
     * registration details.
     *
     * @param acc_id	    Account ID.
     */
    void (*on_reg_state)(pjsua_acc_id acc_id);

    /**
     * Notification when incoming SUBSCRIBE request is received. Application
     * may use this callback to authorize the incoming subscribe request
     * (e.g. ask user permission if the request should be granted).
     *
     * If this callback is not implemented, all incoming presence subscription
     * requests will be accepted.
     *
     * If this callback is implemented, application has several choices on
     * what to do with the incoming request:
     *	- it may reject the request immediately by specifying non-200 class
     *    final response in the \a code argument.
     *	- it may immediately accept the request by specifying 200 as the
     *	  \a code argument. This is the default value if application doesn't
     *	  set any value to the \a code argument. In this case, the library
     *	  will automatically send NOTIFY request upon returning from this
     *	  callback.
     *  - it may delay the processing of the request, for example to request
     *    user permission whether to accept or reject the request. In this 
     *	  case, the application MUST set the \a code argument to 202, and 
     *	  later calls #pjsua_pres_notify() to accept or reject the 
     *	  subscription request.
     *
     * Any \a code other than 200 and 202 will be treated as 200.
     *
     * Application MUST return from this callback immediately (e.g. it must
     * not block in this callback while waiting for user confirmation).
     *
     * @param srv_pres	    Server presence subscription instance. If
     *			    application delays the acceptance of the request,
     *			    it will need to specify this object when calling
     *			    #pjsua_pres_notify().
     * @param acc_id	    Account ID most appropriate for this request.
     * @param buddy_id	    ID of the buddy matching the sender of the
     *			    request, if any, or PJSUA_INVALID_ID if no
     *			    matching buddy is found.
     * @param from	    The From URI of the request.
     * @param rdata	    The incoming request.
     * @param code	    The status code to respond to the request. The
     *			    default value is 200. Application may set this
     *			    to other final status code to accept or reject
     *			    the request.
     * @param reason	    The reason phrase to respond to the request.
     * @param msg_data	    If the application wants to send additional
     *			    headers in the response, it can put it in this
     *			    parameter.
     */
    void (*on_incoming_subscribe)(pjsua_acc_id acc_id,
				  pjsua_srv_pres *srv_pres,
				  pjsua_buddy_id buddy_id,
				  const pj_str_t *from,
				  pjsip_rx_data *rdata,
				  pjsip_status_code *code,
				  pj_str_t *reason,
				  pjsua_msg_data *msg_data);

    /**
     * Notification when server side subscription state has changed.
     * This callback is optional as application normally does not need
     * to do anything to maintain server side presence subscription.
     *
     * @param acc_id	    The account ID.
     * @param srv_pres	    Server presence subscription object.
     * @param remote_uri    Remote URI string.
     * @param state	    New subscription state.
     * @param event	    PJSIP event that triggers the state change.
     */
    void (*on_srv_subscribe_state)(pjsua_acc_id acc_id,
				   pjsua_srv_pres *srv_pres,
				   const pj_str_t *remote_uri,
				   pjsip_evsub_state state,
				   pjsip_event *event);

    /**
     * Notify application when the buddy state has changed.
     * Application may then query the buddy into to get the details.
     *
     * @param buddy_id	    The buddy id.
     */
    void (*on_buddy_state)(pjsua_buddy_id buddy_id);

    /**
     * Notify application on incoming pager (i.e. MESSAGE request).
     * Argument call_id will be -1 if MESSAGE request is not related to an
     * existing call.
     *
     * See also \a on_pager2() callback for the version with \a pjsip_rx_data
     * passed as one of the argument.
     *
     * @param call_id	    Containts the ID of the call where the IM was
     *			    sent, or PJSUA_INVALID_ID if the IM was sent
     *			    outside call context.
     * @param from	    URI of the sender.
     * @param to	    URI of the destination message.
     * @param contact	    The Contact URI of the sender, if present.
     * @param mime_type	    MIME type of the message.
     * @param body	    The message content.
     */
    void (*on_pager)(pjsua_call_id call_id, const pj_str_t *from,
		     const pj_str_t *to, const pj_str_t *contact,
		     const pj_str_t *mime_type, const pj_str_t *body);

    /**
     * This is the alternative version of the \a on_pager() callback with
     * \a pjsip_rx_data argument.
     *
     * @param call_id	    Containts the ID of the call where the IM was
     *			    sent, or PJSUA_INVALID_ID if the IM was sent
     *			    outside call context.
     * @param from	    URI of the sender.
     * @param to	    URI of the destination message.
     * @param contact	    The Contact URI of the sender, if present.
     * @param mime_type	    MIME type of the message.
     * @param body	    The message content.
     * @param rdata	    The incoming MESSAGE request.
     * @param acc_id	    Account ID most suitable for this message.
     */
    void (*on_pager2)(pjsua_call_id call_id, const pj_str_t *from,
		      const pj_str_t *to, const pj_str_t *contact,
		      const pj_str_t *mime_type, const pj_str_t *body,
		      pjsip_rx_data *rdata, pjsua_acc_id acc_id);

    /**
     * Notify application about the delivery status of outgoing pager
     * request. See also on_pager_status2() callback for the version with
     * \a pjsip_rx_data in the argument list.
     *
     * @param call_id	    Containts the ID of the call where the IM was
     *			    sent, or PJSUA_INVALID_ID if the IM was sent
     *			    outside call context.
     * @param to	    Destination URI.
     * @param body	    Message body.
     * @param user_data	    Arbitrary data that was specified when sending
     *			    IM message.
     * @param status	    Delivery status.
     * @param reason	    Delivery status reason.
     */
    void (*on_pager_status)(pjsua_call_id call_id,
			    const pj_str_t *to,
			    const pj_str_t *body,
			    void *user_data,
			    pjsip_status_code status,
			    const pj_str_t *reason);

    /**
     * Notify application about the delivery status of outgoing pager
     * request.
     *
     * @param call_id	    Containts the ID of the call where the IM was
     *			    sent, or PJSUA_INVALID_ID if the IM was sent
     *			    outside call context.
     * @param to	    Destination URI.
     * @param body	    Message body.
     * @param user_data	    Arbitrary data that was specified when sending
     *			    IM message.
     * @param status	    Delivery status.
     * @param reason	    Delivery status reason.
     * @param tdata	    The original MESSAGE request.
     * @param rdata	    The incoming MESSAGE response, or NULL if the
     *			    message transaction fails because of time out 
     *			    or transport error.
     * @param acc_id	    Account ID from this the instant message was
     *			    send.
     */
    void (*on_pager_status2)(pjsua_call_id call_id,
			     const pj_str_t *to,
			     const pj_str_t *body,
			     void *user_data,
			     pjsip_status_code status,
			     const pj_str_t *reason,
			     pjsip_tx_data *tdata,
			     pjsip_rx_data *rdata,
			     pjsua_acc_id acc_id);

    /**
     * Notify application about typing indication.
     *
     * @param call_id	    Containts the ID of the call where the IM was
     *			    sent, or PJSUA_INVALID_ID if the IM was sent
     *			    outside call context.
     * @param from	    URI of the sender.
     * @param to	    URI of the destination message.
     * @param contact	    The Contact URI of the sender, if present.
     * @param is_typing	    Non-zero if peer is typing, or zero if peer
     *			    has stopped typing a message.
     */
    void (*on_typing)(pjsua_call_id call_id, const pj_str_t *from,
		      const pj_str_t *to, const pj_str_t *contact,
		      pj_bool_t is_typing);

    /**
     * Notify application about typing indication.
     *
     * @param call_id	    Containts the ID of the call where the IM was
     *			    sent, or PJSUA_INVALID_ID if the IM was sent
     *			    outside call context.
     * @param from	    URI of the sender.
     * @param to	    URI of the destination message.
     * @param contact	    The Contact URI of the sender, if present.
     * @param is_typing	    Non-zero if peer is typing, or zero if peer
     *			    has stopped typing a message.
     * @param rdata	    The received request.
     * @param acc_id	    Account ID most suitable for this message.
     */
    void (*on_typing2)(pjsua_call_id call_id, const pj_str_t *from,
		       const pj_str_t *to, const pj_str_t *contact,
		       pj_bool_t is_typing, pjsip_rx_data *rdata,
		       pjsua_acc_id acc_id);

    /**
     * Callback when the library has finished performing NAT type
     * detection.
     *
     * @param res	    NAT detection result.
     */
    void (*on_nat_detect)(const pj_stun_nat_detect_result *res);

    /**
     * This callback is called when the call is about to resend the 
     * INVITE request to the specified target, following the previously
     * received redirection response.
     *
     * Application may accept the redirection to the specified target 
     * (the default behavior if this callback is implemented), reject 
     * this target only and make the session continue to try the next 
     * target in the list if such target exists, stop the whole
     * redirection process altogether and cause the session to be
     * disconnected, or defer the decision to ask for user confirmation.
     *
     * This callback is optional. If this callback is not implemented,
     * the default behavior is to NOT follow the redirection response.
     *
     * @param call_id	The call ID.
     * @param target	The current target to be tried.
     * @param e		The event that caused this callback to be called.
     *			This could be the receipt of 3xx response, or
     *			4xx/5xx response received for the INVITE sent to
     *			subsequent targets, or NULL if this callback is
     *			called from within #pjsua_call_process_redirect()
     *			context.
     *
     * @return		Action to be performed for the target. Set this
     *			parameter to one of the value below:
     *			- PJSIP_REDIRECT_ACCEPT: immediately accept the
     *			  redirection (default value). When set, the
     *			  call will immediately resend INVITE request
     *			  to the target.
     *			- PJSIP_REDIRECT_REJECT: immediately reject this
     *			  target. The call will continue retrying with
     *			  next target if present, or disconnect the call
     *			  if there is no more target to try.
     *			- PJSIP_REDIRECT_STOP: stop the whole redirection
     *			  process and immediately disconnect the call. The
     *			  on_call_state() callback will be called with
     *			  PJSIP_INV_STATE_DISCONNECTED state immediately
     *			  after this callback returns.
     *			- PJSIP_REDIRECT_PENDING: set to this value if
     *			  no decision can be made immediately (for example
     *			  to request confirmation from user). Application
     *			  then MUST call #pjsua_call_process_redirect()
     *			  to either accept or reject the redirection upon
     *			  getting user decision.
     */
    pjsip_redirect_op (*on_call_redirected)(pjsua_call_id call_id, 
					    const pjsip_uri *target,
					    const pjsip_event *e);

    /**
     * This callback is called when a NOTIFY request for message summary / 
     * message waiting indication is received.
     *
     * @param acc_id	The account ID.
     * @param mwi_info	Structure containing details of the event,
     *			including the received NOTIFY request in the
     *			\a rdata field.
     */
    void (*on_mwi_info)(pjsua_acc_id acc_id, pjsua_mwi_info *mwi_info);

} pjsua_callback;




/**
 * This structure describes the settings to control the API and
 * user agent behavior, and can be specified when calling #pjsua_init().
 * Before setting the values, application must call #pjsua_config_default()
 * to initialize this structure with the default values.
 */
typedef struct pjsua_config
{

    /** 
     * Maximum calls to support (default: 4). The value specified here
     * must be smaller than the compile time maximum settings 
     * PJSUA_MAX_CALLS, which by default is 32. To increase this 
     * limit, the library must be recompiled with new PJSUA_MAX_CALLS
     * value.
     */
    unsigned	    max_calls;

    /** 
     * Number of worker threads. Normally application will want to have at
     * least one worker thread, unless when it wants to poll the library
     * periodically, which in this case the worker thread can be set to
     * zero.
     */
    unsigned	    thread_cnt;

    /**
     * Number of nameservers. If no name server is configured, the SIP SRV
     * resolution would be disabled, and domain will be resolved with
     * standard pj_gethostbyname() function.
     */
    unsigned	    nameserver_count;

    /**
     * Array of nameservers to be used by the SIP resolver subsystem.
     * The order of the name server specifies the priority (first name
     * server will be used first, unless it is not reachable).
     */
    pj_str_t	    nameserver[4];

    /**
     * Force loose-route to be used in all route/proxy URIs (outbound_proxy
     * and account's proxy settings). When this setting is enabled, the
     * library will check all the route/proxy URIs specified in the settings
     * and append ";lr" parameter to the URI if the parameter is not present.
     *
     * Default: 1
     */
    pj_bool_t	    force_lr;

    /**
     * Number of outbound proxies in the \a outbound_proxy array.
     */
    unsigned	    outbound_proxy_cnt;

    /** 
     * Specify the URL of outbound proxies to visit for all outgoing requests.
     * The outbound proxies will be used for all accounts, and it will
     * be used to build the route set for outgoing requests. The final
     * route set for outgoing requests will consists of the outbound proxies
     * and the proxy configured in the account.
     */
    pj_str_t	    outbound_proxy[4];

    /**
     * Warning: deprecated, please use \a stun_srv field instead. To maintain
     * backward compatibility, if \a stun_srv_cnt is zero then the value of
     * this field will be copied to \a stun_srv field, if present.
     *
     * Specify domain name to be resolved with DNS SRV resolution to get the
     * address of the STUN server. Alternatively application may specify
     * \a stun_host instead.
     *
     * If DNS SRV resolution failed for this domain, then DNS A resolution
     * will be performed only if \a stun_host is specified.
     */
    pj_str_t	    stun_domain;

    /**
     * Warning: deprecated, please use \a stun_srv field instead. To maintain
     * backward compatibility, if \a stun_srv_cnt is zero then the value of
     * this field will be copied to \a stun_srv field, if present.
     *
     * Specify STUN server to be used, in "HOST[:PORT]" format. If port is
     * not specified, default port 3478 will be used.
     */
    pj_str_t	    stun_host;

    /**
     * Number of STUN server entries in \a stun_srv array.
     */
    unsigned	    stun_srv_cnt;

    /**
     * Array of STUN servers to try. The library will try to resolve and
     * contact each of the STUN server entry until it finds one that is
     * usable. Each entry may be a domain name, host name, IP address, and
     * it may contain an optional port number. For example:
     *	- "pjsip.org" (domain name)
     *	- "sip.pjsip.org" (host name)
     *	- "pjsip.org:33478" (domain name and a non-standard port number)
     *	- "10.0.0.1:3478" (IP address and port number)
     *
     * When nameserver is configured in the \a pjsua_config.nameserver field,
     * if entry is not an IP address, it will be resolved with DNS SRV 
     * resolution first, and it will fallback to use DNS A resolution if this
     * fails. Port number may be specified even if the entry is a domain name,
     * in case the DNS SRV resolution should fallback to a non-standard port.
     *
     * When nameserver is not configured, entries will be resolved with
     * #pj_gethostbyname() if it's not an IP address. Port number may be
     * specified if the server is not listening in standard STUN port.
     */
    pj_str_t	    stun_srv[8];

    /**
     * This specifies if the library startup should ignore failure with the
     * STUN servers. If this is set to PJ_FALSE, the library will refuse to
     * start if it fails to resolve or contact any of the STUN servers.
     *
     * Default: PJ_TRUE
     */
    pj_bool_t	    stun_ignore_failure;

    /**
     * Support for adding and parsing NAT type in the SDP to assist 
     * troubleshooting. The valid values are:
     *	- 0: no information will be added in SDP, and parsing is disabled.
     *	- 1: only the NAT type number is added.
     *	- 2: add both NAT type number and name.
     *
     * Default: 1
     */
    int		    nat_type_in_sdp;

    /**
     * Specify whether support for reliable provisional response (100rel and
     * PRACK) should be required by default. Note that this setting can be
     * further customized in account configuration (#pjsua_acc_config).
     *
     * Default: PJ_FALSE
     */
    pj_bool_t	    require_100rel;

    /**
     * Specify whether support for Session Timers should be required by 
     * default. Note that this setting can be further customized in account
     * configuration (#pjsua_acc_config).
     *
     * Default: PJ_FALSE
     */
    pj_bool_t	    require_timer;

    /**
     * Handle unsolicited NOTIFY requests containing message waiting 
     * indication (MWI) info. Unsolicited MWI is incoming NOTIFY requests 
     * which are not requested by client with SUBSCRIBE request. 
     *
     * If this is enabled, the library will respond 200/OK to the NOTIFY
     * request and forward the request to \a on_mwi_info() callback.
     *
     * See also \a mwi_enabled field #on pjsua_acc_config.
     *
     * Default: PJ_TRUE
     *
     */
    pj_bool_t	    enable_unsolicited_mwi;

    /**
     * Specify Session Timer settings, see #pjsip_timer_setting. 
     * Note that this setting can be further customized in account 
     * configuration (#pjsua_acc_config).
     */
    pjsip_timer_setting timer_setting;

    /** 
     * Number of credentials in the credential array.
     */
    unsigned	    cred_count;

    /** 
     * Array of credentials. These credentials will be used by all accounts,
     * and can be used to authenticate against outbound proxies. If the
     * credential is specific to the account, then application should set
     * the credential in the pjsua_acc_config rather than the credential
     * here.
     */
    pjsip_cred_info cred_info[PJSUA_ACC_MAX_PROXIES];

    /**
     * Application callback to receive various event notifications from
     * the library.
     */
    pjsua_callback  cb;

    /**
     * Optional user agent string (default empty). If it's empty, no
     * User-Agent header will be sent with outgoing requests.
     */
    pj_str_t	    user_agent;

#if defined(PJMEDIA_HAS_SRTP) && (PJMEDIA_HAS_SRTP != 0)
    /**
     * Specify default value of secure media transport usage. 
     * Valid values are PJMEDIA_SRTP_DISABLED, PJMEDIA_SRTP_OPTIONAL, and
     * PJMEDIA_SRTP_MANDATORY.
     *
     * Note that this setting can be further customized in account 
     * configuration (#pjsua_acc_config).
     *
     * Default: #PJSUA_DEFAULT_USE_SRTP
     */
    pjmedia_srtp_use	use_srtp;

    /**
     * Specify whether SRTP requires secure signaling to be used. This option
     * is only used when \a use_srtp option above is non-zero.
     *
     * Valid values are:
     *	0: SRTP does not require secure signaling
     *	1: SRTP requires secure transport such as TLS
     *	2: SRTP requires secure end-to-end transport (SIPS)
     *
     * Note that this setting can be further customized in account 
     * configuration (#pjsua_acc_config).
     *
     * Default: #PJSUA_DEFAULT_SRTP_SECURE_SIGNALING
     */
    int		     srtp_secure_signaling;
#endif

    /**
     * Disconnect other call legs when more than one 2xx responses for 
     * outgoing INVITE are received due to forking. Currently the library
     * is not able to handle simultaneous forked media, so disconnecting
     * the other call legs is necessary. 
     *
     * With this setting enabled, the library will handle only one of the
     * connected call leg, and the other connected call legs will be
     * disconnected. 
     *
     * Default: PJ_TRUE (only disable this setting for testing purposes).
     */
    pj_bool_t	     hangup_forked_call;

} pjsua_config;


/**
 * Use this function to initialize pjsua config.
 *
 * @param cfg	pjsua config to be initialized.
 */
PJ_DECL(void) pjsua_config_default(pjsua_config *cfg);


/** The implementation has been moved to sip_auth.h */
#define pjsip_cred_dup	pjsip_cred_info_dup


/**
 * Duplicate pjsua_config.
 *
 * @param pool	    The pool to get memory from.
 * @param dst	    Destination config.
 * @param src	    Source config.
 */
PJ_DECL(void) pjsua_config_dup(pj_pool_t *pool,
			       pjsua_config *dst,
			       const pjsua_config *src);


/**
 * This structure describes additional information to be sent with
 * outgoing SIP message. It can (optionally) be specified for example
 * with #pjsua_call_make_call(), #pjsua_call_answer(), #pjsua_call_hangup(),
 * #pjsua_call_set_hold(), #pjsua_call_send_im(), and many more.
 *
 * Application MUST call #pjsua_msg_data_init() to initialize this
 * structure before setting its values.
 */
struct pjsua_msg_data
{
    /**
     * Additional message headers as linked list. Application can add
     * headers to the list by creating the header, either from the heap/pool
     * or from temporary local variable, and add the header using
     * linked list operation. See pjsip_apps.c for some sample codes.
     */
    pjsip_hdr	hdr_list;

    /**
     * MIME type of optional message body. 
     */
    pj_str_t	content_type;

    /**
     * Optional message body.
     */
    pj_str_t	msg_body;

};


/**
 * Initialize message data.
 *
 * @param msg_data  Message data to be initialized.
 */
PJ_DECL(void) pjsua_msg_data_init(pjsua_msg_data *msg_data);


/**
 * Instantiate pjsua application. Application must call this function before
 * calling any other functions, to make sure that the underlying libraries
 * are properly initialized. Once this function has returned success,
 * application must call pjsua_destroy() before quitting.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_create(void);


/** Forward declaration */
typedef struct pjsua_media_config pjsua_media_config;


/**
 * Initialize pjsua with the specified settings. All the settings are 
 * optional, and the default values will be used when the config is not
 * specified.
 *
 * Note that #pjsua_create() MUST be called before calling this function.
 *
 * @param ua_cfg	User agent configuration.
 * @param log_cfg	Optional logging configuration.
 * @param media_cfg	Optional media configuration.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_init(const pjsua_config *ua_cfg,
				const pjsua_logging_config *log_cfg,
				const pjsua_media_config *media_cfg);


/**
 * Application is recommended to call this function after all initialization
 * is done, so that the library can do additional checking set up
 * additional 
 *
 * Application may call this function anytime after #pjsua_init().
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_start(void);


/**
 * Destroy pjsua. Application is recommended to perform graceful shutdown
 * before calling this function (such as unregister the account from the SIP 
 * server, terminate presense subscription, and hangup active calls), however,
 * this function will do all of these if it finds there are active sessions
 * that need to be terminated. This function will approximately block for
 * one second to wait for replies from remote.
 *
 * Application.may safely call this function more than once if it doesn't
 * keep track of it's state.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_destroy(void);


/**
 * Poll pjsua for events, and if necessary block the caller thread for
 * the specified maximum interval (in miliseconds).
 *
 * Application doesn't normally need to call this function if it has
 * configured worker thread (\a thread_cnt field) in pjsua_config structure,
 * because polling then will be done by these worker threads instead.
 *
 * @param msec_timeout	Maximum time to wait, in miliseconds.
 *
 * @return  The number of events that have been handled during the
 *	    poll. Negative value indicates error, and application
 *	    can retrieve the error as (status = -return_value).
 */
PJ_DECL(int) pjsua_handle_events(unsigned msec_timeout);


/**
 * Create memory pool to be used by the application. Once application
 * finished using the pool, it must be released with pj_pool_release().
 *
 * @param name		Optional pool name.
 * @param init_size	Initial size of the pool.
 * @param increment	Increment size.
 *
 * @return		The pool, or NULL when there's no memory.
 */
PJ_DECL(pj_pool_t*) pjsua_pool_create(const char *name, pj_size_t init_size,
				      pj_size_t increment);


/**
 * Application can call this function at any time (after pjsua_create(), of
 * course) to change logging settings.
 *
 * @param c		Logging configuration.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_reconfigure_logging(const pjsua_logging_config *c);


/**
 * Internal function to get SIP endpoint instance of pjsua, which is
 * needed for example to register module, create transports, etc.
 * Only valid after #pjsua_init() is called.
 * 
 * @return		SIP endpoint instance.
 */
PJ_DECL(pjsip_endpoint*) pjsua_get_pjsip_endpt(void);

/**
 * Internal function to get media endpoint instance.
 * Only valid after #pjsua_init() is called.
 *
 * @return		Media endpoint instance.
 */
PJ_DECL(pjmedia_endpt*) pjsua_get_pjmedia_endpt(void);

/**
 * Internal function to get PJSUA pool factory.
 * Only valid after #pjsua_create() is called.
 *
 * @return		Pool factory currently used by PJSUA.
 */
PJ_DECL(pj_pool_factory*) pjsua_get_pool_factory(void);



/*****************************************************************************
 * Utilities.
 *
 */

/**
 * This structure is used to represent the result of the STUN server 
 * resolution and testing, the #pjsua_resolve_stun_servers() function.
 * This structure will be passed in #pj_stun_resolve_cb callback.
 */
typedef struct pj_stun_resolve_result
{
    /**
     * Arbitrary data that was passed to #pjsua_resolve_stun_servers()
     * function.
     */
    void	    *token;

    /**
     * This will contain PJ_SUCCESS if at least one usable STUN server
     * is found, otherwise it will contain the last error code during
     * the operation.
     */
    pj_status_t	     status;

    /**
     * The server name that yields successful result. This will only
     * contain value if status is successful.
     */
    pj_str_t	     name;

    /**
     * The server IP address. This will only contain value if status 
     * is successful.
     */
    pj_sockaddr	     addr;

} pj_stun_resolve_result;


/**
 * Typedef of callback to be registered to #pjsua_resolve_stun_servers().
 */
typedef void (*pj_stun_resolve_cb)(const pj_stun_resolve_result *result);

/**
 * This is a utility function to detect NAT type in front of this
 * endpoint. Once invoked successfully, this function will complete 
 * asynchronously and report the result in \a on_nat_detect() callback
 * of pjsua_callback.
 *
 * After NAT has been detected and the callback is called, application can
 * get the detected NAT type by calling #pjsua_get_nat_type(). Application
 * can also perform NAT detection by calling #pjsua_detect_nat_type()
 * again at later time.
 *
 * Note that STUN must be enabled to run this function successfully.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_detect_nat_type(void);


/**
 * Get the NAT type as detected by #pjsua_detect_nat_type() function.
 * This function will only return useful NAT type after #pjsua_detect_nat_type()
 * has completed successfully and \a on_nat_detect() callback has been called.
 *
 * @param type		NAT type.
 *
 * @return		When detection is in progress, this function will 
 *			return PJ_EPENDING and \a type will be set to 
 *			PJ_STUN_NAT_TYPE_UNKNOWN. After NAT type has been
 *			detected successfully, this function will return
 *			PJ_SUCCESS and \a type will be set to the correct
 *			value. Other return values indicate error and
 *			\a type will be set to PJ_STUN_NAT_TYPE_ERR_UNKNOWN.
 *
 * @see pjsua_call_get_rem_nat_type()
 */
PJ_DECL(pj_status_t) pjsua_get_nat_type(pj_stun_nat_type *type);


/**
 * Auxiliary function to resolve and contact each of the STUN server
 * entries (sequentially) to find which is usable. The #pjsua_init() must
 * have been called before calling this function.
 *
 * @param count		Number of STUN server entries to try.
 * @param srv		Array of STUN server entries to try. Please see
 *			the \a stun_srv field in the #pjsua_config 
 *			documentation about the format of this entry.
 * @param wait		Specify non-zero to make the function block until
 *			it gets the result. In this case, the function
 *			will block while the resolution is being done,
 *			and the callback will be called before this function
 *			returns.
 * @param token		Arbitrary token to be passed back to application
 *			in the callback.
 * @param cb		Callback to be called to notify the result of
 *			the function.
 *
 * @return		If \a wait parameter is non-zero, this will return
 *			PJ_SUCCESS if one usable STUN server is found.
 *			Otherwise it will always return PJ_SUCCESS, and
 *			application will be notified about the result in
 *			the callback.
 */
PJ_DECL(pj_status_t) pjsua_resolve_stun_servers(unsigned count,
						pj_str_t srv[],
						pj_bool_t wait,
						void *token,
						pj_stun_resolve_cb cb);

/**
 * Cancel pending STUN resolution which match the specified token. 
 *
 * @param token		The token to match. This token was given to 
 *			#pjsua_resolve_stun_servers()
 * @param notify_cb	Boolean to control whether the callback should
 *			be called for cancelled resolutions. When the
 *			callback is called, the status in the result
 *			will be set as PJ_ECANCELLED.
 *
 * @return		PJ_SUCCESS if there is at least one pending STUN
 *			resolution cancelled, or PJ_ENOTFOUND if there is
 *			no matching one, or other error.
 */
PJ_DECL(pj_status_t) pjsua_cancel_stun_resolution(void *token,
						  pj_bool_t notify_cb);


/**
 * This is a utility function to verify that valid SIP url is given. If the
 * URL is valid, PJ_SUCCESS will be returned.
 *
 * @param url		The URL, as NULL terminated string.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_verify_sip_url(const char *url);


/**
 * Schedule a timer entry. Note that the timer callback may be executed
 * by different thread, depending on whether worker thread is enabled or
 * not.
 *
 * @param entry		Timer heap entry.
 * @param delay     The interval to expire.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 *
 * @see pjsip_endpt_schedule_timer()
 */
PJ_DECL(pj_status_t) pjsua_schedule_timer(pj_timer_entry *entry,
					  const pj_time_val *delay);


/**
 * Cancel the previously scheduled timer.
 *
 * @param entry		Timer heap entry.
 *
 * @see pjsip_endpt_cancel_timer()
 */
PJ_DECL(void) pjsua_cancel_timer(pj_timer_entry *entry);


/**
 * This is a utility function to display error message for the specified 
 * error code. The error message will be sent to the log.
 *
 * @param sender	The log sender field.
 * @param title		Message title for the error.
 * @param status	Status code.
 */
PJ_DECL(void) pjsua_perror(const char *sender, const char *title, 
			   pj_status_t status);


/**
 * This is a utility function to dump the stack states to log, using
 * verbosity level 3.
 *
 * @param detail	Will print detailed output (such as list of
 *			SIP transactions) when non-zero.
 */
PJ_DECL(void) pjsua_dump(pj_bool_t detail);

/**
 * @}
 */



/*****************************************************************************
 * TRANSPORT API
 */

/**
 * @defgroup PJSUA_LIB_TRANSPORT PJSUA-API Signaling Transport
 * @ingroup PJSUA_LIB
 * @brief API for managing SIP transports
 * @{
 *
 * PJSUA-API supports creating multiple transport instances, for example UDP,
 * TCP, and TLS transport. SIP transport must be created before adding an 
 * account.
 */


/** SIP transport identification.
 */
typedef int pjsua_transport_id;


/**
 * Transport configuration for creating transports for both SIP
 * and media. Before setting some values to this structure, application
 * MUST call #pjsua_transport_config_default() to initialize its
 * values with default settings.
 */
typedef struct pjsua_transport_config
{
    /**
     * UDP port number to bind locally. This setting MUST be specified
     * even when default port is desired. If the value is zero, the
     * transport will be bound to any available port, and application
     * can query the port by querying the transport info.
     */
    unsigned		port;

    /**
     * Optional address to advertise as the address of this transport.
     * Application can specify any address or hostname for this field,
     * for example it can point to one of the interface address in the
     * system, or it can point to the public address of a NAT router
     * where port mappings have been configured for the application.
     *
     * Note: this option can be used for both UDP and TCP as well!
     */
    pj_str_t		public_addr;

    /**
     * Optional address where the socket should be bound to. This option
     * SHOULD only be used to selectively bind the socket to particular
     * interface (instead of 0.0.0.0), and SHOULD NOT be used to set the
     * published address of a transport (the public_addr field should be
     * used for that purpose).
     *
     * Note that unlike public_addr field, the address (or hostname) here 
     * MUST correspond to the actual interface address in the host, since
     * this address will be specified as bind() argument.
     */
    pj_str_t		bound_addr;

    /**
     * This specifies TLS settings for TLS transport. It is only be used
     * when this transport config is being used to create a SIP TLS
     * transport.
     */
    pjsip_tls_setting	tls_setting;

    /**
     * QoS traffic type to be set on this transport. When application wants
     * to apply QoS tagging to the transport, it's preferable to set this
     * field rather than \a qos_param fields since this is more portable.
     *
     * Default is QoS not set.
     */
    pj_qos_type		qos_type;

    /**
     * Set the low level QoS parameters to the transport. This is a lower
     * level operation than setting the \a qos_type field and may not be
     * supported on all platforms.
     *
     * Default is QoS not set.
     */
    pj_qos_params	qos_params;

} pjsua_transport_config;


/**
 * Call this function to initialize UDP config with default values.
 *
 * @param cfg	    The UDP config to be initialized.
 */
PJ_DECL(void) pjsua_transport_config_default(pjsua_transport_config *cfg);


/**
 * Duplicate transport config.
 *
 * @param pool		The pool.
 * @param dst		The destination config.
 * @param src		The source config.
 */
PJ_DECL(void) pjsua_transport_config_dup(pj_pool_t *pool,
					 pjsua_transport_config *dst,
					 const pjsua_transport_config *src);


/**
 * This structure describes transport information returned by
 * #pjsua_transport_get_info() function.
 */
typedef struct pjsua_transport_info
{
    /**
     * PJSUA transport identification.
     */
    pjsua_transport_id	    id;

    /**
     * Transport type.
     */
    pjsip_transport_type_e  type;

    /**
     * Transport type name.
     */
    pj_str_t		    type_name;

    /**
     * Transport string info/description.
     */
    pj_str_t		    info;

    /**
     * Transport flag (see ##pjsip_transport_flags_e).
     */
    unsigned		    flag;

    /**
     * Local address length.
     */
    unsigned		    addr_len;

    /**
     * Local/bound address.
     */
    pj_sockaddr		    local_addr;

    /**
     * Published address (or transport address name).
     */
    pjsip_host_port	    local_name;

    /**
     * Current number of objects currently referencing this transport.
     */
    unsigned		    usage_count;


} pjsua_transport_info;


/**
 * Create and start a new SIP transport according to the specified
 * settings.
 *
 * @param type		Transport type.
 * @param cfg		Transport configuration.
 * @param p_id		Optional pointer to receive transport ID.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_transport_create(pjsip_transport_type_e type,
					    const pjsua_transport_config *cfg,
					    pjsua_transport_id *p_id);

/**
 * Register transport that has been created by application. This function
 * is useful if application wants to implement custom SIP transport and use
 * it with pjsua.
 *
 * @param tp		Transport instance.
 * @param p_id		Optional pointer to receive transport ID.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_transport_register(pjsip_transport *tp,
					      pjsua_transport_id *p_id);


/**
 * Enumerate all transports currently created in the system. This function
 * will return all transport IDs, and application may then call 
 * #pjsua_transport_get_info() function to retrieve detailed information
 * about the transport.
 *
 * @param id		Array to receive transport ids.
 * @param count		In input, specifies the maximum number of elements.
 *			On return, it contains the actual number of elements.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_enum_transports( pjsua_transport_id id[],
					    unsigned *count );


/**
 * Get information about transports.
 *
 * @param id		Transport ID.
 * @param info		Pointer to receive transport info.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_transport_get_info(pjsua_transport_id id,
					      pjsua_transport_info *info);


/**
 * Disable a transport or re-enable it. By default transport is always 
 * enabled after it is created. Disabling a transport does not necessarily
 * close the socket, it will only discard incoming messages and prevent
 * the transport from being used to send outgoing messages.
 *
 * @param id		Transport ID.
 * @param enabled	Non-zero to enable, zero to disable.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_transport_set_enable(pjsua_transport_id id,
						pj_bool_t enabled);


/**
 * Close the transport. If transport is forcefully closed, it will be
 * immediately closed, and any pending transactions that are using the
 * transport may not terminate properly (it may even crash). Otherwise, 
 * the system will wait until all transactions are closed while preventing 
 * new users from using the transport, and will close the transport when 
 * it is safe to do so.
 *
 * @param id		Transport ID.
 * @param force		Non-zero to immediately close the transport. This
 *			is not recommended!
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_transport_close( pjsua_transport_id id,
					    pj_bool_t force );

/**
 * @}
 */




/*****************************************************************************
 * ACCOUNT API
 */


/**
 * @defgroup PJSUA_LIB_ACC PJSUA-API Accounts Management
 * @ingroup PJSUA_LIB
 * @brief PJSUA Accounts management
 * @{
 *
 * PJSUA accounts provide identity (or identities) of the user who is currently
 * using the application. In SIP terms, the identity is used as the <b>From</b>
 * header in outgoing requests.
 *
 * PJSUA-API supports creating and managing multiple accounts. The maximum
 * number of accounts is limited by a compile time constant
 * <tt>PJSUA_MAX_ACC</tt>.
 *
 * Account may or may not have client registration associated with it.
 * An account is also associated with <b>route set</b> and some <b>authentication
 * credentials</b>, which are used when sending SIP request messages using the
 * account. An account also has presence's <b>online status</b>, which
 * will be reported to remote peer when they subscribe to the account's
 * presence, or which is published to a presence server if presence 
 * publication is enabled for the account.
 *
 * At least one account MUST be created in the application. If no user
 * association is required, application can create a userless account by
 * calling #pjsua_acc_add_local(). A userless account identifies local endpoint
 * instead of a particular user, and it correspond with a particular
 * transport instance.
 *
 * Also one account must be set as the <b>default account</b>, which is used as
 * the account to use when PJSUA fails to match a request with any other
 * accounts.
 *
 * When sending outgoing SIP requests (such as making calls or sending
 * instant messages), normally PJSUA requires the application to specify
 * which account to use for the request. If no account is specified,
 * PJSUA may be able to select the account by matching the destination
 * domain name, and fall back to default account when no match is found.
 */

/**
 * Maximum accounts.
 */
#ifndef PJSUA_MAX_ACC
#   define PJSUA_MAX_ACC	    8
#endif


/**
 * Default registration interval.
 */
#ifndef PJSUA_REG_INTERVAL
#   define PJSUA_REG_INTERVAL	    300
#endif


/**
 * Default maximum time to wait for account unregistration transactions to
 * complete during library shutdown sequence.
 *
 * Default: 4000 (4 seconds)
 */
#ifndef PJSUA_UNREG_TIMEOUT
#   define PJSUA_UNREG_TIMEOUT	    4000
#endif


/**
 * Default PUBLISH expiration
 */
#ifndef PJSUA_PUBLISH_EXPIRATION
#   define PJSUA_PUBLISH_EXPIRATION PJSIP_PUBC_EXPIRATION_NOT_SPECIFIED
#endif


/**
 * Default account priority.
 */
#ifndef PJSUA_DEFAULT_ACC_PRIORITY
#   define PJSUA_DEFAULT_ACC_PRIORITY	0
#endif


/**
 * This macro specifies the URI scheme to use in Contact header
 * when secure transport such as TLS is used. Application can specify
 * either "sip" or "sips".
 */
#ifndef PJSUA_SECURE_SCHEME
#   define PJSUA_SECURE_SCHEME		"sip"
#endif


/**
 * Maximum time to wait for unpublication transaction(s) to complete
 * during shutdown process, before sending unregistration. The library
 * tries to wait for the unpublication (un-PUBLISH) to complete before
 * sending REGISTER request to unregister the account, during library
 * shutdown process. If the value is set too short, it is possible that
 * the unregistration is sent before unpublication completes, causing
 * unpublication request to fail.
 *
 * Default: 2000 (2 seconds)
 */
#ifndef PJSUA_UNPUBLISH_MAX_WAIT_TIME_MSEC
#   define PJSUA_UNPUBLISH_MAX_WAIT_TIME_MSEC	2000
#endif


/**
 * This structure describes account configuration to be specified when
 * adding a new account with #pjsua_acc_add(). Application MUST initialize
 * this structure first by calling #pjsua_acc_config_default().
 */
typedef struct pjsua_acc_config
{
    /**
     * Arbitrary user data to be associated with the newly created account.
     * Application may set this later with #pjsua_acc_set_user_data() and
     * retrieve it with #pjsua_acc_get_user_data().
     */
    void	   *user_data;

    /**
     * Account priority, which is used to control the order of matching
     * incoming/outgoing requests. The higher the number means the higher
     * the priority is, and the account will be matched first.
     */
    int		    priority;

    /** 
     * The full SIP URL for the account. The value can take name address or 
     * URL format, and will look something like "sip:account@serviceprovider".
     *
     * This field is mandatory.
     */
    pj_str_t	    id;

    /** 
     * This is the URL to be put in the request URI for the registration,
     * and will look something like "sip:serviceprovider".
     *
     * This field should be specified if registration is desired. If the
     * value is empty, no account registration will be performed.
     */
    pj_str_t	    reg_uri;

    /**
     * Subscribe to message waiting indication events (RFC 3842).
     *
     * See also \a enable_unsolicited_mwi field on #pjsua_config.
     *
     * Default: no
     */
    pj_bool_t	    mwi_enabled;

    /**
     * If this flag is set, the presence information of this account will
     * be PUBLISH-ed to the server where the account belongs.
     *
     * Default: PJ_FALSE
     */
    pj_bool_t	    publish_enabled;

    /**
     * Event publication options.
     */
    pjsip_publishc_opt	publish_opt;

    /**
     * Maximum time to wait for unpublication transaction(s) to complete
     * during shutdown process, before sending unregistration. The library
     * tries to wait for the unpublication (un-PUBLISH) to complete before
     * sending REGISTER request to unregister the account, during library
     * shutdown process. If the value is set too short, it is possible that
     * the unregistration is sent before unpublication completes, causing
     * unpublication request to fail.
     *
     * Default: PJSUA_UNPUBLISH_MAX_WAIT_TIME_MSEC
     */
    unsigned	    unpublish_max_wait_time_msec;

    /**
     * Authentication preference.
     */
    pjsip_auth_clt_pref auth_pref;

    /**
     * Optional PIDF tuple ID for outgoing PUBLISH and NOTIFY. If this value
     * is not specified, a random string will be used.
     */
    pj_str_t	    pidf_tuple_id;

    /** 
     * Optional URI to be put as Contact for this account. It is recommended
     * that this field is left empty, so that the value will be calculated
     * automatically based on the transport address.
     */
    pj_str_t	    force_contact;

    /**
     * Additional parameters that will be appended in the Contact header
     * for this account. This will affect the Contact header in all SIP 
     * messages sent on behalf of this account, including but not limited to
     * REGISTER, INVITE, and SUBCRIBE requests or responses.
     *
     * The parameters should be preceeded by semicolon, and all strings must
     * be properly escaped. Example:
     *	 ";my-param=X;another-param=Hi%20there"
     */
    pj_str_t	    contact_params;

    /**
     * Additional URI parameters that will be appended in the Contact URI
     * for this account. This will affect the Contact URI in all SIP
     * messages sent on behalf of this account, including but not limited to
     * REGISTER, INVITE, and SUBCRIBE requests or responses.
     *
     * The parameters should be preceeded by semicolon, and all strings must
     * be properly escaped. Example:
     *	 ";my-param=X;another-param=Hi%20there"
     */
    pj_str_t	    contact_uri_params;

    /**
     * Specify whether support for reliable provisional response (100rel and
     * PRACK) should be required for all sessions of this account.
     *
     * Default: PJ_FALSE
     */
    pj_bool_t	    require_100rel;

    /**
     * Specify whether support for Session Timers should be required for all 
     * sessions of this account.
     *
     * Default: PJ_FALSE
     */
    pj_bool_t	    require_timer;

    /**
     * Specify Session Timer settings, see #pjsip_timer_setting. 
     */
    pjsip_timer_setting timer_setting;

    /**
     * Number of proxies in the proxy array below.
     */
    unsigned	    proxy_cnt;

    /** 
     * Optional URI of the proxies to be visited for all outgoing requests 
     * that are using this account (REGISTER, INVITE, etc). Application need 
     * to specify these proxies if the service provider requires that requests
     * destined towards its network should go through certain proxies first
     * (for example, border controllers).
     *
     * These proxies will be put in the route set for this account, with 
     * maintaining the orders (the first proxy in the array will be visited
     * first). If global outbound proxies are configured in pjsua_config,
     * then these account proxies will be placed after the global outbound
     * proxies in the routeset.
     */
    pj_str_t	    proxy[PJSUA_ACC_MAX_PROXIES];

    /** 
     * Optional interval for registration, in seconds. If the value is zero, 
     * default interval will be used (PJSUA_REG_INTERVAL, 300 seconds).
     */
    unsigned	    reg_timeout;

    /**
     * Specify the maximum time to wait for unregistration requests to
     * complete during library shutdown sequence.
     *
     * Default: PJSUA_UNREG_TIMEOUT
     */
    unsigned	    unreg_timeout;

    /** 
     * Number of credentials in the credential array.
     */
    unsigned	    cred_count;

    /** 
     * Array of credentials. If registration is desired, normally there should
     * be at least one credential specified, to successfully authenticate
     * against the service provider. More credentials can be specified, for
     * example when the requests are expected to be challenged by the
     * proxies in the route set.
     */
    pjsip_cred_info cred_info[PJSUA_ACC_MAX_PROXIES];

    /**
     * Optionally bind this account to specific transport. This normally is
     * not a good idea, as account should be able to send requests using
     * any available transports according to the destination. But some
     * application may want to have explicit control over the transport to
     * use, so in that case it can set this field.
     *
     * Default: -1 (PJSUA_INVALID_ID)
     *
     * @see pjsua_acc_set_transport()
     */
    pjsua_transport_id  transport_id;

    /**
     * This option is used to update the UDP transport address and the Contact
     * header of REGISTER request. When this option is  enabled, the library 
     * will keep track of the public IP address from the response of REGISTER
     * request. Once it detects that the address has changed, it will 
     * unregister current Contact, update the Contact with transport address
     * learned from Via header, and register a new Contact to the registrar.
     * This will also update the public name of UDP transport if STUN is
     * configured.
     *
     * Default: 1 (yes)
     */
    pj_bool_t allow_contact_rewrite;

    /**
     * Set the interval for periodic keep-alive transmission for this account.
     * If this value is zero, keep-alive will be disabled for this account.
     * The keep-alive transmission will be sent to the registrar's address,
     * after successful registration.
     *
     * Default: 15 (seconds)
     */
    unsigned	     ka_interval;

    /**
     * Specify the data to be transmitted as keep-alive packets.
     *
     * Default: CR-LF
     */
    pj_str_t	     ka_data;

#if defined(PJMEDIA_HAS_SRTP) && (PJMEDIA_HAS_SRTP != 0)
    /**
     * Specify whether secure media transport should be used for this account.
     * Valid values are PJMEDIA_SRTP_DISABLED, PJMEDIA_SRTP_OPTIONAL, and
     * PJMEDIA_SRTP_MANDATORY.
     *
     * Default: #PJSUA_DEFAULT_USE_SRTP
     */
    pjmedia_srtp_use	use_srtp;

    /**
     * Specify whether SRTP requires secure signaling to be used. This option
     * is only used when \a use_srtp option above is non-zero.
     *
     * Valid values are:
     *	0: SRTP does not require secure signaling
     *	1: SRTP requires secure transport such as TLS
     *	2: SRTP requires secure end-to-end transport (SIPS)
     *
     * Default: #PJSUA_DEFAULT_SRTP_SECURE_SIGNALING
     */
    int		     srtp_secure_signaling;
#endif

} pjsua_acc_config;


/**
 * Call this function to initialize account config with default values.
 *
 * @param cfg	    The account config to be initialized.
 */
PJ_DECL(void) pjsua_acc_config_default(pjsua_acc_config *cfg);


/**
 * Duplicate account config.
 *
 * @param pool	    Pool to be used for duplicating the config.
 * @param dst	    Destination configuration.
 * @param src	    Source configuration.
 */
PJ_DECL(void) pjsua_acc_config_dup(pj_pool_t *pool,
				   pjsua_acc_config *dst,
				   const pjsua_acc_config *src);


/**
 * Account info. Application can query account info by calling 
 * #pjsua_acc_get_info().
 */
typedef struct pjsua_acc_info
{
    /** 
     * The account ID. 
     */
    pjsua_acc_id	id;

    /**
     * Flag to indicate whether this is the default account.
     */
    pj_bool_t		is_default;

    /** 
     * Account URI 
     */
    pj_str_t		acc_uri;

    /** 
     * Flag to tell whether this account has registration setting
     * (reg_uri is not empty).
     */
    pj_bool_t		has_registration;

    /**
     * An up to date expiration interval for account registration session.
     */
    int			expires;

    /**
     * Last registration status code. If status code is zero, the account
     * is currently not registered. Any other value indicates the SIP
     * status code of the registration.
     */
    pjsip_status_code	status;

    /**
     * String describing the registration status.
     */
    pj_str_t		status_text;

    /**
     * Presence online status for this account.
     */
    pj_bool_t		online_status;

    /**
     * Presence online status text.
     */
    pj_str_t		online_status_text;

    /**
     * Extended RPID online status information.
     */
    pjrpid_element	rpid;

    /**
     * Buffer that is used internally to store the status text.
     */
    char		buf_[PJ_ERR_MSG_SIZE];

} pjsua_acc_info;



/**
 * Get number of current accounts.
 *
 * @return		Current number of accounts.
 */
PJ_DECL(unsigned) pjsua_acc_get_count(void);


/**
 * Check if the specified account ID is valid.
 *
 * @param acc_id	Account ID to check.
 *
 * @return		Non-zero if account ID is valid.
 */
PJ_DECL(pj_bool_t) pjsua_acc_is_valid(pjsua_acc_id acc_id);


/**
 * Set default account to be used when incoming and outgoing
 * requests doesn't match any accounts.
 *
 * @param acc_id	The account ID to be used as default.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsua_acc_set_default(pjsua_acc_id acc_id);


/**
 * Get default account to be used when receiving incoming requests (calls),
 * when the destination of the incoming call doesn't match any other
 * accounts.
 *
 * @return		The default account ID, or PJSUA_INVALID_ID if no
 *			default account is configured.
 */
PJ_DECL(pjsua_acc_id) pjsua_acc_get_default(void);


/**
 * Add a new account to pjsua. PJSUA must have been initialized (with
 * #pjsua_init()) before calling this function. If registration is configured
 * for this account, this function would also start the SIP registration
 * session with the SIP registrar server. This SIP registration session
 * will be maintained internally by the library, and application doesn't
 * need to do anything to maintain the registration session.
 *
 *
 * @param acc_cfg	Account configuration.
 * @param is_default	If non-zero, this account will be set as the default
 *			account. The default account will be used when sending
 *			outgoing requests (e.g. making call) when no account is
 *			specified, and when receiving incoming requests when the
 *			request does not match any accounts. It is recommended
 *			that default account is set to local/LAN account.
 * @param p_acc_id	Pointer to receive account ID of the new account.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_add(const pjsua_acc_config *acc_cfg,
				   pj_bool_t is_default,
				   pjsua_acc_id *p_acc_id);


/**
 * Add a local account. A local account is used to identify local endpoint
 * instead of a specific user, and for this reason, a transport ID is needed
 * to obtain the local address information.
 *
 * @param tid		Transport ID to generate account address.
 * @param is_default	If non-zero, this account will be set as the default
 *			account. The default account will be used when sending
 *			outgoing requests (e.g. making call) when no account is
 *			specified, and when receiving incoming requests when the
 *			request does not match any accounts. It is recommended
 *			that default account is set to local/LAN account.
 * @param p_acc_id	Pointer to receive account ID of the new account.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_add_local(pjsua_transport_id tid,
					 pj_bool_t is_default,
					 pjsua_acc_id *p_acc_id);

/**
 * Set arbitrary data to be associated with the account.
 *
 * @param acc_id	The account ID.
 * @param user_data	User/application data.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_set_user_data(pjsua_acc_id acc_id,
					     void *user_data);


/**
 * Retrieve arbitrary data associated with the account.
 *
 * @param acc_id	The account ID.
 *
 * @return		The user data. In the case where the account ID is
 *			not valid, NULL is returned.
 */
PJ_DECL(void*) pjsua_acc_get_user_data(pjsua_acc_id acc_id);


/**
 * Delete an account. This will unregister the account from the SIP server,
 * if necessary, and terminate server side presence subscriptions associated
 * with this account.
 *
 * @param acc_id	Id of the account to be deleted.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_del(pjsua_acc_id acc_id);


/**
 * Modify account information.
 *
 * @param acc_id	Id of the account to be modified.
 * @param acc_cfg	New account configuration.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_modify(pjsua_acc_id acc_id,
				      const pjsua_acc_config *acc_cfg);


/**
 * Modify account's presence status to be advertised to remote/presence
 * subscribers. This would trigger the sending of outgoing NOTIFY request
 * if there are server side presence subscription for this account, and/or
 * outgoing PUBLISH if presence publication is enabled for this account.
 *
 * @see pjsua_acc_set_online_status2()
 *
 * @param acc_id	The account ID.
 * @param is_online	True of false.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_set_online_status(pjsua_acc_id acc_id,
						 pj_bool_t is_online);

/**
 * Modify account's presence status to be advertised to remote/presence
 * subscribers. This would trigger the sending of outgoing NOTIFY request
 * if there are server side presence subscription for this account, and/or
 * outgoing PUBLISH if presence publication is enabled for this account.
 *
 * @see pjsua_acc_set_online_status()
 *
 * @param acc_id	The account ID.
 * @param is_online	True of false.
 * @param pr		Extended information in subset of RPID format
 *			which allows setting custom presence text.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_set_online_status2(pjsua_acc_id acc_id,
						  pj_bool_t is_online,
						  const pjrpid_element *pr);

/**
 * Update registration or perform unregistration. If registration is
 * configured for this account, then initial SIP REGISTER will be sent
 * when the account is added with #pjsua_acc_add(). Application normally
 * only need to call this function if it wants to manually update the
 * registration or to unregister from the server.
 *
 * @param acc_id	The account ID.
 * @param renew		If renew argument is zero, this will start 
 *			unregistration process.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_set_registration(pjsua_acc_id acc_id, 
						pj_bool_t renew);

/**
 * Get information about the specified account.
 *
 * @param acc_id	Account identification.
 * @param info		Pointer to receive account information.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_get_info(pjsua_acc_id acc_id,
					pjsua_acc_info *info);


/**
 * Enumerate all account currently active in the library. This will fill
 * the array with the account Ids, and application can then query the
 * account information for each id with #pjsua_acc_get_info().
 *
 * @see pjsua_acc_enum_info().
 *
 * @param ids		Array of account IDs to be initialized.
 * @param count		In input, specifies the maximum number of elements.
 *			On return, it contains the actual number of elements.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_enum_accs(pjsua_acc_id ids[],
				     unsigned *count );


/**
 * Enumerate account informations.
 *
 * @param info		Array of account infos to be initialized.
 * @param count		In input, specifies the maximum number of elements.
 *			On return, it contains the actual number of elements.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_enum_info( pjsua_acc_info info[],
					  unsigned *count );


/**
 * This is an internal function to find the most appropriate account to
 * used to reach to the specified URL.
 *
 * @param url		The remote URL to reach.
 *
 * @return		Account id.
 */
PJ_DECL(pjsua_acc_id) pjsua_acc_find_for_outgoing(const pj_str_t *url);


/**
 * This is an internal function to find the most appropriate account to be
 * used to handle incoming calls.
 *
 * @param rdata		The incoming request message.
 *
 * @return		Account id.
 */
PJ_DECL(pjsua_acc_id) pjsua_acc_find_for_incoming(pjsip_rx_data *rdata);


/**
 * Create arbitrary requests using the account. Application should only use
 * this function to create auxiliary requests outside dialog, such as
 * OPTIONS, and use the call or presence API to create dialog related
 * requests.
 *
 * @param acc_id	The account ID.
 * @param method	The SIP method of the request.
 * @param target	Target URI.
 * @param p_tdata	Pointer to receive the request.
 *
 * @return		PJ_SUCCESS or the error code.
 */
PJ_DECL(pj_status_t) pjsua_acc_create_request(pjsua_acc_id acc_id,
					      const pjsip_method *method,
					      const pj_str_t *target,
					      pjsip_tx_data **p_tdata);


/**
 * Create a suitable Contact header value, based on the specified target URI 
 * for the specified account.
 *
 * @param pool		Pool to allocate memory for the string.
 * @param contact	The string where the Contact will be stored.
 * @param acc_id	Account ID.
 * @param uri		Destination URI of the request.
 *
 * @return		PJ_SUCCESS on success, other on error.
 */
PJ_DECL(pj_status_t) pjsua_acc_create_uac_contact( pj_pool_t *pool,
						   pj_str_t *contact,
						   pjsua_acc_id acc_id,
						   const pj_str_t *uri);
							   


/**
 * Create a suitable Contact header value, based on the information in the 
 * incoming request.
 *
 * @param pool		Pool to allocate memory for the string.
 * @param contact	The string where the Contact will be stored.
 * @param acc_id	Account ID.
 * @param rdata		Incoming request.
 *
 * @return		PJ_SUCCESS on success, other on error.
 */
PJ_DECL(pj_status_t) pjsua_acc_create_uas_contact( pj_pool_t *pool,
						   pj_str_t *contact,
						   pjsua_acc_id acc_id,
						   pjsip_rx_data *rdata );
							   

/**
 * Lock/bind this account to a specific transport/listener. Normally
 * application shouldn't need to do this, as transports will be selected
 * automatically by the stack according to the destination.
 *
 * When account is locked/bound to a specific transport, all outgoing
 * requests from this account will use the specified transport (this
 * includes SIP registration, dialog (call and event subscription), and
 * out-of-dialog requests such as MESSAGE).
 *
 * Note that transport_id may be specified in pjsua_acc_config too.
 *
 * @param acc_id	The account ID.
 * @param tp_id		The transport ID.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsua_acc_set_transport(pjsua_acc_id acc_id,
					     pjsua_transport_id tp_id);


/**
 * @}
 */


/*****************************************************************************
 * CALLS API
 */


/**
 * @defgroup PJSUA_LIB_CALL PJSUA-API Calls Management
 * @ingroup PJSUA_LIB
 * @brief Call manipulation.
 * @{
 */

/**
 * Maximum simultaneous calls.
 */
#ifndef PJSUA_MAX_CALLS
#   define PJSUA_MAX_CALLS	    32
#endif



/**
 * This enumeration specifies the media status of a call, and it's part
 * of pjsua_call_info structure.
 */
typedef enum pjsua_call_media_status
{
    /** Call currently has no media */
    PJSUA_CALL_MEDIA_NONE,

    /** The media is active */
    PJSUA_CALL_MEDIA_ACTIVE,

    /** The media is currently put on hold by local endpoint */
    PJSUA_CALL_MEDIA_LOCAL_HOLD,

    /** The media is currently put on hold by remote endpoint */
    PJSUA_CALL_MEDIA_REMOTE_HOLD,

    /** The media has reported error (e.g. ICE negotiation) */
    PJSUA_CALL_MEDIA_ERROR

} pjsua_call_media_status;


/**
 * This structure describes the information and current status of a call.
 */
typedef struct pjsua_call_info
{
    /** Call identification. */
    pjsua_call_id	id;

    /** Initial call role (UAC == caller) */
    pjsip_role_e	role;

    /** The account ID where this call belongs. */
    pjsua_acc_id	acc_id;

    /** Local URI */
    pj_str_t		local_info;

    /** Local Contact */
    pj_str_t		local_contact;

    /** Remote URI */
    pj_str_t		remote_info;

    /** Remote contact */
    pj_str_t		remote_contact;

    /** Dialog Call-ID string. */
    pj_str_t		call_id;

    /** Call state */
    pjsip_inv_state	state;

    /** Text describing the state */
    pj_str_t		state_text;

    /** Last status code heard, which can be used as cause code */
    pjsip_status_code	last_status;

    /** The reason phrase describing the status. */
    pj_str_t		last_status_text;

    /** Call media status. */
    pjsua_call_media_status media_status;

    /** Media direction */
    pjmedia_dir		media_dir;

    /** The conference port number for the call */
    pjsua_conf_port_id	conf_slot;

    /** Up-to-date call connected duration (zero when call is not 
     *  established)
     */
    pj_time_val		connect_duration;

    /** Total call duration, including set-up time */
    pj_time_val		total_duration;

    /** Internal */
    struct {
	char	local_info[128];
	char	local_contact[128];
	char	remote_info[128];
	char	remote_contact[128];
	char	call_id[128];
	char	last_status_text[128];
    } buf_;

} pjsua_call_info;



/**
 * Get maximum number of calls configured in pjsua.
 *
 * @return		Maximum number of calls configured.
 */
PJ_DECL(unsigned) pjsua_call_get_max_count(void);

/**
 * Get number of currently active calls.
 *
 * @return		Number of currently active calls.
 */
PJ_DECL(unsigned) pjsua_call_get_count(void);

/**
 * Enumerate all active calls. Application may then query the information and
 * state of each call by calling #pjsua_call_get_info().
 *
 * @param ids		Array of account IDs to be initialized.
 * @param count		In input, specifies the maximum number of elements.
 *			On return, it contains the actual number of elements.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_enum_calls(pjsua_call_id ids[],
				      unsigned *count);


/**
 * Make outgoing call to the specified URI using the specified account.
 *
 * @param acc_id	The account to be used.
 * @param dst_uri	URI to be put in the To header (normally is the same
 *			as the target URI).
 * @param options	Options (must be zero at the moment).
 * @param user_data	Arbitrary user data to be attached to the call, and
 *			can be retrieved later.
 * @param msg_data	Optional headers etc to be added to outgoing INVITE
 *			request, or NULL if no custom header is desired.
 * @param p_call_id	Pointer to receive call identification.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_make_call(pjsua_acc_id acc_id,
					  const pj_str_t *dst_uri,
					  unsigned options,
					  void *user_data,
					  const pjsua_msg_data *msg_data,
					  pjsua_call_id *p_call_id);


/**
 * Check if the specified call has active INVITE session and the INVITE
 * session has not been disconnected.
 *
 * @param call_id	Call identification.
 *
 * @return		Non-zero if call is active.
 */
PJ_DECL(pj_bool_t) pjsua_call_is_active(pjsua_call_id call_id);


/**
 * Check if call has an active media session.
 *
 * @param call_id	Call identification.
 *
 * @return		Non-zero if yes.
 */
PJ_DECL(pj_bool_t) pjsua_call_has_media(pjsua_call_id call_id);


/**
 * Retrieve the media session associated with this call. Note that the media
 * session may not be available depending on the current call's media status
 * (the pjsua_call_media_status information in pjsua_call_info). Application
 * may use the media session to retrieve more detailed information about the
 * call's media.
 *
 * @param call_id	Call identification.
 *
 * @return		Call media session.
 */
PJ_DECL(pjmedia_session*) pjsua_call_get_media_session(pjsua_call_id call_id);


/**
 * Retrieve the media transport instance that is used for this call. 
 * Application may use the media transport to query more detailed information
 * about the media transport.
 *
 * @param cid		Call identification (the call_id).
 *
 * @return		Call media transport.
 */
PJ_DECL(pjmedia_transport*) pjsua_call_get_media_transport(pjsua_call_id cid);


/**
 * Get the conference port identification associated with the call.
 *
 * @param call_id	Call identification.
 *
 * @return		Conference port ID, or PJSUA_INVALID_ID when the 
 *			media has not been established or is not active.
 */
PJ_DECL(pjsua_conf_port_id) pjsua_call_get_conf_port(pjsua_call_id call_id);

/**
 * Obtain detail information about the specified call.
 *
 * @param call_id	Call identification.
 * @param info		Call info to be initialized.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_get_info(pjsua_call_id call_id,
					 pjsua_call_info *info);


/**
 * Attach application specific data to the call. Application can then
 * inspect this data by calling #pjsua_call_get_user_data().
 *
 * @param call_id	Call identification.
 * @param user_data	Arbitrary data to be attached to the call.
 *
 * @return		The user data.
 */
PJ_DECL(pj_status_t) pjsua_call_set_user_data(pjsua_call_id call_id,
					      void *user_data);


/**
 * Get user data attached to the call, which has been previously set with
 * #pjsua_call_set_user_data().
 *
 * @param call_id	Call identification.
 *
 * @return		The user data.
 */
PJ_DECL(void*) pjsua_call_get_user_data(pjsua_call_id call_id);


/**
 * Get the NAT type of remote's endpoint. This is a proprietary feature
 * of PJSUA-LIB which sends its NAT type in the SDP when \a nat_type_in_sdp
 * is set in #pjsua_config.
 *
 * This function can only be called after SDP has been received from remote,
 * which means for incoming call, this function can be called as soon as
 * call is received as long as incoming call contains SDP, and for outgoing
 * call, this function can be called only after SDP is received (normally in
 * 200/OK response to INVITE). As a general case, application should call 
 * this function after or in \a on_call_media_state() callback.
 *
 * @param call_id	Call identification.
 * @param p_type	Pointer to store the NAT type. Application can then
 *			retrieve the string description of the NAT type
 *			by calling pj_stun_get_nat_name().
 *
 * @return		PJ_SUCCESS on success.
 *
 * @see pjsua_get_nat_type(), nat_type_in_sdp
 */
PJ_DECL(pj_status_t) pjsua_call_get_rem_nat_type(pjsua_call_id call_id,
						 pj_stun_nat_type *p_type);

/**
 * Send response to incoming INVITE request. Depending on the status
 * code specified as parameter, this function may send provisional
 * response, establish the call, or terminate the call.
 *
 * @param call_id	Incoming call identification.
 * @param code		Status code, (100-699).
 * @param reason	Optional reason phrase. If NULL, default text
 *			will be used.
 * @param msg_data	Optional list of headers etc to be added to outgoing
 *			response message.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_answer(pjsua_call_id call_id, 
				       unsigned code,
				       const pj_str_t *reason,
				       const pjsua_msg_data *msg_data);

/**
 * Hangup call by using method that is appropriate according to the
 * call state. This function is different than answering the call with
 * 3xx-6xx response (with #pjsua_call_answer()), in that this function
 * will hangup the call regardless of the state and role of the call,
 * while #pjsua_call_answer() only works with incoming calls on EARLY
 * state.
 *
 * @param call_id	Call identification.
 * @param code		Optional status code to be sent when we're rejecting
 *			incoming call. If the value is zero, "603/Decline"
 *			will be sent.
 * @param reason	Optional reason phrase to be sent when we're rejecting
 *			incoming call.  If NULL, default text will be used.
 * @param msg_data	Optional list of headers etc to be added to outgoing
 *			request/response message.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_hangup(pjsua_call_id call_id,
				       unsigned code,
				       const pj_str_t *reason,
				       const pjsua_msg_data *msg_data);

/**
 * Accept or reject redirection response. Application MUST call this function
 * after it signaled PJSIP_REDIRECT_PENDING in the \a on_call_redirected() 
 * callback, to notify the call whether to accept or reject the redirection
 * to the current target. Application can use the combination of
 * PJSIP_REDIRECT_PENDING command in \a on_call_redirected() callback and 
 * this function to ask for user permission before redirecting the call.
 *
 * Note that if the application chooses to reject or stop redirection (by 
 * using PJSIP_REDIRECT_REJECT or PJSIP_REDIRECT_STOP respectively), the
 * call disconnection callback will be called before this function returns.
 * And if the application rejects the target, the \a on_call_redirected() 
 * callback may also be called before this function returns if there is 
 * another target to try.
 *
 * @param call_id	The call ID.
 * @param cmd		Redirection operation to be applied to the current
 *			target. The semantic of this argument is similar
 *			to the description in the \a on_call_redirected()
 *			callback, except that the PJSIP_REDIRECT_PENDING is
 *			not accepted here.
 *
 * @return		PJ_SUCCESS on successful operation.
 */
PJ_DECL(pj_status_t) pjsua_call_process_redirect(pjsua_call_id call_id,
						 pjsip_redirect_op cmd);

/**
 * Put the specified call on hold. This will send re-INVITE with the
 * appropriate SDP to inform remote that the call is being put on hold.
 * The final status of the request itself will be reported on the
 * \a on_call_media_state() callback, which inform the application that
 * the media state of the call has changed.
 *
 * @param call_id	Call identification.
 * @param msg_data	Optional message components to be sent with
 *			the request.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_set_hold(pjsua_call_id call_id,
					 const pjsua_msg_data *msg_data);


/**
 * Send re-INVITE to release hold.
 * The final status of the request itself will be reported on the
 * \a on_call_media_state() callback, which inform the application that
 * the media state of the call has changed.
 *
 * @param call_id	Call identification.
 * @param unhold	If this argument is non-zero and the call is locally
 *			held, this will release the local hold.
 * @param msg_data	Optional message components to be sent with
 *			the request.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_reinvite(pjsua_call_id call_id,
					 pj_bool_t unhold,
					 const pjsua_msg_data *msg_data);

/**
 * Send UPDATE request.
 *
 * @param call_id	Call identification.
 * @param options	Must be zero for now.
 * @param msg_data	Optional message components to be sent with
 *			the request.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_update(pjsua_call_id call_id,
				       unsigned options,
				       const pjsua_msg_data *msg_data);

/**
 * Initiate call transfer to the specified address. This function will send
 * REFER request to instruct remote call party to initiate a new INVITE
 * session to the specified destination/target.
 *
 * If application is interested to monitor the successfulness and 
 * the progress of the transfer request, it can implement 
 * \a on_call_transfer_status() callback which will report the progress
 * of the call transfer request.
 *
 * @param call_id	The call id to be transfered.
 * @param dest		Address of new target to be contacted.
 * @param msg_data	Optional message components to be sent with
 *			the request.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_xfer(pjsua_call_id call_id, 
				     const pj_str_t *dest,
				     const pjsua_msg_data *msg_data);

/**
 * Flag to indicate that "Require: replaces" should not be put in the
 * outgoing INVITE request caused by REFER request created by 
 * #pjsua_call_xfer_replaces().
 */
#define PJSUA_XFER_NO_REQUIRE_REPLACES	1

/**
 * Initiate attended call transfer. This function will send REFER request
 * to instruct remote call party to initiate new INVITE session to the URL
 * of \a dest_call_id. The party at \a dest_call_id then should "replace"
 * the call with us with the new call from the REFER recipient.
 *
 * @param call_id	The call id to be transfered.
 * @param dest_call_id	The call id to be replaced.
 * @param options	Application may specify PJSUA_XFER_NO_REQUIRE_REPLACES
 *			to suppress the inclusion of "Require: replaces" in
 *			the outgoing INVITE request created by the REFER
 *			request.
 * @param msg_data	Optional message components to be sent with
 *			the request.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_xfer_replaces(pjsua_call_id call_id, 
					      pjsua_call_id dest_call_id,
					      unsigned options,
					      const pjsua_msg_data *msg_data);

/**
 * Send DTMF digits to remote using RFC 2833 payload formats.
 *
 * @param call_id	Call identification.
 * @param digits	DTMF string digits to be sent.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_dial_dtmf(pjsua_call_id call_id, 
					  const pj_str_t *digits);

/**
 * Send instant messaging inside INVITE session.
 *
 * @param call_id	Call identification.
 * @param mime_type	Optional MIME type. If NULL, then "text/plain" is 
 *			assumed.
 * @param content	The message content.
 * @param msg_data	Optional list of headers etc to be included in outgoing
 *			request. The body descriptor in the msg_data is 
 *			ignored.
 * @param user_data	Optional user data, which will be given back when
 *			the IM callback is called.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_send_im( pjsua_call_id call_id, 
					 const pj_str_t *mime_type,
					 const pj_str_t *content,
					 const pjsua_msg_data *msg_data,
					 void *user_data);


/**
 * Send IM typing indication inside INVITE session.
 *
 * @param call_id	Call identification.
 * @param is_typing	Non-zero to indicate to remote that local person is
 *			currently typing an IM.
 * @param msg_data	Optional list of headers etc to be included in outgoing
 *			request.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_send_typing_ind(pjsua_call_id call_id, 
						pj_bool_t is_typing,
						const pjsua_msg_data*msg_data);

/**
 * Send arbitrary request with the call. This is useful for example to send
 * INFO request. Note that application should not use this function to send
 * requests which would change the invite session's state, such as re-INVITE,
 * UPDATE, PRACK, and BYE.
 *
 * @param call_id	Call identification.
 * @param method	SIP method of the request.
 * @param msg_data	Optional message body and/or list of headers to be 
 *			included in outgoing request.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_call_send_request(pjsua_call_id call_id,
					     const pj_str_t *method,
					     const pjsua_msg_data *msg_data);


/**
 * Terminate all calls. This will initiate #pjsua_call_hangup() for all
 * currently active calls. 
 */
PJ_DECL(void) pjsua_call_hangup_all(void);


/**
 * Dump call and media statistics to string.
 *
 * @param call_id	Call identification.
 * @param with_media	Non-zero to include media information too.
 * @param buffer	Buffer where the statistics are to be written to.
 * @param maxlen	Maximum length of buffer.
 * @param indent	Spaces for left indentation.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsua_call_dump(pjsua_call_id call_id, 
				     pj_bool_t with_media, 
				     char *buffer, 
				     unsigned maxlen,
				     const char *indent);

/**
 * @}
 */


/*****************************************************************************
 * BUDDY API
 */


/**
 * @defgroup PJSUA_LIB_BUDDY PJSUA-API Buddy, Presence, and Instant Messaging
 * @ingroup PJSUA_LIB
 * @brief Buddy management, buddy's presence, and instant messaging.
 * @{
 *
 * This section describes PJSUA-APIs related to buddies management,
 * presence management, and instant messaging.
 */

/**
 * Max buddies in buddy list.
 */
#ifndef PJSUA_MAX_BUDDIES
#   define PJSUA_MAX_BUDDIES	    256
#endif


/**
 * This specifies how long the library should wait before retrying failed
 * SUBSCRIBE request, and there is no rule to automatically resubscribe 
 * (for example, no "retry-after" parameter in Subscription-State header).
 *
 * This also controls the duration  before failed PUBLISH request will be
 * retried.
 *
 * Default: 300 seconds
 */
#ifndef PJSUA_PRES_TIMER
#   define PJSUA_PRES_TIMER	    300
#endif


/**
 * This structure describes buddy configuration when adding a buddy to
 * the buddy list with #pjsua_buddy_add(). Application MUST initialize
 * the structure with #pjsua_buddy_config_default() to initialize this
 * structure with default configuration.
 */
typedef struct pjsua_buddy_config
{
    /**
     * Buddy URL or name address.
     */
    pj_str_t	uri;

    /**
     * Specify whether presence subscription should start immediately.
     */
    pj_bool_t	subscribe;

    /**
     * Specify arbitrary application data to be associated with with
     * the buddy object.
     */
    void       *user_data;

} pjsua_buddy_config;


/**
 * This enumeration describes basic buddy's online status.
 */
typedef enum pjsua_buddy_status
{
    /**
     * Online status is unknown (possibly because no presence subscription
     * has been established).
     */
    PJSUA_BUDDY_STATUS_UNKNOWN,

    /**
     * Buddy is known to be online.
     */
    PJSUA_BUDDY_STATUS_ONLINE,

    /**
     * Buddy is offline.
     */
    PJSUA_BUDDY_STATUS_OFFLINE,

} pjsua_buddy_status;



/**
 * This structure describes buddy info, which can be retrieved by calling
 * #pjsua_buddy_get_info().
 */
typedef struct pjsua_buddy_info
{
    /**
     * The buddy ID.
     */
    pjsua_buddy_id	id;

    /**
     * The full URI of the buddy, as specified in the configuration.
     */
    pj_str_t		uri;

    /**
     * Buddy's Contact, only available when presence subscription has
     * been established to the buddy.
     */
    pj_str_t		contact;

    /**
     * Buddy's online status.
     */
    pjsua_buddy_status	status;

    /**
     * Text to describe buddy's online status.
     */
    pj_str_t		status_text;

    /**
     * Flag to indicate that we should monitor the presence information for
     * this buddy (normally yes, unless explicitly disabled).
     */
    pj_bool_t		monitor_pres;

    /**
     * If \a monitor_pres is enabled, this specifies the last state of the
     * presence subscription. If presence subscription session is currently
     * active, the value will be PJSIP_EVSUB_STATE_ACTIVE. If presence
     * subscription request has been rejected, the value will be
     * PJSIP_EVSUB_STATE_TERMINATED, and the termination reason will be
     * specified in \a sub_term_reason.
     */
    pjsip_evsub_state	sub_state;

    /**
     * String representation of subscription state.
     */
    const char	       *sub_state_name;

    /**
     * Specifies the last presence subscription termination code. This would
     * return the last status of the SUBSCRIBE request. If the subscription
     * is terminated with NOTIFY by the server, this value will be set to
     * 200, and subscription termination reason will be given in the
     * \a sub_term_reason field.
     */
    unsigned		sub_term_code;

    /**
     * Specifies the last presence subscription termination reason. If 
     * presence subscription is currently active, the value will be empty.
     */
    pj_str_t		sub_term_reason;

    /**
     * Extended RPID information about the person.
     */
    pjrpid_element	rpid;

    /**
     * Extended presence info.
     */
    pjsip_pres_status	pres_status;

    /**
     * Internal buffer.
     */
    char		buf_[512];

} pjsua_buddy_info;


/**
 * Set default values to the buddy config.
 */
PJ_DECL(void) pjsua_buddy_config_default(pjsua_buddy_config *cfg);


/**
 * Get total number of buddies.
 *
 * @return		Number of buddies.
 */
PJ_DECL(unsigned) pjsua_get_buddy_count(void);


/**
 * Check if buddy ID is valid.
 *
 * @param buddy_id	Buddy ID to check.
 *
 * @return		Non-zero if buddy ID is valid.
 */
PJ_DECL(pj_bool_t) pjsua_buddy_is_valid(pjsua_buddy_id buddy_id);


/**
 * Enumerate all buddy IDs in the buddy list. Application then can use
 * #pjsua_buddy_get_info() to get the detail information for each buddy
 * id.
 *
 * @param ids		Array of ids to be initialized.
 * @param count		On input, specifies max elements in the array.
 *			On return, it contains actual number of elements
 *			that have been initialized.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_enum_buddies(pjsua_buddy_id ids[],
					unsigned *count);

/**
 * Find the buddy ID with the specified URI.
 *
 * @param uri		The buddy URI.
 *
 * @return		The buddy ID, or PJSUA_INVALID_ID if not found.
 */
PJ_DECL(pjsua_buddy_id) pjsua_buddy_find(const pj_str_t *uri);


/**
 * Get detailed buddy info.
 *
 * @param buddy_id	The buddy identification.
 * @param info		Pointer to receive information about buddy.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_buddy_get_info(pjsua_buddy_id buddy_id,
					  pjsua_buddy_info *info);

/**
 * Set the user data associated with the buddy object.
 *
 * @param buddy_id	The buddy identification.
 * @param user_data	Arbitrary application data to be associated with
 *			the buddy object.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_buddy_set_user_data(pjsua_buddy_id buddy_id,
					       void *user_data);


/**
 * Get the user data associated with the budy object.
 *
 * @param buddy_id	The buddy identification.
 *
 * @return		The application data.
 */
PJ_DECL(void*) pjsua_buddy_get_user_data(pjsua_buddy_id buddy_id);


/**
 * Add new buddy to the buddy list. If presence subscription is enabled
 * for this buddy, this function will also start the presence subscription
 * session immediately.
 *
 * @param buddy_cfg	Buddy configuration.
 * @param p_buddy_id	Pointer to receive buddy ID.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_buddy_add(const pjsua_buddy_config *buddy_cfg,
				     pjsua_buddy_id *p_buddy_id);


/**
 * Delete the specified buddy from the buddy list. Any presence subscription
 * to this buddy will be terminated.
 *
 * @param buddy_id	Buddy identification.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_buddy_del(pjsua_buddy_id buddy_id);


/**
 * Enable/disable buddy's presence monitoring. Once buddy's presence is
 * subscribed, application will be informed about buddy's presence status
 * changed via \a on_buddy_state() callback.
 *
 * @param buddy_id	Buddy identification.
 * @param subscribe	Specify non-zero to activate presence subscription to
 *			the specified buddy.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_buddy_subscribe_pres(pjsua_buddy_id buddy_id,
						pj_bool_t subscribe);


/**
 * Update the presence information for the buddy. Although the library
 * periodically refreshes the presence subscription for all buddies, some
 * application may want to refresh the buddy's presence subscription
 * immediately, and in this case it can use this function to accomplish
 * this.
 *
 * Note that the buddy's presence subscription will only be initiated
 * if presence monitoring is enabled for the buddy. See 
 * #pjsua_buddy_subscribe_pres() for more info. Also if presence subscription
 * for the buddy is already active, this function will not do anything.
 *
 * Once the presence subscription is activated successfully for the buddy,
 * application will be notified about the buddy's presence status in the
 * on_buddy_state() callback.
 *
 * @param buddy_id	Buddy identification.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_buddy_update_pres(pjsua_buddy_id buddy_id);


/**
 * Send NOTIFY to inform account presence status or to terminate server
 * side presence subscription. If application wants to reject the incoming
 * request, it should set the \a state to PJSIP_EVSUB_STATE_TERMINATED.
 *
 * @param acc_id	Account ID.
 * @param srv_pres	Server presence subscription instance.
 * @param state		New state to set.
 * @param state_str	Optionally specify the state string name, if state
 *			is not "active", "pending", or "terminated".
 * @param reason	If the new state is PJSIP_EVSUB_STATE_TERMINATED,
 *			optionally specify the termination reason. 
 * @param with_body	If the new state is PJSIP_EVSUB_STATE_TERMINATED,
 *			this specifies whether the NOTIFY request should
 *			contain message body containing account's presence
 *			information.
 * @param msg_data	Optional list of headers to be sent with the NOTIFY
 *			request.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsua_pres_notify(pjsua_acc_id acc_id,
				       pjsua_srv_pres *srv_pres,
				       pjsip_evsub_state state,
				       const pj_str_t *state_str,
				       const pj_str_t *reason,
				       pj_bool_t with_body,
				       const pjsua_msg_data *msg_data);

/**
 * Dump presence subscriptions to log.
 *
 * @param verbose	Yes or no.
 */
PJ_DECL(void) pjsua_pres_dump(pj_bool_t verbose);


/**
 * The MESSAGE method (defined in pjsua_im.c)
 */
extern const pjsip_method pjsip_message_method;



/**
 * Send instant messaging outside dialog, using the specified account for
 * route set and authentication.
 *
 * @param acc_id	Account ID to be used to send the request.
 * @param to		Remote URI.
 * @param mime_type	Optional MIME type. If NULL, then "text/plain" is 
 *			assumed.
 * @param content	The message content.
 * @param msg_data	Optional list of headers etc to be included in outgoing
 *			request. The body descriptor in the msg_data is 
 *			ignored.
 * @param user_data	Optional user data, which will be given back when
 *			the IM callback is called.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_im_send(pjsua_acc_id acc_id, 
				   const pj_str_t *to,
				   const pj_str_t *mime_type,
				   const pj_str_t *content,
				   const pjsua_msg_data *msg_data,
				   void *user_data);


/**
 * Send typing indication outside dialog.
 *
 * @param acc_id	Account ID to be used to send the request.
 * @param to		Remote URI.
 * @param is_typing	If non-zero, it tells remote person that local person
 *			is currently composing an IM.
 * @param msg_data	Optional list of headers etc to be added to outgoing
 *			request.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_im_typing(pjsua_acc_id acc_id, 
				     const pj_str_t *to, 
				     pj_bool_t is_typing,
				     const pjsua_msg_data *msg_data);



/**
 * @}
 */


/*****************************************************************************
 * MEDIA API
 */


/**
 * @defgroup PJSUA_LIB_MEDIA PJSUA-API Media Manipulation
 * @ingroup PJSUA_LIB
 * @brief Media manipulation.
 * @{
 *
 * PJSUA has rather powerful media features, which are built around the
 * PJMEDIA conference bridge. Basically, all media "ports" (such as calls, WAV 
 * players, WAV playlist, file recorders, sound device, tone generators, etc)
 * are terminated in the conference bridge, and application can manipulate
 * the interconnection between these terminations freely. 
 *
 * The conference bridge provides powerful switching and mixing functionality
 * for application. With the conference bridge, each conference slot (e.g. 
 * a call) can transmit to multiple destinations, and one destination can
 * receive from multiple sources. If more than one media terminations are 
 * terminated in the same slot, the conference bridge will mix the signal 
 * automatically.
 *
 * Application connects one media termination/slot to another by calling
 * #pjsua_conf_connect() function. This will establish <b>unidirectional</b>
 * media flow from the source termination to the sink termination. To
 * establish bidirectional media flow, application wound need to make another
 * call to #pjsua_conf_connect(), this time inverting the source and 
 * destination slots in the parameter.
 *
 * For example, to stream a WAV file to remote call, application may use
 * the following steps:
 *
 \code
  
  pj_status_t stream_to_call( pjsua_call_id call_id )
  {
     pjsua_player_id player_id;
     
     status = pjsua_player_create("mysong.wav", 0, NULL, &player_id);
     if (status != PJ_SUCCESS)
        return status;

     status = pjsua_conf_connect( pjsua_player_get_conf_port(),
				  pjsua_call_get_conf_port() );
  }
 \endcode
 *
 *
 * Other features of PJSUA media:
 *  - efficient N to M interconnections between media terminations.
 *  - media termination can be connected to itself to create loopback
 *    media.
 *  - the media termination may have different clock rates, and resampling
 *    will be done automatically by conference bridge.
 *  - media terminations may also have different frame time; the
 *    conference bridge will perform the necessary bufferring to adjust
 *    the difference between terminations.
 *  - interconnections are removed automatically when media termination
 *    is removed from the bridge.
 *  - sound device may be changed even when there are active media 
 *    interconnections.
 *  - correctly report call's media quality (in #pjsua_call_dump()) from
 *    RTCP packet exchange.
 */

/**
 * Max ports in the conference bridge. This setting is the default value
 * for pjsua_media_config.max_media_ports.
 */
#ifndef PJSUA_MAX_CONF_PORTS
#   define PJSUA_MAX_CONF_PORTS		254
#endif

/**
 * The default clock rate to be used by the conference bridge. This setting
 * is the default value for pjsua_media_config.clock_rate.
 */
#ifndef PJSUA_DEFAULT_CLOCK_RATE
#   define PJSUA_DEFAULT_CLOCK_RATE	16000
#endif

/**
 * Default frame length in the conference bridge. This setting
 * is the default value for pjsua_media_config.audio_frame_ptime.
 */
#ifndef PJSUA_DEFAULT_AUDIO_FRAME_PTIME
#   define PJSUA_DEFAULT_AUDIO_FRAME_PTIME  20
#endif


/**
 * Default codec quality settings. This setting is the default value
 * for pjsua_media_config.quality.
 */
#ifndef PJSUA_DEFAULT_CODEC_QUALITY
#   define PJSUA_DEFAULT_CODEC_QUALITY	8
#endif

/**
 * Default iLBC mode. This setting is the default value for 
 * pjsua_media_config.ilbc_mode.
 */
#ifndef PJSUA_DEFAULT_ILBC_MODE
#   define PJSUA_DEFAULT_ILBC_MODE	30
#endif

/**
 * The default echo canceller tail length. This setting
 * is the default value for pjsua_media_config.ec_tail_len.
 */
#ifndef PJSUA_DEFAULT_EC_TAIL_LEN
#   define PJSUA_DEFAULT_EC_TAIL_LEN	200
#endif


/**
 * The maximum file player.
 */
#ifndef PJSUA_MAX_PLAYERS
#   define PJSUA_MAX_PLAYERS		32
#endif


/**
 * The maximum file player.
 */
#ifndef PJSUA_MAX_RECORDERS
#   define PJSUA_MAX_RECORDERS		32
#endif


/**
 * This structure describes media configuration, which will be specified
 * when calling #pjsua_init(). Application MUST initialize this structure
 * by calling #pjsua_media_config_default().
 */
struct pjsua_media_config
{
    /**
     * Clock rate to be applied to the conference bridge.
     * If value is zero, default clock rate will be used 
     * (PJSUA_DEFAULT_CLOCK_RATE, which by default is 16KHz).
     */
    unsigned		clock_rate;

    /**
     * Clock rate to be applied when opening the sound device.
     * If value is zero, conference bridge clock rate will be used.
     */
    unsigned		snd_clock_rate;

    /**
     * Channel count be applied when opening the sound device and
     * conference bridge.
     */
    unsigned		channel_count;

    /**
     * Specify audio frame ptime. The value here will affect the 
     * samples per frame of both the sound device and the conference
     * bridge. Specifying lower ptime will normally reduce the
     * latency.
     *
     * Default value: PJSUA_DEFAULT_AUDIO_FRAME_PTIME
     */
    unsigned		audio_frame_ptime;

    /**
     * Specify maximum number of media ports to be created in the
     * conference bridge. Since all media terminate in the bridge
     * (calls, file player, file recorder, etc), the value must be
     * large enough to support all of them. However, the larger
     * the value, the more computations are performed.
     *
     * Default value: PJSUA_MAX_CONF_PORTS
     */
    unsigned		max_media_ports;

    /**
     * Specify whether the media manager should manage its own
     * ioqueue for the RTP/RTCP sockets. If yes, ioqueue will be created
     * and at least one worker thread will be created too. If no,
     * the RTP/RTCP sockets will share the same ioqueue as SIP sockets,
     * and no worker thread is needed.
     *
     * Normally application would say yes here, unless it wants to
     * run everything from a single thread.
     */
    pj_bool_t		has_ioqueue;

    /**
     * Specify the number of worker threads to handle incoming RTP
     * packets. A value of one is recommended for most applications.
     */
    unsigned		thread_cnt;

    /**
     * Media quality, 0-10, according to this table:
     *   5-10: resampling use large filter,
     *   3-4:  resampling use small filter,
     *   1-2:  resampling use linear.
     * The media quality also sets speex codec quality/complexity to the
     * number.
     *
     * Default: 5 (PJSUA_DEFAULT_CODEC_QUALITY).
     */
    unsigned		quality;

    /**
     * Specify default codec ptime.
     *
     * Default: 0 (codec specific)
     */
    unsigned		ptime;

    /**
     * Disable VAD?
     *
     * Default: 0 (no (meaning VAD is enabled))
     */
    pj_bool_t		no_vad;

    /**
     * iLBC mode (20 or 30).
     *
     * Default: 30 (PJSUA_DEFAULT_ILBC_MODE)
     */
    unsigned		ilbc_mode;

    /**
     * Percentage of RTP packet to drop in TX direction
     * (to simulate packet lost).
     *
     * Default: 0
     */
    unsigned		tx_drop_pct;

    /**
     * Percentage of RTP packet to drop in RX direction
     * (to simulate packet lost).
     *
     * Default: 0
     */
    unsigned		rx_drop_pct;

    /**
     * Echo canceller options (see #pjmedia_echo_create())
     *
     * Default: 0.
     */
    unsigned		ec_options;

    /**
     * Echo canceller tail length, in miliseconds.
     *
     * Default: PJSUA_DEFAULT_EC_TAIL_LEN
     */
    unsigned		ec_tail_len;

    /**
     * Audio capture buffer length, in milliseconds.
     *
     * Default: PJMEDIA_SND_DEFAULT_REC_LATENCY
     */
    unsigned		snd_rec_latency;

    /**
     * Audio playback buffer length, in milliseconds.
     *
     * Default: PJMEDIA_SND_DEFAULT_PLAY_LATENCY
     */
    unsigned		snd_play_latency;

    /** 
     * Jitter buffer initial prefetch delay in msec. The value must be
     * between jb_min_pre and jb_max_pre below.
     *
     * Default: -1 (to use default stream settings, currently 150 msec)
     */
    int			jb_init;

    /**
     * Jitter buffer minimum prefetch delay in msec.
     *
     * Default: -1 (to use default stream settings, currently 60 msec)
     */
    int			jb_min_pre;
    
    /**
     * Jitter buffer maximum prefetch delay in msec.
     *
     * Default: -1 (to use default stream settings, currently 240 msec)
     */
    int			jb_max_pre;

    /**
     * Set maximum delay that can be accomodated by the jitter buffer msec.
     *
     * Default: -1 (to use default stream settings, currently 360 msec)
     */
    int			jb_max;

    /**
     * Enable ICE
     */
    pj_bool_t		enable_ice;

    /**
     * Set the maximum number of host candidates.
     *
     * Default: -1 (maximum not set)
     */
    int			ice_max_host_cands;

    /**
     * ICE session options.
     */
    pj_ice_sess_options	ice_opt;

    /**
     * Disable RTCP component.
     *
     * Default: no
     */
    pj_bool_t		ice_no_rtcp;

    /**
     * Enable TURN relay candidate in ICE.
     */
    pj_bool_t		enable_turn;

    /**
     * Specify TURN domain name or host name, in in "DOMAIN:PORT" or 
     * "HOST:PORT" format.
     */
    pj_str_t		turn_server;

    /**
     * Specify the connection type to be used to the TURN server. Valid
     * values are PJ_TURN_TP_UDP or PJ_TURN_TP_TCP.
     *
     * Default: PJ_TURN_TP_UDP
     */
    pj_turn_tp_type	turn_conn_type;

    /**
     * Specify the credential to authenticate with the TURN server.
     */
    pj_stun_auth_cred	turn_auth_cred;

    /**
     * Specify idle time of sound device before it is automatically closed,
     * in seconds. Use value -1 to disable the auto-close feature of sound
     * device
     *
     * Default : 1
     */
    int			snd_auto_close_time;
};


/**
 * Use this function to initialize media config.
 *
 * @param cfg	The media config to be initialized.
 */
PJ_DECL(void) pjsua_media_config_default(pjsua_media_config *cfg);


/**
 * This structure describes codec information, which can be retrieved by
 * calling #pjsua_enum_codecs().
 */
typedef struct pjsua_codec_info
{
    /**
     * Codec unique identification.
     */
    pj_str_t		codec_id;

    /**
     * Codec priority (integer 0-255).
     */
    pj_uint8_t		priority;

    /**
     * Internal buffer.
     */
    char		buf_[32];

} pjsua_codec_info;


/**
 * This structure descibes information about a particular media port that
 * has been registered into the conference bridge. Application can query
 * this info by calling #pjsua_conf_get_port_info().
 */
typedef struct pjsua_conf_port_info
{
    /** Conference port number. */
    pjsua_conf_port_id	slot_id;

    /** Port name. */
    pj_str_t		name;

    /** Clock rate. */
    unsigned		clock_rate;

    /** Number of channels. */
    unsigned		channel_count;

    /** Samples per frame */
    unsigned		samples_per_frame;

    /** Bits per sample */
    unsigned		bits_per_sample;

    /** Number of listeners in the array. */
    unsigned		listener_cnt;

    /** Array of listeners (in other words, ports where this port is 
     *  transmitting to.
     */
    pjsua_conf_port_id	listeners[PJSUA_MAX_CONF_PORTS];

} pjsua_conf_port_info;


/**
 * This structure holds information about custom media transport to
 * be registered to pjsua.
 */
typedef struct pjsua_media_transport
{
    /**
     * Media socket information containing the address information
     * of the RTP and RTCP socket.
     */
    pjmedia_sock_info	 skinfo;

    /**
     * The media transport instance.
     */
    pjmedia_transport	*transport;

} pjsua_media_transport;




/**
 * Get maxinum number of conference ports.
 *
 * @return		Maximum number of ports in the conference bridge.
 */
PJ_DECL(unsigned) pjsua_conf_get_max_ports(void);


/**
 * Get current number of active ports in the bridge.
 *
 * @return		The number.
 */
PJ_DECL(unsigned) pjsua_conf_get_active_ports(void);


/**
 * Enumerate all conference ports.
 *
 * @param id		Array of conference port ID to be initialized.
 * @param count		On input, specifies max elements in the array.
 *			On return, it contains actual number of elements
 *			that have been initialized.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_enum_conf_ports(pjsua_conf_port_id id[],
					   unsigned *count);


/**
 * Get information about the specified conference port
 *
 * @param port_id	Port identification.
 * @param info		Pointer to store the port info.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_conf_get_port_info( pjsua_conf_port_id port_id,
					       pjsua_conf_port_info *info);


/**
 * Add arbitrary media port to PJSUA's conference bridge. Application
 * can use this function to add the media port that it creates. For
 * media ports that are created by PJSUA-LIB (such as calls, file player,
 * or file recorder), PJSUA-LIB will automatically add the port to
 * the bridge.
 *
 * @param pool		Pool to use.
 * @param port		Media port to be added to the bridge.
 * @param p_id		Optional pointer to receive the conference 
 *			slot id.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_conf_add_port(pj_pool_t *pool,
					 pjmedia_port *port,
					 pjsua_conf_port_id *p_id);


/**
 * Remove arbitrary slot from the conference bridge. Application should only
 * call this function if it registered the port manually with previous call
 * to #pjsua_conf_add_port().
 *
 * @param port_id	The slot id of the port to be removed.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_conf_remove_port(pjsua_conf_port_id port_id);


/**
 * Establish unidirectional media flow from souce to sink. One source
 * may transmit to multiple destinations/sink. And if multiple
 * sources are transmitting to the same sink, the media will be mixed
 * together. Source and sink may refer to the same ID, effectively
 * looping the media.
 *
 * If bidirectional media flow is desired, application needs to call
 * this function twice, with the second one having the arguments
 * reversed.
 *
 * @param source	Port ID of the source media/transmitter.
 * @param sink		Port ID of the destination media/received.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_conf_connect(pjsua_conf_port_id source,
					pjsua_conf_port_id sink);


/**
 * Disconnect media flow from the source to destination port.
 *
 * @param source	Port ID of the source media/transmitter.
 * @param sink		Port ID of the destination media/received.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_conf_disconnect(pjsua_conf_port_id source,
					   pjsua_conf_port_id sink);


/**
 * Adjust the signal level to be transmitted from the bridge to the 
 * specified port by making it louder or quieter.
 *
 * @param slot		The conference bridge slot number.
 * @param level		Signal level adjustment. Value 1.0 means no level
 *			adjustment, while value 0 means to mute the port.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_conf_adjust_tx_level(pjsua_conf_port_id slot,
						float level);

/**
 * Adjust the signal level to be received from the specified port (to
 * the bridge) by making it louder or quieter.
 *
 * @param slot		The conference bridge slot number.
 * @param level		Signal level adjustment. Value 1.0 means no level
 *			adjustment, while value 0 means to mute the port.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_conf_adjust_rx_level(pjsua_conf_port_id slot,
						float level);

/**
 * Get last signal level transmitted to or received from the specified port.
 * The signal level is an integer value in zero to 255, with zero indicates
 * no signal, and 255 indicates the loudest signal level.
 *
 * @param slot		The conference bridge slot number.
 * @param tx_level	Optional argument to receive the level of signal
 *			transmitted to the specified port (i.e. the direction
 *			is from the bridge to the port).
 * @param rx_level	Optional argument to receive the level of signal
 *			received from the port (i.e. the direction is from the
 *			port to the bridge).
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsua_conf_get_signal_level(pjsua_conf_port_id slot,
						 unsigned *tx_level,
						 unsigned *rx_level);


/*****************************************************************************
 * File player and playlist.
 */

/**
 * Create a file player, and automatically add this player to
 * the conference bridge.
 *
 * @param filename	The filename to be played. Currently only
 *			WAV files are supported, and the WAV file MUST be
 *			formatted as 16bit PCM mono/single channel (any
 *			clock rate is supported).
 * @param options	Optional option flag. Application may specify
 *			PJMEDIA_FILE_NO_LOOP to prevent playback loop.
 * @param p_id		Pointer to receive player ID.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_player_create(const pj_str_t *filename,
					 unsigned options,
					 pjsua_player_id *p_id);


/**
 * Create a file playlist media port, and automatically add the port
 * to the conference bridge.
 *
 * @param file_names	Array of file names to be added to the play list.
 *			Note that the files must have the same clock rate,
 *			number of channels, and number of bits per sample.
 * @param file_count	Number of files in the array.
 * @param label		Optional label to be set for the media port.
 * @param options	Optional option flag. Application may specify
 *			PJMEDIA_FILE_NO_LOOP to prevent looping.
 * @param p_id		Optional pointer to receive player ID.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_playlist_create(const pj_str_t file_names[],
					   unsigned file_count,
					   const pj_str_t *label,
					   unsigned options,
					   pjsua_player_id *p_id);

/**
 * Get conference port ID associated with player or playlist.
 *
 * @param id		The file player ID.
 *
 * @return		Conference port ID associated with this player.
 */
PJ_DECL(pjsua_conf_port_id) pjsua_player_get_conf_port(pjsua_player_id id);


/**
 * Get the media port for the player or playlist.
 *
 * @param id		The player ID.
 * @param p_port	The media port associated with the player.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsua_player_get_port(pjsua_player_id id,
					   pjmedia_port **p_port);

/**
 * Set playback position. This operation is not valid for playlist.
 *
 * @param id		The file player ID.
 * @param samples	The playback position, in samples. Application can
 *			specify zero to re-start the playback.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_player_set_pos(pjsua_player_id id,
					  pj_uint32_t samples);


/**
 * Close the file of playlist, remove the player from the bridge, and free
 * resources associated with the file player or playlist.
 *
 * @param id		The file player ID.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_player_destroy(pjsua_player_id id);


/*****************************************************************************
 * File recorder.
 */

/**
 * Create a file recorder, and automatically connect this recorder to
 * the conference bridge. The recorder currently supports recording WAV file.
 * The type of the recorder to use is determined by the extension of the file 
 * (e.g. ".wav").
 *
 * @param filename	Output file name. The function will determine the
 *			default format to be used based on the file extension.
 *			Currently ".wav" is supported on all platforms.
 * @param enc_type	Optionally specify the type of encoder to be used to
 *			compress the media, if the file can support different
 *			encodings. This value must be zero for now.
 * @param enc_param	Optionally specify codec specific parameter to be 
 *			passed to the file writer. 
 *			For .WAV recorder, this value must be NULL.
 * @param max_size	Maximum file size. Specify zero or -1 to remove size
 *			limitation. This value must be zero or -1 for now.
 * @param options	Optional options.
 * @param p_id		Pointer to receive the recorder instance.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_recorder_create(const pj_str_t *filename,
					   unsigned enc_type,
					   void *enc_param,
					   pj_ssize_t max_size,
					   unsigned options,
					   pjsua_recorder_id *p_id);


/**
 * Get conference port associated with recorder.
 *
 * @param id		The recorder ID.
 *
 * @return		Conference port ID associated with this recorder.
 */
PJ_DECL(pjsua_conf_port_id) pjsua_recorder_get_conf_port(pjsua_recorder_id id);


/**
 * Get the media port for the recorder.
 *
 * @param id		The recorder ID.
 * @param p_port	The media port associated with the recorder.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsua_recorder_get_port(pjsua_recorder_id id,
					     pjmedia_port **p_port);


/**
 * Destroy recorder (this will complete recording).
 *
 * @param id		The recorder ID.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_recorder_destroy(pjsua_recorder_id id);


/*****************************************************************************
 * Sound devices.
 */

/**
 * Enum all audio devices installed in the system.
 *
 * @param info		Array of info to be initialized.
 * @param count		On input, specifies max elements in the array.
 *			On return, it contains actual number of elements
 *			that have been initialized.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_enum_aud_devs(pjmedia_aud_dev_info info[],
					 unsigned *count);

/**
 * Enum all sound devices installed in the system (old API).
 *
 * @param info		Array of info to be initialized.
 * @param count		On input, specifies max elements in the array.
 *			On return, it contains actual number of elements
 *			that have been initialized.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_enum_snd_devs(pjmedia_snd_dev_info info[],
					 unsigned *count);

/**
 * Get currently active sound devices. If sound devices has not been created
 * (for example when pjsua_start() is not called), it is possible that
 * the function returns PJ_SUCCESS with -1 as device IDs.
 *
 * @param capture_dev   On return it will be filled with device ID of the 
 *			capture device.
 * @param playback_dev	On return it will be filled with device ID of the 
 *			device ID of the playback device.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_get_snd_dev(int *capture_dev,
				       int *playback_dev);


/**
 * Select or change sound device. Application may call this function at
 * any time to replace current sound device.
 *
 * @param capture_dev   Device ID of the capture device.
 * @param playback_dev	Device ID of the playback device.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_set_snd_dev(int capture_dev,
				       int playback_dev);


/**
 * Set pjsua to use null sound device. The null sound device only provides
 * the timing needed by the conference bridge, and will not interract with
 * any hardware.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_set_null_snd_dev(void);


/**
 * Disconnect the main conference bridge from any sound devices, and let
 * application connect the bridge to it's own sound device/master port.
 *
 * @return		The port interface of the conference bridge, 
 *			so that application can connect this to it's own
 *			sound device or master port.
 */
PJ_DECL(pjmedia_port*) pjsua_set_no_snd_dev(void);


/**
 * Change the echo cancellation settings.
 *
 * The behavior of this function depends on whether the sound device is
 * currently active, and if it is, whether device or software AEC is 
 * being used. 
 *
 * If the sound device is currently active, and if the device supports AEC,
 * this function will forward the change request to the device and it will
 * be up to the device on whether support the request. If software AEC is
 * being used (the software EC will be used if the device does not support
 * AEC), this function will change the software EC settings. In all cases,
 * the setting will be saved for future opening of the sound device.
 *
 * If the sound device is not currently active, this will only change the
 * default AEC settings and the setting will be applied next time the 
 * sound device is opened.
 *
 * @param tail_ms	The tail length, in miliseconds. Set to zero to
 *			disable AEC.
 * @param options	Options to be passed to pjmedia_echo_create().
 *			Normally the value should be zero.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsua_set_ec(unsigned tail_ms, unsigned options);


/**
 * Get current echo canceller tail length. 
 *
 * @param p_tail_ms	Pointer to receive the tail length, in miliseconds. 
 *			If AEC is disabled, the value will be zero.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsua_get_ec_tail(unsigned *p_tail_ms);


/**
 * Check whether the sound device is currently active. The sound device
 * may be inactive if the application has set the auto close feature to
 * non-zero (the snd_auto_close_time setting in #pjsua_media_config), or
 * if null sound device or no sound device has been configured via the
 * #pjsua_set_no_snd_dev() function.
 */
PJ_DECL(pj_bool_t) pjsua_snd_is_active(void);

    
/**
 * Configure sound device setting to the sound device being used. If sound 
 * device is currently active, the function will forward the setting to the
 * sound device instance to be applied immediately, if it supports it. 
 *
 * The setting will be saved for future opening of the sound device, if the 
 * "keep" argument is set to non-zero. If the sound device is currently
 * inactive, and the "keep" argument is false, this function will return
 * error.
 * 
 * Note that in case the setting is kept for future use, it will be applied
 * to any devices, even when application has changed the sound device to be
 * used.
 *
 * Note also that the echo cancellation setting should be set with 
 * #pjsua_set_ec() API instead.
 *
 * See also #pjmedia_aud_stream_set_cap() for more information about setting
 * an audio device capability.
 *
 * @param cap		The sound device setting to change.
 * @param pval		Pointer to value. Please see #pjmedia_aud_dev_cap
 *			documentation about the type of value to be 
 *			supplied for each setting.
 * @param keep		Specify whether the setting is to be kept for future
 *			use.
 *
 * @return		PJ_SUCCESS on success or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_snd_set_setting(pjmedia_aud_dev_cap cap,
					   const void *pval,
					   pj_bool_t keep);

/**
 * Retrieve a sound device setting. If sound device is currently active,
 * the function will forward the request to the sound device. If sound device
 * is currently inactive, and if application had previously set the setting
 * and mark the setting as kept, then that setting will be returned.
 * Otherwise, this function will return error.
 *
 * Note that echo cancellation settings should be retrieved with 
 * #pjsua_get_ec_tail() API instead.
 *
 * @param cap		The sound device setting to retrieve.
 * @param pval		Pointer to receive the value. 
 *			Please see #pjmedia_aud_dev_cap documentation about
 *			the type of value to be supplied for each setting.
 *
 * @return		PJ_SUCCESS on success or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_snd_get_setting(pjmedia_aud_dev_cap cap,
					   void *pval);


/*****************************************************************************
 * Codecs.
 */

/**
 * Enum all supported codecs in the system.
 *
 * @param id		Array of ID to be initialized.
 * @param count		On input, specifies max elements in the array.
 *			On return, it contains actual number of elements
 *			that have been initialized.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_enum_codecs( pjsua_codec_info id[],
				        unsigned *count );


/**
 * Change codec priority.
 *
 * @param codec_id	Codec ID, which is a string that uniquely identify
 *			the codec (such as "speex/8000"). Please see pjsua
 *			manual or pjmedia codec reference for details.
 * @param priority	Codec priority, 0-255, where zero means to disable
 *			the codec.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_codec_set_priority( const pj_str_t *codec_id,
					       pj_uint8_t priority );


/**
 * Get codec parameters.
 *
 * @param codec_id	Codec ID.
 * @param param		Structure to receive codec parameters.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_codec_get_param( const pj_str_t *codec_id,
					    pjmedia_codec_param *param );


/**
 * Set codec parameters.
 *
 * @param codec_id	Codec ID.
 * @param param		Codec parameter to set.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsua_codec_set_param( const pj_str_t *codec_id,
					    const pjmedia_codec_param *param);




/**
 * Create UDP media transports for all the calls. This function creates
 * one UDP media transport for each call.
 *
 * @param cfg		Media transport configuration. The "port" field in the
 *			configuration is used as the start port to bind the
 *			sockets.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) 
pjsua_media_transports_create(const pjsua_transport_config *cfg);


/**
 * Register custom media transports to be used by calls. There must
 * enough media transports for all calls.
 *
 * @param tp		The media transport array.
 * @param count		Number of elements in the array. This number MUST
 *			match the number of maximum calls configured when
 *			pjsua is created.
 * @param auto_delete	Flag to indicate whether the transports should be
 *			destroyed when pjsua is shutdown.
 *
 * @return		PJ_SUCCESS on success, or the appropriate error code.
 */
PJ_DECL(pj_status_t) 
pjsua_media_transports_attach( pjsua_media_transport tp[],
			       unsigned count,
			       pj_bool_t auto_delete);


/**
 * @}
 */



/**
 * @}
 */

PJ_END_DECL


#endif	/* __PJSUA_H__ */
