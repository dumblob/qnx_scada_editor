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
#include "filepicker.h"
#include "datasaver.h"

int export_src(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	char *filepath;
	PtFileSelectionInfo_t info;
		int k;
		memset(&info, 0x0, sizeof(PtFileSelectionInfo_t));
		info.num_args = 0;

		k = showFileSelector(&info, "Export to", "Export");

		if (k) {
			return (Pt_CONTINUE);
		}

		if (info.ret == Pt_FSDIALOG_BTN1) {
			filepath = (char*) malloc(sizeof(info.path));
			strcpy(filepath, info.path);
			exportToSrc(filepath);
			free(filepath);
		}



	return Pt_CONTINUE;
}
