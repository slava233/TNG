/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Vert default shader.
 * PROGRAMMER  : CGSG'2018.
 *               Girvits Alexander.
 * LAST UPDATE : 17.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420


layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
layout(location = 4) in vec3 InT;
layout(location = 5) in vec3 InB;

/*
layout(binding = 7) uniform Standart
{
  vec3 CamLoc;
  float Time;
};
*/

uniform vec3 CamLoc;
uniform float Time;

uniform mat4 MatrWVP;
uniform mat4 MatrW;
uniform mat4 MatrN;
/*
uniform float Time;
*/                      	
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPos;
out vec2 DrawTexCoord;
out vec3 DrawT;
out vec3 DrawB;

/* Main funciton.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);  
  DrawColor = InColor;
  DrawNormal = mat3(MatrN) * InNormal;
  DrawT = mat3(MatrN) * InT;
  DrawB = mat3(MatrN) * InB;
  DrawPos = (MatrW * vec4(InPosition, 1)).xyz;
  DrawTexCoord = InTexCoord;
} /* End of 'main' funciton */

/* END OF 'vert.glsl' FILE */
