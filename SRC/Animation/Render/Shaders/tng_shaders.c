/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_shaders.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Shaders handle functions.
 * PROGRAMMER  : CGSG'2018.
 *               Girvits Alexander.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"
#include "../../../tng.h"

#include <stdio.h>

#include "tng_shaders.h"

#define TNG_NUM_OF_COMP_AT_BLOCK 10

/* Shader struct */
typedef struct tagtngSHADER
{
  CHAR Name[TNG_NAME_SIZE]; /* Shaders path prefix */
  UINT PrgNo;               /* shader program Id */
} tngSHADER;

/* Stock for shaders */
static STOCK TNG_Shaders;

/* Default shader */
tngSHADER *TNG_ShaderDefault;

/* Add component block into shader function.
 * ARGUMENTS: 
 *   - shader program Id:
 *       UINT PrgNo;
 * RETURNS: None.
 */
static VOID TNG_ShaderAddComponentStandart( UINT PrgNo );

/* Save text to log file function.
 * ARGUMENTS:
 *   - shader file name:
 *       CHAR *FileName;
 *   - text 1 to save:
 *       CHAR *Stage;
 *   - text 2 to save:
 *       CHAR *Text;
 * RETURNS: None.
 */
static VOID TNG_ShaderLog( CHAR *FileName, CHAR *Stage, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("TNG{SHAD}30.LOG", "a")) != NULL)
  {
    fprintf(F, "%s\n  %s: %s\n", FileName, Stage, Text);
    fclose(F);
  }
} /* End of 'TNG_ShaderLog' function */

/* Text file load to memory function.
 * ARGUMENTS:
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text or NULL if error is occured.
 */
static CHAR * TNG_ShaderLoadTextFile( CHAR *FileName )
{
  FILE *F;
  INT flen;
  CHAR *text;

  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;

  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  /* Allocate memory */
  if ((text = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(text, 0, flen + 1);

  /* Read text */
  fseek(F, 0, SEEK_SET);
  fread(text, 1, flen, F);

  fclose(F);
  return text;
} /* End of 'TNG_ShaderLoadTextFile' function */

/* Load shader program function.
 * ARGUMENTS:
 *   - shader files prefix (directory):
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (UINT) shader program index or 0 if error is occured.
 */
static UINT TNG_ShaderLoad( CHAR *FileNamePrefix )
{
  INT
    i, res,
    shd_type[] =
    {
      GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER,
      GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER
    };
  CHAR *suff[] = {"VERT", "CTRL", "EVAL", "GEOM", "FRAG"};
  INT NS = sizeof(suff) / sizeof(suff[0]);
  UINT prg = 0, shd[sizeof(suff) / sizeof(suff[0])] = {0};
  BOOL isok = TRUE;
  CHAR *txt;
  static CHAR buf[500];

  /* Load shaders */
  for (i = 0; isok && i < NS; i++)
  {
    /* Load text file */
    sprintf(buf, "%s\\%s.GLSL", FileNamePrefix, suff[i]);
    if ((txt = TNG_ShaderLoadTextFile(buf)) == NULL)
      continue;
    /* Create shader */
    if ((shd[i] = glCreateShader(shd_type[i])) == 0)
    {
      free(txt);
      TNG_ShaderLog(FileNamePrefix, suff[i], "Error create shader");
      isok = FALSE;
      break;
    }
    /* Attach text to shader  */
    glShaderSource(shd[i], 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shd[i]);
    glGetShaderiv(shd[i], GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i], sizeof(buf), &res, buf);
      TNG_ShaderLog(FileNamePrefix, suff[i], buf);
      isok = FALSE;
      break;
    }
  }

  /* Create program */
  if (isok)
    if ((prg = glCreateProgram()) == 0)
      isok = FALSE;
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NS; i++)
        if (shd[i] != 0)
          glAttachShader(prg, shd[i]);

      /* Link program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(buf), &res, buf);
        TNG_ShaderLog(FileNamePrefix, "LINK", buf);
        isok = FALSE;
      }
    }

  /* Error handle */
  if (!isok)
  {
    for (i = 0; i < NS; i++)
      if (shd[i] != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i]);
        glDeleteShader(shd[i]);
      }
    if (prg != 0)
      glDeleteProgram(prg);
    return 0;
  }
  return prg;
} /* End of 'TNG_ShaderLoad' function */

/* Shader program free strategy function.
 * ARGUMENTS:
 *   - shader program pointer:
 *       tngSHADER *Shd;
 * RETURNS: None.
 */
