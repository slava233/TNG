/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_primitives.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Main primitives module.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kononov.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <ctype.h>

/* Primitive create function.
 * ARGUMENTS: 
 *   - primitive type:
 *       tngPRIM_TYPE Type;
 *   - vertex data format:
 *       CHAR *VertexFormat;
 *   - vertex data:
 *       VOID *V;
 *   - number of vertexes:
 *       INT NumOfV;
 *   - index array:
 *       INT *Ind;
 *   - number of indices:
 *       INT NumOfI;
 * RETURNS:
 *   (tngPRIM *) pointer to created primitive.
 */
static tngPRIM * TNG_PrimCreate( tngPRIM_TYPE Type, CHAR *VertexFormat, VOID *V, INT NumOfV, INT *Ind, INT NumOfI )
{
  INT i, type, attr_count = 0, all_size = 0, n;
  struct
  {
    INT Count;  /* Component count */
    INT Type;   /* Type of attributes */
    INT Offset; /* Offset to attributes in structure */
  } Attribs[16];
  VEC *P = V;
  tngPRIM Pr;

  memset(&Pr, 0, sizeof(tngPRIM));

  /* Parse attributes parameters */
  while (*VertexFormat != 0)
    if (*VertexFormat == 'i' || *VertexFormat == 'f')
    {
      type = *VertexFormat++;
      if (isdigit((BYTE)*VertexFormat))
        n = *VertexFormat++ - '0';
      else
        n = 1;
      if (attr_count < 16)
      {
        Attribs[attr_count].Count = n;
        Attribs[attr_count].Type = type == 'i' ? GL_UNSIGNED_INT : GL_FLOAT;
        Attribs[attr_count].Offset = all_size;
      }
      all_size += n * 4;
      attr_count++;
    }
    else
      VertexFormat++;

  if (all_size != 0 && V != NULL && NumOfV != 0)
  {
    /* Create OpenGL primitve */
    glGenBuffers(1, &Pr.VBuf);
    glGenVertexArrays(1, &Pr.VA);

    /* Vertexes */
    glBindVertexArray(Pr.VA);

    glBindBuffer(GL_ARRAY_BUFFER, Pr.VBuf);
    glBufferData(GL_ARRAY_BUFFER, all_size * NumOfV, V, GL_STATIC_DRAW);

    /* Store and enable vertex attributes */
    for (i = 0; i < attr_count; i++)
    {
      glVertexAttribPointer(i, Attribs[i].Count, Attribs[i].Type, FALSE, all_size, (VOID *)Attribs[i].Offset);
      glEnableVertexAttribArray(i);
    }
    glBindVertexArray(0);

    /* Indexes */
    if (Ind != NULL && NumOfI != 0)
    {
      glGenBuffers(1, &Pr.IBuf);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr.IBuf);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, Ind, GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      Pr.NumOfI = NumOfI;
    }
    else
      Pr.NumOfI = NumOfV;

    /* Evaluate minimum & maximum primitives */
    Pr.Min = Pr.Max = P[0];
    for (i = 1; i < NumOfV; i++)
    {
      Pr.Min = VecSet(COM_MIN(Pr.Min.X, P->X),  
                      COM_MIN(Pr.Min.Y, P->Y),  
                      COM_MIN(Pr.Min.Z, P->Z)); 
      Pr.Max = VecSet(COM_MAX(Pr.Max.X, P->X),  
                      COM_MAX(Pr.Max.Y, P->Y),  
                      COM_MAX(Pr.Max.Z, P->Z)); 
      P = (VEC *)((BYTE *)P + all_size);
    }
    Pr.Center = VecDivNum(VecAddVec(Pr.Min, Pr.Max), 2);
  }
  else
    Pr.NumOfI = NumOfV;

  Pr.Trans = MatrIdentity();
  Pr.Type = Type;

  return StockAdd(&TNG()->Primitives, &Pr);
} /* End of 'TNG_PrimCreate' function */

/* Primitive deferring function.
 * ARGUMENTS:
 *   - primitive to defer:
 *       tngPRIM *Prim;
 *   - world CS matrix to work with:
 *       MATR World;
 * RETURNS: None.
 */
static VOID TNG_PrimDrawDeferred( tngPRIM *Prim, MATR World )
{
  tngPRIM_DEFERRED DefPrim;

  DefPrim.Prim = Prim;
  DefPrim.World = World;
  DefPrim.Camera = TNG()->Camera;

  StockAdd(&TNG()->PrimitivesDeferred, &DefPrim);
} /* End of 'TNG_DeferPrim' function */

/* Draw primitive function.
 * ARGUMENTS:
 *   - primitive to draw:
 *       tngPRIM *Prim;
 *   - world CS matrix to work with:
 *       MATR World;
 *   - used camera data:
 *       CAMERA Camera;
 * RETURNS: None.
 */
