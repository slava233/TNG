/***************************************************************
 * Copyright (C) 2019
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tng_stock.c
 * PURPOSE     : Tough Nature Graphics project.
 *               Stock module.
 *               Stock fuctions.
 * PROGRAMMER  : CGSG'2018.
 *               Ilya Kuzmin.
 * LAST UPDATE : 19.04.2019.
 * NOTE        : Module prefix 'TNG'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tng.h"

#include <string.h>

/* Init new block stock help fuction.
 * ARGUMENTS:
 *   - stock for add:
 *       STOCK *Stk;
 *   - prev (last) block:
 *       STOCK_BLOCK *OldBlock;
 * RETURNS:
 *  (VOID *) pointer to new block.
 */
static STOCK_BLOCK * InitNewBlock( STOCK *Stk, STOCK_BLOCK *OldBlock )
{
  STOCK_BLOCK *NewBlock, *SaveFirstBlock; /* Store for blocks */
  FREE_ELEMENT *NewFreeElement;           /* Store new element */
  INT i;                                  /* Iterator */

  /* Init block */
  NewBlock = malloc(Stk->SizeOfBlock);
  NewBlock->Next = NULL;
  if (Stk->FirstBlock == Stk->FirstFreeBlock && Stk->FirstBlock != NULL)
  {
    SaveFirstBlock = Stk->FirstBlock;
    Stk->FirstFreeBlock->Next = NewBlock;
    Stk->FirstFreeBlock = Stk->FirstFreeBlock->Next;
    Stk->FirstBlock = SaveFirstBlock;
  }
  else if (OldBlock != NULL)
  {
    OldBlock->Next = NewBlock;
    OldBlock = OldBlock->Next;
  }
  NewBlock->FirstBlockElement = (STOCK_ELEMENT *)((BYTE *)NewBlock + sizeof(STOCK_BLOCK));
  memset(NewBlock->FirstBlockElement, 0, Stk->SizeOfElement * Stk->NumOfElements);

  /* Init free elements list */
  for (i = 0; i < Stk->NumOfElements; i++)
  {
    NewFreeElement = malloc(sizeof(FREE_ELEMENT));
    NewFreeElement->Element = (STOCK_ELEMENT *)((BYTE *)NewBlock->FirstBlockElement + i * Stk->SizeOfElement);
    NewFreeElement->Next = NULL;

    if (Stk->LastFreeElement != NULL)
    {
      Stk->LastFreeElement->Next = NewFreeElement;
      Stk->LastFreeElement = Stk->LastFreeElement->Next;
    }
    else
    {
      Stk->FirstFreeElement = NewFreeElement;
      Stk->LastFreeElement = Stk->FirstFreeElement;
    }
  }

  return NewBlock;
} /* End of 'InitNewBlock' function */

/* Init new element stock help fuction.
 * ARGUMENTS:
 *   - stock for add:
 *       STOCK *Stk;
 *   - item to be add:
 *       VOID *Item;
 * RETURNS:
 *  (VOID *) pointer to new element.
 */
static STOCK_ELEMENT * InitNewElement( STOCK *Stk, VOID *Item )
{
  STOCK_ELEMENT *NewElement;          /* Pointer to new element */
  FREE_ELEMENT *SaveFirstFreeElement; /* Store first free element */
  
  /* Init new element */
  NewElement = Stk->FirstFreeElement->Element;
  NewElement->Validator = STOCK_VALIDATOR;
  NewElement->SelfStockPointer = Stk;
  if (Stk->LastElement != NULL)
  {
    Stk->LastElement->Next = NewElement;
    NewElement->Prev = Stk->LastElement;
    Stk->LastElement = Stk->LastElement->Next;
  }
  else
  {
    Stk->FirstElement = NewElement;
    Stk->LastElement = Stk->FirstElement;
  }
  NewElement->Next = NULL;
  memcpy(NewElement->Data, Item, Stk->SizeOfItem);

  /* Move pointer */
  SaveFirstFreeElement = Stk->FirstFreeElement;
  Stk->FirstFreeElement = Stk->FirstFreeElement->Next;
  if (Stk->FirstFreeElement == NULL)
    Stk->LastFreeElement = Stk->FirstFreeElement;
  free(SaveFirstFreeElement);

  /* Increment elements number */
  Stk->Size++;

  return NewElement;
} /* End of 'InitNewElement' function */

/* Add item to stock fuction.
 * ARGUMENTS:
 *   - stock for add:
 *       STOCK *Stk;
 *   - item to be add:
 *       VOID *Item;
 * RETURNS:
 *  (VOID *) pointer to added element in stock.
 */
