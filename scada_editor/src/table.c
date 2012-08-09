/**
 * table widget
 * Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-05-17 18:32:27 CEST
 */

#include <assert.h>
#include <stdbool.h>
#include "table.h"

int tblDestructionCB(PtWidget_t *w, void *data, PtCallbackInfo_t *cbinfo)
{
  tblWidget_t *tbl = NULL;
  cbinfo = cbinfo; data = data;

  /* destroy widgets */
  if (!tblSetSize(w, 0, 0))
    return Pt_END;  /* Pt_HALT */

  if (PtGetResource(w, Pt_ARG_POINTER, &tbl, 0) != 0)
    return Pt_END;

  free(tbl);

  return( Pt_CONTINUE );
}

PtWidget_t *tblInit(PtWidget_t *parent, PhPoint_t pos, PhDim_t dim)
{
  int c = 0;
  PtArg_t argsScrCon[6];
  tblWidget_t *tbl;
  PtCallback_t callbacks[1];

  assert(parent);

  if ((tbl = (tblWidget_t *)malloc(sizeof(tblWidget_t))) == NULL)
    PtExit(EXIT_FAILURE);

  memset(tbl, 0, sizeof(tblWidget_t));
  tbl->geom = pos;
  tbl->dim = dim;

  /* automatically scroll container resize both X and Y */
  PtSetArg(&argsScrCon[c++], Pt_ARG_SCROLLCONT_RESIZE_FLAGS,
      Pt_RESIZE_XY_ALWAYS, 0);
  PtSetArg(&argsScrCon[c++], Pt_ARG_BASIC_FLAGS,
      Pt_FLAT_FILL      |
      Pt_TOP_OUTLINE    |
      Pt_BOTTOM_OUTLINE |
      Pt_LEFT_OUTLINE   |
      Pt_RIGHT_OUTLINE, 0);
  PtSetArg(&argsScrCon[c++], Pt_ARG_POS, &pos, 0);
  PtSetArg(&argsScrCon[c++], Pt_ARG_DIM, &dim, 0);
  /* save the table into "data" in this container */
  PtSetArg(&argsScrCon[c++], Pt_ARG_POINTER, tbl, 0);

  callbacks[0].event_f = tblDestructionCB;
  callbacks[0].data = NULL;
  PtSetArg(&argsScrCon[c++], Pt_CB_DESTROYED, callbacks,
      sizeof(callbacks)/sizeof(callbacks[0]));

  /* another widget as a child of a parent */
  return PtCreateWidget(PtScrollContainer, parent, c, argsScrCon);
}

int tblSetSize(PtWidget_t *scrollCon, int cols, int rows)
{
  tblWidget_t *tbl = NULL;
  int i, j;

  assert(cols >= 0 && rows >= 0);

  if (PtGetResource(scrollCon, Pt_ARG_POINTER, &tbl, 0) != 0)
    return 0;

  /* destroy all sub-widgets */
  for (i = 0; i < tbl->cols; i++)
  {
    for (j = 0; j < tbl->rows; j++)
    {
      PtDestroyWidget(tbl->m[i][j]);
    }

    free(tbl->m[i]);
    free(tbl->m_type[i]);
  }

  free(tbl->m);
  free(tbl->m_type);

  if ((tbl->cols = cols) == 0 ||
      (tbl->rows = rows) == 0)
    return 1;

  if ((tbl->m = (PtWidget_t ***)malloc(
          sizeof(PtWidget_t **) * tbl->cols)) == NULL)
    PtExit(EXIT_FAILURE);

  if ((tbl->m_type = (PtWidgetClassRef_t ***)malloc(
          sizeof(PtWidgetClassRef_t **) * tbl->cols)) == NULL)
    PtExit(EXIT_FAILURE);

  for (i = 0; i < tbl->cols; i++)
  {
    if ((tbl->m[i] = (PtWidget_t **)calloc(
        tbl->rows, sizeof(PtWidget_t *))) == NULL)
      PtExit(EXIT_FAILURE);

    if ((tbl->m_type[i] = (PtWidgetClassRef_t **)calloc(
        tbl->rows, sizeof(PtWidgetClassRef_t *))) == NULL)
      PtExit(EXIT_FAILURE);
  }

  return 1;
}

int tblLastIndex(PtWidget_t *scrollCon, short row)
{
  tblWidget_t *tbl = NULL;

  if (PtGetResource(scrollCon, Pt_ARG_POINTER, &tbl, 0) != 0)
    PtExit(EXIT_FAILURE);

  if (row)
    return tbl->rows -1;
  else
    return tbl->cols -1;
}

int tblSetDim(PtWidget_t *scrollCon, PhPoint_t pos, PhDim_t dim)
{
  int c = 0;
  PtArg_t argsScrCon[2];
  tblWidget_t *tbl = NULL;

  if (PtGetResource(scrollCon, Pt_ARG_POINTER, &tbl, 0) != 0)
    return 0;

  tbl->geom = pos;
  tbl->dim = dim;

  PtSetArg(&argsScrCon[c++], Pt_ARG_POS, &pos, 0);
  PtSetArg(&argsScrCon[c++], Pt_ARG_DIM, &dim, 0);
  PtSetResources(scrollCon, c, argsScrCon);

  return 1;
}

