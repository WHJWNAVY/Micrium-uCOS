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
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                   TCP/IP APPLICATION INITIALIZATION
*
*                                       ST Microelectronics STM32
*                                                 on the
*
*                                            STM32F746G-DISCO
*                                            Evaluation Board
*
* Filename : app_tcpip.c
* Version  : V3.04.06
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    APP_TCPIP_MODULE
#include  <app_tcpip.h>
#include  <bsp_net_gmac.h>
#include  <Source/sntp-c.h>
#include  <Source/clk.h>

#if (APP_CFG_TCPIP_EN == DEF_ENABLED)
#include <Source/dns-c.h>

#if  APP_CFG_DHCP_C_EN == DEF_ENABLED
#include  <app_dhcp-c.h>
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  APP_TCPIP_ADDR_LEN                     16u             /* Maximum Length of standard TCP/IP Address(s).        */


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

CPU_CHAR    App_Dflt_GW      [16u];
CPU_CHAR    App_SubMsk       [16u];
CPU_CHAR    App_Dflt_IP_Addr [16u];
NET_IF_NBR  Eth1_IF_Nbr;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

         void  AppTCPIP_PrintIPAddr      (       NET_IF_NBR                 if_nbr);
#if (NET_IPv6_CFG_EN == DEF_ENABLED)
static  void  AppTCPIP_AddrAutoCfgResult (       NET_IF_NBR                 if_nbr,
                                          const  NET_IPv6_ADDR             *p_addr_local_cfgd,
                                          const  NET_IPv6_ADDR             *p_addr_global_cfgd,
                                                 NET_IPv6_AUTO_CFG_STATUS   auto_cfg_result);
#endif


/*
*********************************************************************************************************
*                                            AppTCPIP_Init()
*
* Description : Initialize uC/TCP-IP.
*
* Arguments   : perr    Pointer to variable to store NET_ERR return code.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  AppTCPIP_Init (void)
{
    NET_IF_NBR     if_nbr;
#if (APP_CFG_DHCP_C_EN == DEF_DISABLED)
    NET_IPv4_ADDR  ip;
    NET_IPv4_ADDR  msk;
    NET_IPv4_ADDR  gateway;
#else
    CPU_BOOLEAN    started;
#endif
#if (NET_IPv6_CFG_EN == DEF_ENABLED)
    CPU_BOOLEAN    detect_dup_addr;
#endif
    CPU_BOOLEAN    sntp_ok;
    SNTPc_ERR      sntpc_err;
    CPU_BOOLEAN    dnsc_ok;
    NET_ERR        net_err;

                                                                /* --------------------- INIT TCPIP ------------------- */
    APP_TRACE_INFO(("===================================================================\r\n"));
    APP_TRACE_INFO(("=                       TCPIP INITIALIZATION                      =\r\n"));
    APP_TRACE_INFO(("===================================================================\r\n"));
    APP_TRACE_INFO(("Initializing TCPIP...\r\n"));
    APP_TRACE_INFO(("\r\n"));

    net_err = Net_Init(&NetRxTaskCfg,                           /* Initialize uC/TCP-IP.                                */
                       &NetTxDeallocTaskCfg,
                       &NetTmrTaskCfg);
    if (net_err != NET_ERR_NONE) {
        APP_TRACE_INFO(("Net_Init() failed w/err = %d \r\n", net_err));
        return (DEF_FAIL);
    }
                                                                /* -------------- ETHERNET CONFIGURATION -------------- */
    if_nbr  = NetIF_Add((void *)&NetIF_API_Ether,               /* Ethernet         Interface API.                      */
                        (void *)&NetDev_API_GMAC,               /* STM32F746xx      Device API.                         */
                        (void *)&NetDev_BSP_STM32F746G_DISCO_0, /* STM32F746G-DISCO Device BSP.                         */
                        (void *)&NetDev_Cfg_STM32F746_0,        /* STM32F746xx      Device Configuration.               */
                        (void *)&NetPhy_API_Generic,            /* Generic          Phy API.                            */
                        (void *)&NetPhy_Cfg_STM32F746G_DISCO_0, /* STM32F746G-DISCO PHY Configuration.                  */
                                &net_err);
    if (net_err != NET_IF_ERR_NONE) {
        APP_TRACE_INFO(("NetIF_Add() failed w/err = %d \r\n", net_err));
        return (DEF_FAIL);
    }
                                                                /* ------------------ START IF NBR 1 ------------------ */
    NetIF_Start(if_nbr, &net_err);
    if (net_err != NET_IF_ERR_NONE) {
        APP_TRACE_INFO(("NetIF_Start() failed w/err = %d \r\n", net_err));
        return (DEF_FAIL);
    }

    NetIF_LinkStateWaitUntilUp(if_nbr, 20, 200, &net_err);
    if (net_err != NET_IF_ERR_NONE) {
        APP_TRACE_INFO(("NetIF_LinkStateWaitUntilUp() failed w/err = %d \r\n", net_err));
        return (DEF_FAIL);
    }
    Eth1_IF_Nbr = if_nbr;
                                                                /* ------------------- CFG IF NBR 1 ------------------- */