VOID * StockAdd( STOCK *Stk, VOID *Item )
{
  STOCK_ELEMENT *Added; /* Pointer to new element */

  /* Check parameters */
  if (Stk == NULL || Item == NULL)
    return NULL;

  /* Start stock init */
  if (Stk->FirstBlock == NULL)
  {
    /* Start init blocks */
    Stk->FirstBlock = InitNewBlock(Stk, NULL);
    Stk->FirstFreeBlock = Stk->FirstBlock;
  }
  else if (Stk->FirstFreeElement == NULL)
  {
    Stk->LastFreeElement = NULL;
    Stk->FirstFreeBlock = InitNewBlock(Stk, Stk->FirstFreeBlock);
  }
  /* Init first element */
  Added = InitNewElement(Stk, Item);

  return Added->Data;
} /* End of 'StockAdd' function */

/* Deferred delete item from stock fuction.
 * ARGUMENTS:
 *   - pointer to item to be remove:
 *       VOID *Item;
 * RETURNS: None.
 */
VOID StockRemove( VOID *Item )
{
  STOCK_ELEMENT *ItemElement; /* Item's element */
  STOCK *CurrentStock;        /* Current stock */
  DELETED_ELEMENT *NewStore;  /* New store */

  /* Check parameter */
  if (Item == NULL)
    return;

  /* Init */
  ItemElement = (STOCK_ELEMENT *)((BYTE *)Item - sizeof(STOCK_ELEMENT));
  if (ItemElement->Validator != STOCK_VALIDATOR)
    return;
  CurrentStock = ItemElement->SelfStockPointer;

  /* Fill delete elements store */
  if (CurrentStock->FirstDeleteElement != NULL)
  {
    NewStore = malloc(sizeof(DELETED_ELEMENT));
    NewStore->Deleted = ItemElement;
    NewStore->Next = NULL;
    CurrentStock->LastDeleteElement->Next = NewStore;
    CurrentStock->LastDeleteElement = CurrentStock->LastDeleteElement->Next;
  }
  else
  {
    CurrentStock->FirstDeleteElement = malloc(sizeof(DELETED_ELEMENT));
    CurrentStock->FirstDeleteElement->Deleted = ItemElement;
    CurrentStock->FirstDeleteElement->Next = NULL;
    CurrentStock->LastDeleteElement = CurrentStock->FirstDeleteElement;
  }

  /* Decrement elements number */
  CurrentStock->Size--;
} /* End of 'StockRemove' function */

/* Delete item from stock fuction.
 * ARGUMENTS:
 *   - pointer to item to be delete:
 *       VOID *Item;
 * RETURNS: None.
 */
VOID StockDelete( VOID *Item )
{
  STOCK_ELEMENT *ItemElement; /* Item's element */
  STOCK *CurrentStock;        /* Current stock */
  FREE_ELEMENT *NewFreeElement;

  /* Check parameter */
  if (Item == NULL)
    return;

  /* Init */
  ItemElement = (STOCK_ELEMENT *)((BYTE *)Item - sizeof(STOCK_ELEMENT));
  if (ItemElement->Validator != STOCK_VALIDATOR)
    return;
  CurrentStock = ItemElement->SelfStockPointer;

  /* Init free elements */
  NewFreeElement = malloc(sizeof(FREE_ELEMENT));
  NewFreeElement->Element = ItemElement;
  NewFreeElement->Next = NULL;
  if (CurrentStock->LastFreeElement != NULL)
  {
    CurrentStock->LastFreeElement->Next = NewFreeElement;
    CurrentStock->LastFreeElement = CurrentStock->LastFreeElement->Next;
  }
  else
  {
    CurrentStock->FirstFreeElement = NewFreeElement;
    CurrentStock->LastFreeElement = CurrentStock->FirstFreeElement;
  }

  /* Check for first stock element and first free stock element pointers */
  if (ItemElement == CurrentStock->FirstElement)
    CurrentStock->FirstElement = ItemElement->Next;
  if (ItemElement == CurrentStock->LastElement)
    CurrentStock->LastElement = ItemElement->Prev;

  /* Delete element */
  if (ItemElement->Prev != NULL)
    ItemElement->Prev->Next = ItemElement->Next;
  if (ItemElement->Next != NULL)
    ItemElement->Next->Prev = ItemElement->Prev;
  if (CurrentStock->StrategyDelete != NULL)
    CurrentStock->StrategyDelete(ItemElement->Data);
  memset(ItemElement, 0, CurrentStock->SizeOfElement);

  /* Decrement elements number */
  CurrentStock->Size--;
} /* End of 'StockDelete' function */

