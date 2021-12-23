/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_images.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Images handle module.
 * PROGRAMMER  : CGSG'2018.
 *               Andrey Shayda.
 * LAST UPDATE : 26.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "tng_images.h"
#include "png.h"

#define TNG_CHECK_BYTES 4

#pragma pack(push, 1)
#include <tgahead.h>
#pragma pack(pop)

#pragma comment(lib, "SRC/Animation/Render/Textures/Images/libpng")

/* Load image JPG format function prototype (from jpeglib).
 * ARGUMENTS:
 *   - picture data pointer:
 *       tngIMAGE *P;
 *   - new picture name:
 *       CHAR *FileName;   
 * RETURNS:
 *   (BOOL) TRUE for success, FALSE if no memory, or bad file.
 */
BOOL TNG_ImageLoadJPG( tngIMAGE *P, CHAR *FileName );

/* Save image JPG format function prototype (from jpeglib).
 * ARGUMENTS:
 *   - picture data pointer:
 *       tngIMAGE *P;
 *   - new picture name:
 *       CHAR *FileName;   
 * RETURNS:
 *   (BOOL) TRUE for success, FALSE if no memory, or bad file.
 */
BOOL TNG_ImageSaveJPG( tngIMAGE *P, CHAR *FileName );

/* Image create functions.
 * ARGUMENTS:
 *   - image to be created:
 *       tngIMAGE *Im;
 *   - image size:
 *       INT W, H;
 * RETURNS: None.
 */
static BOOL TNG_ImageCreate( tngIMAGE *Im, INT W, INT H )
{
  INT i;

  memset(Im, 0, sizeof(tngIMAGE));
  if ((Im->Pixels = malloc(W * H * sizeof(DWORD) + sizeof(DWORD *) * H)) == NULL)
    return FALSE;
  memset(Im->Pixels, 0, W * H * sizeof(DWORD) + sizeof(DWORD *) * H);
  Im->Rows = (DWORD **)(Im->Pixels + W * H);
  Im->RowsC = (BYTE (**)[4])Im->Rows;

  /* Setup row pointers */
  for (i = 0; i < H; i++)
    Im->Rows[i] = Im->Pixels + W * i;
  Im->W = W;
  Im->H = H;
  return TRUE;
} /* End of 'TNG_ImageCreate' function */

/* Get pixel value function.
 * ARGUMENTS:
 *   - image structure pointer:
 *       tngIMAGE *Im;
 *   - coordinates of pixel:
 *       INT X, Y;
 * RETURNS: 
 *   (VEC4) Vector of color.
 */
static VEC4 TNG_ImageGet( tngIMAGE *Im, INT X, INT Y )
{
  VEC4 V = {0};

  /* Color reading */
  if (X >= 0 && Y >= 0 && X < Im->W && Y < Im->H)
  {
    V.X = ((Im->Rows[Y][X] >> 24) & 0xFF) / 255.0;
    V.Y = ((Im->Rows[Y][X] >> 16) & 0xFF) / 255.0;
    V.Z = ((Im->Rows[Y][X] >> 8) & 0xFF) / 255.0;
    V.W = (Im->Rows[Y][X] & 0xFF) / 255.0;
  }

  return V;
} /* End of 'TNG_ImageGet' function */

/* Flip image vertical function.
 * ARGUMENTS:
 *   - image which must be flip:
 *       tngIMAGE *Im;
 * RETURNS: None.
 */
static VOID TNG_ImageFlipVertical( tngIMAGE *Im )
{
  INT x, y, w = Im->W, h = Im->H, r;
  DWORD **ptr = Im->Rows;

  for (r = 0, y = h - 1; y >= h / 2; y--, r++)
    for (x = 0; x < w; x++)
    {
      DWORD tmp = ptr[y][x];

      ptr[y][x] = ptr[r][x];
      ptr[r][x] = tmp;
    }
} /* End of 'TNG_ImageFlipVertical' function */