#if  APP_CFG_DHCP_C_EN == DEF_ENABLED
    started = AppDHCPc_Init(if_nbr);

    if (started != DEF_YES) {
        APP_TRACE_INFO(("App_DHCPc_Init() returned an error, uC/DHCPc is not started"));
        return (DEF_FAIL);
    }
#else

    ip      = NetASCII_Str_to_IPv4("192.168.2.26",  &net_err);
    msk     = NetASCII_Str_to_IPv4("255.255.255.0", &net_err);
    gateway = NetASCII_Str_to_IPv4("192.168.2.1",   &net_err);

    NetIPv4_CfgAddrAdd(if_nbr,
                       ip,
                       msk,
                       gateway,
                      &net_err);
    if (net_err != NET_IPv4_ERR_NONE) {
        APP_TRACE_INFO(("NetIPv4_CfgAddrAdd() failed w/err = %d \r\n", net_err));
        return (DEF_FAIL);
    }
#endif

    AppTCPIP_PrintIPAddr(if_nbr);                               /* Get & Display IP Address                             */

#if (NET_IPv6_CFG_EN == DEF_ENABLED)
                                                                /* ---------------- CFG IF NBR 2 (IPv6) --------------- */
    NetIPv6_AddrAutoCfgHookSet(if_nbr, AppTCPIP_AddrAutoCfgResult, &net_err);
    if (net_err != NET_IPv6_ERR_NONE) {
        APP_TRACE_INFO(("NetIPv6_AddrAutoCfgHookSet() failed w/ err = %d \r\n", net_err));
        return (DEF_FAIL);
    }

    APP_TRACE_INFO(("Starting IPv6 Auto-Configuration.\r\n\r\n"));
    detect_dup_addr = DEF_TRUE;
    NetIPv6_AddrAutoCfgEn(if_nbr, detect_dup_addr, &net_err);
    if (net_err != NET_IPv6_ERR_NONE) {
        APP_TRACE_INFO(("NetIPv6_AddrAutoCfgEn() failed w/ err = %d \r\n", net_err));
        return (DEF_FAIL);
    }
#endif

#if (NET_EXT_MODULE_CFG_DNS_EN == DEF_ENABLED)
    dnsc_ok = AppDNSc_Init();                                   /* Initialize uC/DNSc DNS client.                       */
    if (dnsc_ok != DEF_OK) {
       APP_TRACE_INFO(("\r\nDNSc failed to initialize\r\n"));
    }
    KAL_Dly(1000);
#endif

    sntp_ok  = SNTPc_Init(&SNTPc_Cfg, &sntpc_err);
    sntp_ok &= App_SNTPc_SetClk(&SNTPc_Cfg);

#if (NET_SECURE_CFG_EN == DEF_ENABLED)
    if (sntp_ok != DEF_OK) {
       APP_TRACE_INFO(("\r\nSNTPc failed to initialize\r\n"));
    }
    return (sntp_ok);
#else
    return (DEF_OK);
#endif
}


/*
*********************************************************************************************************
*                                          App_AutoCfgResult()
*
* Description : Hook function to received the IPv6 Auto-Configuration process result.
*
* Argument(s) : if_nbr               Network Interface number on which Auto-Configuration occurred.
*
*               p_addr_local_cfgd    Pointer to IPv6 link-local address configured, if any.
*                                    DEF_NULL, otherwise.
*
*               p_addr_global_cfgd   Pointer to IPv6 global address configured, if any.
*                                    DEF_NULL, otherwise.
*
*               auto_cfg_result      Result status of the IPv6 Auto-Configuration process.
*
* Return(s)   : None.
*
* Caller(s)   : AppTCPIP_Init().
*
* Note(s)     : None.
*********************************************************************************************************
*/

