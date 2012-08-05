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





int showFileSelector(PtFileSelectionInfo_t *);

int open_file(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo)

{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

		int dataf = -1;
		PtFileSelectionInfo_t datafile;
		dataf = showFileSelector(&datafile);
		//formatf = showFileSelector(&formatfile);

		if(dataf == -1){
			return (Pt_CONTINUE);
		}else{
			//le_open_wrapper();
		}

		fprintf(stderr, "qq \n");

	return (Pt_CONTINUE);

}


int showFileSelector(PtFileSelectionInfo_t *info){

			//PtFileSelectionInfo_t info;
			int k;
			memset(info, 0x0, sizeof(PtFileSelectionInfo_t));
			(*info).num_args = 0;

			k = PtFileSelection(NULL, /* parent */
			NULL, /* pos */
			"Open file", /* title */
			"~", /*  root_dir, tilde is the home directory
			 specified by $HOME */
			NULL, /*  file_spec filter */
			NULL, /*  label of btn1, the Open button,
			 default is "Open" */
			NULL, /*  label of btn2, the Cancel button,
			 default is "Cancel" */
			NULL, /*  Pt_ARG_FS_FORMAT resource of the
			 PtFileSel widget, default is "nsd" */
			info, /* PtFileSelectionInfo_t *info
			 structure, must be specified */
			Pt_FSR_SHOW_HIDDEN | Pt_FSR_NO_FCHECK /* PtFileSelection flags */
			);
			return k;


}
