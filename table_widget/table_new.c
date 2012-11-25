/**
 * table widget - new version
 * Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-05-17 18:32:27 CEST
 */

#include <assert.h>
#include <stdlib.h>  /* malloc */
#include <stdio.h>
#include <stdbool.h>
#include <Pt.h>
#include "table_new.h"



//FIXME uvolnovani pameti z user_data oblasti vsech widgetu,
//  ktere se v prubehu zivota aplikace odstranuji integrovat
//  primo do jejich callbacku cb_destroy()



//PtWidget_t *tblInit(PtWidget_t *parent, PhPoint_t, PhDim_t);
PtWidget_t *tblInit(PtWidget_t *parent, int cols);
{
}

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
tblAddCell(PtWidget_t *scrollCon, int x, ...stejne_parametry_jako_pro_set_array_of_properties...)
{
  tblWidget_t *tbl = NULL;

  if (PtGetResource(scrollCon, Pt_ARG_POINTER, &tbl, 0))
    PtExit(EXIT_FAILURE);

  Pt...info i;
  getProperty(scrollCon, LAYOUT_INFO, &i);

  //FIXME free() ve freeMemory() a nebo pri cb_scrollCon_destroy()
  if (tbl->bool_row == NULL)
    if ((row = calloc(i->pocet_sloupcu, sizeof(bool))) == NULL)
      PtExit(EXIT_FAILURE);

  //FIXME jak tohle resit?
  do radku y vkladat

  // FIXME draggable separator za kazdym widgetem, ktery neni na konci radku


  int PtWidgetInsert ( PtWidget_t *widget, PtWidget_t *new_sibling, int behind );

  if (x_odpovida_poslednimu_indexu_na_radku && cely_radek_zaplneny)
    t
}

// FIXME using hints
// http://www.qnx.com/developers/docs/6.5.0_sp1/topic/com.qnx.doc.photon_prog_guide/geometry.html?cp=17_0_8_5_3#id35
cb_tblDragSep()
{
}

PtWidget_t *tblFocusedWidget(void)
{
  // kontrola, zdali parent je tabulka
  // kontrola, zdali je tato tabulka zrovna visible

  if (OK)
    return ptr;
  else
    return NULL;
}

/** a little expensive operation */
PtWidget_t *tblCoordToPtr(tbl, int x, int y)
{
  PtWidget_t *PtWidgetBrotherBehind( PtWidget_t *widget);
  PtWidget_t *PtWidgetBrotherInFront( PtWidget_t *widget);

  Pt...info i;
  getProperty(scrollCon, LAYOUT_INFO, &i);

  unsigned int count = 0;

  for (child =; child; child =, count++)
  {
    if ((count / i->pocet_sloupcu) == y &&
        (count % i->pocet_sloupcu) == x) return child;
  }

  return NULL;
}

/** a little expensive operation */
short tblPtrToCoord(tbl, ptr, int &x, int &y)
{
  // viz. vyse

  Pt...info i;
  getProperty(scrollCon, LAYOUT_INFO, &i);

  for (child =; child; child =, count++)
  {
    if (child == ptr)
    {
      *y = count / i->pocet_sloupcu;
      *x = count % i->pocet_sloupcu;
      return 0;
    }
  }

  return 1;
}

//FIXME tyhle NEbudou potreba pro jednoduchost (lze si vytahnout
//  properties primo z vraceneho widgetu)
//short tblSetCellContent(PtWidget_t *cell, int x, int y) { }
//short tblGetCellContent(...) { }

tblAddRow()
{
  //FIXME pozor na race condition, kdyz budu pridavat celou row a budu
  //  mit rozpracovany bool_row!!!
  //  nesmim s temi widgety "z bool_row" vubec hybat - proste
  //  pred ne vlozim novy radek (sice se pri vykresleni stane noc divneho,
  //    ale predpoklada se, ze program ihned pote doplni zbyvajici
  //    widgety do "bool_row" a bude vyreseno)
}

tblRemoveRow()
{
}

tblRowCount()
{
  // vycte pocet hotovych radku ze struktury tbl
  return tbl->rows;
}

tblColCount()
{
  // vycte pouze pocet sloupcu ze struktury info toho scrollContaineru
  Pt...info i;
  getProperty(scrollCon, LAYOUT_INFO, &i);

  return i->pocet_sloupcu;
}
