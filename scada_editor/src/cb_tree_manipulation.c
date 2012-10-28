/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

#include "stdbool.h"
#include "dataloader.h"
#include "assert.h"
#include "free_memory.h"
#include "global_vars.h"

#define SCADA_EDITOR_MSG_NEW_ID "Write down a new, unique id:"
#define BUF_MAX 128

extern struct scada_editor_global_vars_s scada_editor_global_vars;

enum en_manipulate_tree_action
{
  SCADA_EDITOR_ADD_ITEM_BEFORE,
  SCADA_EDITOR_ADD_ITEM_AFTER,
  SCADA_EDITOR_RENAME_ITEM
};

int newSkeleton(PtGenTreeItem_t *recent, PtGenTreeItem_t *pattern)
{
  t_table_data *pattern_data = (t_table_data *)((PtTreeItem_t *)pattern)->data;
  assert(pattern_data != NULL);
  PtWidget_t *tbl;

  /* nest deeper */
  if (pattern_data->table == NULL)
  {
    PtTreeItem_t *item = PtTreeAllocItem(ABW_tree_wgt, "1", -1, -1);
    PtTreeAddFirst(ABW_tree_wgt, item, recent);
    newSkeleton(&item->gen, pattern->son);
    tbl = NULL;
  }
  /* we are at the bottom */
  else
  {
    tbl = tblInit(ABW_table_pane, (PhPoint_t){0, 0}, ABW_table_pane->area.size);
    int col_max = tblLastCol(pattern_data->table);
    tblSetSize(tbl, col_max +1, 1);

    int i;
    for (i = 0; i <= col_max; ++i)
    {
      t_xml_info *pattern_info = NULL;
      tblGetCellResource(pattern_data->table, i, 0, Pt_ARG_POINTER,
          &pattern_info, 0);

      t_xml_info *info = NULL;
      if ((info = (t_xml_info *)malloc(sizeof(t_xml_info))) == NULL)
        PtExit(EXIT_FAILURE);
      info->source = xmlStrdup(pattern_info->source);
      info->type = pattern_info->type;

      char *pattern_label = NULL;
      tblGetCellResource(pattern_data->table, i, 0, Pt_ARG_TEXT_STRING,
          &pattern_label, 0);

      setHeaderCell(tbl, i, 0, PtButton,
          (char *)xmlStrdup(BAD_CAST pattern_label), info);
    }
  }

  ((PtTreeItem_t *)recent)->data = newTableData(tbl,
    xmlStrdup(pattern_data->xpath), scada_editor_global_vars.l_head);

  PtTreeExpand(ABW_tree_wgt, (PtTreeItem_t *)recent, NULL);

  return Pt_CONTINUE;
}

void manipulateTree(enum en_manipulate_tree_action action)
{
  /* get selected items */
  PtTreeItem_t **buf = PtTreeSelectedItems(ABW_tree_wgt, NULL);

  /* if something is selected and not in the highest level */
  if (buf[0] != NULL && buf[0]->gen.father != NULL)
  {
    char answer[BUF_MAX];
    char *msg = SCADA_EDITOR_MSG_NEW_ID;

    /* prefill with current content (name) */
    if (action == SCADA_EDITOR_RENAME_ITEM)
    {
      strncpy(answer, buf[0]->string, BUF_MAX -1);
      answer[BUF_MAX -1] = '\0';
    }
    else
    {
      memset(answer, 0, BUF_MAX);
    }

    for (;;)
    {
      int ret = PtPrompt(
          ABW_base, /* PtWidget_t *parent */
          NULL, /* PhPoint_t const *location */
          (SCADA_EDITOR_RENAME_ITEM) ? "Rename item" : "New item", /* char const *title */
          NULL, /* PhImage_t const *image */
          msg, /* char const *message */
          NULL, /* char const *msgFont */
          2, /* int btnCount */
          (const char *[]){"&Ok", "&Cancel"}, /* char const **buttons */
          NULL, /* char const **btnFonts */
          1, /* int defBtn */
          2, /* int escBtn */
          BUF_MAX, /* short textLength */
          answer, /* char *text */
          NULL, /* char const *textFont */
          &(PhDim_t){170, 30}, /* const *text_dim */
          Pt_CENTER | Pt_BLOCK_ALL | Pt_RELATIVE); /* int flags */

      if (ret == 1)
      {
        if (*answer == '\0')
        {
          msg = "Given id must contain at least 1 character!\n"
                SCADA_EDITOR_MSG_NEW_ID;
        }
        else
        {
          bool found = false;
          PtGenTreeItem_t *gen = buf[0]->gen.father->son;

          /* prove for uniqueness */
          while (gen != NULL)
          {
            if (!strcmp(((PtTreeItem_t *)gen)->string, answer))
            {
              found = true;
              break;
            }

            gen = gen->brother;
          }

          if (found)
          {
            msg = "Given id already exists!\n"
                  SCADA_EDITOR_MSG_NEW_ID;
          }
          else
          {
            if (action == SCADA_EDITOR_RENAME_ITEM)
            {
              PtTreeModifyItemString(ABW_tree_wgt, buf[0], answer);
            }
            else
            {
              PtTreeItem_t *item = PtTreeAllocItem(ABW_tree_wgt, answer, -1, -1);
              gen = buf[0]->gen.father->son;

              if (action == SCADA_EDITOR_ADD_ITEM_AFTER)
              {
                PtTreeAddAfter(ABW_tree_wgt, item, gen);
              }
              /* SCADA_EDITOR_ADD_ITEM_BEFORE, current item is the first one */
              else if (gen == &buf[0]->gen)
              {
                PtTreeAddFirst(ABW_tree_wgt, item,
                    (PtTreeItem_t *)buf[0]->gen.father);
              }
              /* SCADA_EDITOR_ADD_ITEM_BEFORE */
              else
              {
                while (gen->brother != &buf[0]->gen) gen = gen->brother;

                PtTreeAddAfter(ABW_tree_wgt, item, gen);
              }

              newSkeleton(&item->gen, &buf[0]->gen);
              PtTreeSelect(ABW_tree_wgt, item);
            }

            break;
          }
        }
      }
      else
      {
        break;
      }
    }
  }

  free(buf);
}

