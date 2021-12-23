/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_anim.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Animation system module.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

/* Animation context */
tngANIM TNG_Anim;

/* Temporary empty function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
static VOID EmptyDestructor( VOID )
{
} /* End of 'EmptyDestructor' function */

/* Array of function subsystem initialization (from different modules) names */
static VOID (*TNG_SystemRegistration[])( VOID ) =
{
  TNG_SubsystemWinInit,
  TNG_SubsystemTimerInit,
  TNG_SubsystemInputInit,
  TNG_SubsystemAudioInit,
  TNG_SubsystemRenderInit,
  TNG_SubsystemTopologyInit,
  TNG_SubsystemUnitInit,
  TNG_SubsystemUnitSetInit,
  TNG_SubsystemParticlesInit,
  TNG_SubsistemCubeMapInit
};

/* Array of function subsystem destructor (from different modules) names */
static VOID (*TNG_SystemDestructor[])( VOID ) =
{
  TNG_SubsystemUnitDestructor,
  TNG_SubsystemRenderDestructor,
  TNG_SubsystemAudioDestructor
};

/* End of work animation system function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
static VOID TNG_AnimClose( VOID )
{
  INT i;

  for (i = 0;
       i < sizeof(TNG_SystemDestructor) / sizeof(TNG_SystemDestructor[0]);
       i++)
    TNG_SystemDestructor[i]();
} /* End of 'AnimClose' function */

/* Work with changes of resize function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
static VOID TNG_AnimResize( VOID )
{
  TNG()->RndResize();
} /* End of 'TNG_AnimResize' function */

/* Copy frame function.
 * ARGUMENTS:
 *   - device context during WM_PAINT message:
 *       HDC hDC;
 * RETURNS: None.
 */
static VOID TNG_AnimCopyFrame( HDC hDC )
{
  TNG()->RndCopyFrame(hDC);
} /* End of 'AnimCopyFrame' function */

/* Main rendering function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
static VOID TNG_AnimRender( VOID )
{
  tngUNIT **U;

  TNG()->TimerResponse();

  TNG()->InputResponse();
  U = StockStart(&TNG()->Units);

  while (U != NULL)
  {
    (*U)->Response(*U, TNG());
    U = StockNext(U);
  }

  TNG()->RndStart();
  U = StockStart(&TNG()->Units);
  while (U != NULL)
  {
    (*U)->Render(*U, TNG());
    U = StockNext(U);
  }

  /* Deffered rendering */
  TNG_AnimDefferedRendering();

  TNG()->RndEnd();

  StockRelease(&TNG()->Units);
  StockRelease(&TNG()->Primitives);
  StockRelease(&TNG()->Lights);
  StockRelease(&TNG()->Targets);
  StockRelease(&TNG()->Geometry);
  StockRelease(&TNG()->Textures);
  StockRelease(&TNG()->Materials);
} /* End of 'AnimRender' function */

/* Do exit unit function.
 * ARGUMENTS: None;
 * RETURNS:
 *  (BOOL) change of exit: TRUE, if user don't have to close program,
 *                         FALSE another.
 */
static BOOL TNG_AnimDoExit( VOID )
{
  return TRUE;
} /* End of 'TNG_AnimDoExit' function */

/* Activate response function.
 * ARGUMENTS:
 *   - activate flag:
 *       BOOL IsActive;
 * RETURNS: None.
 */
static VOID TNG_AnimActivate( BOOL IsActive )
{
  static BOOL IsZero = TRUE, savePause = FALSE;
  TNG()->WindowActive = IsActive;

  if (IsActive)
    if (!IsZero)
      TNG()->IsPause = savePause;
  else
  {
    IsZero = FALSE;
    savePause = TNG()->IsPause;
    TNG()->IsPause = TRUE;
  }
} /* End of 'TNG_AnimActivate' function */

/* Char pressing response function.
 * ARGUMENTS:
 *   - keystroke character code:
 *       BYTE Ch;
 * RETURNS: None.
 */
static VOID TNG_AnimChar( BYTE Ch )
{
} /* End of 'TNG_AnimChar' function */

/* Initialization animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_AnimInit( VOID )
{
  INT i;

  memset(&TNG_Anim, 0, sizeof(tngANIM));

  for (i = 0;
       i < sizeof(TNG_SystemRegistration) / sizeof(TNG_SystemRegistration[0]);
       i++)
    TNG_SystemRegistration[i]();

  TNG()->WinResize = TNG_AnimResize;
  TNG()->WinClose = TNG_AnimClose;
  TNG()->WinPaint = TNG_AnimCopyFrame;
  TNG()->WinTimer = TNG_AnimRender;
  TNG()->WinIdle = TNG_AnimRender;
  TNG()->WinActivate = TNG_AnimActivate;
  TNG()->WinChar = TNG_AnimChar;
  TNG()->DoExit = TNG_AnimDoExit;

  TNG()->WindowActive = TRUE;
} /* End of 'TNG_AnimInit' function */

/* Add unit to stock function
 * ARGUMENTS:
 *   - unit to be added:
 *       tngUNIT *Uni;
 * RETURNS: None.
 */
VOID TNG_AnimUnitAdd( tngUNIT *Uni )
{
  StockAdd(&TNG()->Units, &Uni);
  Uni->Init(Uni, TNG());
} /* End of 'TNG_AnimUnitAdd' function */

/* Registrator animation system work function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID TNG_AnimRun( VOID )
{
  TNG()->WinRun();
} /* End of 'TNG_AnimRun' function */

/* END OF 'tng_anim.c' FILE */
