/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_def.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Base definitions.
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
 * LAST UPDATE : 27.03.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_def_h_
#define __tng_def_h_

#include "Stock/tng_stock.h"
#include "Math/mth_def.h"
#include "Scanner/scanner.h"

#pragma warning(disable: 4244 4305)

/* Debug memory allocation support */
#ifdef _DEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((VOID)0)
#endif /* _DEBUG */

#include <stdlib.h>

/* 64-bit integer data types */
typedef __int64 INT64;
typedef unsigned __int64 UINT64;

/* Forward type references */
typedef struct tagtngPRIM tngPRIM;
typedef struct tagtngMATERIAL tngMATERIAL;
typedef struct tagtngTEXTURE tngTEXTURE;
typedef struct tagtngIMAGE tngIMAGE;
typedef enum tagtngTEX_FMT tngTEX_FMT;
typedef struct tagtngSHADER tngSHADER;
typedef struct tagtngANIM tngANIM;
typedef struct tagtngUNIT tngUNIT;

#endif /* __tng_def_h_ */

/* END OF 'tng_def.h' FILE */