int addTreeItemBefore(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  /* eliminate 'unreferenced' warnings */
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;

  manipulateTree(SCADA_EDITOR_ADD_ITEM_BEFORE);

  return Pt_CONTINUE;
}


int addTreeItemAfter(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;

  manipulateTree(SCADA_EDITOR_ADD_ITEM_AFTER);

  return Pt_CONTINUE;
}


int renameTreeItem(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;

  manipulateTree(SCADA_EDITOR_RENAME_ITEM);

  return Pt_CONTINUE;
}


int removeTreeItem(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;

  PtTreeItem_t **buf = PtTreeSelectedItems(ABW_tree_wgt, NULL);

  if (buf[0] != NULL && buf[0]->gen.father != NULL)
  {
    int ret = PtAlert(
        ABW_base, /* PtWidget_t *parent */
        NULL, /* PhPoint_t const *location */
        "Remove item", /* char const *title */
        NULL, /* PhImage_t const *image */
        "Are you sure you want to remove the selected item\nand its descendants?", /* char const *message */
        NULL, /* char const *msgFont */
        2, /* int btnCount */
        (const char *[]){"&Yes", "&No"}, /* char const **buttons */
        NULL, /* char const **btnFonts */
        2, /* int defBtn */
        2, /* int escBtn */
        Pt_CENTER | Pt_BLOCK_ALL | Pt_RELATIVE); /* int flags */

    if (ret == 1)
    {
      /* save brother if any, otherwise the last sibling in the list */
      PtGenTreeItem_t *to_focus = buf[0]->gen.brother;

      if (to_focus == NULL)
      {
        to_focus = buf[0]->gen.father->son;
        assert(to_focus != NULL);

        while (to_focus->brother != NULL &&
               to_focus->brother != &buf[0]->gen)
          to_focus = to_focus->brother;
      }

      /* gonna remove a singleton => always keep first item in first
         subtree of first subtree of first subtree of ... */
      bool keep_first = (&buf[0]->gen == buf[0]->gen.father->son &&
          buf[0]->gen.brother == NULL);

      freeInnerStructures(buf[0]->gen.son, keep_first);

      t_table_data *data = (t_table_data *)buf[0]->data;

      /* we are at the bottom */
      if (buf[0]->gen.son == NULL)
      {
        /* in case, children was removed by freeInnerStructures()
           => we are not the deepest item with table and therefore
           the table pointer can be NULL */
        if (data->table != NULL)
        {
          if (keep_first)
            tblRemoveRows(data->table, 1, tblLastRow(data->table));
          else
            freeTableAndContent(data->table);
        }
      }

      if (!keep_first)
      {
        assert(data->xpath != NULL);
        xmlFree(data->xpath);
        assert(data->enhanced_xpath != NULL);
        xmlFree(data->enhanced_xpath);
        free(data);

        PtTreeRemoveItem(ABW_tree_wgt, buf[0]);
        PtTreeFreeItems(buf[0]);
      }

      PtTreeSelect(ABW_tree_wgt, to_focus);
    }
  }

  free(buf);

  return Pt_CONTINUE;
}
