/* Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-08-11 07:51:28 CEST */

#include "cb_row_manipulation.h"

int cb_add_row_before(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  /* eliminate 'unreferenced' warnings */
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;
  PtWidget_t *tbl;

  /* get the visible table */
  PtTreeItem_t *tree_item = PtTreeGetCurrent(ABW_tree_wgt);

  /* is some tree item selected? */
  if (tree_item != NULL)
  {
    /* there are items which does not have any tables (these are used
       only for navigation purposes) */
    if (tree_item->data != NULL &&
        (tbl = ((t_table_data *)tree_item->data)->table) != NULL)
    {
      int col, row;

      if (!tblGetFocusedCoord(tbl, &col, &row))
      {
        /* add to the beginning of the table (first line are labels) */
        tblAddRowsAfter(tbl, 0, ROWS_TO_ADD, NULL, 0, NULL);
        row = 1;
      }
      else
      {
        tblAddRowsBefore(tbl, row, ROWS_TO_ADD, NULL, 0, NULL);
      }

      t_xml_info *info = NULL;
      int col_max = tblLastCol(tbl);
      int i, j;

      for (j = row; j < row + ROWS_TO_ADD; ++j)
        for (i = 0; i <= col_max; ++i)
        {
          tblGetCellResource(tbl, i, 0, Pt_ARG_POINTER, &info, 0);
          assert(info != NULL);
          setTypeAndContentOfCell(tbl, i, j, "", info->type);
        }

      /* give focus to the first cell in the last added row */
      tblFocusCell(tbl, 0, j -1);
    }
  }

  return Pt_CONTINUE;
}

int cb_add_row_after(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;
  PtWidget_t *tbl;

  PtTreeItem_t *tree_item = PtTreeGetCurrent(ABW_tree_wgt);

  if (tree_item != NULL)
  {
    if (tree_item->data != NULL &&
        (tbl = ((t_table_data *)tree_item->data)->table) != NULL)
    {
      int col, row;

      if (!tblGetFocusedCoord(tbl, &col, &row))
      {
        /* add to the very end of the table */
        row = tblLastRow(tbl);
      }

      tblAddRowsAfter(tbl, row, ROWS_TO_ADD, NULL, 0, NULL);

      t_xml_info *info = NULL;
      int col_max = tblLastCol(tbl);
      int i, j;

      for (j = row +1; j <= row + ROWS_TO_ADD; ++j)
        for (i = 0; i <= col_max; ++i)
        {
          tblGetCellResource(tbl, i, 0, Pt_ARG_POINTER, &info, 0);
          assert(info != NULL);
          setTypeAndContentOfCell(tbl, i, j, "", info->type);
        }

      /* give focus to the first cell in the first added row */
      tblFocusCell(tbl, 0, row +1);
    }
  }

  return Pt_CONTINUE;
}

int cb_rem_row(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;
  PtWidget_t *tbl;

  PtTreeItem_t *tree_item = PtTreeGetCurrent(ABW_tree_wgt);

  if (tree_item != NULL)
  {
    if (tree_item->data != NULL &&
        (tbl = ((t_table_data *)tree_item->data)->table) != NULL)
    {
      int col, row;

      /* if no row selected, remove the last one */
      if (!tblGetFocusedCoord(tbl, &col, &row)) row = tblLastRow(tbl);

      /* first line are cell names */
      if (row == 0) return Pt_CONTINUE;

      /* remove from to */
      tblRemoveRows(tbl, row, row);

      int tmp = tblLastRow(tbl);
      tmp = (row > 0 && row < tmp) ? row : tmp;

      /* give focus to the first cell in line right after the removed one */
      if (tmp > 0) tblFocusCell(tbl, 0, tmp);
    }
  }

  return Pt_CONTINUE;
}
