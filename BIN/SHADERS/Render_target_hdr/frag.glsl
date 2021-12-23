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
 * LAST UPDATE : 10.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

layout(binding = 0) uniform sampler2D Color;

layout (location = 0) out vec4 OutColor;

in vec2 DrawTexCoord;

/* Main function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */                                            
void main( void )
{
  int i;
  vec4 sampleTex[9];
  float kernel[9] = float[](1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16), offset = 1.0 / 300.0,
  border[9] = float[](1.0, 1.0, 1.0, 1.0, -8.0, 1.0, 1.0, 1.0, 1.0);
  vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
  /* if (brightness >= 1)
    BrightColor = vec4(Color.rgb, 1);
  FragColor = OutColor - BrightColor;*/

  for (i = 0; i < 9; i++)
  {
    sampleTex[i] = vec4(vec3(texture(Color, DrawTexCoord + offsets[i])), 1);
  }
  vec4 col = vec4(0);
  for (i = 0; i < 9; i++)
    col += sampleTex[i] * kernel[i];
  OutColor = vec4(col.rgb, 1);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
