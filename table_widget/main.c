/**
 * table widget demo
 * Author: Jan Pacner xpacne00@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <Pt.h>
#include "table.h"

int main(int argc, char *argv[])
{
  PtWidget_t *tbl = NULL, window = NULL;
  PhPoint_t tbl_pnt;
  PhDim_t tbl_dim;
  PtArg_t args[1];

  /* attach channel to Photon server */
  if (PtInit(NULL) == -1) PtExit(EXIT_FAILURE);

  /* blank window (auto resizing according to content) */
  window = PtCreateWidget(PtWindow, Pt_NO_PARENT, 0, NULL);

  /* table initialization with geometry */
  tbl_pnt.x = 0; tbl_pnt.y = 0;
  tbl_dim.w = 200; tbl_dim.h = 150;
  if ((tbl = tblInit(window, tbl_pnt, tbl_dim)) == NULL)
    PtExit(EXIT_FAILURE);

  /* 3x3 table */
  if (!tblSetSize(tbl, 3, 3)) PtExit(EXIT_FAILURE);

  /* first row will be empty PtButtons, indexing from 0 like C */
  if (!tblExeOnCellArea(tbl, 0,                    0,
                             tblLastColIndex(tbl), 0,
                             PtButton, 0, NULL))
    PtExit(EXIT_FAILURE);

  /* set the last row to PtNumericIntegers */
  if (!tblExeOnCellArea(tbl, 0,                    tblLastRowIndex(tbl),
                             tblLastColIndex(tbl), tblLastRowIndex(tbl),
                             PtNumericInteger, 0, NULL))
    PtExit(EXIT_FAILURE);

  /* the rest of the table will be text with the following default value */
  PtSetArg(&args[0], Pt_ARG_TEXT_STRING,
      "some really long text, that does not fit into", 0);
  if (!tblExeOnCellArea(tbl, -1, 0, 0, 0, PtText, 1, args))
    PtExit(EXIT_FAILURE);

  /* display widget and all his descendants */
  PtRealizeWidget(window);

  /* change button name on the fly (from callbacks etc. too) */
  PtSetArg(&args[0], Pt_ARG_TEXT_STRING, "2. column 1. row", 0);
  if (!tblExeOnCellArea(tbl, 1, 0, 1, 0, NULL, 1, args))
    PtExit(EXIT_FAILURE);

  /* yield control to the Photon widget library */
  PtMainLoop();

  /* table destruction */
  tblDestroy(tbl);

  return (EXIT_SUCCESS);
}
