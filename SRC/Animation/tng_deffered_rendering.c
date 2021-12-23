/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_deffered_anim.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Deffered animation module.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 29.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "tng_deffered_rendering.h"

/* Swap function for deffered rendering.
 * ARGUMENTS:
 *   - elements to be swaped:
 *       tngUNIT_DRAW_DEFFERED *x, *y;
 * RETURNS: None.
 */
static VOID TNG_DefferSwap( tngUNIT_DRAW_DEFFERED *x, tngUNIT_DRAW_DEFFERED *y )
{
  tngUNIT_DRAW_DEFFERED tmp = *x;

  *x = *y;
  *y = tmp;
} /* End of 'TNG_DefferSwap' function */

/* Sort function for deffered rendering.
 * ARGUMENTS:
 *  - array of units:
 *      tngUNIT_DRAW_DEFFERED *A;
 *  - number of units:
 *      INT N;
 * RETURNS: None.
 */
static VOID TNG_DefferQuickSort( tngUNIT_DRAW_DEFFERED *A, INT N )
{
  INT b = 0, e = N - 1;
  FLT x = A[N / 2].Dist;
  
  if (N < 2)
    return;

  while (b <= e)
  {
    while (A[b].Dist < x)
      b++;
    while (A[e].Dist > x)
      e--;

    if (b <= e)
    {
      if (b != e)
        TNG_DefferSwap(&A[b], &A[e]);
      b++, e--;
    }
  }
  TNG_DefferQuickSort(A, e + 1);
  TNG_DefferQuickSort(A + b, N - b);
} /* End of 'TNG_DefferQuickSort' function */

/* Deffered rendering function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
TNG_AnimDefferedRendering( VOID )
{
  INT i;
  tngUNIT *U;

  TNG()->RndStartDefer();
  if (TNG()->DrawDefferedCurrentSize != 0)
  {
    /* Fill distance to viewer in array of units */
    for (i = 0; i < TNG()->DrawDefferedCurrentSize; i++)
    {
      U = TNG()->DrawDeffered[i].Uni;
      TNG()->DrawDeffered[i].Dist = U->GetDistance(U, TNG());
    }

    /* Sort */
    TNG_DefferQuickSort(TNG()->DrawDeffered, TNG()->DrawDefferedCurrentSize);

    /* Draw: from the farest to the nearest */
    for (i = TNG()->DrawDefferedCurrentSize - 1; i >= 0; i--)
      TNG()->DrawDeffered[i].Uni->DefferRender(TNG()->DrawDeffered[i].Uni, TNG());
    memset(TNG()->DrawDeffered, 0, sizeof(tngUNIT_DRAW_DEFFERED) * TNG()->DrawDefferedFullSize);
    TNG()->DrawDefferedCurrentSize = 0;
  }
} /* End of 'TNG_AnimDefferedRendering' function */

/* END OF 'tng_deffered_anim.c' FILE */
