/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Vert font shader.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420


layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec4 InColor;

uniform mat4 MatrWVP;
uniform mat4 MatrN;
uniform mat4 MatrW;

uniform float Time;
                      	
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPos;
out vec2 DrawTexCoord;

/* Main funciton.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);  
  DrawColor = InColor;
  DrawNormal = mat3(MatrN) * vec3(0, 0, 1);
  DrawPos = (MatrW * vec4(InPosition, 1)).xyz;
  DrawTexCoord = InTexCoord;
} /* End of 'main' funciton */

/* END OF 'vert.glsl' FILE */
