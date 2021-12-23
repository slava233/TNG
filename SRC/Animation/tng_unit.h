/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit support declarations.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 29.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_unit_h_
#define __tng_unit_h_

#include "../tng_def.h"

/* Main representation type for deffered rendered objects */
typedef struct tagtngUNIT_DRAW_DEFFERED
{
  tngUNIT *Uni; /* Deffered rendered unit */
  FLT Dist;     /* Distance to viewer */
} tngUNIT_DRAW_DEFFERED;

/* Unit subsytem structure */
typedef struct tagtngSUBSYSTEM_UNIT
{
  STOCK Units;                         /* Place for units */

  tngUNIT_DRAW_DEFFERED *DrawDeffered; /* Array of units waste to deffered rendering */
  INT DrawDefferedCurrentSize;         /* Number of units waste to deffered rendering */
  INT DrawDefferedFullSize;            /* Maximal number of units in array */

  /* Add unit to stock function.
   * ARGUMENTS:
   *   - unit ti be added:
   *       tngUNIT *Uni;
   * RETURNS: None.
   */
  VOID (*UnitAdd)( tngUNIT *Uni );

  /* Unit creation function.
   * ARGUMENTS:
   *   - unit structure size in bytes:
   *       INT Size;
   * RETURNS:
   *   (tngUNIT *) pointer to created unit.
   */
  tngUNIT * (*UnitCreate)( INT Size );

  /* Unit register for deffered rendering function.
   * ARGUMENTS:
   *   - unit to be added:
   *       tngUNIT *Uni;
   * RETURNS: None.
   */
  VOID (*UnitRegisterDefferedRender)( tngUNIT *Uni );

} tngSUBSYSTEM_UNIT;

/* Subsystem unit initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemUnitInit( VOID );

/* Subsystem unit destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemUnitDestructor( VOID );

#endif /* __tng_unit_h_ */

/* END OF 'tng_unit.h' FILE */
