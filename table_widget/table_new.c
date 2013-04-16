/**
 * table widget - new version
 * Jan Pacner xpacne00@stud.fit.vutbr.cz 2013-01-01 14:58:16 CET
 */

#include <assert.h>
#include <stdlib.h>  /* malloc */
#include <stdio.h>
#include <stdbool.h>
#include <Pt.h>
#include "table_new.h"

// FIXME tohle pouzit pro "skryti" widgetu
// make this widget not interactable with Photon events; ???
//Pt_ARG_FLAGS = Pt_BLOCKED




//FIXME translate each comment into english
//FIXME uvolnovani pameti z user_data oblasti vsech widgetu,
//  ktere se v prubehu zivota aplikace odstranuji integrovat
//  primo do jejich callbacku cb_destroy()




PtWidget_t *tblInit(PtWidget_t *parent, int cols)
{
  assert(parent != NULL && cols > 0);

  PtGridLayoutInfo_t *lInfo;

  if ((lInfo = malloc(sizeof(PtGridLayoutInfo_t))) == NULL)
    PtExit(EXIT_FAILURE);

  //*lInfo = PtGridLayoutInfoDflts;
  lInfo->n_cols = cols;
  lInfo->flags |= Pt_EQUAL_ROWS;
  lInfo->h_spacing = 0;  // space between columns in px
  lInfo->v_spacing = 0;  // space betwenn rows    in px
  lInfo->margin.ul.x = 0; // left
  lInfo->margin.ul.y = 0; // top
  lInfo->margin.lr.x = 0; // right
  lInfo->margin.lr.y = 0; // bottom

  int c = 0;
  PtArg_t args[10];  //FIXME 10

  /*
  //FIXME
  PhPoint_t tblPos;
  PhDim_t tblDim;
  //tblPos.x = 70; tblPos.y = 90;
  tblDim.w = 40; tblDim.h = 40;
  tblPos.x = 0; tblPos.y = 0;
  //tblDim.w = 0; tblDim.h = 0;
  //PtSetArg( &args[c++], Pt_ARG_POS, &tblPos, 0);
  PtSetArg( &args[c++], Pt_ARG_DIM, &tblDim, 0);
  */

  PtSetArg( &args[c++], Pt_ARG_LAYOUT, PtGridLayout, lInfo );
  /* disable any frames */
  PtSetArg( &args[c++], Pt_ARG_BASIC_FLAGS, Pt_FALSE,
      Pt_ALL_ETCHES | Pt_ALL_BEVELS | Pt_ALL_OUTLINES ); //Pt_FLAT_FILL
  /* value of pointer is current count of cells */
  PtSetArg( &args[c++], Pt_ARG_POINTER, 0, 0 );
  PtSetArg( &args[c++], Pt_ARG_SCROLLCONT_RESIZE_FLAGS, Pt_RESIZE_XY_ALWAYS, 0 );
  PtSetArg( &args[c++], Pt_ARG_ANCHOR_FLAGS,
      Pt_BOTTOM_ANCHORED_BOTTOM | Pt_TOP_ANCHORED_TOP |
      Pt_LEFT_ANCHORED_LEFT     | Pt_RIGHT_ANCHORED_RIGHT,
      Pt_BOTTOM_IS_ANCHORED | Pt_TOP_IS_ANCHORED |
      Pt_LEFT_IS_ANCHORED   | Pt_RIGHT_IS_ANCHORED );
  //FIXME pridat 3px right_offset a bottom_offset

  return PtCreateWidget( PtScrollContainer, parent, c, args );
}

/*
// najdi prvni widget a pak prochazej cely seznam
_tblFrontSibling()
{
bud:
  PtWidget_t *PtGetParentWidget( void );
  PtWidget_t *PtWidgetParent( PtWidget_t *widget );
  PtWidget_t *PtValidParent( PtWidget_t *widget_parent,
      PtWidgetClassRef_t *class_ref );
nebo:
  my_newTable_pointer_to_scrollContainer();

  return PtWidget_t *PtWidgetChildFront(PtWidget_t *widget);
  return PtWidget_t *PtWidgetChildBack(PtWidget_t *widget);
}
*/

// FIXME kontrola poradi focusu
// zajistit, aby se okno prekreslilo az teprve po pridani celeho noveho radku!
/* POZOR, tato fce kresli az kdyz je nadefinovany cely radek
     dokud neni nadefinovany, tak si interne uchovava potrebne info */
// NElze zvolit radek - vzdycky se pridava do radku "current_row" +1
/**
 * add cell to the new, uncomplete row (== current_row_count + 1)
 * @return
 *   radek_zaplnen a pocet radku zvysen o 1
 *   radek_neni_jeste_zaplnen
 *   nejaka_chyba
 */
