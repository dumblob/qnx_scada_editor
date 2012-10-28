/* Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-08-11 11:06:03 CEST */

#include "free_memory.h"
#include "dataloader.h"
#include <assert.h>
#include <stdbool.h>

#include <fcntl.h>  //FIXME debug
#include <stdio.h>  //FIXME debug

void freeAllMemory()
{
  //FIXME debug
  fcntl(fileno(stdout), F_SETFL, fcntl(fileno(stdout), F_GETFL) | O_SYNC);
  /* free tables, xpaths, etc. */
  freeInnerStructures((PtGenTreeItem_t *)PtTreeRootItem(ABW_tree_wgt), false);

//FIXME debug {{{
static short first_call = 1;
if (first_call)
  first_call = 0;
else
  printf("END in freeAllMemory()\n");
//FIXME }}}

  /* free ABW_tree_wgt content */
  /*PtGenTreeFreeAllItems(ABW_tree_wgt);*/
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
      //data->xpath = NULL;//FIXME

      assert(data->enhanced_xpath != NULL);
      xmlFree(data->enhanced_xpath);
      //data->enhanced_xpath = NULL;//FIXME

      free(data);
      //((PtTreeItem_t *)gen)->data = NULL;
      //data = NULL;//FIXME
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
