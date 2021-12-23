/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_audio.h
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

#ifndef __tng_audio_h_
#define __tng_audio_h_

/* WAV file structure */
typedef struct tagWAVHEADER
{
  CHAR chunkId[4];     /* RIFF */
  ULONG chunkSize;     /* File size */
  CHAR format[4];      /* WAVE */
  CHAR subchunk1Id[4]; /* fmt */
  ULONG subchunk1Size; /* 16 */
  WORD audioFormat;    /* 1 */
  WORD numChannels;    /* Channels number */
  ULONG sampleRate;    /* Sample rate */
  ULONG byteRate;      /* Sample rate * channels numer * bits per sample / 8 */
  WORD blockAlign;     /* Bytes per sample */
  WORD bitsPerSample;  /* Audio depth */
  CHAR subchunk2Id[4]; /* data */
  ULONG subchunk2Size; /* Data size */
} WAVHEADER;

/* Audio buffer structure */
typedef struct tagtngAUDIO_BUFFER
{
  INT 
    Buf[1],      /* Audio buffer */
    Id;          /* Buffer id */
  WAVHEADER WAV; /* WAV structure */
} tngAUDIO_BUFFER;

/* Audio source structure */
typedef struct tagtngAUDIO_SOURCE
{
  tngAUDIO_BUFFER
    *AttBuf;     /* Attached buffer */
  INT Source[1]; /* Audio source */   
  FLT
    Speed,       /* Audio play speed */
    Dist,        /* Distance of hearing */
    RefDist,     /* Reference distance */
    RollOff;     /* Roll Off factor */
  CHAR *Model;   /* Audio model */
  VEC 
    Pos,         /* Audio position */
    V;           /* Audio player speed */
  BOOL 
    IsPause,     /* Audio pause flag */
    IsRepeat;    /* Repeat: 0 - no, 1 - infinity */
} tngAUDIO_SOURCE;

