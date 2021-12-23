/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_anim.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Animation system declarations.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_anim_h_
#define __tng_anim_h_

#include "Render/tng_render.h"
#include "Timer/tng_timer.h"
#include "WindowsDepended/tng_windows_depended.h"
#include "Input/tng_input.h"
#include "Audio/tng_audio.h"
#include "../Topology/tng_topology.h"
#include "tng_unit.h"
#include "tng_unit_set.h"
#include "Particles/tng_particles.h"
#include "tng_deffered_rendering.h"
#include "cubemap/tng_cube_maps.h"

/* Metod for use in hard-level and middle-level system
 * for get animation context
 */
#define TNG() (&TNG_Anim)

/* Declaration animation context structure */
struct tagtngANIM
{
  BOOL WindowActive; /* Window active flag */

  /* Do exit unit function.
   * ARGUMENTS: None.
   * RETURNS:
   *  (BOOL) change of exit: TRUE, if user don't have to close program,
   *                         FALSE another.
   */
  BOOL (*DoExit)( VOID );

  /* Windows depended subsystem */
  union
  {
    tngSUBSYSTEM_WIN;
    tngSUBSYSTEM_WIN SubsystemWin;
  };

  /* Timer subsystem */
  union
  {
    tngSUBSYSTEM_TIMER;
    tngSUBSYSTEM_TIMER SubsystemTimer;
  };

  /* Input subsystem */
  union
  {
    tngSUBSYSTEM_INPUT;
    tngSUBSYSTEM_INPUT SubsystemInput;
  };

  /* */
  union
  {
    tngSUBSYSTEM_RENDER;
    tngSUBSYSTEM_RENDER SubsystemRender;
  };

  /* Audio subsystem */
  union
  {
    tngSUBSYSTEM_AUDIO;
    tngSUBSYSTEM_AUDIO SubsystemAudio;
  };

  /* Topology subsystem */
  union
  {
    tngSUBSYSTEMTOPOLOGY;
    tngSUBSYSTEMTOPOLOGY SubsystemTopology;
  };

  /* Unit subsystem */
  union
  {
    tngSUBSYSTEM_UNIT;
    tngSUBSYSTEM_UNIT SubSystemUnit;
  };

  /* Unit set subsystem */
  union
  {
    tngSUBSYSTEM_UNIT_SET;
    tngSUBSYSTEM_UNIT_SET SubSystemUnitSet;
  };

  /* Particle system subsystem */
  union
  {
    tngSUBSYSTEM_PARTICLES;
    tngSUBSYSTEM_PARTICLES SubSystemParticles;
  };

  /* Cubemap subsystem */
  union
  {
    tngSUBSYSTEM_SKYBOX;
    tngSUBSYSTEM_SKYBOX SubSystemSkyBox;
  };
};

/* Declaration unit (base system object) structure */
struct tagtngUNIT
{
  /* Unit initialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       tngUNIT *Uni;
   *   - animation context:
   *       tngANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Init)( tngUNIT *Uni, tngANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       tngUNIT *Uni;
   *   - animation context:
   *       tngANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Close)( tngUNIT *Uni, tngANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       tngUNIT *Uni;
   *   - animation context:
   *       tngANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Response)( tngUNIT *Uni, tngANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       tngUNIT *Uni;
   *   - animation context:
   *       tngANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Render)( tngUNIT *Uni, tngANIM *Ani );

  /* Unit deffered render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       tngUNIT *Uni;
   *   - animation context:
   *       tngANIM *Ani;
   * RETURNS: None.
   */
  VOID (*DefferRender)( tngUNIT *Uni, tngANIM *Ani );

  /* Unit deffered render distance to viewer obtain function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       tngUNIT *Uni;
   *   - animation context:
   *       tngANIM *Ani;
   * RETURNS:
   *   (FLT) distance to viewer.
   */
  FLT (*GetDistance)( tngUNIT *Uni, tngANIM *Ani );
};

/***
 * Animation module
 ***/

/* Animation context */
extern tngANIM TNG_Anim;

/* Initialization animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_AnimInit( VOID );

/* Registrator animation system work function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_AnimRun( VOID );

/* Add unit to stock function
 * ARGUMENTS:
 *   - unit to be added:
 *       tngUNIT *Uni;
 * RETURNS: None.
 */
VOID TNG_AnimUnitAdd( tngUNIT *Uni );

#endif /* __tng_anim_h_ */

/* END OF 'tng_anim.h' FILE */
