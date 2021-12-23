/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_sk5_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               SK5 Unit default.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 29.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

/* Main discription of default unit */
typedef struct tagtngUNIT_SK5_DEFAULT
{
  tngUNIT;      /* Unit base fields */
  tngPRIM *Prim;   /* Primitive example */
} tngUNIT_SK5_DEFAULT;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_SK5_DEFAULT *Uni, tngANIM *Ani )
{
  static INT pass;
  static struct tagLOC_VERTEX
  {
    VEC Pos;
    VEC4 Color;
  } P[2][3] = 
  {
    {{{0, 0, 0}, {0, 1, 0, 1}}, {{1, 0, 0}, {0, 1, 0, 1}}, {{1, 1, 0}, {0, 1, 0, 1}}},
    {{{0, 0, 2}, {1, 0, 1, 1}}, {{1, 0, 2}, {1, 0, 1, 1}}, {{1, 1, 2}, {1, 0, 1, 1}}}
  };
  INT Ind[3] = {0, 1, 2};

  if (pass == 0)
    Uni->Prim = Ani->PrimCreate(TNG_PRIM_TRIMESH, "f3f4", P[0], 3, Ind, 3);
  else
    Uni->Prim = Ani->PrimCreate(TNG_PRIM_TRIMESH, "f3f4", P[1], 3, Ind, 3);
  pass++;
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_SK5_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_SK5_DEFAULT *Uni, tngANIM *Ani )
{
  static BOOL state = TRUE;

  if (Ani->KeysClick['S'])
    state = !state;

  if (!state)
    Ani->UnitRegisterDefferedRender((tngUNIT *)Uni);
} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_SK5_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->PrimDraw(Uni->Prim, MatrIdentity());
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_SK5_DEFAULT *Uni, tngANIM *Ani )
{
  Ani->PrimDraw(Uni->Prim, MatrIdentity());
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK5_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_SK5_DEFAULT *Uni, tngANIM *Ani )
{
  static INT pass;
  INT returned = -1;

  switch (pass)
  {
  case 0:
    returned = 10;
    break;
  case 1:
    returned = 15;
    break;
  }
  pass++;

  if (pass >= 2)
    pass = 0;

  return returned;
} /* End of 'TNG_UnitGetDistance' function */

/* Unit default creation function.
 * ARGUMENTS: 
 *   - Animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (tngUNIT *) created unit.
 */
tngUNIT * TNG_UnitCreateSK5Default( tngANIM *Ani )
{
  tngUNIT_SK5_DEFAULT *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_SK5_DEFAULT *)TNG()->UnitCreate(sizeof(tngUNIT_SK5_DEFAULT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateSK5Default' function */

/* END OF 'tng_unit_sk5_default.c' FILE  */