/* Audio subsystem structure */
typedef struct tagtngSUBSYSTEM_AUDIO
{
  /* Audio buffers stock */
  STOCK AudioBuffers;

  /* Audio sources stock */
  STOCK AudioSources;

  /* Audio handle functions */

  /* Audio buffer load function.
   * ARGUMENTS: 
   *   - file name:
   *       CHAR *Name;
   * RETURNS: 
   *   (tngAUDIO_BUFFER) pointer to loaded buffer.
   */
  tngAUDIO_BUFFER * (*AudioLoadFromWAV)( CHAR *Name );

  /* Audio source create function.
   * ARGUMENTS: None.
   * RETURNS: 
   *   (tngAUDIO_SOURCE) pointer to created source.
   */
  tngAUDIO_SOURCE * (*AudioSourceCreate)( VOID );

  /* Audio source apply function.
   * ARGUMENTS: 
   * - pointer to 
   *     tngAUDIO_SOURCE *ASrc
   * RETURNS: None.
   */
  VOID (*AudioSourceApply)( tngAUDIO_SOURCE *ASrc );
  
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
  VOID (*AudioSourceSet)( tngAUDIO_SOURCE *Source,
                          tngAUDIO_BUFFER *Buf,
                          BOOL IsRepeat, FLT Speed, CHAR *Model,
                          FLT Dist, VEC Pos, VEC V, BOOL IsPause,
                          FLT RollOff, FLT RefDist );

  /* Get distance to listener function.
   * ARGUMENTS: 
   *  - pointer to source:
   *      tngAUDIO_SOURCE *ASrc; 
   * RETURNS:
   *  (FLT) distance to listener.
   */
  FLT (*GetDistanceToListener)( tngAUDIO_SOURCE *ASrc );

  /* Audio player move function.
   * ARGUMENTS: 
   *  - pointer to source:
   *      tngAUDIO_SOURCE *ASrc; 
   * RETURNS: None.
   */
  VOID (*AudioMove)( tngAUDIO_SOURCE *ASrc );

  /* Audio volume attaching to distance function.
   * ARGUMENTS: 
   *  - pointer to source:
   *      tngAUDIO_SOURCE *ASrc; 
   * RETURNS: None.
   */
  VOID (*AudioAttachVolumeToDistance)( tngAUDIO_SOURCE *ASrc );

  /* Audio play function.
   * ARGUMENTS: 
   *  - pointer to source:
   *      tngAUDIO_SOURCE *ASrc; 
   * RETURNS: None.
   */
  VOID (*AudioPlay)( tngAUDIO_SOURCE *ASrc );

  /* Audio stop function.
   * ARGUMENTS: 
   *  - pointer to source:
   *      tngAUDIO_SOURCE *ASrc; 
   * RETURNS: None.
   */
  VOID (*AudioStop)( tngAUDIO_SOURCE *ASrc );

  /* Audio pause function.
   * ARGUMENTS: 
   *  - pointer to source:
   *      tngAUDIO_SOURCE *ASrc; 
   * RETURNS: None.
   */
  VOID (*AudioPause)( tngAUDIO_SOURCE *ASrc );

  /* Audio attaching buffer to source function.
   * ARGUMENTS:
   *   - pointer to audio source:
   *       tngAUDIO_SOURCE *ASrc;
   *   - pointer to audio buffer:
   *       tngAUDIO_BUFFER *Buf;
   * RETURNS: None.
   */
  VOID (*AudioBufferAttach)( tngAUDIO_SOURCE *ASrc, tngAUDIO_BUFFER *Buf );

  /* Audio setting position to source function.
   * ARGUMENTS:
   *   - pointer to audio source:
   *       tngAUDIO_SOURCE *ASrc;
   *   - speed:
   *       VEC APos;
   * RETURNS: None.
   */
  VOID (*AudioPositionSet)( tngAUDIO_SOURCE *ASrc, VEC APos );

  /* Audio setting speed of player to source function.
   * ARGUMENTS:
   *   - pointer to audio source:
   *       tngAUDIO_SOURCE *ASrc;
   *   - speed of player:
   *       VEC AV;
   * RETURNS: None.
   */
  VOID (*AudioPlayerSpeedSet)( tngAUDIO_SOURCE *ASrc, VEC AV );

  /* Audio setting speed of audio to source function.
   * ARGUMENTS:
   *   - pointer to audio source:
   *       tngAUDIO_SOURCE *ASrc;
   *   - speed of playing:
   *       FLT Speed;
   * RETURNS: None.
   */
  VOID (*AudioSpeedSet)( tngAUDIO_SOURCE *ASrc, FLT Speed );

  /* Audio setting volume of audio to source function.
   * ARGUMENTS:
   *   - pointer to audio source:
   *       tngAUDIO_SOURCE *ASrc;
   *   - volume:
   *       FLT Vol;
   * RETURNS: None.
   */
  VOID (*AudioVolumeSet)( tngAUDIO_SOURCE *ASrc, FLT Vol );

  /* Audio setting repeat status of audio to source function.
   * ARGUMENTS:
   *   - pointer to audio source:
   *       tngAUDIO_SOURCE *ASrc;
   *   - flag of repeat:
   *       BOOL IsRepeat;
   * RETURNS: None.
   */
  VOID (*AudioIsRepeat)( tngAUDIO_SOURCE *ASrc, BOOL IsRepeat );

  /* Audio setting roll off factor to source function.
   * ARGUMENTS:
   *   - pointer to audio source:
   *       tngAUDIO_SOURCE *ASrc;
   *   - roll off factor:
   *       FLT RollOff;
   * RETURNS: None.
   */
  VOID (*AudioRollOffSet)( tngAUDIO_SOURCE *ASrc, FLT RollOff );

  /* Audio setting reference distance to source function.
   * ARGUMENTS:
   *   - pointer to audio source:
   *       tngAUDIO_SOURCE *ASrc;
   *   - roll off factor:
   *       FLT RefDist;
   * RETURNS: None.
   */
  VOID (*AudioRefDistSet)( tngAUDIO_SOURCE *ASrc, FLT RefDist );

  /* Audio setting maximum distance of audio to source function.
   * ARGUMENTS:
   *   - pointer to audio source:
   *       tngAUDIO_SOURCE *ASrc;
   *   - max distance:
   *       FLT Dist;
   * RETURNS: None.
   */
  VOID (*AudioDistanceMaxSet)( tngAUDIO_SOURCE *ASrc, FLT Dist );


} tngSUBSYSTEM_AUDIO;

/* Audio subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemAudioInit( VOID );

/* Audio subsystem destructor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TNG_SubsystemAudioDestructor( VOID );

#endif /* __tng_audio_h_ */
