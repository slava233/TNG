/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : SkyBox vertex shader.
 *               Cube maping system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Alexandr Vashpanov.
 * LAST UPDATE : 13.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

/* Position */
layout (location = 0) in vec3 Pos;

out vec3 TexCoords;
/* Projection matrix */
uniform mat4 MatrP;
/* View matrix */
uniform mat4 MatrV;

/* Main function */
void main()
{
  TexCoords = Pos;
  gl_Position = MatrP * MatrV * vec4(Pos, 1);
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
 