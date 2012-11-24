/* Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-08-11 11:06:03 CEST */

#ifndef NDEBUG
#include <fcntl.h>
#include <stdio.h>
#endif
#include <stdbool.h>
#include <assert.h>
#include "free_memory.h"
#include "dataloader.h"
#include "global_vars.h"
#include "libxml/globals.h"

extern struct scada_ed_global_vars_s scada_ed_global_vars;

void freeAllMemory()
{
#ifndef NDEBUG
  fcntl(fileno(stdout), F_SETFL, fcntl(fileno(stdout), F_GETFL) | O_SYNC);
#endif

  /* free tables, xpaths, etc. */
  freeInnerStructures((PtGenTreeItem_t *)PtTreeRootItem(ABW_tree_wgt), false);

  /* no need to free the global list with attr names and values */

//FIXME debug {{{
static short first_call = 1;
if (first_call)
  first_call = 0;
else
  printf("END in freeAllMemory()\n");
//FIXME }}}

  /* can't use following, because of _keey_first */
  //PtGenTreeFreeAllItems(ABW_tree_wgt);
}


void freeTableAndContent(PtWidget_t *tbl)
{
  int col_max = tblLastCol(tbl);
  int x;

  /* free the structures in the first table line */
  for (x = 0; x <= col_max; ++x)
  {
    t_xml_info *info = NULL;
    tblGetCellResource(tbl, x, 0, Pt_ARG_POINTER, &info, 0);
    assert(info != NULL);
    assert(info->source != NULL);
    xmlFree(info->source);
    free(info);
  }

  PtDestroyWidget(tbl);
}


void freeInnerStructures(PtGenTreeItem_t *gen, bool _keep_first)
{
  while (gen != NULL)
  {
    t_table_data *data = ((PtTreeItem_t *)gen)->data;
    assert(data != NULL);//FIXME

    bool keep_first = (_keep_first && gen->father->son == gen);

    /* no further nesting */
    if (gen->son == NULL)
    {
      assert(data->table != NULL);

      if (keep_first)
        tblRemoveRows(data->table, 1, tblLastRow(data->table));
      else
        freeTableAndContent(data->table);
    }
    /* nest deeper */
    else
    {
      assert(data->table == NULL);
      freeInnerStructures(gen->son, keep_first);
    }

    if (!keep_first)
    {
      assert(data->xpath != NULL);
      xmlFree(data->xpath);

      assert(data->enhanced_xpath != NULL);
      xmlFree(data->enhanced_xpath);

      free(data);
    }

    PtGenTreeItem_t *tmp = gen;
    gen = gen->brother;

    if (!keep_first)
    {
      PtTreeRemoveItem(ABW_tree_wgt, (PtTreeItem_t *)tmp);
      PtTreeFreeItems((PtTreeItem_t *)tmp);
    }
  }
}
