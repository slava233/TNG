/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Ambient light source shaders.
 *               Fragment shader.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 17.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

/* Out color to screen directly */
layout(location = 0) out vec4 OutColor;

/* Ka coefficient & Trans value of object */ 
layout(binding = 1) uniform sampler2D TexKaTrans;
/* Kd coefficient & IsIlluminated value of object */
layout(binding = 2) uniform sampler2D TexKdIs;

/* Main function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  ivec2 fc = ivec2(gl_FragCoord.xy);
  vec4 
    KaTrans = texelFetch(TexKaTrans, fc, 0), /* Ka & Trans coefficients */            
    KdIs = texelFetch(TexKdIs, fc, 0);       /* Kd coefficents & IsIlluminated flag */
  
  if (KdIs.a == 1)
    OutColor = vec4(KaTrans.xyz / 2, 1);
  else
    discard;
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
