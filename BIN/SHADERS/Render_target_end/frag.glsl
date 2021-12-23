/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Target fragment shader.
 *               Rendering system implementation.
 *               Resourses handle.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kuralenok,
 *               Alexandr Vashpanov.
 * LAST UPDATE : 03.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 430

layout(location = 0) out vec4 OutColor;
layout(binding = 0) uniform sampler2D Color;
layout(binding = 1) uniform sampler2D KaTrans;
layout(binding = 2) uniform sampler2D KdIsIl;
layout(binding = 3) uniform sampler2D KsPh;
layout(binding = 4) uniform sampler2D N;
layout(binding = 5) uniform sampler2D Pos;

layout(binding = 0) uniform sampler2D In[6];

in vec2 DrawTexCoord;
 
layout(location = 0) uniform float FrameH;
layout(location = 1) uniform float FrameW;
                                                                                  
/* Main function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */                                            
void main( void )
{
  ivec2 fc = ivec2(gl_FragCoord.xy);
  int i;

  if (texelFetch(KdIsIl, fc, 0).a == 0)
  {
    vec4 c = texelFetch(Color, fc, 0);
    OutColor = vec4(c.rgb, 1);
    //OutColor = vec4(0, 0, 0, 1);
    //return;
  }
  //discard;

  //discard;
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
