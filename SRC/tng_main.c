/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_main.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Startup module.
 * PROGRAMMER  : CGSG'2018.
 *               Pavel Dvas,
 *               Maxim Markov,
 *               Svyatoslav Kuralenok,
 *               Alexandr Vashpanov,
 *               Svyatoslav Kononov,
 *               Andrey Shayda,
 *               Ilya Kuzmin,
 *               Evgenii Hlevnoi,
 *               Sofya Kopeykina,
 *               Artem Fabrika,
 *               Alexander Girvits,
 *               Vasilisa Dmitrieva,
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 15.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  INT i;
  CHAR Buf[1000] = {0}, *StrPoint2, Path[300];
  INT CountOfModuleMembers;

  SetDbgMemHooks();
  TNG_AnimInit();

  GetCurrentDirectory(sizeof(Path), Path);
  strncat(Path, "\\TNG.INI", sizeof(Path) - strlen(Path) - 1);

  /* Load Units */
  CountOfModuleMembers = GetPrivateProfileSection("Unit",
    Buf, sizeof(Buf) / sizeof(Buf[0]), Path);
  StrPoint2 = Buf;
  if (CountOfModuleMembers != 0)
  {
    while (*StrPoint2 != 0 || *(StrPoint2 + 1) != 0)
    {
      if (*StrPoint2 == '=')
      {
        StrPoint2++;
        for (i = 0; i < TNG_UnitRegisterSize; i++)
          if (strcmp(TNG_UnitRegister[i].UnitName, StrPoint2) == 0)
          {
            TNG_AnimUnitAdd(TNG_UnitRegister[i].UnitCreate(TNG()));
            break;
          }
          while (*(StrPoint2 + 1) != 0)
            StrPoint2++;
      }
      StrPoint2++;
    }
  }

  /* Load UnitSets */
  CountOfModuleMembers = GetPrivateProfileSection("UnitSet",
    Buf, sizeof(Buf) / sizeof(Buf[0]), Path);
  StrPoint2 = Buf;
  if (CountOfModuleMembers != 0)
    {
    while (*StrPoint2 != 0 || *(StrPoint2 + 1) != 0)
    {
      if (*StrPoint2 == '=')
      {
        StrPoint2++;
        TNG_AnimUnitAdd(TNG()->UnitSetCreate(StrPoint2));
        while (*(StrPoint2 + 1) != 0)
          StrPoint2++;
      }
      StrPoint2++;
    }
  }
  TNG_AnimRun();
  return 30;
} /* End of 'WinMain' function */

/* END OF 'tng_main.c' FILE */
