/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       HTTP INSTANCE APPLICATION
*
* Filename : app_http-s.c
* Version  : V3.00.06
*********************************************************************************************************
* Note(s)  : (1) This example shows how to initialize uC/HTTPs, initialize a web server instance and start it.
*
*            (2) This example is for :
*
*                  (a) uC/TCPIP - V3.00.01 and up
*                  (b) uC/FS    - V4.x
*
*            (3) This file is an example about how to use uC/HTTPs, It may not cover all case needed by a real
*                application. Also some modification might be needed, insert the code to perform the stated
*                actions wherever 'TODO' comments are found.
*
*                (a) For example this example doesn't manage the link state (plugs and unplugs), this can
*                    be a problem when switching from a network to another network.
*
*                (b) This example is not fully tested, so it is not guaranteed that all cases are cover
*                    properly.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*
* Note(s) : (1) (a) The uC/TCP-IP Network file system abstraction layer folder for traditional file system;
*
*                       '$uC-TCPIP/FS/<fs>/net_<fs>.h'
*
*
*               (b) The static file system API is located under uC/HTTPs File system folder;
*
*                       '$uc-HTTPs/FS/Static/http-s_fs_static.h'
*********************************************************************************************************
*/

#include <app_http-s.h>
#if (APP_CFG_HTTP_S_EN == DEF_ENABLED)
#include  "static_files.h"
#include  <Server/Source/http-s.h>
#include  <http-s_instance_cfg.h>
#include  <Server/FS/Static/http-s_fs_static.h>
#if (NET_SECURE_CFG_EN == DEF_ENABLED)
#include  <Secure/net_secure.h>
#include  <http-s_instance_secure_cfg.h>
#endif


/*
*********************************************************************************************************
*                                             AppHTTPs_Init
*
* Description : Initialize HTTP server.
*
* Arguments   : none.
*
* Returns     : none.
*
* Notes       : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  AppHTTPs_Init (void)
{
    HTTPs_ERR        err_https;
    HTTPs_INSTANCE  *p_https_instance;
    CPU_BOOLEAN      is_fs_static;      
    CPU_BOOLEAN      success;

                                                                /* --------------- ADDING STATIC FILES ---------------- */
    success = HTTPs_FS_Init();
    if (!success) {
        APP_TRACE_INFO(("HTTP static file system initialization failed\n\r"));
        return (DEF_FAIL);
    }

#if (NET_SECURE_CFG_EN == DEF_ENABLED)
    is_fs_static = (HTTPs_CfgInstance_Secure.FS_Type == HTTPs_FS_TYPE_STATIC);
#else
    is_fs_static = (HTTPs_CfgInstance.FS_Type == HTTPs_FS_TYPE_STATIC);
#endif      
    
    if (is_fs_static) {
        success = HTTPs_FS_AddFile(        STATIC_INDEX_HTML_NAME,
                                   (void *)index_html,
                                           STATIC_INDEX_HTML_LEN);
        if (!success) {
            APP_TRACE_INFO(("HTTP static file \"%s\" could not be added.\n\r", STATIC_INDEX_HTML_NAME));
            return (DEF_FAIL);
        }
        
        success = HTTPs_FS_AddFile(        STATIC_LOGO_GIF_NAME,
                                   (void *)logo_gif,
                                           STATIC_LOGO_GIF_LEN);
        if (!success) {
            APP_TRACE_INFO(("HTTP static file \"%s\" could not be added.\n\r", STATIC_INDEX_HTML_NAME));
            return (DEF_FAIL);
        }
        
        success = HTTPs_FS_AddFile(        STATIC_404_HTML_NAME,
                                   (void *)not_found_html,
                                           STATIC_404_HTML_LEN);
        if (!success) {
            APP_TRACE_INFO(("HTTP static file \"%s\" could not be added.\n\r", STATIC_INDEX_HTML_NAME));
            return (DEF_FAIL);
        }
    }

                                                                /* -------------- INITIALIZE HTTP SERVER -------------- */
    APP_TRACE_INFO(("\n\r"));
    APP_TRACE_INFO(("Initialize HTTP server ...\n\r"));

    HTTPs_Init(DEF_NULL, &err_https);
    if (err_https != HTTPs_ERR_NONE) {
        APP_TRACE_INFO(("HTTP server: HTTPs_Init() failed\n\r"));
        return (DEF_FAIL);
    }

    if (err_https == HTTPs_ERR_NONE) {
#if (NET_SECURE_CFG_EN == DEF_ENABLED)
        p_https_instance = HTTPs_InstanceInit(&HTTPs_CfgInstance_Secure,
                                              &HTTPs_TaskCfgInstance_Secure,
                                              &err_https);
#else
        p_https_instance = HTTPs_InstanceInit(&HTTPs_CfgInstance,
                                              &HTTPs_TaskCfgInstance,
                                              &err_https);
#endif
        if (err_https != HTTPs_ERR_NONE) {
            APP_TRACE_INFO(("HTTP server: HTTPs_InstanceInit() failed\n\r"));
            return (DEF_FAIL);
        }

        HTTPs_InstanceStart(p_https_instance, &err_https);
    }

    if (err_https == HTTPs_ERR_NONE) {
        APP_TRACE_INFO(("HTTP server successfully initialized\n\r"));
        success = DEF_OK;
    } else {
        APP_TRACE_INFO(("HTTP server initialization failed\n\r"));
        success = DEF_FAIL;
    }

    return (success);
}
#endif