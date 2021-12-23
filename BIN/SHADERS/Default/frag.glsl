/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Frag default shader.
 * PROGRAMMER  : CGSG'2018.
 *               Girvits Alexander.
 * LAST UPDATE : 17.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

/* Default materials */
layout(location = 0) out vec4 Color;
layout(location = 1) out vec4 KaTrans;
layout(location = 2) out vec4 KdIsIl;
layout(location = 3) out vec4 KsPh;
layout(location = 4) out vec4 N;
layout(location = 5) out vec4 Pos;

in vec3 DrawPos;
in vec3 DrawNormal;
in vec4 DrawColor;
in vec2 DrawTexCoord;
in vec3 DrawT;
in vec3 DrawB;
/*
uniform float Time;
uniform vec3 CamLoc;
*/
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Ph;
uniform float Trans;
uniform bool IsWireFrame;

/*layout(binding = 7) uniform Standart
{
  vec3 CamLoc;
  float Time;
};*/

uniform vec3 CamLoc;
uniform float Time;

uniform bool IsTexture0;
layout(binding = 0) uniform sampler2D TextureKd;
uniform bool IsTexture1;
layout(binding = 1) uniform sampler2D TextureNm;
uniform bool IsTexture2;
layout(binding = 2) uniform sampler2D TextureKs;

/* Main funciton.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  /*
  if (IsWireFrame)
  {
    Color = vec4(1, 1, 1, 1);
    KaTrans = vec4(1, 1, 1, 1);
    N = vec4(1, 1, 1, 1);
    return;
  }
  */
  Color = vec4(1, 0.8, 0.3, 1);

  /* Ambient */
  KaTrans = vec4(Ka / 8, Trans);

  /* Diffuse */
  if (IsTexture0)
    KdIsIl = vec4(texture(TextureKd, DrawTexCoord).rgb, 1);
  else
    KdIsIl = vec4(Kd, 1);

  /* Specular */
  if (IsTexture2)
  {
    vec4 v = texture2D(TextureKs, DrawTexCoord);
    KsPh.rgb = v.rrr;
  }
  else
    KsPh.rgb = Ks;
  KsPh.a = Ph;

  /* Normal */
  if (IsTexture1)
  {
    vec3 NormBump = texture2D(TextureNm, DrawTexCoord).xyz * 2 - vec3(1, 1, 1);
    mat3 TBN = mat3(normalize(DrawT), normalize(DrawB), normalize(DrawNormal));
    N = vec4(normalize(TBN * NormBump.xyz), gl_FragCoord.z);
  }
  else
    N = vec4(normalize(DrawNormal), gl_FragCoord.z);
  //N = vec4(normalize(cross(DrawT, DrawB)), gl_FragCoord.z);
  Pos = vec4(DrawPos, 0);

  //Color = vec4(normalize(cross(cross(DrawT, DrawB), normalize(N.xyz))), 1);
  //Color = vec4(cross(normalize(DrawT), normalize(DrawB)), 1);
  //Color = texelFetch(TextureNm, ivec2(gl_FragCoord.xy), 0).bgra;

} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
