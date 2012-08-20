/* Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-08-11 11:06:03 CEST */

#include "free_memory.h"
#include "dataloader.h"
#include <assert.h>
#include <stdbool.h>

void freeAllMemory()
{
  /* free tables, xpaths, etc. */
  freeInnerStructures((PtGenTreeItem_t *)PtTreeRootItem(ABW_tree_wgt), false);

  /* free ABW_tree_wgt content */
  //FIXME wtf, why does it not work (SIGSEGV)?
  //PtGenTreeFreeAllItems(ABW_tree_wgt);

  /* really nasty hack to overcome the issue above (also not functional) */
  //PhPoint_t pos;
  //PhDim_t dim;
  //PtGetResource(ABW_tree_wgt, Pt_ARG_POS, &pos, 0);
  //PtGetResource(ABW_tree_wgt, Pt_ARG_DIM, &dim, 0);

  //PtDestroyWidget(ABW_tree_wgt);

  //PtArg_t args[2];
  //PtSetArg(&args[0], Pt_ARG_POS, &pos, 0);
  //PtSetArg(&args[1], Pt_ARG_DIM, &dim, 0);
  //ABW_tree_wgt = PtCreateWidget(PtTree, ABW_base, 2, args);

  //PtReRealizeWidget(ABW_tree_wgt);

  /* try to remove the list itself by hand (also not functional) */
  //PtTreeItem_t *tmp = PtTreeRootItem(ABW_tree_wgt);

  //if (tmp != NULL)
  //{
  //  PtTreeRemoveList(ABW_tree_wgt, tmp);
  //  PtTreeFreeItems(tmp);
  //}
  //else
//printf("NO tree root item found!\n");//FIXME
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


void freeInnerStructures(PtGenTreeItem_t *gen, bool keep_first)
{
  while (gen != NULL)
  {
    t_table_data *data = ((PtTreeItem_t *)gen)->data;
    bool keep_first_final = (keep_first && gen->father->son == gen);

    assert(data != NULL);//FIXME
    {
      /* no further nesting */
      if (gen->son == NULL)
      {
        assert(data->table != NULL);

        if (keep_first_final)
          tblRemoveRows(data->table, 1, tblLastRow(data->table));
        else
          freeTableAndContent(data->table);
      }
      /* nest deeper */
      else
      {
        assert(data->table == NULL);
        freeInnerStructures(gen->son, keep_first_final);
      }

      if (!keep_first_final)
      {
        assert(data->xpath != NULL);
printf("volnuji %s\n", data->xpath);//FIXME
        xmlFree(data->xpath);
        data->xpath = NULL;//FIXME
        assert(data->enhanced_xpath != NULL);
printf("volnuji enhanced %s\n", data->enhanced_xpath);//FIXME
        xmlFree(data->enhanced_xpath);
        data->enhanced_xpath = NULL;//FIXME
        free(data);
        data = NULL;//FIXME
printf("END\n");//FIXME
      }
    }

    PtGenTreeItem_t *tmp = gen;
    gen = gen->brother;

    if (!keep_first_final)
    {
      PtTreeRemoveItem(ABW_tree_wgt, (PtTreeItem_t *)tmp);
      PtTreeFreeItems((PtTreeItem_t *)tmp);
    }
  }
}
