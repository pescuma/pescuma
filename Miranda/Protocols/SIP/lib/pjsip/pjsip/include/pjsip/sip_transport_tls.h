/* $Id: sip_transport_tls.h 2998 2009-11-09 08:51:34Z bennylp $ */
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
#ifndef __PJSIP_TRANSPORT_TLS_H__
#define __PJSIP_TRANSPORT_TLS_H__

/**
 * @file sip_transport_tls.h
 * @brief SIP TLS Transport.
 */

#include <pjsip/sip_transport.h>
#include <pj/string.h>
#include <pj/sock_qos.h>


PJ_BEGIN_DECL

/**
 * @defgroup PJSIP_TRANSPORT_TLS TLS Transport
 * @ingroup PJSIP_TRANSPORT
 * @brief API to create and register TLS transport.
 * @{
 * The functions below are used to create TLS transport and register 
 * the transport to the framework.
 */

/**
 * The default SSL method to be used by PJSIP.
 * Default is PJSIP_TLSV1_METHOD
 */
#ifndef PJSIP_SSL_DEFAULT_METHOD
#   define PJSIP_SSL_DEFAULT_METHOD	PJSIP_TLSV1_METHOD
#endif

/** SSL protocol method constants. */
typedef enum pjsip_ssl_method
{
    PJSIP_SSL_UNSPECIFIED_METHOD= 0,	/**< Default protocol method.	*/
    PJSIP_TLSV1_METHOD		= 31,	/**< Use SSLv1 method.		*/
    PJSIP_SSLV2_METHOD		= 20,	/**< Use SSLv2 method.		*/
    PJSIP_SSLV3_METHOD		= 30,	/**< Use SSLv3 method.		*/
    PJSIP_SSLV23_METHOD		= 23	/**< Use SSLv23 method.		*/
} pjsip_ssl_method;




/**
 * TLS transport settings.
 */
typedef struct pjsip_tls_setting
{
    /**
     * Certificate of Authority (CA) list file.
     */
    pj_str_t	ca_list_file;

    /**
     * Public endpoint certificate file, which will be used as client-
     * side  certificate for outgoing TLS connection, and server-side
     * certificate for incoming TLS connection.
     */
    pj_str_t	cert_file;

    /**
     * Optional private key of the endpoint certificate to be used.
     */
    pj_str_t	privkey_file;

    /**
     * Password to open private key.
     */
    pj_str_t	password;

    /**
     * TLS protocol method from #pjsip_ssl_method, which can be:
     *	- PJSIP_SSL_UNSPECIFIED_METHOD(0): default (which will use 
     *                                     PJSIP_SSL_DEFAULT_METHOD)
     *	- PJSIP_TLSV1_METHOD(1):	   TLSv1
     *	- PJSIP_SSLV2_METHOD(2):	   SSLv2
     *	- PJSIP_SSLV3_METHOD(3):	   SSL3
     *	- PJSIP_SSLV23_METHOD(23):	   SSL23
     *
     * Default is PJSIP_SSL_UNSPECIFIED_METHOD (0), which in turn will
     * use PJSIP_SSL_DEFAULT_METHOD, which default value is 
     * PJSIP_TLSV1_METHOD.
     */
    int		method;

    /**
     * TLS cipher list string in OpenSSL format. If empty, then default
     * cipher list of the backend will be used.
     */
    pj_str_t	ciphers;

    /**
     * Optionally specify the server name instance to be contacted when
     * making outgoing TLS connection. This setting is useful when the
     * server is hosting multiple domains for the same TLS listening
     * socket.
     *
     * Default: empty.
     */
    pj_str_t	server_name;

    /**
     * When PJSIP is acting as a client (outgoing TLS connections), 
     * it will always receive a certificate from the peer. 
     * If \a verify_server is disabled (set to zero), PJSIP will not 
     * verifiy the certificate and allows TLS connections to servers 
     * which do not present a valid certificate. 
     * If \a tls_verify_server is non-zero, PJSIP verifies the server 
     * certificate and will close the TLS connection if the server 
     * certificate is not valid.
     *
     * This setting corresponds to OpenSSL SSL_VERIFY_PEER flag.
     * Default value is zero.
     */
    pj_bool_t	verify_server;

    /**
     * When acting as server (incoming TLS connections), setting
     * \a verify_client to non-zero will cause the transport to activate
     * peer verification upon receiving incoming TLS connection.
     *
     * This setting corresponds to OpenSSL SSL_VERIFY_PEER flag.
     * Default value is zero.
     */
    pj_bool_t	verify_client;

    /**
     * When acting as server (incoming TLS connections), reject inocming
     * connection if client doesn't have a valid certificate.
     *
     * This setting corresponds to SSL_VERIFY_FAIL_IF_NO_PEER_CERT flag.
     * Default value is zero.
     */
    pj_bool_t	require_client_cert;

    /**
     * TLS negotiation timeout to be applied for both outgoing and
     * incoming connection. If both sec and msec member is set to zero,
     * the SSL negotiation doesn't have a timeout.
     */
    pj_time_val	timeout;

    /**
     * QoS traffic type to be set on this transport. When application wants
     * to apply QoS tagging to the transport, it's preferable to set this
     * field rather than \a qos_param fields since this is more portable.
     *
     * Default value is PJ_QOS_TYPE_BEST_EFFORT.
     */
    pj_qos_type qos_type;

    /**
     * Set the low level QoS parameters to the transport. This is a lower
     * level operation than setting the \a qos_type field and may not be
     * supported on all platforms.
     *
     * By default all settings in this structure are disabled.
     */
    pj_qos_params qos_params;

    /**
     * Specify if the transport should ignore any errors when setting the QoS
     * traffic type/parameters.
     *
     * Default: PJ_TRUE
     */
    pj_bool_t qos_ignore_error;

} pjsip_tls_setting;