/* Load image from *.G24 file function.
 * ARGUMENTS:
 *   - image to be load:
 *       tngIMAGE *Im;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TNG_ImageLoadG24( tngIMAGE *Im, CHAR *FileName )
{
  INT w = 0, h = 0, x, y, flen, bpp;
  FILE *F;
  BYTE *mem = NULL;

  memset(Im, 0, sizeof(tngIMAGE));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  fread(&w, 2, 1, F);
  fread(&h, 2, 1, F);
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  fseek(F, 4, SEEK_SET);

  /* Check for GSM/G24/G32 file */
  if (flen == 4 + w * h)
    /* Case of GSM file */
    bpp = 1;
  else if (flen == 4 + 3 * w * h)
    /* Case of G24 file */
    bpp = 3;
  else if (flen == 4 + 4 * w * h)
    /* Case of G32 file */
    bpp = 4;
  else
  {
    fclose(F);
    return FALSE;
  }

  /* Allocate memory for one row and create empty image */
  if ((mem = malloc(bpp * w)) == NULL || !TNG()->ImageCreate(Im, w, h))
  {
    if (mem != NULL)
      free(mem);
    fclose(F);
    return FALSE;
  }

  /* Read every image row */
  for (y = 0; y < h; y++)
  {
    fread(mem, bpp, w, F);
    for (x = 0; x < w; x++)
      switch (bpp)
      {
      case 1:
        Im->RowsC[y][x][0] = mem[x];
        Im->RowsC[y][x][1] = mem[x];
        Im->RowsC[y][x][2] = mem[x];
        Im->RowsC[y][x][3] = 0xFF;
        break;
      case 3:
        Im->RowsC[y][x][0] = mem[x * 3 + 0];
        Im->RowsC[y][x][1] = mem[x * 3 + 1];
        Im->RowsC[y][x][2] = mem[x * 3 + 2];
        Im->RowsC[y][x][3] = 0xFF;
        break;
      case 4:
        Im->RowsC[y][x][0] = mem[x * 4 + 0];
        Im->RowsC[y][x][1] = mem[x * 4 + 1];
        Im->RowsC[y][x][2] = mem[x * 4 + 2];
        Im->RowsC[y][x][3] = mem[x * 4 + 3];
        break;
      }
  }
  free(mem);
  fclose(F);
  return TRUE;
} /* End of 'TNG_ImageLoadG24' function */

/* Load image from *.BMP file function.
 * ARGUMENTS:
 *   - image to be load:
 *       - tngIMAGE *Im;
 *    - image file name:
 *         CHAR *FileName;
 * RETURN:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TNG_ImageLoadBMP( tngIMAGE *Im, CHAR *FileName )
{
  HBITMAP hBm;

  memset(Im, 0, sizeof(tngIMAGE));
  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) != NULL)
  {
    INT i;
    DWORD *Bits;
    BITMAP bm;
    BITMAPINFOHEADER bih = {0};
    HBITMAP hDib;
    HDC hDC, hMemDC1, hMemDC2;

    hDC = GetDC(NULL);
    hMemDC1 = CreateCompatibleDC(hDC);
    hMemDC2 = CreateCompatibleDC(hDC);
    ReleaseDC(NULL, hDC);

    GetObject(hBm, sizeof(bm), &bm);

    /* Make DIB in memory */
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biBitCount = 32;
    bih.biWidth = bm.bmWidth;
    bih.biHeight = -bm.bmHeight;
    bih.biPlanes = 1;

    hDib = CreateDIBSection(NULL, (BITMAPINFO *)&bih, DIB_RGB_COLORS, &Bits, NULL, 0);

    SelectObject(hMemDC1, hBm);
    SelectObject(hMemDC2, hDib);
    BitBlt(hMemDC2, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCCOPY);

    if (!TNG_ImageCreate(Im, bm.bmWidth, bm.bmHeight))
    {
      DeleteObject(hDib);
      DeleteObject(hBm);
      DeleteDC(hMemDC1);
      DeleteDC(hMemDC2);
      return FALSE;
    }

    /* Correct alpha-channel */
    for (i = 0; i < bm.bmWidth * bm.bmHeight; i++)
      Im->Pixels[i] = Bits[i] | 0xFF000000;

    DeleteObject(hDib);
    DeleteObject(hBm);
    DeleteDC(hMemDC1);
    DeleteDC(hMemDC2);
    return TRUE;
  } 
  return FALSE;
} /* End of 'TNG_ImageLoadBMP' function */

