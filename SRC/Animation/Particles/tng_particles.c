/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_particles.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Particles system support.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "tng_particles.h"

/* Standart added size */
#define TNG_PRT_BLOCK 500

#if 0
/* Swap function for particles support.
 * ARGUMENTS:
 *   - elements to be swaped:
 *       tngPARTICLE *x, *y;
 * RETURNS: None.
 */
static VOID TNG_ParticlesSwap( tngPARTICLE *x, tngPARTICLE *y )
{
  tngPARTICLE tmp = *x;

  *x = *y;
  *y = tmp;
} /* End of 'TNG_ParticlesSwap' function */

/* Sort function for particles support.
 * ARGUMENTS:
 *  - array of units:
 *      tngPARTICLE *A;
 *  - number of particles:
 *      INT N;
 * RETURNS: None.
 */
static VOID TNG_ParticlesQuickSort( tngPARTICLE *A, INT N )
{
  INT b = 0, e = N - 1;
  FLT x = A[N / 2].Age;
  
  if (N < 2)
    return;

  while (b <= e)
  {
    while (A[b].Age < x)
      b++;
    while (A[e].Age > x)
      e--;

    if (b <= e)
    {
      if (b != e)
        TNG_ParticlesSwap(&A[b], &A[e]);
      b++, e--;
    }
  }
  TNG_ParticlesQuickSort(A, e + 1);
  TNG_ParticlesQuickSort(A + b, N - b);
} /* End of 'TNG_ParticlesQuickSort' function */
#endif

/* Strategy free block of particles function.
 * ARGUMENTS:
 *    - deleted block:
 *        tngPARTICLE **Block;
 * RETURNS: None.
 */
static VOID TNG_BlockFree( tngPARTICLE **Block )
{
  free(*Block);
} /* End of 'TNG_BlockFree' function */

/* Create particles emitter function.
 * ARGUMENTS:
 *    - created emitter:
 *        tngEMITTER *Emi;
 * RETURNS: None.
 */
static VOID TNG_EmitterCreate( tngEMITTER *Emi )
{
  INT i;

  if (Emi == NULL)
    return;

  memset(Emi, 0, sizeof(tngEMITTER));
  Emi->InjectionSpeed = 1;
  Emi->PrtLifeTime = 3;
  Emi->ParticleQueue = malloc(sizeof(tngPARTICLE) * (TNG_PRT_BLOCK + 1));
  StockInit(&Emi->BlockMas, 5, sizeof(tngPARTICLE *), TNG_BlockFree);
  StockAdd(&Emi->BlockMas, &Emi->ParticleQueue);
  Emi->FreeParticleStack = &Emi->ParticleQueue[1];
  Emi->ParticleQueueSize = 0;
  Emi->ParticleStackSize = TNG_PRT_BLOCK;
  /* Set starter particle */
  Emi->ParticleQueue->Next = NULL;
  Emi->ParticleQueue->Age = 0;
  Emi->ParticleQueue->TotalTime = 0;

  memset(Emi->FreeParticleStack, 0, sizeof(tngPARTICLE) * TNG_PRT_BLOCK);
  for (i = 0; i < TNG_PRT_BLOCK - 1; i++)
    Emi->FreeParticleStack[i].Next = &Emi->FreeParticleStack[i + 1];
  Emi->FreeParticleStack[i].Next = NULL;
} /* End of 'TNG_EmitterCreate' function */

/* Delete particles emitter function.
 * ARGUMENTS:
 *    - emitter to be deleted:
 *        tngEMITTER *Emi;
 * RETURNS: None.
 */
static VOID TNG_EmitterFree( tngEMITTER *Emi )
{
  StockClear(&  Emi->BlockMas);
  memset(Emi, 0, sizeof(tngEMITTER));
} /* End of 'TNG_EmitterFree' function */

/* Create new particles function.
 * ARGUMENTS:
 *    - particles emitter:
 *        tngEMITTER *Emi;
 * RETURNS: None.
 */
