/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_set.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit set support.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

typedef struct tagtngUNIT_SET
{
  tngUNIT;
  STOCK Units;    /* Stock of untis */
} tngUNIT_SET;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SET *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitSetInit( tngUNIT_SET *Uni, tngANIM *Ani )
{
  tngUNIT **U;

  U = StockStart(&Uni->Units);
  while (U != NULL)
  {
    (*U)->Init(*U, Ani);
    U = StockNext(U);
  }
} /* End of 'TNG_UnitInit' function */

/* Strategy of delete unit set function.
 * ARGUMENTS:
 *   - Unit set to be deleted:
 *       tngUNIT_SET **Uni;
 * RETURNS: None.
 */
static VOID TNG_UnitSetsUnitDelete( tngUNIT **Uni )
{
  (*Uni)->Close((*Uni), TNG());
  free(*Uni);
} /* End of 'TNG_UnitInUnitSetDelete' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SET *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitSetClose( tngUNIT_SET *Uni, tngANIM *Ani )
{
  StockClear(&Uni->Units);
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit set object:
 *       tngUNIT_SET *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitSetResponse( tngUNIT_SET *Uni, tngANIM *Ani )
{
  tngUNIT **U;

  U = StockStart(&Uni->Units);
  while (U != NULL)
  {
    (*U)->Response(*U, Ani);
    U = StockNext(U);
  }
} /* End of 'TNG_UnitResponse' function */

/* Unit set render function.
 * ARGUMENTS:
 *   - self-pointer to unit set object:
 *       tngUNIT_SET *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitSetRender( tngUNIT_SET *Uni, tngANIM *Ani )
{
  tngUNIT **U;

  U = StockStart(&Uni->Units);
  while (U != NULL)
  {
    (*U)->Render(*U, Ani);
    U = StockNext(U);
  }
} /* End of 'TNG_UnitRender' function */

/* Create unit set function.
 * ARGUMENTS:
 *   - string with units:
 *       CHAR *UnitsFormat;
 * RETURNS:
 *   (tngUNIT *) created unit set.
 */
static tngUNIT * TNG_UnitSetCreate( CHAR *UnitsFormat )
{
  INT i;
  CHAR *StrPoint1 = UnitsFormat, *StrPoint2 = UnitsFormat;
  tngUNIT_SET *Uni;
  tngUNIT *UTmp;


  if (UnitsFormat == NULL || (Uni = (tngUNIT_SET *)TNG()->UnitCreate(sizeof(tngUNIT_SET))) == NULL)
    return NULL;

  StockInit(&Uni->Units, 0, sizeof(tngUNIT *), TNG_UnitSetsUnitDelete);

  while (*StrPoint1 != 0)
  {
    while ((BYTE)*StrPoint2 != '|')
      StrPoint2++;
    *StrPoint2 = 0;
    for (i = 0; i < TNG_UnitRegisterSize; i++)
      if (strcmp(TNG_UnitRegister[i].UnitName, StrPoint1) == 0)
      {
        UTmp = TNG_UnitRegister[i].UnitCreate(TNG());
        StockAdd(&Uni->Units, &UTmp);
        break;
      }
    *StrPoint2 = '|';
    StrPoint2++;
    StrPoint1 = StrPoint2;
  }

  Uni->Init = (VOID *)TNG_UnitSetInit;
  Uni->Close = (VOID *)TNG_UnitSetClose;
  Uni->Response = (VOID *)TNG_UnitSetResponse;
  Uni->Render = (VOID *)TNG_UnitSetRender;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitSetCreate' function */

/* Starter subsystem of unit sets */
static tngSUBSYSTEM_UNIT_SET SubSystemUnitSetAdd =
{
  TNG_UnitSetCreate
};

/* Subsystem unit set initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemUnitSetInit( VOID )
{
  TNG()->SubSystemUnitSet = SubSystemUnitSetAdd;
} /* End of 'TNG_SubsystemUnitSetInit' function */

/* Subsystem unit set destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemUnitSetDestructor( VOID )
{
} /* End of 'TNG_SubsystemUnitSetDestructor' function */

/* END OF 'tng_unit_set.c' FILE */