/* TGA images load function
 * ARGUMENTS:
 *   - images to be load:
 *       tngIMAGE *Im;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE - all loaded right, FALSE - otherwise.
 */
static BOOL TNG_ImageLoadTGA( tngIMAGE *Im, CHAR *FileName )
{
  FILE *F;
  
  tgaFILEHEADER fh = {0};
  tgaFILEFOOTER ff = {0};

  /* Free memory */
  memset(Im, 0, sizeof(tngIMAGE));

  /* Check file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Read file header and footer */
  fread(&fh, sizeof(tgaFILEHEADER), 1, F);
  
  fseek(F, -(INT)sizeof(tgaFILEFOOTER), SEEK_END);
  fread(&ff, sizeof(tgaFILEFOOTER), 1, F);
  fseek(F, sizeof(tgaFILEHEADER), SEEK_SET);

  /* Check for supported image type and create empty image */
  if ((fh.BitsPerPixel != 24 && fh.BitsPerPixel != 32) ||
      fh.ColorMapType != 0 ||
      (fh.ImageDescr & 0xC0) != 0 ||
      !(fh.ImageType == 2 || fh.ImageType == 10) ||
      !TNG_ImageCreate(Im, fh.Width, fh.Height))
  {
    fclose(F);
    return FALSE;
  }

  /* Skip id string */
  fseek(F, fh.IDLength, SEEK_CUR);

  if (fh.ImageType == 2)
  {
    /* Uncompressed image */
    INT x, y, BytesPerPixel = fh.BitsPerPixel == 32 ? 4 : 3;

    /* Read every pixel */
    for (y = 0; y < fh.Height; y++)
      for (x = 0; x < fh.Width; x++)
      {
        DWORD color = 0xFF000000;

        fread(&color, BytesPerPixel, 1, F);
        Im->Rows[(fh.ImageDescr & 0x20) ? y : fh.Height - 1 - y][x] = color;
      }
  }
  else
  {
    /* RLE compresswed image */
    INT x = 0, y = 0, BytesPerPixel = fh.BitsPerPixel == 32 ? 4 : 3, n;
    BYTE rep;

    /* Read pixel stream */
    while (y < Im->H)
    {
      fread(&rep, 1, 1, F);
      n = (rep & 0x7F) + 1;
      if (rep & 0x80)
      {
        DWORD color = 0xFF000000;

        fread(&color, BytesPerPixel, 1, F);
        while (n-- > 0)
        {
          Im->Rows[(fh.ImageDescr & 0x20) ? y : fh.Height - 1 - y][x++] = color;
          if (x >= fh.Width)
            x = 0, y++;
        }
      }
      else
        while (n-- > 0)
        {
          DWORD color = 0xFF000000;

          fread(&color, BytesPerPixel, 1, F);
          Im->Rows[(fh.ImageDescr & 0x20) ? y : fh.Height - 1 - y][x++] = color;
          if (x >= fh.Width)
            x = 0, y++;
        }
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'TNG_ImageLoadTGA' function */

/* Check PNG file function
 * ARGUMENTS:
 *   - image name:
 *       CHAR *FileName;
 * RETURNS:
 *   BOOL (TRUE - all compile, FALSE - compile error);
 */
static BOOL TNG_ImageCheckPNG( CHAR *FileName )
{
  FILE *F;
  CHAR Help[TNG_CHECK_BYTES];
  INT check;

  /* Check file */
  if (FileName == NULL || (F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  /* Check bytes on file */
  if ((check = fread(Help, 1, TNG_CHECK_BYTES, F)) != TNG_CHECK_BYTES)
  {
    fclose(F);
    return FALSE;
  }
  fclose(F);
  return !png_sig_cmp(Help, 0, TNG_CHECK_BYTES);
} /* End of 'TNG_ImageCheckPNG' function */

/* PNG file load function
 * ARGUMENTS:
 *   - Image to save
 *       tngIMAGE *Im;
 *   - image name:
 *       CHAR *FileName;
 * RETURNS:
 *   BOOL (TRUE - all compile, FALSE - compile error);
 */
static BOOL TNG_ImageLoadPNG( tngIMAGE *Im, CHAR *FileName )
{
  png_image Img;

  if (TNG_ImageCheckPNG(FileName) == 0)
    return FALSE;

  memset(&Img, 0, sizeof(Img));
  Img.version = PNG_IMAGE_VERSION;
  if (png_image_begin_read_from_file(&Img, FileName))
  {
    Img.format = PNG_FORMAT_BGRA;
    if (!TNG()->ImageCreate(Im, Img.width, Img.height))
    {
      free(&Img);
      return FALSE;
    } 
    if (png_image_finish_read(&Img, NULL, Im->Pixels, 0, NULL))
    {
      TNG()->ImageFlipVertical(Im);
      return TRUE;
    }
    else
      TNG()->ImageFree(Im);
    return FALSE;
  }
  return FALSE;
} /* End of 'TNG_ImageLoadPNG' function */

/* Save image in *.PNG file function
 * ARGUMENTS:
 *   - image to be save:
 *       tngIMAGE Im;
 *   - Name of file:
 *       CHAR *FileName;
 * RETURNS:
 *   BOOL (TRUE - all save right, FALSE - save error);
 */
static BOOL TNG_ImageSavePNG( tngIMAGE *Im, CHAR *FileName )
{
  png_structp png_ptr;
  png_infop pif;
  FILE *F;

  if ((F = fopen(FileName, "wb")) == NULL)
    return FALSE;
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if ((pif = png_create_info_struct(png_ptr)) == NULL)
  {
    png_destroy_write_struct(&png_ptr, NULL);
    fclose(F);
    return FALSE;
  }

  png_set_IHDR(png_ptr, pif,
               Im->W, Im->H, /* width and height */
               8,           /* bit depth */
               PNG_COLOR_TYPE_RGB_ALPHA, /* color type */
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  png_set_bgr(png_ptr);
  png_init_io(png_ptr, F);
  png_set_rows(png_ptr, pif, (png_bytepp)Im->Rows);
  png_write_png(png_ptr, pif, PNG_TRANSFORM_IDENTITY, NULL);
  png_write_end(png_ptr, pif);
  png_destroy_write_struct(&png_ptr, &pif);
  fclose(F);
  return TRUE;
} /* End of 'TNG_ImageSavePNG' function */

/* Image load function.
 * ARGUMENTS:
 *   - image to be load:
 *       tngIMAGE *Im;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL TNG_ImageLoad( tngIMAGE *Im, CHAR *FileName )
{
  memset(Im, 0, sizeof(tngIMAGE));
  
  if (TNG_ImageLoadJPG(Im, FileName))
    return TRUE;
  if (TNG_ImageLoadTGA(Im, FileName))
    return TRUE;
  if (TNG_ImageLoadPNG(Im, FileName))
    return TRUE;
  if (TNG_ImageLoadBMP(Im, FileName))
    return TRUE;
  if (TNG_ImageLoadG24(Im, FileName))
    return TRUE;
  return FALSE;
} /* End of 'TNG_ImageLoad' function */

/* Save image in G24 file format function.
 * ARGUMENTS:
 *   - image to save:
 *       tngIMAGE *Im;
 *   - name of file:
 *       CHAR *FileName;
 * RETURNS: None.
 */
static VOID TNG_ImageSaveG24( tngIMAGE *Im, CHAR *FileName )
{
  INT x, y;
  FILE *F;
  BYTE *row;

  F = fopen(FileName, "wb");
  if (F == NULL)
    return;
  if ((row = malloc(3 * Im->W)) == NULL)
  {
    fclose(F);
    return;
  }
  fwrite(&Im->W, 2, 1, F);
  fwrite(&Im->H, 2, 1, F);
  /* Store image rows */
  for (y = 0; y < Im->H; y++)
  {
    /* Store to 'row' every row pixel color [BGR] */
    for (x = 0; x < Im->W; x++)
    {
      row[x * 3 + 0] = Im->Rows[y][x] & 0xFF;
      row[x * 3 + 1] = (Im->Rows[y][x] >> 8) & 0xFF;
      row[x * 3 + 2] = (Im->Rows[y][x] >> 16) & 0xFF;
    }
    /* Write row buffer to file */
    fwrite(row, 3, Im->W, F);
  }
  free(row);
  fclose(F);
} /* End of 'TNG_ImageSaveG24' function */

/* Save image in G32 file function.
 * ARGUMENTS:
 *   - image to be save:
 *       tngIMAGE *Im;
 *   - name of file:
 *       CHAR *FileName;
 * RETURNS: None.
 */
static VOID TNG_ImageSaveG32( tngIMAGE *Im, CHAR *FileName )
{
  FILE *F;

  F = fopen(FileName, "wb");
  if (F == NULL)
    return;
  fwrite(&Im->W, 2, 1, F);
  fwrite(&Im->H, 2, 1, F);
  fwrite(Im->Pixels, 4, Im->W * Im->H, F);
  fclose(F);
} /* End of 'TNG_ImageSaveG32' function */

/* Save image in BMP file.
 * ARGUMENTS:
 *   - image to save:
 *       tngIMAGE *Im;
 *   - name of file:
 *       CHAR *FileName;
 * RETURNS: None.
 */
VOID TNG_ImageSaveBMP( tngIMAGE *Im, CHAR *FileName )
{
  FILE *F;
  INT x, y, bpl;
  BYTE *row;
  BITMAPFILEHEADER bfh = {0};
  BITMAPINFOHEADER bmi = {0};

  F = fopen(FileName, "wb");
  if (F == NULL)
    return;

  /* Fill headers */
  bpl = ((Im->W * 3 + 3) >> 2) << 2;
  if ((row = malloc(bpl)) == NULL)
  {
    fclose(F);
    return;
  }
  memset(row, 0, bpl);

  bfh.bfType = 'B' | ('M' << 8);
  bfh.bfSize = sizeof(bfh) + sizeof(bmi) + Im->H * bpl;
  bfh.bfReserved1 = bfh.bfReserved2 = 0;
  bfh.bfOffBits = sizeof(bfh) + sizeof(bmi);

  bmi.biSize = sizeof(bmi);
  bmi.biWidth = Im->W;
  bmi.biHeight = Im->H;
  bmi.biBitCount = 24;
  bmi.biPlanes = 1;
  bmi.biSizeImage = Im->H * bpl;
  bmi.biCompression = BI_RGB;
  bmi.biXPelsPerMeter = bmi.biYPelsPerMeter = 1;
  bmi.biClrImportant = bmi.biClrUsed = 0;

  /* Store headers */
  fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, F);
  fwrite(&bmi, sizeof(BITMAPINFOHEADER), 1, F);

  /* Store image rows */
  for (y = 0; y < Im->H; y++)
  {
    /* Store to 'row' every row pixel color [BGR] */
    for (x = 0; x < Im->W; x++)
    {
      row[x * 3 + 0] = Im->Rows[y][x] & 0xFF;
      row[x * 3 + 1] = (Im->Rows[y][x] >> 8) & 0xFF;
      row[x * 3 + 2] = (Im->Rows[y][x] >> 16) & 0xFF;
    }
    /* Write row buffer to file */
    fwrite(row, 1, bpl, F);
  }
  free(row);
  fclose(F);
} /* End of 'TNG_ImageSaveBMP' function */

/* Image free function.
 * ARGUMENTS:
 *   - Image to be free up:
 *       tngIMAGE *Im;
 * RETURNS: None.
 */
static VOID TNG_ImageFree( tngIMAGE *Im )
{ 
  if (Im->Pixels == NULL)
    return;
  free(Im->Pixels);
} /* End of 'TNG_ImageFree' function */

/* Filled textures subsytem data */
static tngSUBSYSTEM_IMAGES SubsystemImages =
{
  TNG_ImageCreate,
  TNG_ImageGet,
  TNG_ImageFlipVertical,
  TNG_ImageLoad,
  TNG_ImageSaveG24,
  TNG_ImageSaveG32,
  TNG_ImageSaveBMP,
  TNG_ImageSavePNG,
  TNG_ImageSaveJPG,
  TNG_ImageFree
};

/* Images destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemImagesDestructor( VOID )
{

} /* End of 'TNG_SubsystemImagesDestructor' function */

/* Images subsystem init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID TNG_SubsystemImagesInit( VOID )
{
  TNG()->SubsystemImages = SubsystemImages;
} /* End of 'TNG_SubsystemImagesInit' function */

/* END OF 'tng_images.c' FILE */
