/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_font.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Fonts support implementation module.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <tng.h>

#include <stdio.h>
#include <string.h>

/***
 * BMF Font handle types
 ***/

/* Common info: 
 *   Sinature (4 bytes): "BMF\3".
 * Set of blocks: 1 byte 'block Id (1-5) plus 4 bytes block size in bytes.
 */

#pragma pack(push, 1)

/* Block type 1: info.
* This structure gives the layout of the fields. Remember that there should
 * be no padding between members. Allocate the size of the block using the
 * blockSize, as following the block comes the font name, including the
 * terminating null char. Most of the time this block can simply be ignored. */
typedef struct tagtngFNTINFO
{
  WORD FontSize;  /* Base letter height value */
  BYTE BitField;  /* Info bits -
                   *   bit 0: smooth - set to 1 if smoothing was turned on,
                   *   bit 1: unicode - set to 1 if it is the unicode charset,
                   *   bit 2: italic - the font is italic,
                   *   bit 3: bold - the font is bold,
                   *   bit 4: fixedHeigth,
                   *   bits 5-7: reserved. */
  BYTE CharSet;   /* The name of the OEM charset used (when not unicode, e.g. RUSSIAN_CHARSET - 204). */
  WORD StretchH;  /* The font height stretch in percentage. 100% means no stretch. */
  BYTE AntiAlias; /* The supersampling level used. 1 means no supersampling was used. */
  BYTE            /* The padding for each character (up, right, down, left). */
    PaddingUp, PaddingRight, PaddingDown, PaddingLeft;
  BYTE            /* The spacing for each character (horizontal, vertical). */
    SpacingHoriz, SpacingVert;
  BYTE Outline;   /* The outline thickness for the characters in pixels */
  /* CHAR FontName[]; - String - null terminated string */
} tngFNTINFO; /* End of 'tngFNTINFO' structure */

/* Block type 2: common */
typedef struct tagtngFNTCOMMON
{
  WORD LineHeight; /* This is the distance in pixels between each line of text. */
  WORD Base;       /* The number of pixels from the absolute top of the line to the base of the characters. */
  WORD ScaleW;     /* The width of the texture, normally used to scale the x pos of the character image. */
  WORD ScaleH;     /* The height of the texture, normally used to scale the y pos of the character image. */
  WORD Pages;      /* The number of texture pages included in the font. */
  BYTE BitField;   /* Info bits - bits 0-6: reserved,
                    *    bit 7: packed - set to 1 if the monochrome characters have been packed into each
                    *                    of the texture channels. In this case alphaChnl describes what
                    *                    is stored in each channel. */
  BYTE
    /* By channels info:
     *   set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds
     *   the glyph and the outline, 3 if its set to zero, and 4 if its set to one. */
    AlphaChnll, RedChnl, GreenChnl, BlueChnl;
} tngFNTCOMMON; /* End of 'tngFNTCOMMON' structure */

/* Block type 3: pages
 * This block gives the name of each texture file with the image 
 * data for the characters. The string pageNames holds the names
 * separated and terminated by null chars. Each filename has the
 * same length, so once you know the size of the first name, you
 * can easily determine the position of each of the names. The id
 * of each page is the zero-based index of the string name. */
typedef struct tagtngFNTPAGES
{
  BYTE Dummy;
  /* CHAR pageNames[]; - Strings - null terminated strings */
} tngFNTPAGES; /* End of 'tngFNTPAGES' structure */

/* Block type 4: chars
 * The number of characters in the file can be computed by taking the size
 * of the block and dividing with the size of the charInfo structure, i.e.:
 *   numChars = charsBlock.blockSize/20. */
typedef struct tagtngFNTCHARS
{
  DWORD Id;       /* The character id (character code). */
  WORD X;         /* The left position of the character image in the texture. */
  WORD Y;         /* The top position of the character image in the texture. */
  WORD Width;     /* The width of the character image in the texture. */
  WORD Height;    /* The height of the character image in the texture. */
  SWORD XOffset;  /* How much the current position should be offset when copying the image from the texture to the screen. */
  SWORD YOffset;  /* How much the current position should be offset when copying the image from the texture to the screen. */
  SWORD XAdvance; /* How much the current position should be advanced after drawing the character. */
  BYTE Page;      /* The texture page where the character image is found. */
  BYTE Chnl;      /* The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels). */
} tngFNTCHARS; /* End of 'tngFNTCHARS' structure */

/* Block type 5: kerning pairs
 * This block is only in the file if there are any kerning
 * pairs with amount differing from 0. */
