/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : geom.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Draw light source shaders.
 *               Geomerty shader.
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

/* Format of in vertices */
layout(points) in;
/* Format and max_vertices of out vertices */
layout(triangle_strip, max_vertices = 8) out;

/* Light position */
layout(location = 0) uniform vec3 LightPos;

/* Cut off distance of light source */
layout(location = 1) uniform vec2 LightCutOffDist;

/* World-View-Projection matrrix */
layout(location = 2) uniform mat4 MatrWVP;

/* Camera right direction */
layout(location = 6) uniform vec3 CamRight;

/* Camera up direction */
layout(location = 7) uniform vec3 CamUp;

/* Light color */
layout(location = 8) uniform vec3 LightColor;

/* Texture coordinate output parameters */
out vec2 DrawTexCoords;

/* Output color */
out vec4 DrawColor;

/* Main function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
void main( void )
{
  float s = 1;

  DrawColor = vec4(LightColor, 1);
  for (int i = 0; i < 1; i++)
  {
    /* Setup rectangle on screen */
    DrawTexCoords = vec2(0, 1);
    gl_Position = MatrWVP * vec4((LightPos + s * (-CamRight + CamUp)), 1);
    EmitVertex();
  
    DrawTexCoords = vec2(0, 0);
    gl_Position = MatrWVP * vec4((LightPos + s * (-CamRight - CamUp)), 1);
    EmitVertex();
  
    DrawTexCoords = vec2(1, 1);
    gl_Position = MatrWVP * vec4((LightPos + s * (CamRight + CamUp)), 1);
    EmitVertex(); 
  
    DrawTexCoords = vec2(1, 0);
    gl_Position = MatrWVP * vec4((LightPos + s * (CamRight - CamUp)), 1);
    EmitVertex();
    EndPrimitive();
  
    s = LightCutOffDist.y;
    DrawColor = vec4(1, 0, 0, 1);
  }
} /* End of 'main' function */

/* END OF 'geom.glsl' FILE */