#if (NET_IPv6_CFG_EN == DEF_ENABLED)
static  void  AppTCPIP_AddrAutoCfgResult (       NET_IF_NBR                 if_nbr,
                                          const  NET_IPv6_ADDR             *p_addr_local_cfgd,
                                          const  NET_IPv6_ADDR             *p_addr_global_cfgd,
                                                 NET_IPv6_AUTO_CFG_STATUS   auto_cfg_result)
{
    CPU_CHAR  ip_str[NET_ASCII_LEN_MAX_ADDR_IPv6];
    NET_ERR   net_err;


    switch (auto_cfg_result) {
        case NET_IPv6_AUTO_CFG_STATUS_FAILED:
             APP_TRACE_INFO(("IPv6 Auto-Configuration failed.\r\n\r\n"));
             break;


        case NET_IPv6_AUTO_CFG_STATUS_SUCCEEDED:
             APP_TRACE_INFO(("IPv6 Auto-Configuration succeeded.\r\n\r\n"));
             break;


        case NET_IPv6_AUTO_CFG_STATUS_LINK_LOCAL:
             APP_TRACE_INFO(("IPv6 Auto-Configuration succeeded. (Link-Local only.)\r\n\r\n"));
             break;


        default:
             APP_TRACE_INFO(("Unknown IPv6 Auto-Configuration result.\r\n\r\n"));
             break;
    }

    if (p_addr_local_cfgd != DEF_NULL) {
        NetASCII_IPv6_to_Str((NET_IPv6_ADDR *)p_addr_local_cfgd, ip_str, DEF_NO, DEF_YES, &net_err);
        APP_TRACE_INFO(("\tLink-Local Address: %s\r\n", ip_str));
    }

    if (p_addr_global_cfgd != DEF_NULL) {
        NetASCII_IPv6_to_Str((NET_IPv6_ADDR *)p_addr_global_cfgd, ip_str, DEF_NO, DEF_YES, &net_err);
        APP_TRACE_INFO(("\tGlobal Address    : %s\r\n", ip_str));
    }
}
#endif


/*
*********************************************************************************************************
*                                        AppTCPIP_PrintIPAddr()
*
* Description : Obtain and Display IP Address, Subnet Mask, and Default Gateway on TCPIP IF Connection.
*
* Arguments   : if_nbr      Interface Number to obtain interface information.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  AppTCPIP_PrintIPAddr (NET_IF_NBR  if_nbr)
{
    NET_IPv4_ADDR     ip_addr_tbl[NET_IPv4_CFG_IF_MAX_NBR_ADDR];
    NET_IPv4_ADDR     subnet_addr;
    NET_IPv4_ADDR     gateway_addr;
    NET_IP_ADDRS_QTY  ip_addr_tbl_qty;
    NET_ERR           net_err;
    CPU_CHAR          ip_disp_addr[APP_TCPIP_ADDR_LEN];
    CPU_CHAR          sub_disp_addr[APP_TCPIP_ADDR_LEN];
    CPU_CHAR          gate_disp_addr[APP_TCPIP_ADDR_LEN];


    Mem_Clr(ip_disp_addr,   sizeof(ip_disp_addr));              /* Init Var(s).                                         */
    Mem_Clr(sub_disp_addr,  sizeof(sub_disp_addr));
    Mem_Clr(gate_disp_addr, sizeof(gate_disp_addr));

    ip_addr_tbl_qty = sizeof(ip_addr_tbl) / sizeof(NET_IPv4_ADDR);

    NetIPv4_GetAddrHost(if_nbr,                                 /* Get IPv4 Host Addresses based on Interface Number.   */
                        &ip_addr_tbl[0u],
                        &ip_addr_tbl_qty,
                        &net_err);

    if (net_err != NET_IPv4_ERR_NONE) {
        APP_TRACE_INFO(("NetIPv4_GetAddrHost() failed w/err = %d \r\n", net_err));
        return;
    }
                                                                /* Get IPv4 Subnet Mask & Dflt Gateway from Host Addr   */
    subnet_addr  = NetIPv4_GetAddrSubnetMask (ip_addr_tbl[0u], &net_err);
    gateway_addr = NetIPv4_GetAddrDfltGateway(ip_addr_tbl[0u], &net_err);

    NetASCII_IPv4_to_Str( ip_addr_tbl[0u],                      /* Set IPv4 IP Address to string.                       */
                         &ip_disp_addr[0u],
                          DEF_NO,
                         &net_err);
    Str_Copy(&App_Dflt_IP_Addr[0u], &ip_disp_addr[0u]);

    NetASCII_IPv4_to_Str( subnet_addr,                          /* Set IPv4 Subnet Mask to String.                      */
                         &sub_disp_addr[0u],
                          DEF_NO,
                         &net_err);
    Str_Copy(&App_SubMsk[0u], &sub_disp_addr[0u]);

    NetASCII_IPv4_to_Str( gateway_addr,                         /* Set IPv4 Default Gateway to String.                  */
                         &gate_disp_addr[0u],
                          DEF_NO,
                         &net_err);
    Str_Copy(&App_Dflt_GW[0u], &gate_disp_addr[0u]);

                                                                /* -------------------- DISPLAY ADDR ------------------ */
    APP_TRACE_INFO(("- ETHERNET CONFIGURATION - \r\n"));
    APP_TRACE_INFO(("IP Address      : %s \r\n", ip_disp_addr  ));
    APP_TRACE_INFO(("Subnet Mask     : %s \r\n", sub_disp_addr ));
    APP_TRACE_INFO(("Default Gateway : %s \r\n", gate_disp_addr));
    APP_TRACE_INFO(("\r\n"));
}


