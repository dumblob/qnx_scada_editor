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

extern char *viewpath;

int
new_file( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;




		int formatf = -1;

		PtFileSelectionInfo_t formatfile;
		formatf = showFileSelector(&formatfile, "Select format file...");


		if (formatf == -1) {
			return (Pt_CONTINUE);
		} else {


			if (viewpath != NULL) {
				free(viewpath);
			}


			viewpath = (char*) malloc(sizeof(formatfile.path));
			strcpy(viewpath, formatfile.path);
			parseFile(NULL, formatfile.path);
		}


	return( Pt_CONTINUE );

	}

