/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit support module.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 29.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "tng_unit.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (tngUNIT *) pointer to created unit.
 */
static tngUNIT * TNG_UnitCreate( INT Size )
{
  tngUNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(tngUNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = TNG_UnitInit;
  Uni->Close = TNG_UnitClose;
  Uni->Response = TNG_UnitResponse;
  Uni->Render = TNG_UnitRender;
  Uni->DefferRender = TNG_UnitDefferRender;
  Uni->GetDistance = TNG_UnitGetDistance;

  return Uni;
} /* End of 'TNG_UnitCreate' function */

/* Unit register for deffered rendering function.
 * ARGUMENTS:
 *   - unit to be added:
 *       tngUNIT *Uni;
 * RETURNS: None.
 */
VOID TNG_UnitRegisterDefferedRender( tngUNIT *Uni )
{
  if (TNG()->DrawDefferedFullSize == 0)
  {
    TNG()->DrawDeffered = malloc(sizeof(tngUNIT_DRAW_DEFFERED));
    memset(TNG()->DrawDeffered, 0, sizeof(tngUNIT_DRAW_DEFFERED));
    TNG()->DrawDeffered->Uni = Uni;
    TNG()->DrawDefferedFullSize = ++TNG()->DrawDefferedCurrentSize;
  }
  else if (TNG()->DrawDefferedFullSize == TNG()->DrawDefferedCurrentSize)
  {
    INT i;
    tngUNIT_DRAW_DEFFERED *tmp1, *tmp2;

    tmp1 = malloc(sizeof(tngUNIT_DRAW_DEFFERED) * TNG()->DrawDefferedFullSize * 2);
    memset(tmp1, 0, sizeof(tngUNIT_DRAW_DEFFERED) * TNG()->DrawDefferedFullSize * 2);
    for (i = 0; i <= TNG()->DrawDefferedFullSize; i++)
      tmp1[i].Uni = (i == TNG()->DrawDefferedFullSize ? Uni : TNG()->DrawDeffered[i].Uni);
    TNG()->DrawDefferedFullSize = ++TNG()->DrawDefferedCurrentSize;
    /* Swap */
    tmp2 = tmp1;
    tmp1 = TNG()->DrawDeffered;
    TNG()->DrawDeffered = tmp2;
    free(tmp1);
  }
  else
  {
    TNG()->DrawDeffered[TNG()->DrawDefferedCurrentSize].Uni = Uni;
    TNG()->DrawDefferedCurrentSize++;
  }
} /* End of 'TNG_UnitRegisterDefferedRender' function */

/* Add unit to stock function.
 * ARGUMENTS:
 *   - unit to be added:
 *       tngUNIT *Uni;
 * RETURNS: None.
 */
static VOID TNG_UnitAdd( tngUNIT *Uni )
{
  StockAdd(&TNG()->Units, &Uni);
  Uni->Init(Uni, TNG());
} /* End of 'TNG_UnitAdd' function */

/* Strategy of delete for every units function.
 * ARGUMENTS:
 *   - pointer to unit to be deleted:
 *       tngUNIT **Uni;
 * RETURNS: None.
 */
static VOID TNG_UnitDelete( tngUNIT **Uni )
{
  (*Uni)->Close(*Uni, TNG());
  free(*Uni);
} /* End of 'TNG_UnitDelete' function */

/* Starter subsystem of units */
static tngSUBSYSTEM_UNIT SubSystemUnitAdd =
{
  {0},                           /* Stock of units */
  NULL,                          /* Array of units waste to deffered rendering */
  0,                             /* Number of units waste to deffered rendering */
  0,                             /* Maximal number of units in array */
  TNG_UnitAdd,                   /* Unit add to unit stock function pointer */
  TNG_UnitCreate,                /* Create unit function pointer */
  TNG_UnitRegisterDefferedRender /* Register to deffered rendering function pointer */
};

/* Subsystem unit initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemUnitInit( VOID )
{
  TNG()->SubSystemUnit = SubSystemUnitAdd;
  StockInit(&TNG()->Units, 0, sizeof(tngUNIT *), TNG_UnitDelete);
} /* End of 'TNG_SubsystemUnitInit' function */

/* Subsystem unit destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemUnitDestructor( VOID )
{
  StockClear(&TNG()->Units);
  free(TNG()->DrawDeffered);
} /* End of 'TNG_SubsystemUnitDestructor' function */

/* END OF 'tng_unit.c' FILE */
