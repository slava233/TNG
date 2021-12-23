/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Draw light source shaders.
 *               Fragment shader.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 10.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 430
                      
/* Out color to screen directly */
layout(location = 0) out vec4 OutColor;

/* Color of objects to light */
layout(binding = 6) uniform sampler2D TexGlow;

/* Kd coefficient & IsIlluminated value of object */
layout(binding = 2) uniform sampler2D TexKdIs;

/* Normals of object */
layout(binding = 4) uniform sampler2D TexNormal;

/* Texture coordinate input parameters */
in vec2 DrawTexCoords;

/* Input color */
in vec4 DrawColor;


/* Main function.
 * ARGUMENTS: None.        
 * RETURNS: None.
 */
void main( void )
{
  ivec2 fc = ivec2(gl_FragCoord.xy);
  vec4 
    KdIs = texelFetch(TexKdIs, fc, 0),     /* Kd coefficents & IsIlluminated flag */
    Normal =  texelFetch(TexNormal, fc, 0);  /* Normal of object */

  vec4 
    tc = texture(TexGlow, DrawTexCoords);   

  /* Check is illuminated coefficients */
  //if (KdIs.a == 1)
  if (KdIs.a == 0 || gl_FragCoord.z < Normal.a)
  {
    OutColor = vec4(DrawColor.rgb * tc.rgb, DrawColor.a);
  }
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
