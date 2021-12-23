/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Spot light source shaders.
 *               Fragment shader.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 17.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420

/* Out color to screen directly */
layout(location = 0) out vec4 OutColor;

/* Light position */
uniform vec3 LightPos;
/* Light color */
uniform vec3 LightColor;
/* Light attenuation coefficients */
uniform vec3 LightAtt;
/* Camera location */
//uniform vec3 CamLoc;

/*
layout(binding = 7) uniform Standart
{
  vec3 CamLoc;
  float Time;
};
*/

uniform vec3 CamLoc;
uniform float Time;

/* Color of objects to light */
layout(binding = 0) uniform sampler2D TexColor;
/* Ka coefficient & Trans value of object */ 
layout(binding = 1) uniform sampler2D TexKaTrans;
/* Kd coefficient & IsIlluminated value of object */
layout(binding = 2) uniform sampler2D TexKdIs;
/* Ks coefficient & Phong coefficients of object */
layout(binding = 3) uniform sampler2D TexKsPh;
/* Normals of object */
layout(binding = 4) uniform sampler2D TexNormal;
/* Position of object */
layout(binding = 5) uniform sampler2D TexPos;

/* Shade object function.
 * ARGUMENTS:
 *   - illumination coefficients:
 *       vec3 Ka, Kd, Ks;
 *   - Phong coefficient:
 *       float Ph;
 *   - position of object:
 *       vec3 Pos;
 *   - normals of object:
 *       vec3 N;
 *   - light position:
 *       vec3 LPos;
 *   - light color:
 *       vec3 LColor;
 * RETURNS:
 *   (vec3) new illuminated color.
 */
vec3 Shade( vec3 Ka, vec3 Kd, vec3 Ks, float Ph, vec3 Pos, vec3 N, vec3 L, vec3 LC )
{
  vec3 
    color,                       /* main color */   
    coloratt,                    /* attenuated color */ 
    V = normalize(Pos - CamLoc), /* Camera direction at exact point */ 
    R = reflect(V, N);           /* Vector for specular reflection */

  /* Ambient */
  color = Ka / 8;
  /* Diffuse */
  coloratt = Kd * max(dot(N, L), 0.0) * LC;
  /* Specular */
  coloratt += Ks * pow(max(dot(R, L), 0.0), Ph) * LC;

  /* Fake attenuation coefficients(cc, cl, cq) */
  float cc = 1, cl = 0.5, cq = 0;
  /* Distance between LightPos & Pos */ 
  float d = length(LightPos - Pos);
  /* Attenuation coefficient */
  float F = min(1 / cc + cl * d + cq * d * d, 1);

  /* Attenuation */
  coloratt *= F;
  color += coloratt;

  return color;
} /* End of 'Shade' funciton */

/* Main function. 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{ 
  ivec2 fc = ivec2(gl_FragCoord.xy);
  vec4 
    Color = texelFetch(TexColor, fc, 0),     /* Color of object */
    KaTrans = texelFetch(TexKaTrans, fc, 0), /* Ka & Trans coefficients */
    KdIs = texelFetch(TexKdIs, fc, 0),       /* Kd coefficents & IsIlluminated flag */
    KsPh = texelFetch(TexKsPh, fc, 0),       /* Ks & Phong coefficients */
    Normal =  texelFetch(TexNormal, fc, 0),  /* Normal of object */
    Pos = texelFetch(TexPos, fc, 0);         /* Postion of object */

  /* Check is illuminated coefficients */
  if (KdIs.a == 1)
  {
    OutColor = vec4(Shade(KaTrans.xyz, KdIs.xyz, KsPh.xyz, KsPh.a, Pos.xyz, normalize(Normal.xyz), normalize(LightPos), LightColor), 1);
  }
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
