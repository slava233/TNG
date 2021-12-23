/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_set.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit set support declarations.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_unit_set_h_
#define __tng_unit_set_h_

#include "../tng_def.h"

/* Unit subsytem structure */
typedef struct tagtngSUBSYSTEM_UNIT_SET
{
  /* Create unit set function.
   * ARGUMENTS:
   *   - string with units:
   *       CHAR *UnitsFormat;
   * RETURNS:
   *   (tngUNIT *) created unit set.
   */
   tngUNIT * (*UnitSetCreate)( CHAR *UnitsFormat );
} tngSUBSYSTEM_UNIT_SET;

/* Subsystem unit set initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemUnitSetInit( VOID );

/* Subsystem unit set destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemUnitSetDestructor( VOID );

#endif /* __tng_unit_set_h_ */

/* END OF 'tng_unit_set.h' FILE */
