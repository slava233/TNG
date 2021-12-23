/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_unit_sk3_audio.c
 * PURPOSE     : Tough Nature Graphics project.
 *               SK3 Unit audio.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kononov.
 * LAST UPDATE : 27.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

#include <al.h>
#include <alc.h>
#include <alut.h>
#pragma comment(lib, "alut")
#pragma comment(lib, "openal32")

/* Main structure for unit of this ("tng_unit_sk3_audio.c") file  */
typedef struct tagtngUNIT_SK3_AUDIO
{
  tngUNIT;              /* Unit base fields */
  tngAUDIO_BUFFER *Buf; /* Audio buffer */
  tngAUDIO_SOURCE *Src; /* Audio source */
  tngTOPOLOGY Sphere;   /* Audio sphere */
  tngPRIM *Primitive;   /* Sphere primitive */
  FLT R;                /* Radius of sphere */
  DBL Time;             /* Spinning time */
  FLT RS;               /* Radius of spinning */ 
} tngUNIT_SK3_AUDIO;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_AUDIO *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitInit( tngUNIT_SK3_AUDIO *Uni, tngANIM *Ani )
{
  /* Setting sphere radius */
  Uni->R = 0.5;
  
  /* Setting spinning radius */
  Uni->RS = 5;

  /* Setting spinning time */
  Uni->Time = 0;

  /* Creating sphere primitive to see audio player */
  Ani->CreateSphere(&Uni->Sphere, VecSet(0, 0, 0), Uni->R, 30, 30);
  Uni->Primitive = Ani->PrimFromTopo(&Uni->Sphere);

  /* Loading audio buffer from ".wav" file */
  Uni->Buf = Ani->AudioLoadFromWAV("rocky.wav");

  /* Creating audio source */
  Uni->Src = Ani->AudioSourceCreate();

  /* Setting audio source parameters */
  Ani->AudioSourceSet(Uni->Src,           /* Pointer to source */
                      Uni->Buf,           /* Pointer to attaching buffer */
                      TRUE,               /* Repeat (FALSE - no, TRUE - infinity) */
                      1.0,                /* Playing speed */
                      "EXPONENTIAL",      /* Audio model */
                      50,                 /* Hearing distance */
                      VecSet(-Uni->RS,
                             5,
                             0),          /* Position of audio player */
                      VecSet(0,
                             0,
                             0),          /* Audio player speed (!per frames, not sec) */
                             FALSE,       /* Audio pause flag */   
                             1,           /* Roll off factor */
                             25);         /* Reference distance */

  /* Start play */
  Ani->AudioPlay(Uni->Src); 
} /* End of 'TNG_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_AUDIO *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitClose( tngUNIT_SK3_AUDIO *Uni, tngANIM *Ani )
{
  Ani->PrimFree(Uni->Primitive);
} /* End of 'TNG_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_AUDIO *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitResponse( tngUNIT_SK3_AUDIO *Uni, tngANIM *Ani )
{
  FLT si, co;

  /* Pause */
  if (Ani->KeysClick['M'] || !Ani->WindowActive)
    Ani->AudioPause(Uni->Src);

  if (!Uni->Src->IsPause)
  {
    /* Set audio player speed */
    si = sin(Uni->Time / 60) / 60, /* Ordinary speed of spinning */
    co = cos(Uni->Time / 60) / 60; /* in ordinary circle  */

    Ani->AudioPlayerSpeedSet(Uni->Src, VecSet(si * Uni->RS, 0, co * Uni->RS));

    /* Audio player move */
    Ani->AudioMove(Uni->Src);

    /* Changing frame time */
    Uni->Time++;
  }

  /* Attach volume to distance to listener */
  Ani->AudioAttachVolumeToDistance(Uni->Src);
} /* End of 'TNG_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_AUDIO *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitRender( tngUNIT_SK3_AUDIO *Uni, tngANIM *Ani )
{
  /* Draw audio player sphere */
  Ani->PrimDraw(Uni->Primitive, MatrTranslate(Uni->Src->Pos));
} /* End of 'TNG_UnitRender' function */

/* Unit deffered render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_AUDIO *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS: None.
 */
static VOID TNG_UnitDefferRender( tngUNIT_SK3_AUDIO *Uni, tngANIM *Ani )
{
} /* End of 'TNG_UnitDefferRender' function */

/* Unit deffered render distance to viewer obtain function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tngUNIT_SK3_AUDIO *Uni;
 *   - animation context:
 *       tngANIM *Ani;
 * RETURNS:
 *   (FLT) distance to viewer.
 */
static FLT TNG_UnitGetDistance( tngUNIT_SK3_AUDIO *Uni, tngANIM *Ani )
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
tngUNIT * TNG_UnitCreateSK3Audio( tngANIM *Ani )
{
  tngUNIT_SK3_AUDIO *Uni;

  /* Memory allocation */
  if ((Uni = (tngUNIT_SK3_AUDIO *)TNG()->UnitCreate(sizeof(tngUNIT_SK3_AUDIO))) == NULL)
    return NULL;

  Uni->Init = (VOID *)TNG_UnitInit;
  Uni->Close = (VOID *)TNG_UnitClose;
  Uni->Response = (VOID *)TNG_UnitResponse;
  Uni->Render = (VOID *)TNG_UnitRender;
  Uni->DefferRender = (VOID *)TNG_UnitDefferRender;
  Uni->GetDistance = (VOID *)TNG_UnitGetDistance;

  return (tngUNIT *)Uni;
} /* End of 'TNG_UnitCreateSK3Audio' function */

/* END OF 'tng_unit_sk3_audio.c' FILE  */
