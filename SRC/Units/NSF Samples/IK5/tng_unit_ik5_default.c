/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_ik5_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               IK5 Unit default.
 * PROGRAMMER  : CGSG'2018.
 *               Ilya Kuzmin.
 * LAST UPDATE : 28.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

/* Main structure for unit of this ("tng_unit_ik5_default.c") file  */
typedef struct tagtngUNIT_IK5_DEFAULT
{
  tngUNIT;      /* Unit base fields */
  INT Distance; /* field-example */
} tngUNIT_IK5_DEFAULT;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_IK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_IK5_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_IK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_IK5_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_IK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_IK5_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_IK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_IK5_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_IK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_IK5_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_IK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_IK5_DEFAULT *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Unit default creation function.
 * ARGUMENTS: 
 *   - Animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) created unit.
 */
tngUNIT * TNG_UnitCreateIK5Default( tngANIM *Ani )
{
  tngUNIT_IK5_DEFAULT *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_IK5_DEFAULT *)TNG()->UnitCreate(sizeof(tngUNIT_IK5_DEFAULT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateIK5Default' function */

/* END OF 'tng_unit_ik5_default.c' FILE  */
