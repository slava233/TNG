/******`*********************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : geom.glsl
 * PURPOSE     : Dust geometry shader.
 *               Particle system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Sofya Kopeykina.
 * LAST UPDATE : 17.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 430

/* Format of in vertices */
layout(points) in;
/* Format and max_vertices of out vertices */
layout(triangle_strip, max_vertices = 4) out;

/* Camera right direction */
layout(location = 6) uniform vec3 CamRight;

/* Camera up direction */
layout(location = 7) uniform vec3 CamUp;


uniform mat4 MatrWVP;

out vec2 DrawTexCoord;

out vec3 Position;

/* Main function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
void main( void )
{
/* Setup rectangle on screen */
  DrawTexCoord = vec2(0, 1);
  gl_Position = MatrWVP * vec4(-CamRight + CamUp, 1);
  EmitVertex();

  DrawTexCoord = vec2(0, 0);
  gl_Position = MatrWVP * vec4(-CamRight - CamUp, 1);
  EmitVertex();
  
  DrawTexCoord = vec2(1, 1);
  gl_Position = MatrWVP * vec4(CamRight + CamUp, 1);
  EmitVertex(); 
  
  DrawTexCoord = vec2(1, 0);
  gl_Position = MatrWVP * vec4(CamRight - CamUp, 1);
  EmitVertex();
  EndPrimitive();

  Position = (vec4(0, 0, 0, 0) * MatrWVP).xyz;
} /* End of 'main' function */                      
/* END OF 'geom.glsl' FILE */
