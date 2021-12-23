/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_timer.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Timer handle declaration module.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 26.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_timer_h_
#define __tng_timer_h_

#include "../../tng_def.h"

/* Timer subsytem timer inheritance structure */
typedef struct tagtngSUBSYSTEM_TIMER
{
  /* Timer data */
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per seond value */
  BOOL IsPause;                  /* Pause flag */

  /* Timer handle functions */

  /* Timer response function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*TimerResponse)( VOID );
} tngSUBSYSTEM_TIMER;

/* Timer initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemTimerInit( VOID );

#endif /* __tng_timer_h_ */

/* END OF 'tng_timer.h' FILE */
