/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_windows_main.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Window Depended Support.
 *               Main window creation and main loop module.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 26.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#define TNG_WND_CLASS_NAME "CGSG'2018 Tough Nature Graphics Animation System Main Window Class Name"

/* Enable/disable full screen window mode function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_WinFlipFullScreen( VOID )
{
  static RECT SaveRect;

  if (!TNG()->IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* save old window size */
    GetWindowRect(TNG()->hWnd, &SaveRect);

    /* determine monitor with window */
    hmon = MonitorFromWindow(TNG()->hWnd, MONITOR_DEFAULTTONEAREST);

    /* obtain monitor information */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* go to full screen mode */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(TNG()->hWnd, GWL_STYLE), FALSE);

    SetWindowPos(TNG()->hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    TNG()->IsFullScreen = TRUE;
  }
  else
  {
    /* restore window size */
    SetWindowPos(TNG()->hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
    TNG()->IsFullScreen = FALSE;
  }
} /* End of 'TNG_WinFlipFullScreen' function */

/* Main message loop function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (INT) return code.
 */
static INT TNG_WinRun( VOID )
{
  MSG msg;

  /* Message loop */
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
      if (TNG()->WinIsInit && TNG()->WinIdle != NULL)
      {
        TNG()->WinIdle();
        TNG()->WinPaint(NULL);
      }
  return msg.wParam;
} /* End of 'TNG_WinRun' function */

/* Dummy callback function.
 * ARGUMENTS: None.
 * RETUNRS: None.
 */
static VOID TNG_WinDummy( VOID )
{
} /* End of 'TNG_WinDummy' function */

/* Filled windows depended subsytem data */
static tngSUBSYSTEM_WIN SubsystemWin =
{
  NULL,  /* Main window handle */
  0, 0,  /* Main window size */
  0,     /* Mouse wheel counter */
  FALSE, /* Active flag */
  FALSE, /* Full screen flag */
  FALSE, /* Initialization flag */

  TNG_WinRun,
  TNG_WinFlipFullScreen,

  /* Callbacks */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

/* Windows depended initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemWinInit( VOID )
{
  WNDCLASS cs;

  TNG()->SubsystemWin = SubsystemWin;

  /* Window class register */
  cs.style = CS_VREDRAW | CS_HREDRAW;
  cs.cbClsExtra = 0;
  cs.cbWndExtra = 0;
  cs.hInstance = GetModuleHandle(NULL);
  cs.hIcon = LoadIcon(NULL, IDI_ERROR);
  cs.hCursor = LoadCursor(NULL, IDC_HAND);
  cs.hbrBackground = (HBRUSH)COLOR_WINDOW;
  cs.lpszMenuName = NULL;
  cs.lpszClassName = TNG_WND_CLASS_NAME;
  cs.lpfnWndProc = TNG_WinMainWindowFunc;

  if (!RegisterClass(&cs))
  {
    MessageBox(NULL, "Error window class register", "TNG Error", MB_ICONERROR | MB_OK);
    ExitProcess(0);
  }

  /* Window creation */
  TNG()->hWnd =
    CreateWindow(TNG_WND_CLASS_NAME, "CGSG Tough Nature Graphics project",
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL, cs.hInstance, NULL);

  /* Show and update window */
  ShowWindow(TNG()->hWnd, SW_SHOWNORMAL);
  UpdateWindow(TNG()->hWnd);
} /* End of 'TNG_SubsystemWinInit' function */

/* END OF 'tng_windows_main.c' FILE */
