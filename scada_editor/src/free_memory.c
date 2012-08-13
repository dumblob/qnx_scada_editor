/* Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-08-11 11:06:03 CEST */

#include "free_memory.h"
#include "dataloader.h"
#include <assert.h>

void freeAllMemory()
{
  /* free tables, xpaths, etc. */
  freeInnerStructures((PtGenTreeItem_t *)PtTreeRootItem(ABW_tree_wgt));

  /* free ABW_tree_wgt content */
  //FIXME wtf, why does it not work (SIGSEGV)?
  //PtTreeFreeAllItems(ABW_tree_wgt);

  //void PtTreeRemoveList(PtWidget_t *tree, PtTreeItem_t *first);
  //PtTreeItem_t *PtTreeRootItem(PtTreeWidget_t const *tree);
  //int PtTreeFreeItems( PtTreeItem_t *item );

  //PtTreeItem_t *tmp = PtTreeRootItem(ABW_tree_wgt);
  //PtTreeRemoveList(ABW_tree_wgt, tmp);
  //PtTreeFreeItems(tmp);
}

void freeInnerStructures(PtGenTreeItem_t *gen)
{
  while (gen != NULL)
  {
    t_table_data *data = ((PtTreeItem_t *)gen)->data;

    if (data != NULL)
    {
      /* no further nesting */
      if (gen->son == NULL)
      {
        assert(data->table != NULL);
        int col_max = tblLastCol(data->table);
        int x;

        /* free the structures in the first table line */
        for (x = 0; x <= col_max; ++x)
        {
          t_xml_info *info = NULL;
          tblGetCellResource(data->table, x, 0, Pt_ARG_POINTER, &info, 0);
          assert(info != NULL);
          assert(info->source != NULL);
          xmlFree(info->source);
          free(info);
        }

        PtDestroyWidget(data->table);
      }
      /* nest deeper */
      else
      {
        assert(data->table == NULL);
        freeInnerStructures(gen->son);
      }

      assert(data->xpath != NULL);
      xmlFree(data->xpath);
      free(data);
    }

    gen = gen->brother;
  }
}
