/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Dust fragment shader.
 *               Particle system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Kopeykina Sofya.
 * LAST UPDATE : 26.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 430

layout(location = 0) out vec4 Color;

in vec2 DrawTexCoord;

uniform bool IsTexture0;
layout(binding = 0) uniform sampler2D TextureDust;

/* Main Function */
void main( void )
{
  if (IsTexture0)
    Color = ((vec4(1, 1, 1, 1) - vec4(texture(TextureDust, DrawTexCoord).rgb, 1)) / 10) * vec4(0.34, 0.56, 0.8, 1);
  else
    Color = vec4(1, 0, 1, 1);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