/**
 * Initialize TLS setting with default values.
 *
 * @param tls_opt   The TLS setting to be initialized.
 */
PJ_INLINE(void) pjsip_tls_setting_default(pjsip_tls_setting *tls_opt)
{
    pj_memset(tls_opt, 0, sizeof(*tls_opt));
    tls_opt->qos_type = PJ_QOS_TYPE_BEST_EFFORT;
    tls_opt->qos_ignore_error = PJ_TRUE;
}


/**
 * Copy TLS setting.
 *
 * @param pool	    The pool to duplicate strings etc.
 * @param dst	    Destination structure.
 * @param src	    Source structure.
 */
PJ_INLINE(void) pjsip_tls_setting_copy(pj_pool_t *pool,
				       pjsip_tls_setting *dst,
				       const pjsip_tls_setting *src)
{
    pj_memcpy(dst, src, sizeof(*dst));
    pj_strdup_with_null(pool, &dst->ca_list_file, &src->ca_list_file);
    pj_strdup_with_null(pool, &dst->cert_file, &src->cert_file);
    pj_strdup_with_null(pool, &dst->privkey_file, &src->privkey_file);
    pj_strdup_with_null(pool, &dst->password, &src->password);
    pj_strdup_with_null(pool, &dst->ciphers, &src->ciphers);
}


/**
 * Register support for SIP TLS transport by creating TLS listener on
 * the specified address and port. This function will create an
 * instance of SIP TLS transport factory and register it to the
 * transport manager.
 *
 * @param endpt		The SIP endpoint.
 * @param opt		Optional TLS settings.
 * @param local		Optional local address to bind, or specify the
 *			address to bind the server socket to. Both IP 
 *			interface address and port fields are optional.
 *			If IP interface address is not specified, socket
 *			will be bound to PJ_INADDR_ANY. If port is not
 *			specified, socket will be bound to any port
 *			selected by the operating system.
 * @param a_name	Optional published address, which is the address to be
 *			advertised as the address of this SIP transport. 
 *			If this argument is NULL, then the bound address
 *			will be used as the published address.
 * @param async_cnt	Number of simultaneous asynchronous accept()
 *			operations to be supported. It is recommended that
 *			the number here corresponds to the number of
 *			processors in the system (or the number of SIP
 *			worker threads).
 * @param p_factory	Optional pointer to receive the instance of the
 *			SIP TLS transport factory just created.
 *
 * @return		PJ_SUCCESS when the transport has been successfully
 *			started and registered to transport manager, or
 *			the appropriate error code.
 */
PJ_DECL(pj_status_t) pjsip_tls_transport_start(pjsip_endpoint *endpt,
					       const pjsip_tls_setting *opt,
					       const pj_sockaddr_in *local,
					       const pjsip_host_port *a_name,
					       unsigned async_cnt,
					       pjsip_tpfactory **p_factory);



PJ_END_DECL

/**
 * @}
 */

#endif	/* __PJSIP_TRANSPORT_TLS_H__ */