/* Release stock fuction.
 * ARGUMENTS:
 *   - stock to be release:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockRelease( STOCK *Stk )
{
  DELETED_ELEMENT
    *CurrentDeleteElement, /* Pointer to current element for delete */
    *Save;                 /* Save element for delete */
  FREE_ELEMENT *NewFreeElement;

  /* Check parameter */
  if (Stk == NULL || Stk->FirstBlock == NULL)
    return;

  /* Init current element */
  CurrentDeleteElement = Stk->FirstDeleteElement;

  if (CurrentDeleteElement == NULL || CurrentDeleteElement->Deleted == NULL)
    return;

  /* Delete elements with TRUE flag to delete */
  while (CurrentDeleteElement != NULL)
  {
    /* Init free elements */
    NewFreeElement = malloc(sizeof(FREE_ELEMENT));
    NewFreeElement->Element = CurrentDeleteElement->Deleted;
    NewFreeElement->Next = NULL;
    if (Stk->LastFreeElement != NULL)
    {
      Stk->LastFreeElement->Next = NewFreeElement;
      Stk->LastFreeElement = Stk->LastFreeElement->Next;
    }
    else
    {
      Stk->FirstFreeElement = NewFreeElement;
      Stk->LastFreeElement = Stk->FirstFreeElement;
    }

    /* Check for first stock element and first free stock element pointers */
    if (CurrentDeleteElement->Deleted == Stk->FirstElement)
      Stk->FirstElement = CurrentDeleteElement->Deleted->Next;
    if (CurrentDeleteElement->Deleted == Stk->FirstElement)
      Stk->LastElement = CurrentDeleteElement->Deleted->Prev;

    /* Delete element */
    if (CurrentDeleteElement->Deleted == NULL)
      return;
    if (CurrentDeleteElement->Deleted->Prev != NULL)
      CurrentDeleteElement->Deleted->Prev->Next = CurrentDeleteElement->Deleted->Next;
    if (CurrentDeleteElement->Deleted->Next != NULL)
      CurrentDeleteElement->Deleted->Next->Prev = CurrentDeleteElement->Deleted->Prev;
    if (Stk->StrategyDelete != NULL)
      Stk->StrategyDelete(CurrentDeleteElement->Deleted->Data);
    memset(CurrentDeleteElement->Deleted, 0, Stk->SizeOfElement);

    /* Move pointer to current delete element */
    CurrentDeleteElement = CurrentDeleteElement->Next;
  }

  /* Clear deleted elements store */
  while (Stk->FirstDeleteElement != NULL)
  {
    Save = Stk->FirstDeleteElement;
    Stk->FirstDeleteElement = Stk->FirstDeleteElement->Next;
    free(Save);
  }
} /* End of 'StockRelease' function */

/* Return pointer to first item fuction.
 * ARGUMENTS:
 *   - stock for return:
 *       STOCK *Stk;
 * RETURNS:
 *   (VOID *) pointer to first item.
 */
VOID * StockStart( STOCK *Stk )
{
  /* Check parameter */
  if (Stk == NULL || Stk->FirstBlock == NULL)
    return NULL;

  return Stk->FirstElement->Data;
} /* End of 'StockStart' function */

/* Move pointer to next item function.
 * ARGUMENTS:
*   - item to be move:
 *       VOID *Item;
 * RETURNS: 
 *   (VOID *) pointer to current item.
 */
VOID * StockNext( VOID *Item )
{
  STOCK_ELEMENT *ItemElement; /* Item's element */

  /* Check parameter */
  if (Item == NULL)
    return NULL;

  /* Init */
  ItemElement = (STOCK_ELEMENT *)((BYTE *)Item - sizeof(STOCK_ELEMENT));
  if (ItemElement->Validator != STOCK_VALIDATOR)
    return NULL;

  return ItemElement->Next == NULL ? NULL : ItemElement->Next->Data;
} /* End of 'StockNext' function */

/* Get item by index function.
 * ARGUMENTS:
 *   - stock for get:
 *       STOCK *Stk;
 *   - index:
 *       INT Index;
 * RETURNS:
 *   (VOID *) item.
 */
VOID * StockGet( STOCK *Stk, INT Index )
{
  INT i;                          /* Element's iterator */
  STOCK_ELEMENT *CurrentElement;  /* Pointer to current element */

  /* Check parameter */
  if (Stk == NULL || Stk->FirstBlock == NULL)
    return NULL;

  /* Start init current element */
  CurrentElement = Stk->FirstBlock->FirstBlockElement;
  /* Find need element by index */
  for (i = 0; i < Index; i++)
    CurrentElement = CurrentElement->Next;

  return CurrentElement->Data;
} /* End of 'StockGet' function */

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
VOID StockInit( STOCK *Stk, INT NumOfElements, INT SizeOfType, VOID (*StrategyDelete)( VOID *Item ) )
{
  /* Check parameter */
  if (Stk == NULL)
    return;

  /* Init sizes */
  Stk->SizeOfItem = SizeOfType;
  Stk->SizeOfElement = Stk->SizeOfItem + sizeof(STOCK_ELEMENT);
  Stk->NumOfElements = (NumOfElements > 0 ? NumOfElements : 30);
  Stk->SizeOfBlock = sizeof(STOCK_BLOCK) + Stk->SizeOfElement * Stk->NumOfElements;
  Stk->Size = 0;

  /* Set NULL to pointers */
  Stk->FirstFreeElement = Stk->LastFreeElement = NULL;
  Stk->FirstDeleteElement = Stk->LastDeleteElement = NULL;
  Stk->FirstElement = Stk->LastElement = NULL;
  Stk->FirstBlock = Stk->FirstFreeBlock = NULL; 

  /* Init delete method */
  Stk->StrategyDelete = StrategyDelete;
} /* End of 'StockInit' function */

