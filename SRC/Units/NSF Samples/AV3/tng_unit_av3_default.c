/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_av3_default.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit support module.
 * PROGRAMMER  : CGSG'2018.
 *               Alexandr Vashpanov.
 * LAST UPDATE : 18.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

#include <stdio.h>

/* Main discription of default unit */
typedef struct tagtngUNIT_AV3_DEFAULT
{
  tngUNIT;                          /* Field which representate unit */
  INT NumberOfSkybox;               /* Number of using skybox */
} tngUNIT_AV3_DEFAULT;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_av3_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_AV3_DEFAULT *Uni, tngANIM *Ani )
{ 
  Ani->CubeMapCreate("SkyStorm");
  Uni->NumberOfSkybox = -1;
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_av3_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_AV3_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_av3_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_AV3_DEFAULT *Uni, tngANIM *Ani )
{
  /* Array of names of cubemaps */
  CHAR *Name[] = {"SkyNebula", "SkyStorm", "SkyStd", "SkyLight", "SkyDefault"};

  if (Ani->KeysClick['H'] && Ani->Keys[VK_SHIFT])                 
  {
    Uni->NumberOfSkybox++;

    if (Uni->NumberOfSkybox == 5)
      Uni->NumberOfSkybox = 0;
    /* Load new skybox*/
    Ani->SkyBox = TNG()->LoadCubeMap(Name[Uni->NumberOfSkybox], ".BMP");
  }
} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_av3_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_AV3_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_av3_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_AV3_DEFAULT *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_av3_DEFAULT *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_AV3_DEFAULT *Uni, tngANIM *Ani )
{
  return -1;
} /* End of 'TNG_UnitGetDistance' function */

/* Unit default creation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
tngUNIT * TNG_UnitCreateAV3Default( tngANIM *Ani )
{
  tngUNIT_AV3_DEFAULT *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_AV3_DEFAULT *)TNG()->UnitCreate(sizeof(tngUNIT_AV3_DEFAULT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateav3Default' function */

/* END OF 'tng_unit_av3_default.c' FILE */