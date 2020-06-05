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
*                                                EXAMPLE
*
*                                  HTTP CLIENT APPLICATION FUNCTIONS FILE
*
* Filename : http-c_app.c
* Version  : V3.00.06
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                           INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#define    HTTPc_APP_MODULE
#include  <stdio.h>

#include  "http-c_app.h"
#include  "http-c_hooks.h"


#if (HTTPc_APP_FS_MODULE_PRESENT == DEF_YES)
#include  <fs_app.h>
#endif

#if (NET_SECURE_CFG_EN == DEF_ENABLED)
#ifdef NET_SECURE_EMSSL_MODULE
#include  <certificates.h>
#include  <SSL.h>
#endif
#include  <Secure/net_secure.h>
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL (STATIC) VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

        HTTPc_REQ_OBJ   HTTPcApp_Req;
        HTTPc_RESP_OBJ  HTTPcApp_Resp;
static  CPU_CHAR        HTTPcApp_ConnBuf[HTTPc_APP_CFG_CONN_BUF_SIZE];
static  HTTPc_KEY_VAL   HTTPcApp_ReqQueryStrTbl[HTTPc_APP_CFG_QUERY_STR_NBR_MAX];
static  CPU_CHAR        HTTPcApp_ReqQueryStrKeyTbl[HTTPc_APP_CFG_QUERY_STR_NBR_MAX][HTTPc_APP_CFG_QUERY_STR_KEY_LEN_MAX];
static  CPU_CHAR        HTTPcApp_ReqQueryStrValTbl[HTTPc_APP_CFG_QUERY_STR_NBR_MAX][HTTPc_APP_CFG_QUERY_STR_VAL_LEN_MAX];
static  CPU_INT16U      SecureServerPort = 8080u;


/*
*********************************************************************************************************
*                                            HTTPcApp_Init()
*
* Description : Initialize the uC/HTTP-client stack for the example application.
*
* Argument(s) : None.
*
* Return(s)   : DEF_OK,   if initialization was successful.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  HTTPcApp_Init (void)
{
    HTTPc_ERR  httpc_err;
#if (NET_SECURE_CFG_EN == DEF_ENABLED)
    NET_ERR    net_err;
#endif

                                                                /* ------------- INITIALIZE CLIENT SUITE -------------- */
#if (HTTPc_CFG_MODE_ASYNC_TASK_EN == DEF_ENABLED)
    HTTPc_Init(&HTTPc_Cfg, &HTTPc_TaskCfg, DEF_NULL, &httpc_err);
    if (httpc_err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }
#else
    HTTPc_Init(&HTTPc_Cfg, DEF_NULL, DEF_NULL, &httpc_err);
    if (httpc_err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }
#endif

#if (NET_SECURE_CFG_EN == DEF_ENABLED)
    NetSecure_CA_CertInstall(&emSSL_MQTT_DST_Certificate,
                              emSSL_MQTT_DST_Certificate.pData->CertDERLen,
                              NET_SECURE_CERT_FMT_DER,
                             &net_err);
    if (net_err != NET_SECURE_ERR_NONE) {
        return (DEF_FAIL);
    }

    NetSecure_CA_CertInstall(&emSSL_MQTT_LetsEncrypt_Certificate,
                              emSSL_MQTT_LetsEncrypt_Certificate.pData->CertDERLen,
                              NET_SECURE_CERT_FMT_DER,
                             &net_err);
    if (net_err != NET_SECURE_ERR_NONE) {
        return (DEF_FAIL);
    }
#endif

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                       HTTPcApp_ServerConn()
*
* Description : This function establishes a connection (persistent) to the web server.
*
* Argument(s) : p_conn    A reference to the data structure to be used for the connection
*
* Return(s)   : DEF_OK,   if persistent connection was successfully established
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  HTTPcApp_ServerConn (HTTPc_CONN_OBJ *p_conn)
{
    HTTPc_FLAGS  flags;
    CPU_SIZE_T   str_len;
    CPU_BOOLEAN  persist;
    HTTPc_ERR    err;
    NET_ERR      err_net;

                                                                /* ----------- INIT NEW CONNECTION & REQUEST ---------- */
    HTTPc_ConnClr(p_conn, &err);
    if (err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }    

#if (HTTPc_CFG_MODE_ASYNC_TASK_EN == DEF_ENABLED)
    HTTPc_ConnSetParam(         p_conn,
                                HTTPc_PARAM_TYPE_CONN_CLOSE_CALLBACK,
                       (void *)&HTTPcApp_ConnCloseCallback,
                               &err);
    if (err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }
#endif

    persist = DEF_NO;

    HTTPc_ConnSetParam(         p_conn,
                                HTTPc_PARAM_TYPE_PERSISTENT,
                       (void *)&persist,
                               &err);

    HTTPc_ConnSetParam(         p_conn,
                                HTTPc_PARAM_TYPE_SERVER_PORT,
                       (void *)&SecureServerPort,
                               &err);
#if (NET_SECURE_CFG_EN == DEF_ENABLED)
    HTTPc_ConnSetParam(         p_conn,
                                HTTPc_PARAM_TYPE_SECURE_TRUST_CALLBACK,
                       (void *)&HTTPcApp_TrustCallback,
                               &err);

    HTTPc_ConnSetParam(         p_conn,
                                HTTPc_PARAM_TYPE_SECURE_COMMON_NAME,
                       (void *)&HTTP_SERVER_HOSTNAME,
                               &err);
#endif
                                                                /* ----------------- OPEN CONNECTION ------------------ */
    str_len = Str_Len(HTTP_SERVER_HOSTNAME);
    flags   = HTTPc_FLAG_NONE;

    HTTPc_ConnOpen(p_conn,
                   HTTPcApp_ConnBuf,
                   HTTPc_APP_CFG_CONN_BUF_SIZE,
                   HTTP_SERVER_HOSTNAME,
                   str_len,
                   flags,
                  &err);

    if (err != HTTPc_ERR_NONE) {
        HTTPc_APP_TRACE_DBG(("Connect error: %i\n\r", err));
        HTTPcApp_CloseConn (p_conn);
        return (DEF_FAIL);
    }
    NetSock_CfgSecure(p_conn->SockID_reserved, DEF_YES, &err_net);

    return (DEF_OK);
}    


