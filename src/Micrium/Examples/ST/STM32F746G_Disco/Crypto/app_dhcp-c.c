/*
*********************************************************************************************************
*                                              EXAMPLE CODE
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
*               You can find information about uC/DHCPc by visiting https://doc.micrium.com
*
*               You can contact us at: http://www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                              uC/DHCP-c
*                                           APPLICATION CODE
*
* Filename : app_dhcp-c.c
* Version  : V2.10.01
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <app_dhcp-c.h>


/*
*********************************************************************************************************
*                                               ENABLE
*********************************************************************************************************
*/

#if (APP_CFG_DHCP_C_EN == DEF_ENABLED)

#include  <IP/IPv4/net_ipv4.h>
#include  <IF/net_if_ether.h>

#include  <os.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                    LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           AppDHCPc_Init()
*
* Description : Initialize DHCP client for specified interface.
*
* Argument(s) : if_nbr      Interface number to start DHCP management.
*
* Return(s)   : none.
*
* Caller(s)   : App_TCPIP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  AppDHCPc_Init (NET_IF_NBR  if_nbr)
{
    DHCPc_OPT_CODE  req_param[DHCPc_CFG_PARAM_REQ_TBL_SIZE];
    CPU_BOOLEAN     cfg_done;
    CPU_BOOLEAN     dly;
    DHCPc_STATUS    dhcp_status;
    NET_IPv4_ADDR   ip;
    NET_IPv4_ADDR   msk;
    NET_IPv4_ADDR   gateway;
    NET_ERR         err_net;
    DHCPc_ERR       err_dhcp;
    OS_ERR          os_err;


                                                                /* -------------- INITIALIZE DHCP CLIENT -------------- */
    APP_TRACE_INFO(("\r\n"));
    APP_TRACE_INFO(("===================================================================\r\n"));
    APP_TRACE_INFO(("=                       DHCPc INITIALIZATION                      =\r\n"));
    APP_TRACE_INFO(("===================================================================\r\n"));
    APP_TRACE_INFO(("Initialize DHCP client ...\n\r"));

    err_dhcp = DHCPc_Init();
    if (err_dhcp != DHCPc_ERR_NONE) {
        APP_TRACE_INFO(("DHCPc_Init() failed w/err = %d \r\n", err_dhcp));
        return (DEF_FAIL);
    }

                                                                /* ----------- START INTERFACE'S DHCP CLIENT ---------- */
    Mem_Clr(req_param, DHCPc_CFG_PARAM_REQ_TBL_SIZE);


    DHCPc_Start( if_nbr,
                &req_param[0],
                 0u,
                &err_dhcp);
    if (err_dhcp != DHCPc_ERR_NONE) {
        APP_TRACE_INFO(("DHCPc_Start() failed w/err = %d \r\n", err_dhcp));
        return (DEF_FAIL);
    }

    APP_TRACE_INFO(("DHCP address configuration started\n\r"));

                                                                /* ------ CHECK INTERFACE'S DHCP CONFIGURE STATUS ----- */
    dhcp_status = DHCP_STATUS_NONE;
    cfg_done    = DEF_NO;
    dly         = DEF_NO;

    while (cfg_done != DEF_YES) {
        if (dly == DEF_YES) {
            OSTimeDlyHMSM( 0u, 0u, 0u, 100u,
                           OS_OPT_TIME_HMSM_STRICT,
                          &os_err);
        }

        dhcp_status = DHCPc_ChkStatus(if_nbr, &err_dhcp);
        switch (dhcp_status) {
            case DHCP_STATUS_CFGD:
                 APP_TRACE_INFO(("DHCP address configured\n\r"));
                 cfg_done = DEF_YES;
                 break;


            case DHCP_STATUS_CFGD_NO_TMR:
                 APP_TRACE_INFO(("DHCP address configured (no timer)\n\r"));
                 cfg_done = DEF_YES;
                 break;


            case DHCP_STATUS_CFGD_LOCAL_LINK:
                 APP_TRACE_INFO(("DHCP address configured (link-local)\n\r"));
                 cfg_done = DEF_YES;
                 break;


            case DHCP_STATUS_FAIL:
                 APP_TRACE_INFO(("DHCP address configuration failed\n\r"));
                 cfg_done = DEF_YES;
                 break;


            case DHCP_STATUS_CFG_IN_PROGRESS:
            default:
                 dly = DEF_YES;
                 break;
        }
    }

                                                                /* ------------ DISPLAY INTERFACE'S ADDRESS ----------- */
    APP_TRACE_INFO(("\n\r"));

    if (dhcp_status == DHCP_STATUS_FAIL) {
        APP_TRACE_INFO(("Configured default IP address after DHCP failed"));

        ip      = NetASCII_Str_to_IPv4("192.168.1.240", &err_net);
        msk     = NetASCII_Str_to_IPv4("255.255.255.0", &err_net);
        gateway = NetASCII_Str_to_IPv4("192.168.1.1",   &err_net);

        (void)NetIPv4_CfgAddrAdd(if_nbr,
                                 ip,
                                 msk,
                                 gateway,
                                &err_net);
        if (err_net != NET_IPv4_ERR_NONE) {
            APP_TRACE_INFO(("NetIPv4_CfgAddrAdd() failed w/err = %d \r\n", err_net));
            return (DEF_FAIL);
        }
    }

    return (DEF_OK);
}
#endif
