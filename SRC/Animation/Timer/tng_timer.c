/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_timer.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Timer handle module.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 26.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../tng.h"

/***
 * Semiglobal timer data
 ***/

static UINT64
  StartTime,    /* Start program time */
  OldTime,      /* Previous frame time */
  OldTimeFPS,   /* Old time FPS measurement */
  PauseTime,    /* Time during pause period */
  TimePerSec,   /* Timer resolution */
  FrameCounter; /* Frames counter */

/* Timer response function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  TNG()->GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  TNG()->GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;

  /* Time with pause */
  if (TNG()->IsPause)
  {
    TNG()->DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    TNG()->DeltaTime = TNG()->GlobalDeltaTime;
    TNG()->Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    TNG()->FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
} /* End of 'TNG_TimerResponse' function */

/* Filled timer subsytem data */
static tngSUBSYSTEM_TIMER SubsystemTimer =
{
  0, 0,  /* Global time and interframe interval */
  0, 0,  /* Time with pause and interframe interval */
  30.0,  /* Frames per second value */
  FALSE, /* Pause flag */
  /* Timer handle functions */
  TNG_TimerResponse
};

/* Timer initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTimerInit( VOID )
{
  LARGE_INTEGER t;

  TNG()->SubsystemTimer = SubsystemTimer;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  TNG()->IsPause = FALSE;
  TNG()->FPS = 30.0;
  PauseTime = 0;
} /* End of 'TNG_SubsystemTimerInit' function */

/* END OF 'tng_timer.c' FILE */
