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

int
save_file( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	return save_file_as(widget, apinfo, cbinfo);

	if (filepath == NULL) {
			return save_file_as(widget, apinfo, cbinfo);
	}

	save_data();

	return( Pt_CONTINUE );

	}

