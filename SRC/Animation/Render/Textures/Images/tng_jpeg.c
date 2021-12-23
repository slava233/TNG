/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_jpeg.c
 * PURPOSE     : Tough Nature Graphics project.
 *               JPEG handle module.
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
#include <setjmp.h>

#include "tng_images.h"
#include "jpeglib.h"

#pragma comment(lib, "SRC/Animation/Render/Textures/Images/jpeglib.lib")

#include "jpghead/jhead.h"

/* Reference to Exif handle function */
int main_JPEG( int argc, char **argv, ImageInfo_t *Exif );

/* Error message structure */
struct my_error_mgr 
{
  struct jpeg_error_mgr pub; /* "public" fields */

  jmp_buf setjmp_buffer;     /* for return to caller */
};

/* Error handle */
typedef struct my_error_mgr * my_error_ptr;

/* Error handle function.
 * ARGUMENTS:
 *   - jpeg common info pointer
 *       j_common_ptr cinfo;
 * RETURNS: None.
 */
static VOID my_error_exit( j_common_ptr cinfo )
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
} /* End of 'my_error_exit' function */

/* Load image JPG format function.
 * ARGUMENTS:
 *   - picture data pointer:
 *       tngIMAGE *P;
 *   - new picture name:
 *       CHAR *FileName;   
 * RETURNS:
 *   (BOOL) TRUE for success, FALSE if no memory, or bad file.
 */
BOOL TNG_ImageLoadJPG( tngIMAGE *P, CHAR *FileName )
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE *F;
  INT row_stride, i, Rotate = 0, W, H;
  JSAMPARRAY buffer;
  ImageInfo_t Exif;
  CHAR *Args[] =
  {
    "TNG.EXE",
    "-nofinfo",
    /* "-q", */
    "-nofinfo",
    FileName
  };

  if (main_JPEG(sizeof(Args) / sizeof(Args[0]), Args, &Exif) == 0)
    Rotate = Exif.Orientation;
  else
    return FALSE;

  if ((F = fopen(FileName, "rb")) == NULL) 
    return FALSE;
  
  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) 
  {
    /* If we have error */
    jpeg_destroy_decompress(&cinfo);
    fclose(F);
    return FALSE;
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Specify data source (eg, a file) */
  jpeg_stdio_src(&cinfo, F);

  /* Read file parameters with jpeg_read_header() */
  jpeg_read_header(&cinfo, TRUE);

  /* Start decompressor */
  jpeg_start_decompress(&cinfo);
  
  if (Rotate == 6 || Rotate == 8)
  {
    W = cinfo.output_height;
    H = cinfo.output_width;
  }
  else
  {
    W = cinfo.output_width;
    H = cinfo.output_height;
  }

  if (!TNG()->ImageCreate(P, W, H))
  {
    jpeg_destroy_decompress(&cinfo);
    fclose(F);
    return FALSE;
  }
  row_stride = cinfo.output_width * cinfo.output_components;
 
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  
  while (cinfo.output_scanline < cinfo.output_height) 
  {
    jpeg_read_scanlines(&cinfo, buffer, 1);

    for (i = 0; i < (INT)cinfo.output_width; i++)
    {
      INT
        R = buffer[0][i * 3],
        G = buffer[0][i * 3 + 1],
        B = buffer[0][i * 3 + 2];

      if (Rotate == 6)
        P->Rows[i][cinfo.output_scanline - 1] = COM_MAKELONG0123(B, G, R, 0xFF);
      else if (Rotate == 8)
        P->Rows[H - 1 - i][cinfo.output_scanline - 1] = COM_MAKELONG0123(B, G, R, 0xFF);
      else if (Rotate == 3)
        P->Rows[H - 1 - (cinfo.output_scanline - 1)][W - 1 -i] = COM_MAKELONG0123(B, G, R, 0xFF);
      else
        P->Rows[cinfo.output_scanline - 1][i] = COM_MAKELONG0123(B, G, R, 0xFF);
    }
  }

  /* Finish decompression */
  jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);
  fclose(F);

  return TRUE;
} /* End of 'TNG_ImageLoadJPG' function*/

/* Save image JPG format function.
 * ARGUMENTS:
 *   - picture data pointer:
 *       tngIMAGE *P;
 *   - new picture name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE for success, FALSE if no memory, or bad file.
 */
BOOL TNG_ImageSaveJPG( tngIMAGE *P, CHAR *FileName )
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *F;
  JSAMPARRAY row_pointer;
  INT row_stride;
  INT i;

  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Specify data destination (eg, a file) */

  /* Open file */
  if ((F = fopen(FileName, "wb")) == NULL) 
  {
    jpeg_destroy_compress(&cinfo);
    return FALSE;
  }
  jpeg_stdio_dest(&cinfo, F);

  /* Set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  cinfo.image_width = P->W;        /* image width and height, in pixels */
  cinfo.image_height = P->H;
  cinfo.input_components = 3;      /* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB;  /* colorspace of input image */

  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, 100, TRUE);

  /* Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* while (scan lines remain to be written) */
  /*   jpeg_write_scanlines(...); */

  row_stride = P->W * 3;  /* JSAMPLEs per row in image_buffer */
  row_pointer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  while (cinfo.next_scanline < cinfo.image_height) 
  { 
    for (i = 0; i < P->W; i++)
    {
      INT
        R = P->Rows[cinfo.next_scanline][i] & 0xFF,
        G = (P->Rows[cinfo.next_scanline][i] >> 8) & 0xFF,
        B = (P->Rows[cinfo.next_scanline][i] >> 16) & 0xFF;

      row_pointer[0][i * 3] = B;
      row_pointer[0][i * 3 + 1] = G;
      row_pointer[0][i * 3 + 2] = R;
    }
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Finish compression */
  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(F);

  /* Release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  return TRUE;
} /* End of 'TNG_ImageSaveJPG' function */

/* END OF 'JPEG.C' FILE */