// insert behind the given new_sibling widget
// table, existing_sibling, class, n_args, args
PtWidget_t *tblAddCell(PtWidget_t *scrollCon, PtWidget_t *new_sibling,
    PtWidgetClassRef_t *class, int n_args, PtArg_t *args)
    /* FIXME PtArg_t const *args) */
{
  assert(scrollCon != NULL);

  uintptr_t widget_count;
  if (PtGetResource(scrollCon, Pt_ARG_POINTER, &widget_count, 0))
    PtExit(EXIT_FAILURE);

  // FIXME malloc() ???
  //PtGridLayoutData_t lData = PtGridLayoutDataDflts;
  PtGridLayoutData_t *lData;
  if ((lData = malloc(sizeof(PtGridLayoutData_t))) == NULL)
    PtExit(EXIT_FAILURE);
  lData->flags |= Pt_H_ALIGN_FILL | Pt_V_ALIGN_FILL;
  //PtSetResource(scrollCon, Pt_ARG_LAYOUT_DATA, lData, PtGridLayout);

  PtSetArg( &args[n_args++], Pt_ARG_LAYOUT_DATA, lData, PtGridLayout );
  PtWidget_t *new_cell = PtCreateWidget(class, scrollCon, n_args, args);
  if (new_cell == NULL) return NULL;

  /* reorder due to focus order */
  //FIXME fakt jenom focus a nebo cele umisteni v seznamu vsech widgetu?????
  if (new_sibling != NULL)
    /* 1 behind; 0 in front of */
    PtWidgetInsert ( new_cell, new_sibling, 1 );
  /* else add behind the last widget */

  PtSetResource(scrollCon, Pt_ARG_POINTER, widget_count +1, 0);
  //FIXME
  PtReRealizeWidget(scrollCon);
  return new_cell;

  //FIXME free() ve freeMemory() a nebo pri cb_scrollCon_destroy()
  //ukladat do interni struktury typ widgetu v danem sloupci?
  //if ((row = calloc(i->pocet_sloupcu, sizeof(bool))) == NULL)
  //  PtExit(EXIT_FAILURE);

  // FIXME draggable separator za kazdym widgetem (vcetne toho, ktery je uplne
  //   na konci radku
}

bool tblRowsComplete(PtWidget_t *scrollCon)
{
  uintptr_t widget_count;
  if (PtGetResource(scrollCon, Pt_ARG_POINTER, &widget_count, 0))
    PtExit(EXIT_FAILURE);

  PtGridLayoutInfo_t *lInfo;
  if (PtGetResource(scrollCon, Pt_ARG_LAYOUT_INFO, &lInfo, 0))
    PtExit(EXIT_FAILURE);

  fprintf(stderr, "widget_count %p column_count %d\n",
      (void *)widget_count, lInfo->n_cols);

  return (widget_count % lInfo->n_cols) ? false : true;
}

// FIXME using hints
// http://www.qnx.com/developers/docs/6.5.0_sp1/topic/com.qnx.doc.photon_prog_guide/geometry.html?cp=17_0_8_5_3#id35
void cb_tblDragSep()
{
}

//PtWidget_t *tblFocusedWidget(void)
//{
//  // kontrola, zdali parent je tabulka
//  // kontrola, zdali je tato tabulka zrovna visible
//
//  if (OK)
//    return ptr;
//  else
//    return NULL;
//}
//
///** a little expensive operation */
//PtWidget_t *tblCoordToPtr(tbl, int x, int y)
//{
//  PtWidget_t *PtWidgetBrotherBehind( PtWidget_t *widget);
//  PtWidget_t *PtWidgetBrotherInFront( PtWidget_t *widget);
//
//  Pt...info i;
//  getProperty(scrollCon, LAYOUT_INFO, &i);
//
//  unsigned int count = 0;
//
//  for (child =; child; child =, count++)
//  {
//    if ((count / i->pocet_sloupcu) == y &&
//        (count % i->pocet_sloupcu) == x) return child;
//  }
//
//  return NULL;
//}
//
///** a little expensive operation */
//short tblPtrToCoord(tbl, ptr, int &x, int &y)
//{
//  // viz. vyse
//
//  Pt...info i;
//  getProperty(scrollCon, LAYOUT_INFO, &i);
//
//  for (child =; child; child =, count++)
//  {
//    if (child == ptr)
//    {
//      *y = count / i->pocet_sloupcu;
//      *x = count % i->pocet_sloupcu;
//      return 0;
//    }
//  }
//
//  //FIXME
//  PtWidget_t *cur, *window;
//  int d;
//  cur = window;
//  d = Pt_TRAVERSE_START;
//  while ((d = PtWidgetTree( window, &cur, d )) != Pt_TRAVERSE_DONE)
//    PtSetResources( cur, 1, argt );
//
//  return 0;
//}
//
////FIXME tyhle NEbudou potreba pro jednoduchost (lze si vytahnout
////  properties primo z vraceneho widgetu)
////short tblSetCellContent(PtWidget_t *cell, int x, int y) { }
////short tblGetCellContent(...) { }
//
//tblAddRow()
//{
//  //FIXME pozor na race condition, kdyz budu pridavat celou row a budu
//  //    mit rozpracovany last_row_complete:
//  //  nesmim s temi widgety z "last_row_complete" vubec hybat - proste
//  //    pred ne vlozim novy radek (sice se pri vykresleni nestane nic
//  //    divneho, ale predpoklada se, ze program ihned pote doplni
//  //    zbyvajici widgety do "last_row_complete" a bude vyreseno)
//  //    za posledni radek v pripade rozpracovaneho "last_row_complete"
//  //      NElze nic vlozit (assert())
//}
//
//tblRemoveRow()
//{
//  //FIXME je mozne odstranit i posledni rozpracovany "last_row_complete"
//  //  radek!
//}
//
//tblRowCount()
//{
//  // vycte pocet hotovych radku ze struktury tbl
//  return tbl->rows;
//}
//
//tblColCount()
//{
//  // vycte pouze pocet sloupcu ze struktury info toho scrollContaineru
//  Pt...info i;
//  getProperty(scrollCon, LAYOUT_INFO, &i);
//
//  return i->pocet_sloupcu;
//}
