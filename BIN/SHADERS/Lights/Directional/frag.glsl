/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Nature Graphics project.
 *               Directional light source shaders.
 *               Fragment shader.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 25.04.2019.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
#version 420
                      
/* Light color */
uniform vec3 LightColor;
/* Light direction */
uniform vec3 LightDir;
/* Camera location */
uniform vec3 CamLoc;

uniform float Time;

uniform bool ShadowCast;

uniform mat4 MatrShadow;
uniform mat4 MatrVP;

/* Out color to screen directly */
layout(location = 0) out vec4 OutColor;

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
/* Shadow map */
layout(binding = 8) uniform sampler2DShadow ShadowMap;
layout(binding = 8) uniform sampler2D SMT;

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
 *   - light direction:
 *       vec3 LDir;
 *   - light color:
 *       vec3 LColor;
 * RETURNS:
 *   (vec3) new illuminated color.
 */
vec3 Shade( vec3 Ka, vec3 Kd, vec3 Ks, float Ph, vec3 Pos, vec3 N, vec3 LDir, vec3 LC )
{
  vec3 
    color,                       /* main color */  
    V = normalize(Pos - CamLoc), /* Camera direction at exact point */ 
    R = reflect(V, N);           /* Vector for specular reflection */
  float shd = 1;
  if (ShadowCast)
  {   
    vec4 ShadowCoord = mat4(vec4(0.5, 0, 0, 0),
                            vec4(0, 0.5, 0, 0),
                            vec4(0, 0, 0.5, 0),
                            vec4(0.5, 0.5, 0.5, 1)) * MatrShadow * vec4(Pos, 1), shadow;
    shd = textureProj(ShadowMap, ShadowCoord);
  }                        
  /* Ambient */
  color = Ka / 8;
  /* Diffuse */
  color += Kd * max(dot(N, LDir), 0.0) * LC * shd;
  /* Specular */
  color += Ks * pow(max(dot(N, R), 0.0), Ph) * LC * shd;
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
    Normal = texelFetch(TexNormal, fc, 0),   /* Normal of object */
    Pos = texelFetch(TexPos, fc, 0);         /* Postion of object */

  float c = textureProj(ShadowMap, MatrShadow * vec4(Pos.xyz, 1));    /* Color of object */
  c = textureProj(ShadowMap, vec4(Pos.xyz, 1));    /* Color of object */
  c = texelFetch(SMT, ivec2(fc / 8), 0).r;     /* Color of object */
  OutColor = vec4(c, c, c, c);
  //return;

  /* Check is illuminated coefficients */
  if (KdIs.a == 1)
  {
    OutColor = vec4(Shade(KaTrans.xyz, KdIs.xyz, KsPh.xyz, KsPh.a, Pos.xyz, normalize(Normal.xyz), normalize(LightDir), LightColor), 1);
  }
  else
    discard;
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