typedef struct tagtngFNTKERNING_PAIRS
{
  DWORD First;  /* The first character id. */
  DWORD Second; /* The second character id. */
  WORD Amount;  /* How much the x position should be adjusted when drawing the second character immediately following the first. */
} tngFNTKERNING_PAIRS; /* End of 'tngFNTKERNING_PAIRS' structure */


#pragma pack(pop)

/* Font representation type */
typedef struct tagtngFONT_DESCR
{
  /* Texture file name */
  CHAR TextureFileName[TNG_NAME_SIZE];
  /* Texture reference */
  tngTEXTURE *Tex;
  /* Material reference */
  tngMATERIAL *Mtl;

  /* Font description */
  CHAR FontName[TNG_NAME_SIZE]; /* Font face name */
  tngFNTINFO Info;        /* Font information */
  tngFNTCOMMON Common;    /* Font common information */
  tngFNTCHARS Chars[256]; /* Every character information */
} tngFONT_DESCR;

/***
 * Fonts storage data
 ***/

/* Default font description */
tngFONT_DESCR TNG_FntDefault;

/* Font load function.
 * ARGUMENTS:
 *   - font structure pointer:
 *       tngFONT_DESCR *Fnt;
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TNG_FntLoad( tngFONT_DESCR *Fnt, CHAR *FileName )
{
  DWORD Sign;
  FILE *F;
  tngMATERIAL mtl = {{0}};
  CHAR
    Path[_MAX_PATH],
    Path1[_MAX_PATH],
    Drive[_MAX_DRIVE],
    Dir[_MAX_DIR],
    FName[_MAX_FNAME],
    FExt[_MAX_EXT],
    Drive1[_MAX_DRIVE],
    Dir1[_MAX_DIR],
    FName1[_MAX_FNAME],
    FExt1[_MAX_EXT];
  static CHAR font_mtl_prefix[] = "Font material: ";

  memset(Fnt, 0, sizeof(tngFONT_DESCR));
  strcpy(Path1, "BIN/FONTS/");
  strncat(Path1, FileName, _MAX_PATH - 12);
  if ((F = fopen(Path1, "rb")) == NULL)
    return FALSE;

  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"BMF\x03")
  {
    fclose(F);
    return FALSE;
  }
  while (TRUE)
  {
    INT i, ch;
    BYTE BlkId;
    DWORD BlkLen, pos;

    if (fread(&BlkId, 1, 1, F) != 1 ||
        fread(&BlkLen, 4, 1, F) != 1)
      break;
    pos = ftell(F);

    switch (BlkId)
    {
    case 1:
      fread(&Fnt->Info, sizeof(tngFNTINFO), 1, F);
      i = 0;
      while ((ch = fgetc(F)) != EOF && ch != 0)
        if (i < TNG_NAME_SIZE - 1)
          Fnt->FontName[i++] = ch;
      Fnt->FontName[i] = 0;
      break;
    case 2:
      fread(&Fnt->Common, sizeof(tngFNTCOMMON), 1, F);
      break;
    case 3:
      i = 0;
      while ((ch = fgetc(F)) != EOF && ch != 0)
        if (i < TNG_NAME_SIZE - 1)
          Fnt->TextureFileName[i++] = ch;
      Fnt->TextureFileName[i] = 0;
      _splitpath(Path1, Drive, Dir, FName, FExt);
      _splitpath(Fnt->TextureFileName, Drive1, Dir1, FName1, FExt1);
      _makepath(Path, Drive, Dir, FName1, FExt1); /* ".BMP"); */
      Fnt->Tex = TNG()->TextureAddFromFile(Path);
      break;
    case 4:
      for (i = 0; i < BlkLen / sizeof(tngFNTCHARS); i++)
      {
        tngFNTCHARS c;

        fread(&c, sizeof(tngFNTCHARS), 1, F);
        if (c.Id < 256)
          Fnt->Chars[c.Id] = c;
      }
      break;
    }

    fseek(F, pos + BlkLen, SEEK_SET);
  }
  strncpy(mtl.Name, font_mtl_prefix, TNG_NAME_SIZE - 1);
  strncat(mtl.Name, Fnt->FontName, TNG_NAME_SIZE - 1 - sizeof(font_mtl_prefix));
  mtl.Shd = TNG()->ShaderAdd("Fonts");
  mtl.Tex[0] = Fnt->Tex;
  Fnt->Mtl = TNG()->MaterialAddStruct(&mtl);

  fclose(F);
  return TRUE;
} /* End of 'TNG_FntLoad' function */

