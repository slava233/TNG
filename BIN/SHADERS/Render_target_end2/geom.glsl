/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : geom.glsl
 * PURPOSE     : Target geometry shader.
 *               Rendering system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kuralenok,
 *               Alexandr Vashpanov.
 * LAST UPDATE : 01.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

/* Format of in vertices */
layout(points) in;
/* Format and max_vertices of out vertices */
layout(triangle_strip, max_vertices = 4) out;

out vec2 DrawTexCoord;

/* The main shader function */
void main( void )
{                      
  DrawTexCoord = vec2(0, 1);
  gl_Position = vec4(-1, 1, 0, 1);
  EmitVertex();

  DrawTexCoord = vec2(0, 0);      
  gl_Position = vec4(-1, -1, 0, 1);
  EmitVertex();   
                
  DrawTexCoord = vec2(1, 1);                                      
  gl_Position = vec4(1, 1, 0, 1); 
  EmitVertex();  

  DrawTexCoord = vec2(1, 0);                     
  gl_Position = vec4(1, -1, 0, 1);                                
  EmitVertex();  
  EndPrimitive();
} /* End of 'main' function */

/* END OF 'geom.glsl' FILE */
