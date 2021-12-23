/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_particles.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Particles system declarations.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_particles_h_
#define __tng_particles_h_

#include "../../tng_def.h"

/* Main representation particle type */
typedef struct tagtngPARTICLE tngPARTICLE;

struct tagtngPARTICLE
{
  tngPARTICLE *Next;  /* Next particle */
  FLT
    Age,              /* Ñurrent particle lifetime since birth in seconds */
    TotalTime;        /* Every particle life time in seconds */
  BOOL ImBlock;       /* Pointer to block flag */
};

/* Representation particles in system type */
typedef struct tagtngEMITTER
{
  tngPARTICLE *ParticleQueue;      /* Particles in memory */
  tngPARTICLE *FreeParticleStack;  /* Storage free particles */
  STOCK BlockMas;                  /* Storage of block pointer */
  INT ParticleStackSize;           /* Count of free particle */
  INT ParticleQueueSize;           /* Count of particles in queue */
  FLT InjectionSpeed;              /* Particles amount per seconds */
  FLT InjectionRest;               /* Particle remainder */
  /* Temporary */
  FLT PrtLifeTime;                 /* Starter life time in seconds */
} tngEMITTER;

/* Unit subsytem structure */
typedef struct tagtngSUBSYSTEM_PARTICLES
{
  /* Create particles emitter function.
   * ARGUMENTS:
   *    - created emitter:
   *        tngEMITTER *Emi;
   * RETURNS: None.
   */
   VOID (*EmitterCreate)( tngEMITTER *Emi );

  /* Delete particles emitter function.
   * ARGUMENTS:
   *    - emitter to be deleted:
   *        tngEMITTER *Emi;
   * RETURNS: None.
   */
  VOID (*EmitterFree)( tngEMITTER *Emi );

  /* Update particles function.
   * ARGUMENTS:
   *    - particles emitter:
   *        tngEMITTER *Emi;
   *    - time between frames:
   *        FLT DeltaTime;
   * RETURNS: None.
   */
  VOID (*ParticlesHandleTime)( tngEMITTER *Emi, FLT DeltaTime );
} tngSUBSYSTEM_PARTICLES;

/* Subsystem particle system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemParticlesInit( VOID );

#endif /* __tng_particles_h_ */

/* END OF 'tng_particles.h' FILE */