/* Build primitive from string with specified font function.
 * ARGUMENTS:
 *   - font structure pointer:
 *       vg4FONT *Fnt;
 *   - string to be converted:
 *       CHAR *Str;
 *   - primitive color:
 *       VEC4 Color;
 *   - font letter size:
 *       FLT FontSize;
 * RETURNS: None.
 *   (tngPRIM *) builded primitive pointer.
 */
tngPRIM * TNG_FntStringToPrim( tngFONT_DESCR *Fnt, CHAR *Str, VEC4 Color, FLT FontSize )
{
  INT i, len, size, noofv = 0;
  FLT
    /* Interline distance */
    h_size = (FLT)Fnt->Common.LineHeight / (FLT)Fnt->Common.ScaleH,
    /* Base line distance */
    base = (FLT)Fnt->Common.Base / (FLT)Fnt->Common.ScaleH,
    /* Character size */
    c_size = FontSize / base,
    /* Output position */
    posx = 0, posy = 0;
  struct
  {
    VEC P;  /* Vertex position */
    VEC2 T; /* Vertex texture coordinates */
    VEC4 C; /* Vertex color */
  } *V, *vptr;
  INT *Ind;
  tngFNTCHARS *cd;
  tngPRIM *Pr;

  /* Allocate memory for primitive data */
  len = strlen(Str);
  size = sizeof(V[0]) * 4 * len + sizeof(INT) * 5 * len;
  if ((V = malloc(size)) == NULL)
    return NULL;
  Ind = (INT *)(V + len * 4);

  /* Setup default vertex values */
  for (i = 0; i < len * 4; i++)
    V[i].C = Color;

  vptr = V;
  while (*Str != 0)
  {
    if (*Str == '\n')
      posx = 0, posy -= base * c_size;
    else
      if ((cd = &Fnt->Chars[(BYTE)*Str])->Id != 0)
      {
        FLT
          tx = (FLT)cd->X / (FLT)Fnt->Common.ScaleW,
          ty = (FLT)cd->Y / (FLT)Fnt->Common.ScaleH,
          tdx = (FLT)cd->Width / (FLT)Fnt->Common.ScaleW,
          tdy = (FLT)cd->Height / (FLT)Fnt->Common.ScaleH,
          xoff = (FLT)cd->XOffset / (FLT)Fnt->Common.ScaleW,
          yoff = (FLT)cd->YOffset / (FLT)Fnt->Common.ScaleH,
          xadv = (FLT)cd->XAdvance / (FLT)Fnt->Common.ScaleW;

        /* left-top */
        vptr->P = VecSet(posx + xoff * c_size, posy - yoff * c_size, 0);
        vptr++->T = Vec2Set(tx, 1 - ty);

        /* left-bottom */
        vptr->P = VecSet(posx + xoff * c_size, posy - (yoff + tdy) * c_size, 0);
        vptr++->T = Vec2Set(tx, 1 - (ty + tdy));

        /* right-top */
        vptr->P = VecSet(posx + (xoff + tdx) * c_size, posy - yoff * c_size, 0);
        vptr++->T = Vec2Set(tx + tdx, 1 - ty);

        /* right-bottom */
        vptr->P = VecSet(posx + (xoff + tdx) * c_size, posy - (yoff + tdy) * c_size, 0);
        vptr++->T = Vec2Set(tx + tdx, 1 - (ty + tdy));

        posx += xadv * c_size;
        noofv++;
      }
    Str++;
  }
  /* Fill index array */
  for (i = 0; i < noofv; i++)
  {
    Ind[i * 5 + 0] = i * 4 + 0;
    Ind[i * 5 + 1] = i * 4 + 1;
    Ind[i * 5 + 2] = i * 4 + 2;
    Ind[i * 5 + 3] = i * 4 + 3;
    Ind[i * 5 + 4] = -1;
  }
  Pr = TNG()->PrimCreate(TNG_PRIM_STRIP, "f3f2f4", V, noofv * 4, Ind, noofv * 5 - 1);
  free(V);
  Pr->Mtl = Fnt->Mtl;
  return Pr;
} /* End of 'TNG_FntStringToPrim' function */

/* Fonts support init function. 
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID TNG_SubsystemFontsInit( VOID )
{
  TNG()->FntStringToPrim = (VOID *)TNG_FntStringToPrim;
  TNG()->FntDefault = &TNG_FntDefault;
  TNG_FntLoad(&TNG_FntDefault, "Fnt_Bookman.fnt");
} /* End of 'TNG_SubsystemFontsInit' function */

/* Fonts support destruct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemFontsDestructor( VOID )
{
} /* End of 'TNG_SubsystemFontsDestructor' function */

/* END OF 'tng_font.c' FILE */
