/**
 * testing QNX Photon GUI
 * Author: Jan Pacner xpacne00@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <Pt.h>
#include <time.h>
#include "main_testing.h"
#include "table.h"

int push_button_cb00(PtWidget_t *w, void *data, PtCallbackInfo_t *cbinfo)
{
  char *cell_text = NULL;
  int c = 0;
  PtArg_t args[1];
  PtWidget_t *ww = (PtWidget_t *)data;

  w = w; cbinfo = cbinfo;
  printf( "BT 00 was pushed\n" );
  tblGetCellResource(ww, 0, 0, Pt_ARG_TEXT_STRING, &cell_text, 0);
  printf("OBSAH cell 0,0: %s\n", cell_text);
  tblSetSize(ww, 3, 4);
  tblExeOnCellArea(ww,
      0, 0,
      tblLastCol(ww), 0,
      PtNumericFloat, 0, NULL);
  tblExeOnCellArea(ww,
      0, 1,
      tblLastCol(ww), 1,
      PtButton, 0, NULL);
  tblExeOnCellArea(ww,
      0, 2,
      tblLastCol(ww), 2,
      PtNumericInteger, 0, NULL);
  tblExeOnCellArea(ww,
      -10, 3,
      tblLastCol(ww), 3,
      PtText, 0, NULL);

  /* change labels/content on the fly */
  PtSetArg(&args[c++], Pt_ARG_TEXT_STRING, "testovaci popisek bez newline", 0);
  tblExeOnCellArea(ww,
      tblLastCol(ww), 0,
      tblLastCol(ww), tblLastRow(ww),
      NULL, c, args);

  /*PtExit( EXIT_SUCCESS );*/
  return Pt_CONTINUE;
}

int push_button_cb01(PtWidget_t *w, void *data, PtCallbackInfo_t *cbinfo)
{
  int x, y, c = 0;
  PtArg_t args[1];
  PtWidget_t *ww = (PtWidget_t *)data;
  char arr[50];

  w = w; cbinfo = cbinfo;
  printf( "BT 01 was pushed\n" );
  srand(time(NULL));
  sprintf(arr, "%d", rand());
  PtSetArg(&args[c++], Pt_ARG_TEXT_STRING, arr, 0);

  if (!tblGetFocusedCoord(ww, &x, &y))
  {
    x = 5; y = 1;
  }

  tblAddRowsBefore(ww, (y < 0) ? 0 : y, 2, PtButton, c, args);
  /*tblAddRowsBefore(ww, tblLastRow(ww), 1, PtButton, c, args);*/
  /*tblAddRowsAfter(ww, 0, 1, PtButton, c, args);*/
  /*tblAddRowsAfter(ww, tblLastRow(ww), 2, PtButton, c, args);*/

  /* NOTE, when the table is empty (i.e. contains no cells), tblSetSize()
     must be used first, because tblAddRows() doesn't know anything about
     the width (i.e. the column count) of the new table! */

  return Pt_CONTINUE;
}

int push_button_cb02(PtWidget_t *w, void *data, PtCallbackInfo_t *cbinfo)
{
  PtWidget_t *ww = (PtWidget_t *)data;

  w = w; cbinfo = cbinfo;
  printf( "BT 02 was pushed\n" );
  /* NOTE, when the table is empty, this will be a wrong call (the given
     indices will not exist => assertion failed */
  tblRemoveRows(ww, 1, tblLastRow(ww) -1);

  return Pt_CONTINUE;
}

int main( int argc, char *argv[] )
{
  int n;
  PtWidget_t *window = NULL;
  PtArg_t args[4];
  PtCallback_t callbacks[1];
  PhPoint_t btnPos;

  /* table (scroll widget) */
  PtWidget_t *tbl = NULL;
  PhPoint_t tblPos;
  PhDim_t tblDim;

  argc = argc;
  argv = argv;

  /* attach channel to Photon server */
  if (PtInit(NULL) == -1) PtExit(EXIT_FAILURE);

  /* class, parent, list elements count, list of init values for widget
     resources */
  window = PtCreateWidget(PtWindow, Pt_NO_PARENT, 0, NULL);

  /* create new table */
  tblPos.x = 70; tblPos.y = 90;
  tblDim.w = 200; tblDim.h = 150;
  tbl = tblInit(window, tblPos, tblDim);
  tblSetSize(tbl, 4, 5);

  /* reset table geometry */
  tblPos.x = 90; tblPos.y = 150;
  tblDim.w = 500; tblDim.h = 400;
  tblSetDim(tbl, tblPos, tblDim);
  tblExeOnCellArea(tbl,
      0, -1,
      tblLastCol(tbl), tblLastRow(tbl),
      PtText, 0, NULL);

  /* BUTTON 00 */
  callbacks[0].event_f = push_button_cb00;
  callbacks[0].data = tbl;
  n = 0; btnPos.x = 0; btnPos.y = 0;
  /* macro; setup init values list */
  PtSetArg(&args[n++], Pt_ARG_POS, &btnPos, 0);
  PtSetArg(&args[n++], Pt_ARG_TEXT_STRING,
      "Change table!", 0);
  PtSetArg(&args[n++], Pt_CB_ACTIVATE, callbacks,
      sizeof(callbacks)/sizeof(callbacks[0]));
  /* another widget as a child of window */
  PtCreateWidget(PtButton, window, n, args);

  /* BUTTON 01 */
  callbacks[0].event_f = push_button_cb01;
  callbacks[0].data = tbl;
  n = 0; btnPos.x = 0; btnPos.y = 20;
  PtSetArg(&args[n++], Pt_ARG_POS, &btnPos, 0);
  /* disable focusing this button */
  PtSetArg(&args[n++], Pt_ARG_FLAGS, Pt_FALSE, Pt_GETS_FOCUS);
  PtSetArg(&args[n++], Pt_ARG_TEXT_STRING,
      "Add some rows (before the focused one).", 0);
  PtSetArg(&args[n++], Pt_CB_ACTIVATE, callbacks,
      sizeof(callbacks)/sizeof(callbacks[0]));
  PtCreateWidget(PtButton, window, n, args);

  /* BUTTON 02 */
  callbacks[0].event_f = push_button_cb02;
  callbacks[0].data = tbl;
  n = 0; btnPos.x = 0; btnPos.y = 40;
  PtSetArg(&args[n++], Pt_ARG_POS, &btnPos, 0);
  PtSetArg(&args[n++], Pt_ARG_TEXT_STRING,
      "Remove some rows.", 0);
  PtSetArg(&args[n++], Pt_CB_ACTIVATE, callbacks,
      sizeof(callbacks)/sizeof(callbacks[0]));
  PtCreateWidget(PtButton, window, n, args);

  PtRealizeWidget(window);

  /* yield control to the Photon widget library */
  PtMainLoop();

  /* will never be reached */
  return EXIT_SUCCESS;
}
