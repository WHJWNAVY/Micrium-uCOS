/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2014 - 2016  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       emSSL * Embedded Transport Layer Security                    *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product for in-house use.         *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emSSL version: V2.54a                                        *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller GmbH
Licensed to:              Silicon Laboratories Inc., 400 W. Cesar Chavez, Austin, TX 78701, USA
Licensed SEGGER software: emSSL
License number:           SSL-00158
License model:            OEM LICENSE DISTRIBUTION AGREEMENT, signed June 3rd, 2018 and June 5th, 2018
----------------------------------------------------------------------
File        : SEGGER_Shell.h
Purpose     : Standardized command line interface functions.
Revision    : $Rev: 9275 $
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef SEGGER_SHELL_H
#define SEGGER_SHELL_H

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "SEGGER.h"
#include "SEGGER_MEM.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define SEGGER_SHELL_HELP_FLAG_SHOW_USAGE       0x0001   // Usage: foo -bar
#define SEGGER_SHELL_HELP_FLAG_SHOW_OPTIONS     0x0002   //   -x   Exclude baz.
#define SEGGER_SHELL_HELP_FLAG_SHOW_COMMAND     0x0004   // foo


/*********************************************************************
*
*       Types required for API
*
**********************************************************************
*/

struct SEGGER_SHELL_CONTEXT;

// Errors returned by the CLI.
typedef enum {
  SEGGER_SHELL_ERROR_MORE_ARGS_EXPECTED        = -400,
  SEGGER_SHELL_ERROR_COMMAND_NOT_RECOGNIZED    = -401,
  SEGGER_SHELL_ERROR_COMMAND_ALREADY_INSTALLED = -402,
  SEGGER_SHELL_ERROR_OUT_OF_MEMORY             = -403
} SEGGER_SHELL_ERROR;

// A shell command with name, help, and execution function
typedef struct SEGGER_SHELL_COMMAND_API {
  const char * sName;
  const char * sDescription;
  const char * sSyntax;
  const char * sOptions;
  int        (*pfExecute)(struct SEGGER_SHELL_CONTEXT *pContext);
} SEGGER_SHELL_COMMAND_API;

// Linked list of shell commands with management data.
typedef struct SEGGER_SHELL_COMMAND {
  const SEGGER_SHELL_COMMAND_API * pAPI;
  struct SEGGER_SHELL_COMMAND    * pNext;
  U8                               Flags;
} SEGGER_SHELL_COMMAND;

typedef struct {
  void     (*pfPrintString) (const char *sFormat, va_list va);
  int      (*pfGetString)   (char *pInput, unsigned InputByteCnt);
  unsigned (*pfGetWidth)    (void);
} SEGGER_SHELL_CONSOLE_API;

// Shell context contains enough to extract command arguments.
typedef struct SEGGER_SHELL_CONTEXT {
  unsigned               argc;                 // Well-known argc, count of command line arguments
  char                ** argv;                 // Well-known argv, vector of command line arguments
  char                 * pWork;                // Command line working storage
  unsigned               ArgumentIdx;          // Index into arguments being extracted
  char                 * sProgramName;         // Selected command or provided program name
  SEGGER_SHELL_COMMAND * pCommands;            // List of commands understood by the shell
  int                    Done;                 // Nonzero if the shell is closed
  const char           * sName;                // Name of shell context.
  SEGGER_MEM_CONTEXT   * pMemory;              // Memory allocation API
  const SEGGER_SHELL_CONSOLE_API * pConsoleAPI; // Console I/O API
} SEGGER_SHELL_CONTEXT;

/*********************************************************************
*
*       Public const data
*
**********************************************************************
*/

// Common system commands.
extern const SEGGER_SHELL_COMMAND_API SEGGER_SHELL_ByeAPI;
extern const SEGGER_SHELL_COMMAND_API SEGGER_SHELL_ExitAPI;
extern const SEGGER_SHELL_COMMAND_API SEGGER_SHELL_QuitAPI;
extern const SEGGER_SHELL_COMMAND_API SEGGER_SHELL_HelpAPI;
extern const SEGGER_SHELL_COMMAND_API SEGGER_SHELL_QuestionMarkAPI;
extern const SEGGER_SHELL_COMMAND_API SEGGER_SHELL_CommandsAPI;


