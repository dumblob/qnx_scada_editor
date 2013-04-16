/**
 * table widget
 * Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-05-17 18:32:27 CEST
 */

/* FIXME missing features:
   posun fokusu mezi bunkami (stejneho typu?) jinak nez tabulatorem
     napr. pomoci Ctrl+sipky
   prohazovani radku a sloupcu
   PtCreateWidgetClass()
   use Pt_ARG_USER_DATA instead of Pt_ARG_POINTER (useful for PhAB)
   tblSetDim()
     @param type of column width resize (TBL_RESIZE_COL...)
       proportionally (de|in)crease width of all columns
       (de|in)increase last column width to fit the size
       do not touch the actual column width
       set the initial/compiled_in values
     @param type of row height resize (TBL_RESIZE_ROW...)
       see above
     #define TBL_RESIZE_COL_PROPORT  0
     #define TBL_RESIZE_COL_LAST     1
     #define TBL_RESIZE_COL_UNTOUCH  2
     #define TBL_RESIZE_COL_INIT     3
     #define TBL_RESIZE_ROW_PROPORT  0
     #define TBL_RESIZE_ROW_LAST     1
     #define TBL_RESIZE_ROW_UNTOUCH  2
     #define TBL_RESIZE_ROW_INIT     3
   PhPoint_t tblGetPos(PtWidget_t *);
   PhDim_t   tblGetDim(PtWidget_t *);
*/

#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <Pt.h>

#define TBL_INIT_CELL_WIDTH 100  /* pixels */
#define TBL_INIT_CELL_HEIGHT 26  /* pixels */
#define tblLastCol(ptr) tblLastIndex((ptr), false)
#define tblLastRow(ptr) tblLastIndex((ptr), true)
#define tblAddRowsBefore(a, b, c, d, e, f) \
  tblAddRows((a), true, (b), (c), (d), (e), (f))
#define tblAddRowsAfter(a, b, c, d, e, f) \
  tblAddRows((a), false, (b), (c), (d), (e), (f))

typedef struct
{
  int cols, rows;               /* item count                             */
  PhPoint_t geom;               /* geometry (position) in px              */
  PhDim_t dim;                  /* dimensions in px                       */
  PtWidget_t ***m;              /* matrix with widgets                    */
  PtWidgetClassRef_t ***m_type; /* matrix with corresponding widget types
                                   (Pt_ARG_USER_DATA would be overhead)   */
} tblWidget_t;

/**
 * table widget destruction callback
 * @param calling widget
 * @param data (NULL means "not used")
 * @param cbinfo (not used)
 * @return Pt_CONTINUE if OK; otherwise Pt_END
 */
int tblDestructionCB(PtWidget_t *, void *, PtCallbackInfo_t *);

/**
 * table widget initialization
 * @param parent widget/window
 * @param top left point of the widget
 * @param dimensions
 * @return ScrollContainer containing tbl widget if OK; otherwise NULL
 */
PtWidget_t *tblInit(PtWidget_t *, PhPoint_t, PhDim_t);

/**
 * change table widget size (number of rows/columns)
 *   Note: It will destroy existing widgets.
 * @param ScrollContainer containing tbl widget
 * @param new column count; if value < 0, leave it untouched
 * @param new row count; if value < 0, leave it untouched
 * @return 1 if OK; otherwise 0
 */
int tblSetSize(PtWidget_t *, int, int);

/**
 * get last column/row index
 * @param ScrollContainer containing tbl widget
 * @param which index to return; 0 column, 1 row
 * @return last column or row index
 */
int tblLastIndex(PtWidget_t *, bool);

/**
 * change table widget dimensions
 * @param ScrollContainer containing tbl widget
 * @param new table widget top left point
 * @param new table widget dimensions
 * @return 1 if OK; otherwise 0
 */
int tblSetDim(PtWidget_t *, PhPoint_t, PhDim_t);

/**
 * set the cell content for all cells in the area denoted by two cells
 *   Note: If one of x or y (no matter top or bottom) is < 0, the
 *   execution is performed on all "unset" cells.
 * @param ScrollContainer containing tbl widget
 * @param top left cell x
 * @param top left cell y
 * @param bottom right cell x
 * @param bottom right cell y
 * @param class of the new sub-widget
 *   If class is NULL and PtArg_t const * is NULL, cells are cleared (there
 *     will be hole/free_space/nothing).
 *   If class is NULL and PtArg_t const * is not NULL, these arguments are
 *     applied to the existing cell (if any).
 *   If class is not NULL, widget will be (re)created from scratch.
 *   Note: Only PtText PtButton PtNumericFloat PtNumericInteger
 *     widgets are supported.
 * @param number of arguments being passed to the widget library
 * @param pointer to an array containing n_args PtArg_t entries
 *   Note: Don't set any geometry, it will not be used anyway.
 * @return 1 if OK; otherwise 0
 */
int tblExeOnCellArea(PtWidget_t *, int, int, int, int,
    PtWidgetClassRef_t *, int, PtArg_t const *);

/**
 * get given property from cell
 * @param ScrollContainer containing tbl widget
 * @param column index
 * @param row index
 * @param type (e.g. Pt_ARG_COLOR)
 * @param address of the appropriate pointer (to store the value)
 * @param length of the resource type (used primarily for arrays)
 * @return 1 if OK; otherwise 0
 */
int tblGetCellResource(PtWidget_t *, int, int, long, void *, long);

/**
 * set column width (and shift the rest if needed)
 * @param ScrollContainer containing tbl widget
 * @param which column (index) to resize
 * @param the new size
 * @return 1 if OK; otherwise 0
 */
int tblSetColWidth(PtWidget_t *, int, int);

/**
 * set row height (and shift the rest if needed)
 * @param ScrollContainer containing tbl widget
 * @param which row (index) to resize
 * @param the new size
 * @return 1 if OK; otherwise 0
 */
int tblSetRowHeight(PtWidget_t *, int, int);

/**
 * get coordinates of the focused cell
 *   Note: One of col/row index could be NULL, but not both!
 * @param ScrollContainer containing tbl widget
 * @param column index
 * @param row index
 * @return 1 if focused widget found in tbl; 0 if not found
 */
int tblGetFocusedCoord(PtWidget_t *, int *, int *);

/**
 * add row before/behind the given row index
 * @param ScrollContainer containing tbl widget
 * @param TBL_BEFORE_ROW/TBL_AFTER_ROW
 * @param row index (the "fixed point")
 * @param how many rows to add
 * @param widget class (see tblExeOnCellArea())
 * @param number of arguments (see tblExeOnCellArea())
 * @param pointer to an array (see tblExeOnCellArea())
 * @return 1 if OK; otherwise 0
 */
int tblAddRows(PtWidget_t *, bool, int, int,
    PtWidgetClassRef_t *, int, PtArg_t const *);

/**
 * remove rows
 * @param ScrollContainer containing tbl widget
 * @param row index from
 * @param row index to
 * @return 1 if OK; otherwise 0
 */
int tblRemoveRows(PtWidget_t *, int, int);

/**
 * give focus to cell
 * @param ScrollContainer containing tbl widget
 * @param col index
 * @param row index
 * @return focused widget ptr (see PtGiveFocus())
 */
PtWidget_t *tblFocusCell(PtWidget_t *, int, int);

#endif
