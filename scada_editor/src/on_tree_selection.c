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
#include "dataloader.h"

int on_tree_selection(PtWidget_t *widget, ApInfo_t *apinfo,
		PtCallbackInfo_t *cbinfo)
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtTreeItem_t *item, **buf;
	PhPoint_t tblPos;
	PhDim_t tblDim;
	PtWidget_t *tbl = NULL;
	t_table_data *tbldt = NULL;

	tblPos.x = -5000;
	tblPos.y = -5000;
	tblDim.w = ABW_table_pane->area.size.w;
	tblDim.h = ABW_table_pane->area.size.h;

	buf = PtTreeAllItems(ABW_tree_wgt, NULL);

	int i = 0;
	for (i = 0; (item = buf[i]) != NULL; ++i)
	{
		if (item->data != NULL && ((t_table_data *)item->data)->table != NULL) {
			tbldt = item->data;
			tbl = tbldt->table;
			tblSetDim(tbl, tblPos, tblDim);
		}
	}

	free(buf);

	PtTreeCallback_t *cbdata;

	cbdata = cbinfo->cbdata;
	tblPos.x = 0;
	tblPos.y = 0;

	if (cbdata->item->data != NULL &&
			((t_table_data *)cbdata->item->data)->table != NULL) {
		tbldt = cbdata->item->data;
		tbl = tbldt->table;
		tblSetDim(tbl, tblPos, tblDim);
	}

	return (Pt_CONTINUE);
}