static VOID TNG_ShaderDelete( tngSHADER *Shd )
{
  UINT i, n, shds[5];

  if (Shd->PrgNo == 0)
    return;

  /* Obtain program shaders count */
  glGetAttachedShaders(Shd->PrgNo, 5, &n, shds);
  for (i = 0; i < n; i++)
  {
    glDetachShader(Shd->PrgNo, shds[i]);
    glDeleteShader(shds[i]);
  }
  glDeleteProgram(Shd->PrgNo);
} /* End of 'TNG_ShaderFree' function */

/* Add shader program from file function.
 * ARGUMENTS:
 *   - shader files prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS: 
 *  (tngSHADER *) pointer to added shader.
 */
static tngSHADER * TNG_ShaderAdd( CHAR *FileNamePrefix )
{
  tngSHADER *Shd = StockStart(&TNG_Shaders), shad;
  static CHAR Buf[1000];

  /* Look for exists shader */
  while (Shd != NULL)
  {
    if (strcmp(Shd->Name, FileNamePrefix) == 0)
      return Shd;
    Shd = StockNext(Shd);
  }

  sprintf(Buf, "BIN/SHADERS/%s", FileNamePrefix);
  shad.PrgNo = TNG_ShaderLoad(Buf);
  glUseProgram(shad.PrgNo);
  /*
  TNG_ShaderAddComponentStandart(shad.PrgNo);
  */
  glUseProgram(0);
  strncpy(shad.Name, FileNamePrefix, TNG_NAME_SIZE - 1);
  return StockAdd(&TNG_Shaders, &shad);
} /* End of 'TNG_ShaderAdd' function */

/* Update all shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TNG_ShadersUpdate( VOID )
{
  tngSHADER *Shd = StockStart(&TNG_Shaders);
  static DBL SaveTime;
  static CHAR Buf[1000];

  if (SaveTime == 0)
    SaveTime = TNG()->GlobalTime;
  if (TNG()->GlobalTime - SaveTime < 3)
    return;
  SaveTime = TNG()->GlobalTime;

  while (Shd != NULL)
  {
    sprintf(Buf, "BIN/SHADERS/%s", Shd->Name);
    TNG_ShaderDelete(Shd);
    Shd->PrgNo = TNG_ShaderLoad(Buf);
    glUseProgram(Shd->PrgNo);
    /* TNG_ShaderAddComponentStandart(Shd->PrgNo); */
    glUseProgram(0);
    Shd = StockNext(Shd);
  }
} /* End of 'TNG_ShadersUpdate' function */

/* Remove shader program function.
 * ARGUMENTS:
 *   - shader pointer:
 *       tngSHADER *Shd;
 * RETURNS: None.
 */
static VOID TNG_ShaderFree( tngSHADER *Shd )
{
  TNG_ShaderDelete(Shd);
  /*StockDelete(Shd);*/
} /* End of 'TNG_ShaderDelete' function */

/* Correct shader number function.
 * ARGUMENTS:
 *   - shader pointer:
 *       tngSHADER *Shd;
 * RETURNS: 
 *   (UINT) Shader program Id.
 */
static UINT TNG_ShaderApply( tngSHADER *Shd )
{
  if (Shd == NULL)
    Shd = TNG_ShaderDefault;
  glUseProgram(Shd->PrgNo);
  return Shd->PrgNo;
} /* End of 'TNG_ShadersApply' function */

/* Add component block into shader function.
 * ARGUMENTS: 
 *   - shader program Id:
 *       UINT PrgNo;
 *   - matrix world:
 *       MATR World;
 *   - multiplication matrixes view and proj:
 *       MATR MatrVP.
 * RETURNS: None.
 */
