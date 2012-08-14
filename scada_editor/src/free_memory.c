/* Jan Pacner xpacne00@stud.fit.vutbr.cz 2012-08-11 11:06:03 CEST */

#include "free_memory.h"
#include "dataloader.h"
#include <assert.h>

void freeAllMemory()
{
  /* free tables, xpaths, etc. */
  //freeInnerStructures((PtGenTreeItem_t *)PtTreeRootItem(ABW_tree_wgt));

  /* free ABW_tree_wgt content */
  //FIXME wtf, why does it not work (SIGSEGV)?
  //PtGenTreeFreeAllItems(ABW_tree_wgt);

  /* really nasty hack to overcome the issue above */
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
        /* occurs when there is a new file */
        if (data->table != NULL)
        {
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