/*
*********************************************************************************************************
*                                          App_SNTPc_SetClk()
*
* Description : Get the time from an SNTP server and set the uc/CLK module.
*
* Argument(s) : p_sntp_cfg   Pointer to SNTP server configuration.
*
* Return(s)   : DEF_FAIL,   Operation failed.
*               DEF_OK,     Operation is successful
*
* Caller(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN App_SNTPc_SetClk (SNTPc_CFG *p_sntp_cfg)
{
    SNTP_TS      ntp_ts;
    SNTP_PKT     sntp_pkt;
    SNTPc_ERR    sntp_err;
    CPU_BOOLEAN  ret_val;

                                                                /* ----------- REQUEST TS FROM SNTP SERVER ------------ */
    ret_val = SNTPc_ReqRemoteTime( p_sntp_cfg,                  /* Send a SNTP request to the specified NTP server.     */
                                  &sntp_pkt,
                                  &sntp_err);
    if (ret_val == DEF_FAIL) {
        return (DEF_FAIL);
    }
                                                                /* ----- COMPUTE NTP TIMESTAMP FROM SERVER ANSWER ----- */
    ntp_ts = SNTPc_GetRemoteTime(&sntp_pkt, &sntp_err);         /* Get the local time from the received SNTP message... */
    if (sntp_err != SNTPc_ERR_NONE) {                           /* ...packet in the form of an NTP time stamp.          */
        return (DEF_FAIL);
    }
                                                                /* --------------------- SET CLK ---------------------- */
    ret_val = Clk_SetTS_NTP(ntp_ts.Sec);                        /* Set the local time using uC/CLK.                     */
    if (ret_val == DEF_FAIL) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            AppInit_DNSc()
*
* Description : This function initialize uC/DNSc. This function returns only the DHCP negotiation is completed.
*
* Argument(s) : None.
*
* Return(s)   : DEF_OK,   Completed successfully.
*
*               DEF_FAIL, Initialization failed.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Prior to do any call to DNS client the module must be initialized. If the process is successful,
*                   the DNS client s tasks are started (if applicable), and its various data structures are initialized.
*********************************************************************************************************
*/

#if (NET_EXT_MODULE_CFG_DNS_EN == DEF_ENABLED)
CPU_BOOLEAN  AppDNSc_Init (void)
{
    DNSc_ERR  dns_err;
                                                                /* --------------- INITIALIZE uC/DHCPc ---------------- */
                                                                /* See Note #1.                                         */
#if (DNSc_CFG_MODE_ASYNC_EN == DEF_DISABLED)
    DNSc_Init(&DNSc_Cfg, DEF_NULL, &dns_err);

#else
    DNSc_Init(&DNSc_Cfg, &DNSc_CfgTask, &dns_err);
#endif

    if (dns_err != DNSc_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}
#endif

/*
*********************************************************************************************************
*                                             ENABLE END
*********************************************************************************************************
*/

#endif
