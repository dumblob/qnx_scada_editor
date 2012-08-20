/* Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-08-11 11:06:03 CEST */

#ifndef FREE_MEMORY_H
#define FREE_MEMORY_H

#include "ablibs.h"
#include <stdbool.h>

void freeAllMemory();
void freeTableAndContent(PtWidget_t *);
void freeInnerStructures(PtGenTreeItem_t *, bool);

#endif
