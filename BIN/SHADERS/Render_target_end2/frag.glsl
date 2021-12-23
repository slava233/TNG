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
 * LAST UPDATE : 18.04.2019
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

layout(location = 0) out vec4 OutColor;
layout(binding = 0) uniform sampler2D Color;
layout(binding = 1) uniform sampler2D Color0;
layout(binding = 2) uniform sampler2D KaTrans;
layout(binding = 3) uniform sampler2D KdIsIl;
layout(binding = 4) uniform sampler2D KsPh;
layout(binding = 5) uniform sampler2D N;
layout(binding = 6) uniform sampler2D Pos;
layout(binding = 7) uniform samplerCube SkyBox;

layout(binding = 1) uniform sampler2D In[6];

in vec2 DrawTexCoord;
 
uniform float FrameH;
uniform float FrameW;
uniform float GTime, Time, FPS, IsDebug, IsBlur, IsSobel, IsEmbossing, IsHigh_pass, IsMonochrome, IsRobert, IsNorm;
uniform vec3 CamLoc;
    
/* Main function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */                                            
void main( void )
{
  ivec2 fc = ivec2(gl_FragCoord.xy);
  vec4 color = texelFetch(Color, fc, 0);
  int y = int(gl_FragCoord.y), x = int(gl_FragCoord.x / 2);

  if (y < 3 && x < 300 * abs(sin(Time)))
  {
    if (x < 300 * abs(sin(GTime)))
      OutColor = vec4(0, 1, 1, 1);
    else
      OutColor = vec4(1, 0, 0, 1);
    return;
  }
  if (y < 13 && x < FPS)
  {
    if (y < 25)
      OutColor = vec4(1, 0, 0, 1);
    if (y < 2 ||
        (y < 8 && x % 10 == 0) ||
        (y < 5 && x % 5 == 0) ||
        (y < 18 && x % 100 == 0) ||
        (y < 13 && x % 50 == 0))
      OutColor = vec4(0.78, 0.9, 1, 1);
    return;
  }
  else if (y < 2 ||
           (y < 8 && x % 10 == 0) ||
           (y < 5 && x % 5 == 0) ||
           (y < 18 && x % 100 == 0) ||
           (y < 13 && x % 50 == 0))
  {
    OutColor = vec4(1, 1, 1, 1);
    return;
  }

  int i;
  vec4 sampleTex[9];
  float
    blur[9] = float[](1, 2, 1, 2, 4, 2, 1, 2, 1),
    sobel_x[9] = float[](3, 10, 3, 0, 0, 0, -3, -10, -3),
    sobel_y[9] = float[](3, 0, -3, 10, 0, -10, 3, 0, -3),
    embossing[9] = float[](4, 0, 0, 0, 0, 0, 0, 0, -4),
    offset = 1.0 / 300.0;
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
  for (i = 0; i < 9; i++)
  {
    sampleTex[i] = vec4(vec3(texture(Color, DrawTexCoord + offsets[i])), 1);
  }
  vec4 col = vec4(0), luma = vec4(0.29, 0.6, 0.11, 0);
  float lum = max(0, dot(luma, color));
  for (i = 0; i < 9; i++)
  {
    vec4 V = sampleTex[i], M = V * vec4(0.7,0.7,0.7,1);
    if (IsNorm == 1)
      col = color; 
    if (IsMonochrome == 1)
      col = vec4(M.rrr + M.ggg + M.bbb, 1) / 2;
    if (IsSobel == 1)
      col += V * (sobel_x[i] * sobel_y[i]) / 6;
    if (IsBlur == 1)
      col += V * blur[i] / 10;
    if (IsEmbossing == 1)
      col = M * embossing[i] * 20;
    if (IsHigh_pass == 1)
      col += pow(color, vec4(1.0 / (lum + 0.1), 1.0 / (lum + 0.1), 1.0 / (lum + 0.1), 1)) * 0.23;
    if (IsRobert == 1)
    {
      sampleTex[4] = sqrt((sampleTex[4] - sampleTex[2]) * (sampleTex[4] - sampleTex[2]) +
        (sampleTex[1] - sampleTex[5]) * (sampleTex[1] - sampleTex[5]));
      col += sampleTex[4] / 6;
    }
  }
                                 
  if (IsDebug == 1)
  {
    int sc = 4;
    for (i = 0; i < sc && i < In.length(); i++)
    {
      if (fc.x < FrameW / sc && fc.y >= i * FrameH / sc && fc.y < (i + 1) * FrameH / sc)
      {
        fc.y -= int(i * FrameH / sc);
        OutColor = vec4(texelFetch(In[i], fc * sc, 0).xyz, 1);
        return;
      } 
    }
    for (; i < In.length(); i++)
    {
      if (fc.y < FrameH / sc && fc.x >= (i - sc + 1) * FrameW / sc && fc.x < (i - sc + 2) * FrameW / sc)
      {
        fc.x -= int((i - sc + 1) * FrameW / sc);
        OutColor = vec4(texelFetch(In[i], fc * sc, 0).xyz, 1);
        return;
      } 
    }
    if (fc.y < FrameH / sc)
      if (fc.x > (sc - 1) * FrameW / sc)
      {
        fc.x -= int((sc - 1) * FrameW / sc);
        vec4 cc = texelFetch(N, fc * sc, 0);
        float t = cc.a;
        t = (t + 1) / 2;
        t = clamp(t * 30 - 22, 0, 8) / 8;
        t = 1 - clamp(t, 0, 1);
        OutColor = vec4(vec3(t), 1);
        return;
      }
  }
  vec4 c = texelFetch(KdIsIl, fc, 0);
  if (c.a == 1)
  {
    vec3 ks = texelFetch(KsPh, fc, 0).xyz;
    vec3 p = texelFetch(Pos, fc, 0).xyz;
    vec3 n = normalize(texelFetch(N, fc, 0).xyz);
    vec3 v = normalize(p - CamLoc);
    vec3 r = reflect(v, n);
    vec4 c = texture(SkyBox, r);
    OutColor = vec4(col.rgb/* + 0.4 * ks * c.rgb*/, 1);
  }                                              
  else
    OutColor = vec4(color.rgb, 1);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
