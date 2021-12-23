/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_units_register.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Unit registration handle declaration module.
 * PROGRAMMER  : CGSG'2018.
 *               Vasilisa Dmitrieva.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

/***
 * Unit creation function references
 ***/

/* Unit default creation function */
tngUNIT * TNG_UnitCreateSK5Default( tngANIM *Ani );

/* Unit particle test creation function */
tngUNIT * TNG_UnitCreateSK5PrtTest( tngANIM *Ani );

/* EH5 default unit creation function */
tngUNIT * TNG_UnitCreateEH5Default( tngANIM *Ani );

/* Test unit PD2 creation function */
tngUNIT * TNG_UnitCreatePD2Default( tngANIM *Ani );

/* Unit topology default creation function */
tngUNIT * TNG_UnitCreateMM2Default( tngANIM *Ani );

/* Unit as4_default creation function */
tngUNIT * TNG_UnitCreateAS4Default( tngANIM *Ani );

/* Unit default creation function */
tngUNIT * TNG_UnitCreateSK3Default( tngANIM *Ani );

/* Unit audio creation function */
tngUNIT * TNG_UnitCreateSK3Audio( tngANIM *Ani );

/* Unit primitive creation function */
tngUNIT * TNG_UnitCreateSK3Primitive( tngANIM *Ani );

/* Unit default creation function */
tngUNIT * TNG_UnitCreateSK4Default( tngANIM *Ani );

/* Unit default creation function */
tngUNIT * TNG_UnitCreateAV3Default( tngANIM *Ani );

/* Unit default creation function */
tngUNIT * TNG_UnitCreateAF5Default( tngANIM *Ani );

/* Unit default creation function */
tngUNIT * TNG_UnitCreateVD6Default( tngANIM *Ani );

/* Unit default creation function */
tngUNIT * TNG_UnitCreateAG6Default( tngANIM *Ani );

/* Unit default creation function */
tngUNIT * TNG_UnitCreateIK5Default( tngANIM *Ani );

/* Space port unit create function */
tngUNIT * TNG_UnitCreateEH5Cosmo( tngANIM *Ani );

/* Lighting demonstration unit creation function. */
tngUNIT * TNG_UnitCreateLightingDemo( tngANIM *Ani );

/* Test unit VG4 helic creation function */
tngUNIT * TNG_UnitCreateVG4Helic( tngANIM *Ani );

/***
 * Unit register
 ***/

/* Unit register array */
tngUNIT_REGISTER_ENTRY TNG_UnitRegister[] =
{
  /* Simple samples */
  {"SK5 Default", TNG_UnitCreateSK5Default},
  {"EH5 Default", TNG_UnitCreateEH5Default},
  {"PD2 Default", TNG_UnitCreatePD2Default},
  {"MM2 Default", TNG_UnitCreateMM2Default},
  {"AS4 Default", TNG_UnitCreateAS4Default},
  {"SK3 Default", TNG_UnitCreateSK3Default},
  {"SK4 Default", TNG_UnitCreateSK4Default},
  {"AV3 Default", TNG_UnitCreateAV3Default},
  {"AF5 Default", TNG_UnitCreateAF5Default},
  {"VD6 Default", TNG_UnitCreateVD6Default},
  {"AG6 Default", TNG_UnitCreateAG6Default},
  {"IK5 Default", TNG_UnitCreateIK5Default},

  /* Model drawing */
  {"VG4 Helic", TNG_UnitCreateVG4Helic},

  /* Tests */
  {"SK5 Particle Test", TNG_UnitCreateSK5PrtTest},
  {"SK3 Audio", TNG_UnitCreateSK3Audio},
  {"SK3 Primitive", TNG_UnitCreateSK3Primitive},
  {"EH5 Cosmo", TNG_UnitCreateEH5Cosmo},

  /* Demonstrations */
  {"Lighting Demo", TNG_UnitCreateLightingDemo}
}; /* End of 'TNG_UnitRegister' array */

/* Unit register size */
#define TNG_UNIT_REGISTER_SIZE (sizeof(TNG_UnitRegister) / sizeof(TNG_UnitRegister[0]))
INT TNG_UnitRegisterSize = TNG_UNIT_REGISTER_SIZE;


/* END OF 'tng_units_register.c' FILE */
