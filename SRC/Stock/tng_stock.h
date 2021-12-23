/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_stock.h
 * PURPOSE     : Tough Nature Graphics project.
 *               Stock module.
 *               Stock fuctions prototypes.
 * PROGRAMMER  : CGSG'2018.
 *               Ilya Kuzmin.
 * LAST UPDATE : 19.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tng_stock_h_
#define __tng_stock_h_

#include <commondf.h>

/* Stock validation value */
#define STOCK_VALIDATOR 0x11235813

/* Macros for init */
#define STOCK_INIT(StockName, NumOfElements, Type, FuncName) StockInit(&StockName, NumOfElements, sizeof(Type), FuncName)

/* Type for store elements in one module */
typedef struct tagSTOCK STOCK;

/* Type for store element in one module */
typedef struct tagSTOCK_ELEMENT STOCK_ELEMENT;
struct tagSTOCK_ELEMENT
{
  STOCK_ELEMENT *Next, *Prev;  /* Pointers to next and prev elements */
  DWORD Validator;             /* Magic number for check element by item */
  STOCK *SelfStockPointer;     /* Pointer to self stock */

  BYTE Data[0];                /* Place to store data */
};

/* Store elements for delete in release */
typedef struct tagDELETED_ELEMENT DELETED_ELEMENT;
struct tagDELETED_ELEMENT
{
  DELETED_ELEMENT *Next;  /* Pointer to next delete element */
  STOCK_ELEMENT *Deleted; /* Pointer to delete element in stock */
};

/* Store pointers to free elements */
typedef struct tagFREE_ELEMENT FREE_ELEMENT;
struct tagFREE_ELEMENT
{
  FREE_ELEMENT *Next;     /* Pointer to next free element */
  STOCK_ELEMENT *Element; /* Pointer to free element in stock */
};

/* Blocks for divede store */
typedef struct tagSTOCK_BLOCK STOCK_BLOCK;
struct tagSTOCK_BLOCK
{
  STOCK_ELEMENT
    *FirstBlockElement;      /* Pointer to first element in block */
  STOCK_BLOCK *Next;         /* Pointer to next block */
};

/* Type for store elements in one module */
struct tagSTOCK
{
  INT
    SizeOfItem,          /* Size of one item */
    SizeOfBlock,         /* Size of one block */
    SizeOfElement,       /* Size of one element */
    NumOfElements,       /* Number of elements in one block */
    Size;                /* Number of elements */
  STOCK_BLOCK
    *FirstBlock,         /* Pointer to first block */
    *FirstFreeBlock;     /* Pointer to first free block */
  STOCK_ELEMENT
    *FirstElement,       /* Pointer to first element */
    *LastElement;        /* Pointer to last element */
  DELETED_ELEMENT
    *FirstDeleteElement, /* Pointer to first element for delete in release */
    *LastDeleteElement;  /* Pointer to last element for delete in release */
  FREE_ELEMENT
    *FirstFreeElement,   /* Pointer to first free element */
    *LastFreeElement;    /* Pointer to last free element */

  /* Items' strategy free function.
   * ARGUMENTS:
   *   - item for clear:
   *       VOID *Item;
   * RETURNS: None.
   */
  VOID (*StrategyDelete)( VOID *Item );
};

/* Stock initialization function.
 * ARGUMENTS:
 *   - stock to be init:
 *       STOCK *Stk;
 *   - elements in one block number:
 *       INT NumOfElements;
 *   - elements type size:
 *       INT SizeOfType;
 *   - pointer to free function:
 *       VOID (*StrategyDelete)( VOID *Item );
 * RETURNS: None.
 */
VOID StockInit( STOCK *Stk, INT NumOfElements, INT SizeOfType, VOID (*StrategyDelete)( VOID *Item ) );

/* Add item to stock fuction.
 * ARGUMENTS:
 *   - stock for add:
 *       STOCK *Stk;
 *   - item to be add:
 *       VOID *Item;
 * RETURNS:
 *  (VOID *) pointer to added element in stock.
 */
VOID * StockAdd( STOCK *Stk, VOID *Item );

/* Deferred delete item from stock fuction.
 * ARGUMENTS:
 *   - pointer to item to be remove:
 *       VOID *Item;
 * RETURNS: None.
 */
VOID StockRemove( VOID *Item );

/* Delete item from stock fuction.
 * ARGUMENTS:
 *   - pointer to item to be delete:
 *       VOID *Item;
 * RETURNS: None.
 */
VOID StockDelete( VOID *Item );

/* Release stock fuction.
 * ARGUMENTS:
 *   - stock to be release:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockRelease( STOCK *Stk );

/* Return pointer to first item fuction.
 * ARGUMENTS:
 *   - stock for return:
 *       STOCK *Stk;
 * RETURNS:
 *   (VOID *) pointer to first item.
 */
VOID * StockStart( STOCK *Stk );

/* Move pointer to next item function.
 * ARGUMENTS:
 *   - item to be move:
 *       VOID *Item;
 * RETURNS: 
 *   (VOID *) pointer to current item.
 */
VOID * StockNext( VOID *Item );

/* Get item by index function.
 * ARGUMENTS:
 *   - stock for get:
 *       STOCK *Stk;
 *   - index:
 *       INT Index;
 * RETURNS:
 *   (VOID *) item.
 */
VOID * StockGet( STOCK *Stk, INT Index );

/* Clear stock function.
 * ARGUMENTS:
 *   - stock to be clear:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockClear( STOCK *Stk );

/* Empty stock function.
 * ARGUMENTS:
 *   - stock to be empty:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockEmpty( STOCK *Stk );

#endif /* __tng_stock_h_ */

/* END OF 'tng_stock.h' FILE */