static VOID TNG_ParticlesNew( tngEMITTER *Emi, INT CountOfNewParticles )
{
  INT offset;
  tngPARTICLE *prtfirst, *prt;

  if (CountOfNewParticles <= 0)
    return;

  if (CountOfNewParticles > Emi->ParticleStackSize)
  {
    INT CountOfBlocks = CountOfNewParticles / TNG_PRT_BLOCK + 1, i;
    tngPARTICLE *tmp = malloc(sizeof(tngPARTICLE) * CountOfBlocks * TNG_PRT_BLOCK);

    StockAdd(&Emi->BlockMas, &tmp);
    memset(tmp, 0, sizeof(tngPARTICLE) * CountOfBlocks * TNG_PRT_BLOCK);
    for (i = 0; i < CountOfBlocks * TNG_PRT_BLOCK - 1; i++)
      tmp[i].Next = &tmp[i + 1];
    tmp[i].Next = Emi->FreeParticleStack;
    Emi->FreeParticleStack = tmp;
    Emi->ParticleStackSize += CountOfBlocks * TNG_PRT_BLOCK;
    /* tmp = NULL; */
  }

  prtfirst = prt = Emi->FreeParticleStack;
  for (offset = 0; offset < CountOfNewParticles; offset++)
  {
    prt->Age = (offset + Emi->InjectionRest) / Emi->InjectionSpeed;
    prt->TotalTime = Emi->PrtLifeTime; /* Temporary */
    offset < CountOfNewParticles - 1 ? prt = prt->Next : (Emi->FreeParticleStack = prt->Next,
                                                          prt->Next = Emi->ParticleQueue->Next);
  }
  Emi->ParticleQueue->Next = prtfirst;
  Emi->ParticleStackSize -= CountOfNewParticles;
  Emi->ParticleQueueSize += CountOfNewParticles;
  #if 0
  tmp1 = malloc(sizeof(tngPARTICLE) * (Emi->ParticleQueueSize = Emi->ParticleQueueSize + (CountOfNewParticles > 0 ? CountOfNewParticles : TNG_PRT_BLOCK)));
  memset(tmp1, 0, sizeof(tngPARTICLE) * Emi->ParticleQueueSize);
  memcpy(tmp1, Emi->ParticleQueue, sizeof(tngPARTICLE) * OldSize);

  /* Swap */
  tmp2 = tmp1;
  tmp1 = Emi->ParticleQueue;
  Emi->ParticleQueue = tmp2;
  free(tmp1);
  #endif
} /* End of 'TNG_ParticlesNew' function */

/* Update particles function.
 * ARGUMENTS:
 *    - particles emitter:
 *        tngEMITTER *Emi;
 *    - time between frames:
 *        FLT DelatTime;
 * RETURNS: None.
 */
static VOID TNG_ParticlesHandleTime( tngEMITTER *Emi, FLT DeltaTime )
{
  INT NewParticlesAmount = 0;
  FLT NewParticles = 0;
  tngPARTICLE *prt = Emi->ParticleQueue->Next, *prev = Emi->ParticleQueue;

  /* Create new particles */
  NewParticlesAmount = NewParticles = Emi->InjectionSpeed * DeltaTime + Emi->InjectionRest;
  Emi->InjectionRest = NewParticles - NewParticlesAmount;

  while (prt != NULL)
  {
    prt->Age += DeltaTime;
    if (prt->Age > prt->TotalTime)
    {
      prt->Age = 0;
      prt->TotalTime = 0;
      prev->Next = prt->Next;
      prt->Next = Emi->FreeParticleStack;
      Emi->FreeParticleStack = prt;
      prt = prev->Next;
      continue;
    }
    prev = prt;
    prt = prt->Next;
  }
  TNG_ParticlesNew(Emi, NewParticlesAmount);
  #if 0
  for (i = 0, j = 0; i < Emi->ParticleQueueSize; i++)
  {
    Emi->ParticleQueue[i].Age += DeltaTime;
    if (Emi->ParticleQueue[i].Age > Emi->ParticleQueue[i].TotalTime)
    {
      Emi->ParticleQueue[i].Age = Emi->ParticleQueue[i].TotalTime = 0;
      if (NewParticlesAmount > 0)
      {
        Emi->ParticleQueue[i].Age = (j + Emi->InjectionRest) / Emi->InjectionSpeed;
        Emi->ParticleQueue[i].TotalTime = Emi->PrtLifeTime; /* Temporary */
        j++;
        NewParticlesAmount--;
      }
    }
  }
  if (NewParticlesAmount > 0)
  {
    TNG_ParticlesNew(Emi, NewParticlesAmount);
    for (; i < Emi->ParticleQueueSize; i++)
    {
      Emi->ParticleQueue[i].Age = (j + Emi->InjectionRest) / Emi->InjectionSpeed;
      Emi->ParticleQueue[i].TotalTime = Emi->PrtLifeTime; /* Temporary */
      j++;
    }
  }
  TNG_ParticlesQuickSort(Emi->ParticleQueue, Emi->ParticleQueueSize);
  #endif
} /* End of 'TNG_ParticleHadleTime' function */

/* Starter subsystem of particle systems */
static tngSUBSYSTEM_PARTICLES SubSystemParticlesAdd =
{
  /* Handle functions */
  TNG_EmitterCreate,
  TNG_EmitterFree,
  TNG_ParticlesHandleTime
};

/* Subsystem particle system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemParticlesInit( VOID )
{
  TNG()->SubSystemParticles = SubSystemParticlesAdd;
} /* End of 'TNG_SubsystemParticleSystemInit' function */

/* END OF 'tng_particles.c' FILE */