/*
*********************************************************************************************************
*                                       HTTPcApp_ServerReq()
*
* Description : This function sends a GET request to the webserver.
*
* Argument(s) : p_conn    A reference to the connection for this request
*
* Return(s)   : DEF_OK,   if persistent connection was successfully established
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  HTTPcApp_ServerReq (HTTPc_CONN_OBJ *p_conn)
{
    HTTPc_FLAGS  flags;
    CPU_SIZE_T   str_len;
    HTTPc_ERR    err;  
     
    
#if (HTTPc_CFG_USER_DATA_EN == DEF_ENABLED)
    HTTPcApp_Req.UserDataPtr = (void *)&HTTPcApp_Data[0];
#endif
  
    HTTPc_ReqClr(&HTTPcApp_Req, &err);
    if (err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }
                                                             /* ------------ SET REQ/RESP HOOK FUNCTIONS ----------- */
    HTTPc_ReqSetParam(        &HTTPcApp_Req,
                               HTTPc_PARAM_TYPE_RESP_HDR_HOOK,
                      (void *)&HTTPcApp_RespHdrHook,
                              &err);
    if (err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }

    HTTPc_ReqSetParam(        &HTTPcApp_Req,
                               HTTPc_PARAM_TYPE_RESP_BODY_HOOK,
                      (void *)&HTTPcApp_RespBodyHook,
                              &err);
    if (err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }

                                                                /* --------- SET REQ/RESP CALLBACK FUNCTIONS ---------- */
#if (HTTPc_CFG_MODE_ASYNC_TASK_EN == DEF_ENABLED)
    HTTPc_ReqSetParam(        &HTTPcApp_Req,
                               HTTPc_PARAM_TYPE_TRANS_COMPLETE_CALLBACK,
                      (void *)&HTTPcApp_TransDoneCallback,
                              &err);
    if (err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }

    HTTPc_ReqSetParam(        &HTTPcApp_Req,
                               HTTPc_PARAM_TYPE_TRANS_ERR_CALLBACK,
                      (void *)&HTTPcApp_TransErrCallback,
                              &err);
    if (err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }
#endif

    flags   = HTTPc_FLAG_NONE;
    str_len = Str_Len(HTTPc_APP_CFG_URI);

    (void)HTTPc_ReqSend(p_conn,
                        &HTTPcApp_Req,
                        &HTTPcApp_Resp,
                        HTTP_METHOD_GET,
                        HTTPc_APP_CFG_URI,
                        str_len,
                        flags,
                       &err);
    if (err != HTTPc_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                       HTTPcApp_CloseConn()
*
* Description : Close the connection to the web server.
*
* Argument(s) : p_conn  Data structure corresponding to the connection to be closed
*
* Return(s)   : DEF_OK,   if persistent connection was successfully established
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  HTTPcApp_CloseConn (HTTPc_CONN_OBJ *p_conn)
{
    HTTPc_ERR  err;  


    HTTPc_ConnClose (p_conn, HTTPc_FLAG_NONE, &err);
    if (err != HTTPc_ERR_NONE) {
        HTTPc_APP_TRACE_DBG(("Close error: %i\n\r", err));
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (NET_SECURE_CFG_EN == DEF_ENABLED)
CPU_BOOLEAN  HTTPcApp_TrustCallback (void                              *ptr, 
                                     NET_SOCK_SECURE_UNTRUSTED_REASON   reason)
{
    CPU_BOOLEAN  ret_code;


    switch (reason) {
        case NET_SOCK_SECURE_UNTRUSTED_BY_CA:
        case NET_SOCK_SECURE_EXPIRE_DATE:
        case NET_SOCK_SECURE_INVALID_DATE:
        case NET_SOCK_SECURE_UNKNOWN:
             ret_code = DEF_NO;
             break;


        case NET_SOCK_SECURE_SELF_SIGNED:
        default:
             break;
    }
    return (ret_code);
}
#endif
