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
*                               HTTP INSTANCE SERVER CONFIGURATION FILE
*
* Filename : http-s_instance_cfg.h
* Version  : V3.00.06
*********************************************************************************************************
*/

#include  <Server/Source/http-s.h>
#include  <Source/net_type.h>
#if (APP_CFG_FS_EN == DEF_ENABLED)
#include  <FS/uC-FS-V4/net_fs_v4.h>
#include  <fs_app.h>
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                   FILES & FOLDERS STRING DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  HTTPs_CFG_INSTANCE_STR_FOLDER_ROOT               "\\"

#define  HTTPs_CFG_INSTANCE_STR_FILE_DEFAULT              "index.html"
#define  HTTPs_CFG_INSTANCE_STR_FILE_ERR_404              "404.html"

#define  HTTPs_CFG_INSTANCE_STR_FOLDER_UPLOAD             "\\"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                   HTTP SERVER INSTANCE CONFIGURATION
*********************************************************************************************************
*********************************************************************************************************
*/

extern  const  HTTPs_HOOK_CFG  HTTPs_Hooks_AppBasic;
extern  const  NET_TASK_CFG    HTTPs_TaskCfgInstance;
extern  const  HTTPs_CFG       HTTPs_CfgInstance;

