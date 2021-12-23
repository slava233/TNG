/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_windows_messages.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Window Depended Support.
 *               Main window messages handle module.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

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
LRESULT CALLBACK TNG_WinMainWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *mm;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    mm = (MINMAXINFO *)lParam;
    mm->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 47, 1, NULL);
    return 0;
  case WM_SIZE:
    TNG()->W = LOWORD(lParam);
    TNG()->H = HIWORD(lParam);
    if (TNG()->WinIsInit)
    {
      if (TNG()->WinResize != NULL)
        TNG()->WinResize();
      if (TNG()->WinTimer != NULL)
        TNG()->WinTimer();
      if (TNG()->WinPaint != NULL)
        TNG()->WinPaint(NULL);
    }
    return 0;
  case WM_TIMER:
    if (!TNG()->WinIsInit)
    {
      TNG()->WinIsInit = TRUE;
      if (TNG()->WinInit != NULL)
        TNG()->WinInit();
      if (TNG()->WinResize != NULL)
        TNG()->WinResize();
      if (TNG()->WinTimer != NULL)
        TNG()->WinTimer();
      if (TNG()->WinPaint != NULL)
        TNG()->WinPaint(NULL);
    }
    else
    {
      if (TNG()->WinTimer != NULL)
        TNG()->WinTimer();
      if (TNG()->WinPaint != NULL)
        TNG()->WinPaint(NULL);
    }
    return 0;
  case WM_ACTIVATE:
    TNG()->IsActive = (UINT)LOWORD(wParam) == WA_CLICKACTIVE || (UINT)LOWORD(wParam) == WA_ACTIVE;
    if (TNG()->WinIsInit && TNG()->WinActivate != NULL)
      TNG()->WinActivate(TNG()->IsActive);
    return 0;
  case WM_MOUSEWHEEL:
    TNG()->MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    if (TNG()->WinIsInit && TNG()->WinPaint != NULL)
      TNG()->WinPaint(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_LBUTTONDOWN:
  case WM_RBUTTONDOWN:
  case WM_MBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
  case WM_RBUTTONUP:
  case WM_MBUTTONUP:
    ReleaseCapture();
    return 0;
  case WM_CHAR:
    if (TNG()->WinIsInit && TNG()->WinChar != NULL)
      TNG()->WinChar((BYTE)wParam);
    return 0;
  case WM_DESTROY:
    if (TNG()->WinIsInit && TNG()->WinClose != NULL)
      TNG()->WinClose();
    KillTimer(hWnd, 47);
    PostQuitMessage(30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'TNG_WinMainWindowFunc' function */

/* END OF 'tng_windows_messages.c' FILE */