static VOID TNG_ShaderAddComponentMatrix( UINT PrgNo, MATR MatrW, MATR MatrVP )
{
  UINT IndexBlock, Indices[6], UboHandle;
  INT BlockSize, Offset[6], i, NumOfMatrix = 6;
  BYTE *BlockBuffer;
  MATR 
     MatrN = MatrTranspose(MatrInverse(MatrW)), 
     WVP = MatrMulMatr(MatrW, MatrVP);
  CHAR *names[] = {"MatrWVP", "MatrW", "MatrP", "MatrVP", "MatrN", "MatrV"};

  /* Find the uniform index buffer */
  IndexBlock = glGetUniformBlockIndex(PrgNo, "Matrix");
                            
  /* Determine block size  */
  glGetActiveUniformBlockiv(PrgNo, IndexBlock, GL_UNIFORM_BLOCK_DATA_SIZE, &BlockSize);
  BlockBuffer = malloc(BlockSize);

  if (BlockBuffer == NULL)
    return;

  /* Get indexes values */
  glGetUniformIndices(PrgNo, NumOfMatrix, names, Indices);

  /* Check for exist */
  for (i = 0; i < NumOfMatrix; i++)
    if (Indices[i] == GL_INVALID_INDEX)
    {
      if (i + 1 != NumOfMatrix && Indices[NumOfMatrix - 1] != GL_INVALID_INDEX)
      {
        Indices[i] = Indices[NumOfMatrix - 1];
        names[i] = names[NumOfMatrix - 1];
      }
      i--;
      NumOfMatrix--;
    }
  /* Get offsets values */
  glGetActiveUniformsiv(PrgNo, NumOfMatrix, Indices, GL_UNIFORM_OFFSET, Offset);
  

  /* Copy the uniform values into the buffers */
  for (i = 0; i < NumOfMatrix; i++)
    if (strcmp(names[i], "MatrWVP") == 0)
      memcpy(BlockBuffer + Offset[i], &WVP, sizeof(MATR));
    else if (strcmp(names[i], "MatrW") == 0)
      memcpy(BlockBuffer + Offset[i], &MatrW, sizeof(MATR));
    else if (strcmp(names[i], "MatrP") == 0)
      memcpy(BlockBuffer + Offset[i], &TNG()->Camera.ProjMatr, sizeof(MATR));
    else if (strcmp(names[i], "MatrVP") == 0)
      memcpy(BlockBuffer + Offset[i], &MatrVP, sizeof(MATR));
    else if (strcmp(names[i], "MatrN") == 0)
      memcpy(BlockBuffer + Offset[i], &MatrN, sizeof(MATR));
    else if (strcmp(names[i], "MatrV") == 0)
      memcpy(BlockBuffer + Offset[i], &TNG()->Camera.ViewMatr, sizeof(MATR));

  /* Create object buffer */
  glGenBuffers(1, &UboHandle);
  glBindBuffer(GL_UNIFORM_BUFFER, UboHandle);
  glBufferData(GL_UNIFORM_BUFFER, BlockSize, BlockBuffer, GL_DYNAMIC_DRAW);

  /* Associated with shader */
  glBindBufferBase(GL_UNIFORM_BUFFER, 8, UboHandle);
  free(BlockBuffer);
} /* End of 'TNG_ShaderAddComponentMatrix' function */

/* Add component block into shader function.
 * ARGUMENTS: 
 *   - shader program Id:
 *       UINT PrgNo;
 * RETURNS: None.
 */
static VOID TNG_ShaderAddComponentStandart( UINT PrgNo )
{
  UINT IndexBlock, Indices[5], UboHandle;
  INT BlockSize, Offset[5], i, NumOfComp = 5;
  BYTE *BlockBuffer;
  CHAR *names[] = {"Time", "FrameW", "FrameH", "IsWireFrame", "CamLoc"};


  /* Find the uniform index buffer */
  IndexBlock = glGetUniformBlockIndex(PrgNo, "Standart");
                            
  /* Determine block size  */
  glGetActiveUniformBlockiv(PrgNo, IndexBlock, GL_UNIFORM_BLOCK_DATA_SIZE, &BlockSize);
  BlockBuffer = malloc(BlockSize);

  if (BlockBuffer == NULL)
    return;

  /* Get indexes values */
  glGetUniformIndices(PrgNo, NumOfComp, names, Indices);

  /* Check for exist */
  for (i = 0; i < NumOfComp; i++)
    if (Indices[i] == GL_INVALID_INDEX)
    {
      if (i + 1 != NumOfComp && Indices[NumOfComp - 1] != GL_INVALID_INDEX)
      {
        Indices[i] = Indices[NumOfComp - 1];
        names[i] = names[NumOfComp - 1];
      }
      i--;
      NumOfComp--;
    }
  /* Get offsets values */
  glGetActiveUniformsiv(PrgNo, NumOfComp, Indices, GL_UNIFORM_OFFSET, Offset);

  /* Copy the uniform values into the buffers */
  memcpy(BlockBuffer + Offset[0], &TNG()->Time, sizeof(FLOAT));
  memcpy(BlockBuffer + Offset[1], &TNG()->FrameW, sizeof(INT));
  memcpy(BlockBuffer + Offset[2], &TNG()->FrameH, sizeof(INT));
  memcpy(BlockBuffer + Offset[3], &TNG()->IsWireFrame, sizeof(INT));
  memcpy(BlockBuffer + Offset[4], &TNG()->Camera.Loc, sizeof(VEC));

  /* Create object buffer */
  glGenBuffers(1, &UboHandle);
  glBindBuffer(GL_UNIFORM_BUFFER, UboHandle);
  glBufferData(GL_UNIFORM_BUFFER, BlockSize, BlockBuffer, GL_DYNAMIC_DRAW);

  /* Associated with shader */
  glBindBufferBase(GL_UNIFORM_BUFFER, 7, UboHandle);

  free(BlockBuffer);
} /* End of 'TNG_ShaderAddComponentStandart' function */