int tblExeOnCellArea(PtWidget_t *scrollCon,
    int tlx, int tly, int brx, int bry,
    PtWidgetClassRef_t *class, int n_args, PtArg_t const *args)
{
  tblWidget_t *tbl = NULL;
  bool on_unset = false;
  int i, j;

  int c;
  PtArg_t argsCell[4];
  PhPoint_t pos;
  PhDim_t dim;

  if (PtGetResource(scrollCon, Pt_ARG_POINTER, &tbl, 0) != 0)
    return 0;

  assert(tlx < tbl->cols && tly < tbl->rows &&
         brx < tbl->cols && bry < tbl->rows);
  assert(class == NULL           ||
         class == PtButton       ||
         class == PtText         ||
         class == PtNumericFloat ||
         class == PtNumericInteger);

  if (args == NULL)
    assert(n_args == 0);
  else
    assert(n_args > 0);

  /* we will be setting all unset cells */
  if (tlx < 0 || tly < 0 || brx < 0 || bry < 0)
  {
    on_unset = true;
    tlx = 0;
    tly = 0;
    brx = tbl->cols -1;
    bry = tbl->rows -1;
  }

  /* FIXME nasty hack needed for proper compilation */
  i = tlx; j = tly;

  for (tly = j; tly <= bry; tly++)
  {
    for (tlx = i; tlx <= brx; tlx++)
    {
      c = 0;

      if (on_unset)
      {
        /* current cell is unset */
        if (tbl->m[tlx][tly] == NULL)
        {
          /* create new widget */
          tbl->m[tlx][tly] = PtCreateWidget(class, scrollCon, n_args, args);
          tbl->m_type[tlx][tly] = class;
        }
      }
      else
      {
        if (class == NULL)
        {
          if (args == NULL)
          {
            /* destroy widget */
            PtDestroyWidget(tbl->m[tlx][tly]);
            tbl->m[tlx][tly] = NULL;
          }
          else
          {
            /* update widget's (if any) properties */
            /* FIXME if (tbl->m[tlx][tly] == NULL) ...??? */
            PtSetResources(tbl->m[tlx][tly], n_args, args);
          }
        }
        else
        {
          /* destroy widget and create a new one with desired properties */
          PtDestroyWidget(tbl->m[tlx][tly]);

          /* create new widget */
          tbl->m[tlx][tly] = PtCreateWidget(class, scrollCon, n_args, args);
          PtSetArg(&argsCell[c++], Pt_ARG_BASIC_FLAGS,
          /*PtSetResource(tbl->m[tlx][tly], Pt_ARG_BASIC_FLAGS,*/
              Pt_TOP_OUTLINE    |
              Pt_BOTTOM_OUTLINE |
              Pt_LEFT_OUTLINE   |
              Pt_RIGHT_OUTLINE  |
              ((class == PtButton)
                ? Pt_STATIC_GRADIENT | Pt_TOP_BEVEL
                : Pt_FLAT_FILL
              ), 0);
          tbl->m_type[tlx][tly] = class;
        }
      }

      /* rewrite user args POS + DIM for sure */
      if (tbl->m[tlx][tly] != NULL)
      {
        pos.x = tlx * (TBL_INIT_CELL_WIDTH -1);
        pos.y = tly * TBL_INIT_CELL_HEIGHT;

        dim.w = TBL_INIT_CELL_WIDTH;
        dim.h = TBL_INIT_CELL_HEIGHT;

        PtSetArg(&argsCell[c++], Pt_ARG_POS, &pos, 0);
        PtSetArg(&argsCell[c++], Pt_ARG_MINIMUM_DIM, &dim, 0);
        PtSetArg(&argsCell[c++], Pt_ARG_MAXIMUM_DIM, &dim, 0);

        PtSetResources(tbl->m[tlx][tly], c, argsCell);
      }
    }
  }

  /* hide this widget (but NOT his descendants) */
  PtUnrealizeWidget(scrollCon);
  /* display widget and all his descendants */
  PtRealizeWidget(scrollCon);

  return 1;
}

int tblGetCellResource(PtWidget_t *scrollCon, int col, int row,
    long type, void *value, long len)
{
  tblWidget_t *tbl = NULL;

  if (PtGetResource(scrollCon, Pt_ARG_POINTER, &tbl, 0) != 0)
    return 0;

  assert(col >= 0 && col < tbl->cols &&
         row >= 0 && row < tbl->rows);

  return PtGetResource(tbl->m[col][row], type, value, len);
}

int tblSetColWidth(PtWidget_t *scrollCon, int col, int size)
{
  col = col; size = size;
  scrollCon = scrollCon;

  return 1;
}

int tblSetRowHeight(PtWidget_t *scrollCon, int row, int size)
{
  row = row; size = size;
  scrollCon = scrollCon;

  return 1;
}
