/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_units_register.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit registration module.
 * PROGRAMMER  : CGSG'2018.
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_units_register_h_
#define __tng_units_register_h_

#include "../tng_def.h"

/* Unit register entry representation type */
typedef struct tagtngUNIT_REGISTER_ENTRY
{
  /* Unit name for profile loading */
  CHAR *UnitName;

  /* Unit creation function.
   * ARGUMENTS:
   *   - animation context:
   *       tngANIM *Ani;
   * RETURNS:
   *   (tngUNIT *) created unit.
   */
  tngUNIT * (*UnitCreate)( tngANIM *Ani );
} tngUNIT_REGISTER_ENTRY;

/* Unit register array */
extern tngUNIT_REGISTER_ENTRY TNG_UnitRegister[];

/* Unit register size */
extern INT TNG_UnitRegisterSize;

#endif /* __tng_units_register_h_ */

/* END OF 'tng_units_register.h' FILE */
