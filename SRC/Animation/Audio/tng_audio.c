/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_audio.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Main audio module.
 * PROGRAMMER  : CGSG'2018.
 *               Svyatoslav Kononov.
 * LAST UPDATE : 27.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <al.h>
#include <alc.h>
#include <alut.h>
#pragma comment(lib, "alut")
#pragma comment(lib, "openal32")

/* Audio buffer load function.
 * ARGUMENTS: 
 *   - file name:
 *       CHAR *Name;
 * RETURNS: 
 *   (tngAUDIO_BUFFER) pointer to loaded buffer.
 */
static tngAUDIO_BUFFER * TNG_AudioLoadFromWAV( CHAR *Name )
{
  tngAUDIO_BUFFER Buffer = {0};
  WAVHEADER WAV;
  FILE *F;
  INT format;
  UINT size, freq;
  VOID *mem;
  CHAR 
    *adress = "BIN/AUDIO/",
    buf[1000],
    loop;

  sprintf(buf, "%s%s", adress, Name);

  if ((F = fopen(buf, "rb")) ==  NULL)
    return NULL;

  fread_s(&WAV, sizeof(WAVHEADER), sizeof(WAVHEADER), 1, F);

  fclose(F);

  alGenBuffers(1, Buffer.Buf);

  alutLoadWAVFile(buf, &format, &mem, &size, &freq, &loop);
  alBufferData(Buffer.Buf[0], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

  Buffer.Id = TNG()->AudioBuffers.Size;
  Buffer.WAV = WAV;

  return StockAdd(&TNG()->AudioBuffers, &Buffer);
} /* End of 'TNG_AudioLoadFromWAV' function */

/* Audio attaching buffer to source function.
 * ARGUMENTS:
 *   - pointer to audio source:
 *       tngAUDIO_SOURCE *ASrc;
 *   - pointer to audio buffer:
 *       tngAUDIO_BUFFER *Buf;
 * RETURNS: None.
 */
static VOID TNG_AudioBufferAttach( tngAUDIO_SOURCE *ASrc, tngAUDIO_BUFFER *Buf )
{
  alSourcef(ASrc->Source[0], AL_BUFFER, ASrc->AttBuf->Buf[0]);

  ASrc->AttBuf = Buf;
} /* End of 'TNG_AudioBufferAttach' function */

/* Audio setting position to source function.
 * ARGUMENTS:
 *   - pointer to audio source:
 *       tngAUDIO_SOURCE *ASrc;
 *   - speed:
 *       VEC APos;
 * RETURNS: None.
 */
static VOID TNG_AudioPositionSet( tngAUDIO_SOURCE *ASrc, VEC APos )
{
  FLT Pos[3] =
  {
    APos.X, APos.Y, APos.Z
  };

  alSourcefv(ASrc->Source[0], AL_POSITION, Pos);

  ASrc->Pos = APos;
} /* End of 'TNG_AudioPositionSet' function */

/* Audio setting speed of player to source function.
 * ARGUMENTS:
 *   - pointer to audio source:
 *       tngAUDIO_SOURCE *ASrc;
 *   - speed of player:
 *       VEC AV;
 * RETURNS: None.
 */
static VOID TNG_AudioPlayerSpeedSet( tngAUDIO_SOURCE *ASrc, VEC AV )
{
  FLT V[3] =
  {
    AV.X, AV.Y, AV.Z
  };

  alSourcefv(ASrc->Source[0], AL_VELOCITY, V);

  ASrc->V = AV;
} /* End of 'TNG_AudioPlayerSpeedSet' function */

/* Audio setting speed of audio to source function.
 * ARGUMENTS:
 *   - pointer to audio source:
 *       tngAUDIO_SOURCE *ASrc;
 *   - speed of playing:
 *       FLT Speed;
 * RETURNS: None.
 */
static VOID TNG_AudioSpeedSet( tngAUDIO_SOURCE *ASrc, FLT Speed )
{
  alSourcef(ASrc->Source[0], AL_PITCH, Speed);

  ASrc->Speed = Speed;
} /* End of 'TNG_AudioSpeedSet' function */

/* Audio setting volume of audio to source function.
 * ARGUMENTS:
 *   - pointer to audio source:
 *       tngAUDIO_SOURCE *ASrc;
 *   - volume:
 *       FLT Vol;
 * RETURNS: None.
 */
static VOID TNG_AudioVolumeSet( tngAUDIO_SOURCE *ASrc, FLT Vol )
{
  alSourcef(ASrc->Source[0], AL_GAIN, Vol);
} /* End of 'TNG_AudioVolumeSet' function */

/* Audio setting roll off factor to source function.
 * ARGUMENTS:
 *   - pointer to audio source:
 *       tngAUDIO_SOURCE *ASrc;
 *   - roll off factor:
 *       FLT RollOff;
 * RETURNS: None.
 */
static VOID TNG_AudioRollOffSet( tngAUDIO_SOURCE *ASrc, FLT RollOff )
{
  alSourcef(ASrc->Source[0], AL_ROLLOFF_FACTOR, RollOff);
} /* End of 'TNG_AudioRollOffSet' function */

/* Audio setting reference distance to source function.
 * ARGUMENTS:
 *   - pointer to audio source:
 *       tngAUDIO_SOURCE *ASrc;
 *   - roll off factor:
 *       FLT RefDist;
 * RETURNS: None.
 */
static VOID TNG_AudioRefDistSet( tngAUDIO_SOURCE *ASrc, FLT RefDist )
{
  alSourcef(ASrc->Source[0], AL_REFERENCE_DISTANCE, RefDist);
} /* End of 'TNG_AudioRefDistSet' function */

/* Audio setting maximum distance of audio to source function.
 * ARGUMENTS:
 *   - pointer to audio source:
 *       tngAUDIO_SOURCE *ASrc;
 *   - max distance:
 *       FLT Dist;
 * RETURNS: None.
 */
static VOID TNG_AudioDistanceMaxSet( tngAUDIO_SOURCE *ASrc, FLT Dist )
{
  alSourcef(ASrc->Source[0], AL_MAX_DISTANCE, Dist);
} /* End of 'TNG_AudioDistanceMaxSet' function */

/* Audio setting repeat status of audio to source function.
 * ARGUMENTS:
 *   - pointer to audio source:
 *       tngAUDIO_SOURCE *ASrc;
 *   - flag of repeat:
 *       BOOL IsRepeat;
 * RETURNS: None.
 */
static VOID TNG_AudioIsRepeat( tngAUDIO_SOURCE *ASrc, BOOL IsRepeat )
{
  INT R;

  if (IsRepeat)
    R = 1;
  else
    R = 0;

  alSourcef(ASrc->Source[0], AL_LOOPING, R);

  ASrc->IsRepeat = IsRepeat;
} /* End of 'TNG_AudioIsRepeat' function */

/* Audio source apply function.
 * ARGUMENTS: 
 * - pointer to 
 *     tngAUDIO_SOURCE *ASrc
 * RETURNS: None.
 */
static VOID TNG_AudioSourceApply( tngAUDIO_SOURCE *ASrc )
{
  if (ASrc->AttBuf != NULL)
    alSourcei(ASrc->Source[0], AL_BUFFER, ASrc->AttBuf->Buf[0]);
  TNG_AudioIsRepeat(ASrc, ASrc->IsRepeat);
  TNG_AudioSpeedSet(ASrc, ASrc->Speed);
  TNG_AudioDistanceMaxSet(ASrc, ASrc->Dist);
  TNG_AudioPositionSet(ASrc, ASrc->Pos);
  TNG_AudioPlayerSpeedSet(ASrc, ASrc->V);
  TNG_AudioRollOffSet(ASrc, ASrc->RollOff);
  TNG_AudioRefDistSet(ASrc, ASrc->RefDist);
} /* End of 'TNG_AudioApply' function */

/* Audio source set function.
 * ARGUMENTS: 
 * - pointer to source:
 *     tngAUDIO_SOURCE *Source;
 * - pointer to attaching buffer:
 *     tngAUDIO_BUFFER *Buf;
 * - flag of repeat (TRUE - infinity, FALSE - no repeat):
 *     BOOL IsRepeat; 
 * - speed of playing:
 *     FLT Speed;
 * - audio model:
 *     CHAR *Model;
 * - distance of hearing:
 *     FLT Dist;
 * - audio player position:
 *     VEC Pos;
 * - audio player speed:
 *     VEC V;
 * - pause state:
 *     BOOL IsPause;
 * - roll off factor:
 *     FLT RollOff;
 * - reference distance:
 *     FLT RefDist;
 * RETURNS: None.
 */
static VOID TNG_AudioSourceSet( tngAUDIO_SOURCE *Source,
                                tngAUDIO_BUFFER *Buf,
                                BOOL IsRepeat, FLT Speed, CHAR *Model,
                                FLT Dist, VEC Pos, VEC V, BOOL IsPause,
                                FLT RollOff, FLT RefDist )
{
  Source->AttBuf = Buf;
  Source->IsRepeat = IsRepeat;
  Source->Speed = Speed;
  Source->Model = Model;
  Source->Dist = Dist;
  Source->Pos = Pos;
  Source->V = V;
  Source->IsPause = IsPause;
  Source->RollOff = RollOff;
  Source->RefDist = RefDist;

  TNG_AudioSourceApply(Source);
} /* End of 'TNG_AudioSourceSet' function */

/* Audio source create function.
 * ARGUMENTS: None.
 * RETURNS: 
 *   (tngAUDIO_SOURCE) pointer to created source.
 */
static tngAUDIO_SOURCE * TNG_AudioSourceCreate( VOID )
{
  tngAUDIO_SOURCE Source = {0};

  alGenSources(1, Source.Source);

  TNG_AudioSourceSet(&Source, NULL, TRUE, 1, "", 100,
                     VecSet(0, 0, 0), VecSet(0, 0, 0), FALSE, 1, 50);

  return StockAdd(&TNG()->AudioSources, &Source); 
} /* End of 'TNG_AudioSourceCreate' function */

/* Get distance to listener function.
 * ARGUMENTS: 
 *  - pointer to source:
 *      tngAUDIO_SOURCE *ASrc; 
 * RETURNS:
 *  (FLT) distance to listener.
 */
static FLT TNG_GetDistanceToListener( tngAUDIO_SOURCE *ASrc )
{
  return VecLen(VecSubVec(TNG()->Camera.Loc, ASrc->Pos));
} /* End of 'TNG_GetDistanceToListener' function */

/* Audio player move function.
 * ARGUMENTS: 
 *  - pointer to source:
 *      tngAUDIO_SOURCE *ASrc; 
 * RETURNS: None.
 */
static VOID TNG_AudioMove( tngAUDIO_SOURCE *ASrc )
{
  TNG_AudioPositionSet(ASrc, VecAddVec(ASrc->Pos, ASrc->V));
} /* End of 'TNG_AudioMove' function */

/* Audio volume attaching to distance function.
 * ARGUMENTS: 
 *  - pointer to source:
 *      tngAUDIO_SOURCE *ASrc; 
 * RETURNS: None.
 */
static VOID TNG_AudioAttachVolumeToDistance( tngAUDIO_SOURCE *ASrc )
{
  FLT 
    dist = TNG_GetDistanceToListener(ASrc),
    model;

  if (!strcmp(ASrc->Model, "LINEAR"))
  {
    dist = COM_MIN(dist, ASrc->Dist);
    model = 1 - ASrc->RollOff * (dist - ASrc->RefDist) / (ASrc->Dist - ASrc->RefDist);
  }
  else if (!strcmp(ASrc->Model, "INVERSE"))
    model = ASrc->RefDist / (ASrc->RefDist + ASrc->RollOff * (dist - ASrc->RefDist));
  else if (!strcmp(ASrc->Model, "EXPONENTIAL"))
    model = pow((dist / ASrc->RefDist), (-ASrc->RollOff));
  else
    model = 0;

  TNG_AudioVolumeSet(ASrc, model);
} /* End of 'TNG_AudioAttachVolumeToDistance' function */

/* Audio play function.
 * ARGUMENTS: 
 *  - pointer to source:
 *      tngAUDIO_SOURCE *ASrc; 
 * RETURNS: None.
 */
static VOID TNG_AudioPlay( tngAUDIO_SOURCE *ASrc )
{
  alSourcePlay(ASrc->Source[0]);
} /* End of 'TNG_AudioPlay' function */

/* Audio stop function.
 * ARGUMENTS: 
 *  - pointer to source:
 *      tngAUDIO_SOURCE *ASrc; 
 * RETURNS: None.
 */
static VOID TNG_AudioStop( tngAUDIO_SOURCE *ASrc )
{
  alSourceStop(ASrc->Source[0]);
} /* End of 'TNG_AudioStop' function */

/* Audio pause function.
 * ARGUMENTS: 
 *  - pointer to source:
 *      tngAUDIO_SOURCE *ASrc; 
 * RETURNS: None.
 */
static VOID TNG_AudioPause( tngAUDIO_SOURCE *ASrc )
{
  UINT val;

  alGetSourcei(ASrc->Source[0], AL_SOURCE_STATE, &val);
  if (val == AL_PLAYING)
    alSourcePause(ASrc->Source[0]);
  else
    alSourcePlay(ASrc->Source[0]);

  ASrc->IsPause = !ASrc->IsPause;
} /* End of 'TNG_AudioPause' function */

/* Audio subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemAudioInit( VOID )
{
  alutInit(NULL, 0);
  alGetError();

  StockInit(&TNG()->AudioBuffers, 0, sizeof(tngAUDIO_BUFFER), NULL);
  StockInit(&TNG()->AudioSources, 0, sizeof(tngAUDIO_SOURCE), NULL);

  TNG()->AudioLoadFromWAV = TNG_AudioLoadFromWAV;
  TNG()->AudioSourceApply = TNG_AudioSourceApply;
  TNG()->AudioSourceCreate = TNG_AudioSourceCreate;
  TNG()->AudioSourceSet = TNG_AudioSourceSet;
  TNG()->GetDistanceToListener = TNG_GetDistanceToListener;
  TNG()->AudioMove = TNG_AudioMove;
  TNG()->AudioAttachVolumeToDistance = TNG_AudioAttachVolumeToDistance;
  TNG()->AudioPlay = TNG_AudioPlay;
  TNG()->AudioStop = TNG_AudioStop;
  TNG()->AudioPause = TNG_AudioPause;
  TNG()->AudioBufferAttach = TNG_AudioBufferAttach;
  TNG()->AudioPositionSet = TNG_AudioPositionSet; 
  TNG()->AudioPlayerSpeedSet = TNG_AudioPlayerSpeedSet; 
  TNG()->AudioSpeedSet = TNG_AudioSpeedSet;
  TNG()->AudioVolumeSet = TNG_AudioVolumeSet;
  TNG()->AudioIsRepeat = TNG_AudioIsRepeat;
  TNG()->AudioRollOffSet = TNG_AudioRollOffSet;
  TNG()->AudioRefDistSet = TNG_AudioRefDistSet;
} /* End of 'TNG_SubsystemAudioInit' function */

/* Audio subsystem destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemAudioDestructor( VOID )
{
  StockClear(&TNG()->AudioBuffers);
  StockClear(&TNG()->AudioSources);
} /* End of 'TNG_SubsystemAudioDestructor' function */

/* END OF 'tng_audio.c' FILE */

