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
#include "datasaver.h"

extern char *filepath;

int save_file_as(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)

{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtFileSelectionInfo_t info;
	int k;


	memset(&info, 0x0, sizeof(PtFileSelectionInfo_t));
	info.num_args = 0;

	k = PtFileSelection(NULL, /* parent */
	NULL, /* pos */
	"Save As", /* title */
	"~", /*  root_dir, tilde is the home directory specified by $HOME */
	NULL, /*  file_spec filter */
	"Save", /*  label of btn1, the Open button,	 default is "Open" */
	NULL, /*  label of btn2, the Cancel button,	 default is "Cancel" */
	NULL, /*  Pt_ARG_FS_FORMAT resource of the PtFileSel widget, default is "nsd" */
	&info, /* PtFileSelectionInfo_t *info structure, must be specified */
	Pt_FSR_CONFIRM_EXISTING | Pt_FSR_SHOW_HIDDEN | Pt_FSR_NO_FCHECK /* PtFileSelection flags */
	);

	if (k) {
		fprintf(stderr, "\nPtFileSelection failed.\n");
		return (Pt_CONTINUE);
	}

	if (info.ret == Pt_FSDIALOG_BTN1) {

		if (filepath != NULL) {
			free(filepath);
		}
		filepath = (char*) malloc(sizeof(info.path));
		strcpy(filepath, info.path);
		save_data();

	}

	return (Pt_CONTINUE);

}

