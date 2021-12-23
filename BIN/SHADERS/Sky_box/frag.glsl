/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : SkyBox fragment shader.
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

layout(location = 0) out vec4 Color;
layout(location = 1) out vec4 KaTrans;
layout(location = 2) out vec4 KdIsIl;
layout(location = 3) out vec4 KsPh;
layout(location = 4) out vec4 N;
layout(location = 5) out vec4 Pos;

in vec3 TexCoords;

layout(binding = 7) uniform samplerCube SkyBox;

uniform float Time;

uniform float FrameW, FrameH, CamProjDist, CamProjSize;
uniform vec3 CamLoc, CamRight, CamUp, CamDir;

/* Main Function */
void main( void )
{   
  float ratio_x, ratio_y;

  ratio_x = ratio_y = CamProjSize / 2;

  if (FrameW > FrameH) 
    ratio_x *= FrameW / FrameH;
  else               
    ratio_y *= FrameH / FrameW;

  float
    w = ratio_x * CamProjSize,
    h = ratio_y * CamProjSize;

  vec3 r =
    CamDir * CamProjSize / 12 +
      CamRight * w * (2 * gl_FragCoord.x / FrameW - 1) +
      CamUp * h * (2 * gl_FragCoord.y / FrameH - 1);

  Color = texture(SkyBox, normalize(r));//gl_FragCoord.xyx);//TexCoords);
  //Color = vec4(CamDir, 1);
  vec4 sb = texture(SkyBox, vec3(gl_FragCoord.x, gl_FragCoord.y, 0));//gl_FragCoord.xyx);//TexCoords);
  //Color = sb.rgbr;
  KdIsIl = vec4(sb.rgb, 0);
  N = vec4(1, 0, 1, 11);
  Pos = vec4(0, 0, 0, 1);
  //Color = vec4(1, 1, 1, 1);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
