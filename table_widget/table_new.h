/**
 * table widget - new version
 * Jan Pacner xpacne00@stud.fit.vutbr.cz 2013-01-01 14:58:16 CET
 */

#ifndef LOCAL_table_h
#define LOCAL_table_h

#include <stdbool.h>

PtWidget_t *tblInit(PtWidget_t *, int);
PtWidget_t *tblAddCell(PtWidget_t *, PtWidget_t *,
    PtWidgetClassRef_t *, int, PtArg_t *);
bool tblRowsComplete(PtWidget_t *);

//FIXME pouzivam Grid layout, ktery ma pouze parametr "cols", urcujici

//typedef struct {
//  //FIXME tohle budu moct kdykoliv spocitat!
//  //unsigned short rows;
//  /* indicates if the last row is full of widgets */
//  //bool last_row_complete;
//
//  /* count of widgets in the table */
//  //FIXME z toho muzeme spocitat kolik je radku a zdali je posledni radek
//  //  cely/hotovy
//  unsigned short widget_count;
//} tblWidget_t;

/* count of widgets in the table */
//typedef uintptr_t tblWidget_t;

//FIXME doplnit signatury fci

#endif
