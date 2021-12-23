/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Frag font shader.
 * PROGRAMMER  : CGSG'2018.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

/* Default materials */
layout(location = 0) out vec4 Color;

in vec2 DrawTexCoord;
/*
layout(location = 1) out vec4 KaTrans;
layout(location = 2) out vec4 KdIsIl;
layout(location = 3) out vec4 KsPh;
layout(location = 4) out vec4 N;
layout(location = 5) out vec4 Pos;

in vec3 DrawPos;
in vec3 DrawNormal;
in vec4 DrawColor;

uniform vec3 CamLoc;
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Ph;
uniform float Trans;
*/
uniform float Time;

uniform bool IsTexture0;
layout(binding = 0) uniform sampler2D TextureFnt;

/* Main funciton.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  Color = vec4(1, 1, 1, 1);
  vec4 c = texture(TextureFnt, vec2(1, -1) * DrawTexCoord);
  Color = vec4(c.rgb, c.r);

  return;
  /* Ambient * /
  KaTrans = vec4(1, 1, 1, Trans);

  /* Diffuse * /
  if (IsTexture0)
    KdIsIl = vec4(2.8 * texture(TextureKd, vec2(1, -1) * DrawTexCoord).rgb, 0);
  else
    KdIsIl = vec4(Kd, 0);
  Color = vec4(2.8 * texture(TextureKd, vec2(1, -1) * DrawTexCoord).rgb, texture(TextureKd, vec2(1, -1) * DrawTexCoord).r);
  //KdIsIl = vec4(1, 1, 1, 1);

  /* Specular * /
  KsPh.rgb = Ks;
  KsPh.a = Ph;

  /* Normal * /
  N = vec4(normalize(DrawNormal), gl_FragCoord.z);
  //N = vec4(normalize(cross(DrawT, DrawB)), gl_FragCoord.z);
  Pos = vec4(DrawPos, 0);
  */
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
