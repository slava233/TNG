/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_windows_depended.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Window Depended Support declaration module.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 27.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_windows_depended_h_
#define __tng_windows_depended_h_

#include "../../tng_def.h"

/* Windows depended subsytem timer inheritance structure */
typedef struct tagtngSUBSYSTEM_WIN
{
  HWND hWnd;          /* Main window handle */
  INT W, H;           /* Main window size */
  INT MouseWheel;     /* Mouse wheel counter */
  BOOL IsActive;      /* Active flag */
  BOOL IsFullScreen;  /* Full screen flag */
  BOOL WinIsInit;     /* Initialization flag */

  /* Main message loop function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (INT) return code.
   */
  INT (*WinRun)( VOID );

  /* Enable/disable full screen window mode function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*WinFlipFullScreen)( VOID );

  /***
   * Callback function pointers
   ***/

  /* Initialize function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*WinInit)( VOID );

  /* Deinitialize function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*WinClose)( VOID );

  /* Resize handle function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*WinResize)( VOID );

  /* Timer response function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*WinTimer)( VOID );

  /* Idle response function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*WinIdle)( VOID );

  /* Activate response function.
   * ARGUMENTS:
   *   - activate flag:
   *       BOOL IsActive;
   * RETURNS: None.
   */
  VOID (*WinActivate)( BOOL IsActive );

  /* Char pressing response function.
   * ARGUMENTS:
   *   - keystroke character code:
   *       BYTE Ch;
   * RETURNS: None.
   */
  VOID (*WinChar)( BYTE Ch );

  /* Paint message response function.
   * ARGUMENTS:
   *   - device context during WM_PAINT message:
   *       HDC hDC;
   * RETURNS: None.
   */
  VOID (*WinPaint)( HDC hDC );
} tngSUBSYSTEM_WIN;

/* Windows depended initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemWinInit( VOID );

/* Main window message handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - message identifier (see WM_***):
 *       UINT Msg;
 *   - message first parameter:
 *       WPARAM wParam;
 *   - message second parameter:
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message return code.
 */
LRESULT CALLBACK TNG_WinMainWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif /* __tng_windows_depended_h_ */

/* END OF 'tng_windows_depended.h' FILE */