static VOID TNG_PrimDrawLocal( tngPRIM *Prim, MATR World, CAMERA Camera )
{
  INT
    loc,
    PrimType =
      Prim->Type == TNG_PRIM_POINTS ? GL_POINTS :
      Prim->Type == TNG_PRIM_LINES  ? GL_LINES :
      Prim->Type == TNG_PRIM_STRIP  ? GL_TRIANGLE_STRIP :
      Prim->Type == TNG_PRIM_LINE_STRIP  ? GL_LINE_STRIP :
      Prim->Type == TNG_PRIM_LINE_STRIP_ADJACENCY ? GL_LINE_STRIP_ADJACENCY :
      Prim->Type == TNG_PRIM_PATCH ? GL_PATCHES :
      Prim->Type == TNG_PRIM_TRIMESH  ? GL_TRIANGLES : GL_POINTS;
  MATR 
     W = MatrMulMatr(Prim->Trans, World),
     N = MatrTranspose(MatrInverse(W)), 
     WVP = MatrMulMatr(W, Camera.VP);
  UINT ProgId;
  VEC v;

  glLoadMatrixf(WVP.M[0]);

  ProgId = TNG()->MaterialApply(Prim->Mtl);

  /*
  TNG()->ShaderAddComponent(ProgId, "Standart()", "Default", 0);
  TNG()->ShaderAddComponentMatrix(ProgId, W, Camera.VP);
  */

  /* Setup transform coefficients */
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.M[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, Camera.VP.M[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, W.M[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrV")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, Camera.ViewMatr.M[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrN")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, N.M[0]);
  if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1)
    glUniform3fv(loc, 1, &Camera.Loc.X);
  v = CamRight(&Camera);
  if ((loc = glGetUniformLocation(ProgId, "CamRight")) != -1)
    glUniform3fv(loc, 1, &v.X);
  v = CamUp(&Camera);
  if ((loc = glGetUniformLocation(ProgId, "CamUp")) != -1)
    glUniform3fv(loc, 1, &v.X);
  v = CamDir(&Camera);
  if ((loc = glGetUniformLocation(ProgId, "CamDir")) != -1)
    glUniform3fv(loc, 1, &v.X);
  if ((loc = glGetUniformLocation(ProgId, "CamProjDist")) != -1)
    glUniform1f(loc, Camera.ProjDist);
  if ((loc = glGetUniformLocation(ProgId, "CamProjSize")) != -1)
    glUniform1f(loc, Camera.ProjSize);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, TNG()->Time);
  if ((loc = glGetUniformLocation(ProgId, "FrameW")) != -1)
    glUniform1f(loc, TNG()->FrameW);
  if ((loc = glGetUniformLocation(ProgId, "FrameH")) != -1)
    glUniform1f(loc, TNG()->FrameH);

  glBindVertexArray(Prim->VA);

  if (Prim->Type == TNG_PRIM_PATCH)
    glPatchParameteri(GL_PATCH_VERTICES, Prim->NumOfI);

  if (Prim->IBuf != 0)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->IBuf);
    glDrawElements(PrimType, Prim->NumOfI, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  else
    glDrawArrays(PrimType, 0, Prim->NumOfI);

  glUseProgram(0);

  glBindVertexArray(0);
} /* End of 'TNG_PrimDrawLocal' function */

/* Draw primitive function.
 * ARGUMENTS:
 *   - primitive to draw:
 *       tngPRIM *Prim;
 *   - world CS matrix to work with:
 *       MATR World;
 * RETURNS: None.
 */
static VOID TNG_PrimDraw( tngPRIM *Prim, MATR World )
{
  TNG_PrimDrawLocal(Prim, World, TNG()->Camera);
} /* End of 'TNG_PrimDraw' function */

/* Draw all deferred primitives function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_PrimDrawAllDeferred( VOID )
{
  tngPRIM_DEFERRED *DefPrim = StockStart(&TNG()->PrimitivesDeferred);

  /* Draw every deferred primitive */
  while (DefPrim != NULL)
  {
    TNG_PrimDrawLocal(DefPrim->Prim, DefPrim->World, DefPrim->Camera);
    DefPrim = StockNext(DefPrim);
  }
  StockEmpty(&TNG()->PrimitivesDeferred);
} /* End of 'TNG_DeferedPrimDraw' function */

/* Stock strategy delete primitive function.
 * ARGUMENTS:
 *   - primitive to be delete:
 *       tngPRIM *Prim;
 * RETURNS: None.
 */
static VOID TNG_PrimDelete( tngPRIM *Prim )
{
  if (Prim->VA != 0)
  {
    glBindVertexArray(Prim->VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Prim->VA);
  }
  if (Prim->VBuf != 0)
    glDeleteBuffers(1, &Prim->VBuf);
  if (Prim->IBuf != 0)
    glDeleteBuffers(1, &Prim->IBuf);
} /* End of 'TNG_PrimDelete' function */

/* Delete primitive function.
 * ARGUMENTS:
 *   - primitive to delete:
 *       tngPRIM *Prim;
 * RETURNS: None.
 */
static VOID TNG_PrimFree( tngPRIM *Prim )
{
  StockRemove(Prim);
} /* End of 'TNG_PrimFree' function */

/* Primitives subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemPrimitivesInit( VOID )
{
  StockInit(&TNG()->Primitives, 0, sizeof(tngPRIM), TNG_PrimDelete);
  StockInit(&TNG()->PrimitivesDeferred, 0, sizeof(tngPRIM_DEFERRED), NULL);
  TNG()->PrimCreate = TNG_PrimCreate;
  TNG()->PrimFree = TNG_PrimFree;
  TNG()->PrimDraw = TNG_PrimDraw;
  TNG()->PrimDrawDeferred = TNG_PrimDrawDeferred;
  TNG()->PrimDrawAllDeferred = TNG_PrimDrawAllDeferred;
} /* End of 'TNG_SubsystemPrimitivesInit' function */

/* Primitives subsystem destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemPrimitivesDestructor( VOID )
{
  StockClear(&TNG()->Primitives);
  StockClear(&TNG()->PrimitivesDeferred);
} /* End of 'TNG_SubsystemPrimitivesDestructor' function */

/* END OF 'tng_primitives.c' FILE */