/*********************************************************************
*
*       API functions
*
**********************************************************************
*/

/*********************************************************************
*
*       Initialization functions
*/
void SEGGER_SHELL_Init             (SEGGER_SHELL_CONTEXT *pSelf, const SEGGER_SHELL_CONSOLE_API *pConsole, SEGGER_MEM_CONTEXT *pMem);
int  SEGGER_SHELL_ParseInput       (SEGGER_SHELL_CONTEXT *pSelf, char *sInput);
void SEGGER_SHELL_Inherit          (SEGGER_SHELL_CONTEXT *pSelf, SEGGER_SHELL_CONTEXT *pFrom, const char *sName);
void SEGGER_SHELL_InheritExternal  (SEGGER_SHELL_CONTEXT *pSelf, char *sProgramName, int argc, char *argv[]);

/*********************************************************************
*
*       Shell command set management functions
*/
int  SEGGER_SHELL_AddCommandAPI    (SEGGER_SHELL_CONTEXT *pSelf, const SEGGER_SHELL_COMMAND_API *pCommand);
int  SEGGER_SHELL_AddCommand       (SEGGER_SHELL_CONTEXT *pSelf, SEGGER_SHELL_COMMAND *pCommand);

/*********************************************************************
*
*       Command line execution functions
*/
int  SEGGER_SHELL_Process          (SEGGER_SHELL_CONTEXT *pSelf);
int  SEGGER_SHELL_Enter            (SEGGER_SHELL_CONTEXT *pSelf);
void SEGGER_SHELL_Exit             (SEGGER_SHELL_CONTEXT *pSelf);
int  SEGGER_SHELL_IsExited         (const SEGGER_SHELL_CONTEXT *pSelf);

/*********************************************************************
*
*       Command line utility functions
*/
void SEGGER_SHELL_SetName          (SEGGER_SHELL_CONTEXT *pSelf, const char *sName);
void SEGGER_SHELL_PrintHelp        (SEGGER_SHELL_CONTEXT *pSelf, unsigned Flags);
void SEGGER_SHELL_PrintUsage       (SEGGER_SHELL_CONTEXT *pSelf, const SEGGER_SHELL_COMMAND_API *pAPI, unsigned Flags);
void SEGGER_SHELL_PrintCommands    (SEGGER_SHELL_CONTEXT *pSelf);
int  SEGGER_SHELL_ExecuteHelp      (SEGGER_SHELL_CONTEXT *pSelf);
int  SEGGER_SHELL_FindCommand      (const SEGGER_SHELL_CONTEXT *pSelf, const char *sName, const SEGGER_SHELL_COMMAND_API **pCommandAPI);

/*********************************************************************
*
*       Command line argument functions
*/
int  SEGGER_SHELL_ReadNextArg      (SEGGER_SHELL_CONTEXT *pSelf, char **pArg);
int  SEGGER_SHELL_PeekNextArg      (SEGGER_SHELL_CONTEXT *pSelf, char **pArg);
int  SEGGER_SHELL_HasUnreadArgs    (const SEGGER_SHELL_CONTEXT *pSelf);
int  SEGGER_SHELL_CountTotalArgs   (const SEGGER_SHELL_CONTEXT *pSelf);
int  SEGGER_SHELL_CountUnreadArgs  (const SEGGER_SHELL_CONTEXT *pSelf);
void SEGGER_SHELL_ResetArgIterator (SEGGER_SHELL_CONTEXT *pSelf, unsigned Index);
int  SEGGER_SHELL_ReadNextU32      (SEGGER_SHELL_CONTEXT *pSelf, U32 *pArg, U32 DefaultValue);

/*********************************************************************
*
*       Input functions
*/
int  SEGGER_SHELL_InputText        (SEGGER_SHELL_CONTEXT *pSelf, char *sText, unsigned TextByteCnt);

/*********************************************************************
*
*       Error decoding functions
*/
void SEGGER_SHELL_Printf           (SEGGER_SHELL_CONTEXT *pSelf, const char *sFormat, ...);
void SEGGER_SHELL_PrintError       (SEGGER_SHELL_CONTEXT *pSelf, int Status);
void SEGGER_SHELL_DecodeError      (int Status, char *sErrorStr, unsigned NumBytesErrorStr);

#ifdef __cplusplus
}
#endif

#endif

/****** End Of File *************************************************/