/* Add component block into shader function.
 * ARGUMENTS: 
 *   - shader program Id:
 *       UINT PrgNo;
 *   - string with shader block:
 *       CHAR *Str;
 *   - name block:
 *       CHAR *NameBlock;
 *   - index block:
 *       INT Bind.
 * RETURNS: None.
 */
static VOID TNG_ShaderAddComponent( UINT PrgNo, CHAR *Str, CHAR *NameBlock, INT Bind )
{
  UINT IndexBlock, Indices[TNG_NUM_OF_COMP_AT_BLOCK], UboHandle;
  INT BlockSize, Offset[TNG_NUM_OF_COMP_AT_BLOCK], i, m;
  ARGS Args = Scanner(Str);
  BYTE *BlockBuffer;
  FLOAT Var[8];
  CHAR *names[TNG_NUM_OF_COMP_AT_BLOCK];

  for (i = 0; i < Args.NumOfScans; i++)
    if (strcmp(Args.Scan[i].Name, "Standart") == 0)
      TNG_ShaderAddComponentStandart(PrgNo);

  for (i = 0; i < Args.NumOfScans; i++)
  {
    names[i] = malloc(sizeof(CHAR) * 100);
    sprintf(names[i], "%s", Args.Scan[i].Name);
  }

  /* Find the uniform index buffer */
  IndexBlock = glGetUniformBlockIndex(PrgNo, NameBlock);
                            
  /* Determine block size  */
  glGetActiveUniformBlockiv(PrgNo, IndexBlock, GL_UNIFORM_BLOCK_DATA_SIZE, &BlockSize);
  BlockBuffer = malloc(BlockSize);

  if (BlockBuffer == NULL)
  {
    for (i = 0; i < Args.NumOfScans; i++)
      free(names[i]);
    return;
  }

  /* Get indexes values */
  glGetUniformIndices(PrgNo, Args.NumOfScans, names, Indices);

  /* Check for exist */
  for (i = 0; i < Args.NumOfScans; i++)
    if (Indices[i] == GL_INVALID_INDEX)
    {
      if (i + 1 != Args.NumOfScans && Indices[Args.NumOfScans - 1] != GL_INVALID_INDEX)
      {
        Indices[i] = Indices[Args.NumOfScans - 1];
        Args.Scan[i] = Args.Scan[Args.NumOfScans - 1];
      }
      i--;
      Args.NumOfScans--;
    }
  /* Get offsets values */
  glGetActiveUniformsiv(PrgNo, Args.NumOfScans, Indices, GL_UNIFORM_OFFSET, Offset);

  /* Copy the uniform values into the buffers */
  for (i = 0; i < Args.NumOfScans; i++)
  {
    for (m = 0; m < Args.Scan[i].NumOfComp; m++)
      Var[m] = Args.Scan[i].Var[m];
    memcpy(BlockBuffer + Offset[i], Var, Args.Scan[i].NumOfComp * sizeof(FLOAT));
  }
  /* Create object buffer */
  glGenBuffers(1, &UboHandle);
  glBindBuffer(GL_UNIFORM_BUFFER, UboHandle);
  glBufferData(GL_UNIFORM_BUFFER, BlockSize, BlockBuffer, GL_DYNAMIC_DRAW);

  /* Associated with shader */
  glBindBufferBase(GL_UNIFORM_BUFFER, Bind, UboHandle);
  for (i = 0; i < Args.NumOfScans; i++)
    free(names[i]);
  free(BlockBuffer);
} /* End of 'TNG_ShaderAddComponentConst' function */