/* Clear stock function.
 * ARGUMENTS:
 *   - stock to be clear:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockClear( STOCK *Stk )
{
  /* Store for delete */
  STOCK_BLOCK *DeleteBlock;
  FREE_ELEMENT *DeleteFreeElement;
  DELETED_ELEMENT *DeleteElement;

  /* Check parameter */
  if (Stk == NULL || Stk->FirstBlock == NULL)
    return;

  /* Delete elements */
  if (Stk->StrategyDelete != NULL)
    while (Stk->FirstElement != NULL)
    {
      Stk->StrategyDelete(Stk->FirstElement->Data);
      Stk->FirstElement = Stk->FirstElement->Next;
    }

  /* Clear blocks */
  while (Stk->FirstBlock != NULL)
  {
    DeleteBlock = Stk->FirstBlock;
    Stk->FirstBlock = Stk->FirstBlock->Next;
    free(DeleteBlock);
  }

  /* Clear lists */
  while (Stk->FirstFreeElement != NULL)
  {
    DeleteFreeElement = Stk->FirstFreeElement;
    Stk->FirstFreeElement = Stk->FirstFreeElement->Next;
    free(DeleteFreeElement);
  }
  while (Stk->FirstDeleteElement != NULL)
  {
    DeleteElement = Stk->FirstDeleteElement;
    Stk->FirstDeleteElement = Stk->FirstDeleteElement->Next;
    free(DeleteElement);
  }
} /* End of 'StockClear' function */

/* Empty stock function.
 * ARGUMENTS:
 *   - stock to be empty:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID StockEmpty( STOCK *Stk )
{
  /* Store delete block */
  STOCK_BLOCK *SaveFirstBlock;
  FREE_ELEMENT *NewFreeElement, *DeleteFreeElement;
  INT i;

  /* Check parameter */
  if (Stk == NULL || Stk->FirstBlock == NULL)
    return;

  /* Delete elements */
  if (Stk->StrategyDelete != NULL)
    while (Stk->FirstElement != NULL)
    {
      Stk->StrategyDelete(Stk->FirstElement->Data);
      Stk->FirstElement = Stk->FirstElement->Next;
    }
  Stk->FirstElement = Stk->LastElement = NULL;
  Stk->FirstDeleteElement = Stk->LastDeleteElement = NULL;

  /* Clear free elements before delete */
  while (Stk->FirstFreeElement != NULL)
  {
    DeleteFreeElement = Stk->FirstFreeElement;
    Stk->FirstFreeElement = Stk->FirstFreeElement->Next;
    free(DeleteFreeElement);
  }
  Stk->FirstFreeElement = Stk->LastFreeElement = NULL;

  /* Init free elements */
  SaveFirstBlock = Stk->FirstBlock;
  while (SaveFirstBlock != NULL)
  {
    for (i = 0; i < Stk->NumOfElements; i++)
    {
      NewFreeElement = malloc(sizeof(FREE_ELEMENT));
      NewFreeElement->Element = (STOCK_ELEMENT *)((BYTE *)SaveFirstBlock->FirstBlockElement + i * Stk->SizeOfElement);
      NewFreeElement->Next = NULL;

      if (Stk->LastFreeElement != NULL)
      {
        Stk->LastFreeElement->Next = NewFreeElement;
        Stk->LastFreeElement = Stk->LastFreeElement->Next;
      }
      else
      {
        Stk->FirstFreeElement = NewFreeElement;
        Stk->LastFreeElement = Stk->FirstFreeElement;
      }
    }
    SaveFirstBlock = SaveFirstBlock->Next;
  }

  /* Clear blocks */
  SaveFirstBlock = Stk->FirstBlock;
  while (SaveFirstBlock != NULL)
  {
    memset(SaveFirstBlock->FirstBlockElement, 0, Stk->SizeOfElement * Stk->NumOfElements);
    SaveFirstBlock = SaveFirstBlock->Next;
  }

  /* Zeroing elements number */
  Stk->Size = 0;
} /* End of 'StockEmpty' function */

/* END OF 'tng_stock.c' FILE */