/* Add component light block into shader function.
 * ARGUMENTS: 
 *   - shader program Id:
 *       UINT PrgNo;
 *   - values block:
 *       tngBLOCK Values;
 *   - index block:
 *       INT Bind.
 * RETURNS: None.
 */
static VOID TNG_ShaderAddComponentLight( UINT PrgNo, tngLIGHT *Light, INT Bind )
{
  UINT IndexBlock, Indices[6], UboHandle;
  INT BlockSize, Offset[6], i, NumOfComp = 6;
  BYTE *BlockBuffer;
  static CHAR *names[] = {"LightPos", "LightDir", "LightColor", "LightAtt", "CutOffAngle", "CutOffDist"};

  /* Find the uniform index buffer */
  IndexBlock = glGetUniformBlockIndex(PrgNo, "Lights");
                            
  /* Determine block size  */
  glGetActiveUniformBlockiv(PrgNo, IndexBlock, GL_UNIFORM_BLOCK_DATA_SIZE, &BlockSize);
  BlockBuffer = malloc(BlockSize);

  if (BlockBuffer == NULL)
    return;

  /* Get indexes values */
  glGetUniformIndices(PrgNo, NumOfComp, names, Indices);

  /* Check for exist */
  for (i = 0; i < NumOfComp; i++)
    if (Indices[i] == GL_INVALID_INDEX)
    {
      if (i + 1 != NumOfComp && Indices[NumOfComp - 1] != GL_INVALID_INDEX)
      {
        Indices[i] = Indices[NumOfComp - 1];
        names[i] = names[NumOfComp - 1];
      }
      i--;
      NumOfComp--;
    }
  /* Get offsets values */
  glGetActiveUniformsiv(PrgNo, NumOfComp, Indices, GL_UNIFORM_OFFSET, Offset);

  /* Copy the uniform values into the buffers */
  for (i = 0; i < NumOfComp; i++)
    if (strcmp(names[i], "LightPos") == 0)
      memcpy(BlockBuffer + Offset[i], &Light->Pos, sizeof(VEC));
    else if (strcmp(names[i], "LightDir") == 0)
      memcpy(BlockBuffer + Offset[i], &Light->Dir, sizeof(VEC));
    else if (strcmp(names[i], "LightColor") == 0)
      memcpy(BlockBuffer + Offset[i], &Light->Color, sizeof(VEC));
    else if (strcmp(names[i], "LightAtt") == 0)
      memcpy(BlockBuffer + Offset[i], &Light->Att, sizeof(VEC));
    else if (strcmp(names[i], "CutOffAngle") == 0)
      memcpy(BlockBuffer + Offset[i], &Light->CutOffAngle, sizeof(VEC2));
    else if (strcmp(names[i], "CutOffDist") == 0)
      memcpy(BlockBuffer + Offset[i], &Light->CutOffDist, sizeof(VEC2));

  /* Create object buffer */
  glGenBuffers(1, &UboHandle);
  glBindBuffer(GL_UNIFORM_BUFFER, UboHandle);
  glBufferData(GL_UNIFORM_BUFFER, BlockSize, BlockBuffer, GL_DYNAMIC_DRAW);

  /* Associated with shader */
  glBindBufferBase(GL_UNIFORM_BUFFER, Bind, UboHandle);
  free(BlockBuffer);
} /* End of 'TNG_ShaderAddComponentLight' function */


/* Shaders sources subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemShadersInit( VOID )
{
  StockInit(&TNG_Shaders, 0, sizeof(tngSHADER), TNG_ShaderDelete);

  TNG()->ShaderAdd = TNG_ShaderAdd;
  TNG()->ShadersUpdate = TNG_ShadersUpdate;
  TNG()->ShaderDelete = TNG_ShaderDelete;
  TNG()->ShaderApply = TNG_ShaderApply;
  TNG()->ShaderAddComponent = TNG_ShaderAddComponent;
  TNG()->ShaderAddComponentMatrix = TNG_ShaderAddComponentMatrix;
  TNG()->ShaderAddComponentLight = TNG_ShaderAddComponentLight;

  TNG_ShaderDefault = TNG()->ShaderAdd("Default");
} /* End of 'TNG_SubsystemShadersInit' funciton */                           

/* Shader sources subsystem destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemShadersDestructor( VOID )
{
  StockClear(&TNG_Shaders);
} /* End of 'TNG_SubsystemShadersDestructor' funciton */

/* END OF 'tng_shaders.c' FILE */
